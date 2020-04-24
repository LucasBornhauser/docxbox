// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_EXT_EXT_MINIZ_CPP_HPP_
#define DOCXBOX_EXT_EXT_MINIZ_CPP_HPP_

#include <docxbox/docx/xml/docx_xml_indent.h>

#include <memory>
#include <string>
#include <vector>

#include <vendor/miniz-cpp/zip_file.hpp>

// Static extension methods to miniz-cpp

class miniz_cpp_ext {
 public:
  static void CreateSubDirectories(
      const std::string &path_extract,
      const std::vector<miniz_cpp::zip_info> &file_list) {
    for (const auto& file_in_zip : file_list) {
      if (helper::String::Contains(file_in_zip.filename, "/")) {
        auto directories = helper::String::Explode(file_in_zip.filename, '/');

        std::string path = path_extract;
        int amount_directories = directories.size();

        for (int i = 0; i < amount_directories - 1; i++) {
          path += "/" + directories[i];
          mkdir(path.c_str(), 0777);
        }
      }
    }
  }

  static void ReduceExtractToImages(
      const std::string &path_extract,
      const std::vector<miniz_cpp::zip_info> &file_list) {
    for (const auto &file_in_zip : file_list) {
      const char *path_file_within_docx = file_in_zip.filename.c_str();

      const std::basic_string<char,
                              std::char_traits<char>,
                              std::allocator<char>>
          &path_file_absolute = path_extract + "/" + path_file_within_docx;

      if (
          helper::String::EndsWith(file_in_zip.filename, ".xml")
              || helper::String::EndsWith(file_in_zip.filename, ".rels")) {
        // Remove all .xml and .rels files
        helper::File::Remove(path_file_absolute.c_str());
      } else {
        // Move all other (=media) files into root of extraction directory
        std::string path_extract_file =
            path_extract + "/"
                + helper::File::GetLastPathSegment(file_in_zip.filename);

        rename(path_file_absolute.c_str(), path_extract_file.c_str());
      }
    }

    // Remove all (now empty) sub-directories
    miniz_cpp_ext::RemoveSubDirectories(path_extract, file_list);
  }

  // Precondition: the directories contain no (more) files
  static void RemoveSubDirectories(
      const std::string &path_extract,
      const std::vector<miniz_cpp::zip_info> &file_list) {
    for (const auto& file_in_zip : file_list) {
      if (helper::String::Contains(file_in_zip.filename, "/")) {
        auto directories = helper::String::Explode(file_in_zip.filename, '/');

        int amount_directories = directories.size();

        for (int i = amount_directories - 1; i > 0; i--) {
          std::string path_remove = path_extract;

          for (int j = 0; j < i; j++) {
            path_remove += "/" + directories[j];
          }

          helper::File::Remove(path_remove.c_str());
        }
      }
    }
  }

  static bool RemoveExtract(const std::string &path_extract,
                            const std::vector<miniz_cpp::zip_info> &file_list) {
    // Remove all files
    for (const auto &file_in_zip : file_list)
      helper::File::Remove(
          std::string(path_extract + "/" + file_in_zip.filename).c_str());

    RemoveSubDirectories(path_extract, file_list);

    return helper::File::Remove(path_extract.c_str());
  }

  static std::string PrintDir(
      miniz_cpp::zip_file &docx_file,
      bool as_json = false,
      bool images_only = false,
      const std::string& file_ending = "") {
    std::string out;
    
    if (as_json) {
      out += "[";
    } else {
      out += "   Length        Date  Time   Name\n"
             "---------  ---------- -----   ----\n";
    }

    int index_file = 0;

    uint32_t size_total = 0;

    std::vector<std::string> filenames;
    std::vector<uint32_t> sizes;
    std::vector<std::string> dates;
    std::vector<std::string> times;

    bool filter_by_ending = !file_ending.empty();

    for (auto &member : docx_file.infolist()) {

      if ((images_only && !helper::File::IsWordCompatibleImage(member.filename))
          || (filter_by_ending
              && !helper::String::EndsWith(member.filename, file_ending)))
        continue;

      if (as_json) {
        out += (index_file == 0 ? "" : ",");

        out += R"({"file":")" + member.filename + "\",";

        out += "\"length\":" + std::to_string(member.file_size) + ",";

        out += R"("date":")"
            + std::to_string(member.date_time.month) + "/"
            + std::to_string(member.date_time.day) + "/"
            + std::to_string(member.date_time.year) + "\",";

        out += R"("time":")" + std::to_string(member.date_time.hours) + ":"
                + std::to_string(member.date_time.minutes) + "\"}";
      } else {
        int amount_digits = helper::Numeric::GetAmountDigits(member.file_size);

        if (amount_digits < 9)
          out += helper::String::Repeat(" ", 9 - amount_digits);

        out += std::to_string(member.file_size) + "  ";

        out += std::to_string(member.date_time.month) + "/"
               + std::to_string(member.date_time.day) + "/"
               + std::to_string(member.date_time.year) + " ";

        out += std::to_string(member.date_time.hours) + ":"
               + std::to_string(member.date_time.minutes) + "     ";

        out += member.filename + "\n";
      }

      size_total += member.file_size;

      index_file++;
    }

    if (as_json) return out + "]";

    out += "---------                     -------\n";

    int amount_digits = helper::Numeric::GetAmountDigits(size_total);

    if (amount_digits < 9)
      out += helper::String::Repeat(" ", 9 - amount_digits);

    out += std::to_string(size_total) + "                     "
             + std::to_string(index_file)
             + " file" + (index_file == 1 ? "" : "s") + "\n";

    return out;
  }

  // Indent all contained XML files
  static bool IndentXmlFiles(
      const std::string &path_extract,
      const std::vector<miniz_cpp::zip_info> &file_list) {
    for (const auto& file_in_zip : file_list) {
      if (!helper::String::EndsWith(file_in_zip.filename, ".xml")) continue;

      if (!docx_xml_indent::IndentXml(
          path_extract + "/" + file_in_zip.filename)) return false;
    }

    return true;
  }
};

#endif  // DOCXBOX_EXT_EXT_MINIZ_CPP_HPP_
