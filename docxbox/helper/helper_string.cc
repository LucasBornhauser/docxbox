// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/helper/helper_string.h>
#include <iostream>

namespace helper {

// Check whether given string starts w/ given prefix
bool String::StartsWith(const char *str, const char *prefix) {
  return 0 == strncmp(str, prefix, strlen(prefix));
}

// Check whether given string ends w/ given string
bool String::EndsWith(std::string const &value, std::string const &ending) {
  return ending.size() <= value.size()
      && std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool String::IsAnyOf(
    const std::string& str,
    std::vector<std::string> endings) {
  uint16_t amount_endings = endings.size();

  for (int i = 0; i < amount_endings; ++i) {
    if (str == endings[i]) return true;
  }

  return false;
}

bool String::Contains(const std::string &haystack, const char *needle) {
  return std::string::npos != haystack.find(needle);
}

void String::Replace(
    std::string &haystack,
    const char *needle,
    const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  index = haystack.find(needle, index);

  if (std::string::npos != index)
    haystack.replace(index, needle_len, replacement);
}

int String::ReplaceAll(
    std::string &haystack,
    const std::string &needle,
    const std::string &replacement) {
  // Get first occurrence
  size_t pos = haystack.find(needle);

  int amount_replaced = 0;

  // Repeat till end is reached
  while (pos != std::string::npos) {
    // Replace this occurrence of Sub String
    haystack.replace(pos, needle.size(), replacement);

    // Get the next occurrence from the current position
    pos = haystack.find(needle, pos + replacement.size());

    amount_replaced++;
  }

  return amount_replaced;
}

// Get sub string between given surrounding left- and right-hand-side delimiters
// return std::string  The enclosed sub-string or an empty string
std::string String::GetSubStrBetween(
    const std::string &str,
    const char *lhs,
    const char *rhs,
    u_int32_t &offset) {
  size_t offsetStart = str.find(lhs, offset);

  if (std::string::npos == offsetStart) return "";

  size_t offsetEnd = str.find(rhs, offsetStart);

  if (std::string::npos == offsetEnd) return "";

  // Exclude LHS
  offsetStart += strlen(lhs);

  offset = offsetStart;

  return str.substr(offsetStart, offsetEnd - offsetStart);
}

std::string String::GetSubStrBetween(
    const std::string &str,
    const char *lhs,
    const char *rhs) {
    u_int32_t offset = 0;

  return GetSubStrBetween(str, lhs, rhs, offset);
}

int String::OffsetChar(const std::string &str, char c, int offset) {
  char ch;

  int16_t len = str.length();

  do {
    ch = str[offset];

    if (ch == c) return offset;

    ++offset;
  } while (offset < len);

  return -1;
}

// Split given string by given character delimiter into vector of strings
std::vector<std::string> String::Explode(
    std::string const &str,
    char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);)
    result.push_back(std::move(token));

  return result;
}

// Trim from start (in place)
void String::LTrim(std::string &s) {
  s.erase(
      s.begin(),
      std::find_if(
          s.begin(),
          s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// Trim from end (in place)
void String::RTrim(std::string &s) {
  s.erase(
      std::find_if(
          s.rbegin(),
          s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace)))
          .base(),
      s.end());
}

// Trim from both ends (in place)
void String::Trim(std::string &s) {
  LTrim(s);
  RTrim(s);
}

extern bool String::IsNumeric(
    std::string str,
    bool trim,
    bool can_contain_punctuation,
    bool can_contain_spaces) {
  if (str.empty()) return false;

  if (trim) Trim(str);

  if (str.empty() && !can_contain_spaces) return false;

  for (char i : str) {
    if ((can_contain_spaces && i == ' ')
        || (can_contain_punctuation && ispunct(i))) continue;

    if (!isdigit(i)) return false;
  }

  return true;
}

bool String::IsAllUpper(const std::string& str) {
  for (auto c : str)
    if (isalpha(c) && !isupper(c)) return false;

  return true;
}

bool String::IsJson(const std::string &str) {
  const char *kStr = str.c_str();

  return
      ((StartsWith(kStr, "{") && EndsWith(str, "}"))
       || (StartsWith(kStr, "[") && EndsWith(str, "]")))
       && Contains(kStr, "\"")
       && Contains(kStr, ":");
}

std::string String::ToLower(std::string str) {
  std::transform(
      str.begin(),
      str.end(),
      str.begin(),
      [](unsigned char c) {
        return std::tolower(c);
      });

  return str;
}

std::string String::ToUpper(const std::string& str) {
  std::string upper;

  for (auto c : str) upper += toupper(c);

  return upper;
}

u_int32_t String::GetMaxLength(const std::vector<std::string>& strings) {
  u_int32_t max = 0;

  for (const auto& str : strings) {
    u_int32_t len = str.length();

    if (len > max) max = len;
  }

  return max;
}

std::string String::Repeat(const std::string& str, u_int16_t amount) {
  std::string out;

  for (u_int16_t i = 0; i < amount; i++) {
    out+= str;
  }

  return out;
}

std::string String::RenderSideBySide(
    const std::string &left,
    const std::string &right,
    int amount_spaces_gap) {
  auto lines_left = Explode(left, '\n');
  auto lines_right = Explode(right, '\n');

  uint32_t amount_lines_right = lines_right.size();

  int index = 0;

  std::string gap = amount_spaces_gap < 1
      ? ""
      : Repeat(" ", amount_spaces_gap);

  std::string out;

  int len_left_max = GetMaxLength(lines_left);
  int len_right_max = GetMaxLength(lines_right);

  for (auto line_left : lines_left) {
    auto line_right = amount_lines_right > index ? lines_right[index] : "";

    uint32_t len_left = line_left.length();
    uint32_t len_right = line_right.length();

    std::string left_margin_right = len_left < len_left_max
        ? Repeat(" ", len_left_max - len_left)
        : "";

    std::string right_margin_right = len_right < len_right_max
        ? Repeat(" ", len_right_max - len_right)
        : "";

    std::string style_on;
    std::string style_off;

    if (line_left != line_right) {
      style_on = kAnsiReverse;
      style_off = kAnsiReset;
    }

    out += style_on + line_left.append(left_margin_right).append(style_off)
        .append(gap)
        .append(style_on).append(line_right).append(right_margin_right)
          .append(style_off)
        .append("\n");

    ++index;
  }

  return out;
}

std::string String::GetRandomNumericString(
    u_int32_t length,
    bool starts_with_zero) {
  std::string str = starts_with_zero
    ? "0"
    : std::to_string(std::rand() % 9);

  if (length == 1) return str;

  while (str.length() < length) str += std::to_string(std::rand() % 9);

  return str;
}

}  // namespace helper
