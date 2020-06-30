// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/visitor/docx_xml_remove.h>

docx_xml_remove::docx_xml_remove(
    int argc, const std::vector<std::string>& argv) : docx_xml(argc, argv) {}

bool docx_xml_remove::RemoveBetweenStringsInXml(const std::string& path_xml,
                                                const std::string &lhs,
                                                const std::string &rhs) {
  std::string xml;
  helper::File::GetFileContents(path_xml, &xml);

  if (!helper::String::Contains(xml, "w:document")
      || !helper::String::Contains(xml, "w:body")) return true;

  doc_.LoadFile(path_xml.c_str());

  if (doc_.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body =
      doc_.FirstChildElement("w:document")->FirstChildElement("w:body");

  LocateNodesBetweenText(body, lhs.c_str(), rhs.c_str());

  if (!nodes_to_be_removed_.empty()
      && found_lhs_
      && found_rhs_) {
    RemoveNodes(&nodes_to_be_removed_);

    return tinyxml2::XML_SUCCESS != doc_.SaveFile(path_xml.c_str(), true)
           ? docxbox::AppLog::NotifyError("Failed saving: " + path_xml)
           : true;
  }

  return true;
}

void docx_xml_remove::LocateNodesBetweenText(tinyxml2::XMLElement *node,
                                             const char *lhs,
                                             const char *rhs) {
  if (!node || node->NoChildren()) return;

//  if (found_lhs_ && !found_rhs_) nodes_to_be_removed_.push_back(node);

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *tag = sub_node->Value();

//    if (found_lhs_ && !found_rhs_) nodes_to_be_removed_.push_back(sub_node);

    if (tag) {
      // TODO(kay): check - need to collect for removal(?):
      //  all w:p's between LHS and RHS w:t which don't contain neither of them

      if (0 == strcmp(tag, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (text.empty() || (found_lhs_ && found_rhs_)) continue;

        if (!found_lhs_) {
          if (helper::String::Contains(text, lhs)) {
            found_lhs_ = true;

            nodes_to_be_removed_.push_back(sub_node);
          }
        } else if (!found_rhs_) {
          // Collect all runs until also right-hand-side string is found
          nodes_to_be_removed_.push_back(sub_node);

          if (helper::String::Contains(text, rhs)) found_rhs_ = true;
        }
      }
    }

    LocateNodesBetweenText(sub_node, lhs, rhs);
  } while (!(found_lhs_ && found_rhs_)
      && (sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_remove::RemoveNodes(std::vector<tinyxml2::XMLElement*> *nodes) {
  amount_removed_ = 0;

  for (auto node : *nodes) {
    if (nullptr == node) continue;

    tinyxml2::XMLNode *kParent = node->Parent();

    if (kParent) {
      kParent->DeleteChild(node);

      ++amount_removed_;
    }
  }

  return amount_removed_ > 0;
}

int docx_xml_remove::GetAmountRemoved() {
  return amount_removed_;
}
