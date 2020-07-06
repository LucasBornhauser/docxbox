// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_DELEGATE_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_DELEGATE_H_

#include <docxbox/docx/component/numbering.h>
#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_heading.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_image.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_link.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_list.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_paragraph.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_table.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_text.h>

#include <string>

class docx_renderer_delegate {
 public:
  void SetPathExtract(const std::string &path_extract);

 protected:
  int argc_;
  char **argv_;

  std::string path_extract_;

  std::string replacement_xml_root_tag_ = "w:r";

  // @see docx_xml_replace::SetImageRelationshipId()
  std::string image_relationship_id_ = "";

  std::string hyperlink_relationship_id_ = "";

  std::string RenderMarkupFromJson(const std::string& json);

  std::string &RenderHeading(int level,
                             const std::string &json,
                             std::string *markup);

  std::string &RenderHyperlink(const std::string &json, std::string *markup);
  std::string &RenderImage(const std::string &json, std::string *markup);

  std::string &RenderList(const std::string &json, std::string *markup);

  std::string &RenderParagraph(const std::string &json, std::string *markup);
  std::string &RenderTable(const std::string &json, std::string *markup);
  std::string &RenderText(const std::string &json, std::string *markup);
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_DELEGATE_H_
