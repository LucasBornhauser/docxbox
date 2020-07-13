// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/component/docx_xml_rels.h>
#include <docxbox/docx/component/numbering.h>
#include <docxbox/docx/component/rels.h>

#include <utility>

docx_xml_rels::docx_xml_rels(
    std::string path_extract,
    int argc,
    const std::vector<std::string>& argv) : docx_xml(
    argc,
    argv) {
  path_xml_file_ = path_extract + "/word/_rels/document.xml.rels";
  path_extract_ = std::move(path_extract);

  helper::File::GetFileContents(path_xml_file_, &xml_);
}

// Get (insert if not exists) relationship id of given image target
std::string docx_xml_rels::GetRelationShipIdByTarget(
    const std::string &target, rels::RelationType relation_type) {
  doc_.Parse(xml_.c_str());

  if (doc_.ErrorID() != 0) {
    docxbox::AppLog::NotifyError("Failed parse word/_rels/document.xml.rels");

    return "";
  }

  tinyxml2::XMLElement *relationships = doc_.FirstChildElement("Relationships");

  auto relationship = relationships->FirstChildElement();

  const char* target_key = "Target";

  bool target_exists = false;

  std::string relationship_id;

  do {
    auto attr_target = relationship->FindAttribute(target_key);
    if (!attr_target) continue;

    std::string target_value = attr_target->Value();

    GetRelationshipId(relationship, &relationship_id);

    if (target_value == target) {
      // TODO(kay) check targets, seems like path prefix causes not identifying
      //  existing targets
      docxbox::AppLog::NotifyInfo("Found relationship ID to: " + target);
      target_exists = true;

      break;
    }
  } while ((relationship = relationship->NextSiblingElement()));

  if (target_exists) return relationship_id;

  // TODO(kay): ensure functionality when there aren't any pre-existing nodes
  if (relationship_id.length() < 4) return "";

  auto last_id = std::stoi(relationship_id.substr(3));

  relationship_id = "rId" + std::to_string(last_id + 1);

  const std::basic_string<char,
                          std::char_traits<char>,
                          std::allocator<char>>
      &kRelationshipTag = docx_renderer_rels::RenderRelationship(
      target,
      relation_type,
      relationship_id) + "</Relationships>";

  helper::String::Replace(&xml_, "</Relationships>", kRelationshipTag.c_str());

  docxbox::AppLog::NotifyInfo("Add relationship ID to: " + target);

  if (SaveXml(true)) return relationship_id;

  docxbox::AppLog::NotifyError("Failed save word/_rels/document.xml.rels");

  return "";
}

void docx_xml_rels::GetRelationshipId(
    const tinyxml2::XMLElement *relationship,
    std::string *relationship_id) const {
  const char* id_key = "Id";
  *relationship_id = relationship->FindAttribute(id_key)->Value();
}
