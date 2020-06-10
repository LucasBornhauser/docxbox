// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_REMOVE_H_
#define DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_REMOVE_H_

#include <docxbox/app/app_status.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_remove:docx_xml {
 public:
  docx_xml_remove(int argc, char **argv);

  bool RemoveBetweenStringsInXml(
      const std::string& path_xml,
      const std::string& lhs,
      const std::string& rhs);

  static bool RemoveNodes(std::vector<tinyxml2::XMLElement*> nodes);

 private:
  bool found_lhs_ = false;
  bool found_rhs_ = false;

  std::vector<tinyxml2::XMLElement*> nodes_to_be_removed_;

  void LocateNodesBetweenText(
      tinyxml2::XMLElement *node,
      const std::string &lhs,
      const std::string &rhs);
};

#endif  // DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_REMOVE_H_
