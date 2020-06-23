// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_STYLES_H_
#define DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_STYLES_H_

#include <docxbox/docx/component/styles.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/renderer/component/docx_renderer_styles.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class docx_xml_styles: docx_xml {
 public:
  explicit docx_xml_styles(
      std::string path_extract,
      int argc = 0,
      const std::vector<std::string>& argv = {});

  // Get (insert if not exists) style ID
  std::string GetStyleId(
      styles::StyleType type, styles::StyleName name);

 private:
  std::string path_extract_;

  static std::string GetStyleIdPrefixByType(styles::StyleType type);
};

#endif  // DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_STYLES_H_
