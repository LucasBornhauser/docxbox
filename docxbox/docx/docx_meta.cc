// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_meta.h>

docx_meta::docx_meta(int argc, char **argv) {
    argc_ = argc;
    argv_ = argv;
}

void docx_meta::SetPathExtract(const std::string &path) {
  path_extract_ = path;
}

void docx_meta::SetOutputAsJson(bool output_as_json) {
  output_as_json_ = output_as_json;
}

std::string docx_meta::FetchAttributeFromAppXml(
    const char *tag_name,
    const std::string &label) {

  return FetchAttributeFromAppXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label);
}

std::basic_string<char> docx_meta::GetRhsTagByTagName(const char *tag_name) {
  return std::string("</") + tag_name + ">";
}

std::basic_string<char> docx_meta::GetLhsTagByTagName(const char *tag_name) {
  std::string attributes;

  if (helper::String::StartsWith(tag_name, "dcterms:"))
    attributes = kWmlAttributeDcTermsXsi;

  return std::string("<") + tag_name + attributes + ">";
}

void docx_meta::GetLhsTagByAttribute(const docx_meta::Attribute &attribute,
                                     const char *&lhs_of_value) {
  switch (attribute) {
    case Attribute_Created:
      lhs_of_value = GetLhsTagByTagName(kWmlTagDcTermsCreated).c_str();
      break;
    case Attribute_Creator:
      lhs_of_value = GetLhsTagByTagName(kWmlTagDcCreator).c_str();
      break;
    case Attribute_Title:
      lhs_of_value = GetLhsTagByTagName(kWmlTagDcTitle).c_str();
      break;
    case Attribute_Language:
      lhs_of_value = GetLhsTagByTagName(kWmlTagDcLanguage).c_str();
      break;
    case Attribute_Revision:
      lhs_of_value = GetLhsTagByTagName(kWmlTagCpRevision).c_str();
      break;
    case Attribute_LastModifiedBy:
      lhs_of_value = GetLhsTagByTagName(kWmlTagCpLastModifiedBy).c_str();
      break;
    case Attribute_Modified:
      lhs_of_value = GetLhsTagByTagName(kWmlTagDcTermsModified).c_str();
      break;
    case Attribute_LastPrinted:
      lhs_of_value = GetLhsTagByTagName(kWmlTagCpLastPrinted).c_str();
      break;
    default:throw "Attribute unknown";
  }
}
void docx_meta::GetRhsTagByAttribute(const docx_meta::Attribute &attribute,
                                     const char *&rhs_of_value) {
  switch (attribute) {
    case Attribute_Created:
      rhs_of_value = GetRhsTagByTagName(kWmlTagDcTermsCreated).c_str();
      break;
    case Attribute_Creator:
      rhs_of_value = GetRhsTagByTagName(kWmlTagDcCreator).c_str();
      break;
    case Attribute_Title:
      rhs_of_value = GetRhsTagByTagName(kWmlTagDcTitle).c_str();
      break;
    case Attribute_Language:
      rhs_of_value = GetRhsTagByTagName(kWmlTagDcLanguage).c_str();
      break;
    case Attribute_Revision:
      rhs_of_value = GetRhsTagByTagName(kWmlTagCpRevision).c_str();
      break;
    case Attribute_LastModifiedBy:
      rhs_of_value = GetRhsTagByTagName(kWmlTagCpLastModifiedBy).c_str();
      break;
    case Attribute_Modified:
      rhs_of_value = GetRhsTagByTagName(kWmlTagDcTermsModified).c_str();
      break;
    case Attribute_LastPrinted:
      rhs_of_value = GetRhsTagByTagName(kWmlTagCpLastPrinted).c_str();
      break;
    default:throw "Attribute unknown";
  }
}

std::string docx_meta::FetchAttributeFromAppXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(app_xml_, lhs_of_value)) return "";

  std::string value =
      helper::String::GetSubStrBetween(app_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

std::string docx_meta::FetchAttributeFromCoreXml(
    const char* tag_name,
    const std::string &label) {
  return FetchAttributeFromCoreXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label);
}

std::string docx_meta::FetchAttributeFromCoreXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(core_xml_, lhs_of_value)) return "";

  std::string value = helper::String::GetSubStrBetween(
      core_xml_,
      lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

docx_meta::Attribute docx_meta::ResolveAttributeByName(
    const std::string &attribute) {
  if (attribute == "created") return Attribute_Created;
  if (attribute == "creator") return Attribute_Creator;
  if (attribute == "description") return Attribute_Description;
  if (attribute == "keywords") return Attribute_Keywords;
  if (attribute == "language") return Attribute_Language;
  if (attribute == "lastModifiedBy") return Attribute_LastModifiedBy;
  if (attribute == "lastPrinted") return Attribute_LastPrinted;
  if (attribute == "modified") return Attribute_Modified;
  if (attribute == "revision") return Attribute_Revision;
  if (attribute == "subject") return Attribute_Subject;
  if (attribute == "title") return Attribute_Title;

  return Attribute_Unknown;
}

bool docx_meta::InitModificationArguments() {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX filename")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "Meta attribute to be set")) return false;

  attribute_ = ResolveAttributeByName(argv_[3]);

  if (attribute_ == Attribute::Attribute_Unknown) {
    std::cout
        << "Invalid argument: Unknown or unsupported attribute: "
        << argv_[3] << "\n";

    return false;
  }

  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      4,
      "Value to set attribute to"))
    return false;

  value_ = argv_[4];

  return true;
}

bool docx_meta::UpsertAttribute() {
  path_core_xml_ = path_extract_ + "/docProps/core.xml";

  LoadCoreXml(path_core_xml_);

  try {
    return AttributeExistsInCoreXml(attribute_)
           ? UpdateCoreAttribute(attribute_, value_)
           : InsertCoreAttribute(attribute_, value_);
  } catch (std::string &message) {
    std::cout << message;

    return false;
  }
}

bool docx_meta::UpdateCoreAttribute(
    Attribute attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  const char *lhs_of_value;
  const char *rhs_of_value;

  try {
    GetLhsTagByAttribute(attribute, lhs_of_value);
    GetRhsTagByAttribute(attribute, rhs_of_value);
  } catch (std::string &message) {
    std::cout << message;

    return false;
  }

  core_xml_ = helper::String::Replace(
      core_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromCoreXml(lhs_of_value, rhs_of_value, "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool docx_meta::InsertCoreAttribute(
    Attribute attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  const char *lhs_of_value;
  const char *rhs_of_value;

  GetLhsTagByAttribute(attribute, lhs_of_value);
  GetLhsTagByAttribute(attribute, rhs_of_value);

  helper::String::Replace(
      core_xml_,
      kWordMlCorePropertiesRhs,
      (lhs_of_value + value + rhs_of_value + kWordMlCorePropertiesRhs).c_str());

  return true;
}

// Check whether core.xml of current DOCX contains given attribute
bool docx_meta::AttributeExistsInCoreXml(Attribute attribute) {
  EnsureIsLoadedCoreXml();

  const char *lhs_of_value;

  try {
    GetLhsTagByAttribute(attribute, lhs_of_value);
  } catch (std::string &message) {
    std::cout << message;

    return false;
  }

  return helper::String::Contains(core_xml_, lhs_of_value);
}

void docx_meta::EnsureIsLoadedCoreXml() {
  if (core_xml_.empty()) {
    path_core_xml_ = path_extract_ + "/docProps/core.xml";
    core_xml_ = helper::File::GetFileContents(path_core_xml_);
  }
}

void docx_meta::LoadCoreXml(std::string path) {
  core_xml_ = helper::File::GetFileContents(path);
}

bool docx_meta::SaveCoreXml() {
  if (helper::File::FileExists(core_xml_))
    helper::File::Remove(path_core_xml_.c_str());

  if (!helper::File::WriteToNewFile(path_core_xml_, core_xml_))
    throw "Failed saving: " + path_core_xml_;

  return true;
}

void docx_meta::CollectFromAppXml(std::string path_app_xml_current,
                                  std::string app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml:
  // output prematurely
  if (has_collected_from_app_xml_) Output();

  path_app_xml_ = std::move(path_app_xml_current);

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(kWmlTagApplication, "Application");

  attributes_.emplace_back("xmlSchema", ExtractXmlSchemaFromAppXml(app_xml));

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(kWmlTagTemplate, "Template");

  has_collected_from_app_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

std::string docx_meta::ExtractXmlSchemaFromAppXml(std::string &app_xml) const {
  u_int32_t offset = 0;

  std::string xml_schema = helper::String::GetSubStrBetween(
      app_xml,
      kWordMlXmlSchemeLhs,
      kWordMlXmlSchemeRhs,
      offset);

  // Remove last segment from schema,
  // leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema, '/');

  helper::String::Replace(
      xml_schema,
      (std::string("/") + segments[ segments.size() - 1]).c_str(),
      "");
  return xml_schema;
}

void docx_meta::CollectFromCoreXml(std::string path_core_xml_current) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml:
  // output prematurely
  if (has_collected_from_core_xml_) Output();

  path_core_xml_ = std::move(path_core_xml_current);

  FetchAttributeFromCoreXml(kWmlTagDcTermsCreated, "created");
  FetchAttributeFromCoreXml(kWmlTagDcCreator, "created");
  FetchAttributeFromCoreXml(kWmlTagDcDescription, "description");
  FetchAttributeFromCoreXml(kWmlTagDcKeywords, "keywords");
  FetchAttributeFromCoreXml(kWmlTagDcLanguage, "language");
  FetchAttributeFromCoreXml(kWmlTagCpLastModifiedBy, "lastModifiedBy");
  FetchAttributeFromCoreXml(kWmlTagCpLastPrinted, "lastPrinted");
  FetchAttributeFromCoreXml(kWmlTagDcTermsModified, "modified");
  FetchAttributeFromCoreXml(kWmlTagCpRevision, "revision");
  FetchAttributeFromCoreXml(kWmlTagDcSubject, "subject");
  FetchAttributeFromCoreXml(kWmlTagDcTitle, "title");

  has_collected_from_core_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

void docx_meta::Output() {
  if (has_collected_from_app_xml_ || has_collected_from_core_xml_) {
    if (output_as_json_)
      OutputJson();
    else
      OutputPlain();
  }

  Clear();
}

void docx_meta::OutputPlain() {
  for (std::tuple<std::string, std::string> attribute : attributes_) {
    std::cout
      << std::get<0>(attribute) << ": "
      << std::get<1>(attribute) << "\n";
  }
}

void docx_meta::OutputJson() {
  std::cout << "{";

  int i = 0;

  for (std::tuple<std::string, std::string> attribute : attributes_) {
    if (i > 0) std::cout << ",";

    std::cout
      << "\"" << std::get<0>(attribute) << "\": "
      << "\"" << std::get<1>(attribute) << "\"";

    i++;
  }

  std::cout << "}";
}

void docx_meta::Clear() {
  has_collected_from_app_xml_ = false;
  has_collected_from_core_xml_ = false;

  path_app_xml_ = "";
  path_core_xml_ = "";

  attributes_.clear();
}