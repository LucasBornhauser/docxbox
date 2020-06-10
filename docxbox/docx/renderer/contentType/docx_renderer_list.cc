// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_list.h>
#include <docxbox/docx/component/contentTypes.h>

// Constructor
docx_renderer_list::docx_renderer_list(
    std::string path_extract, const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

void docx_renderer_list::SetIsOrdered(bool is_ordered) {
  is_ordered_ = is_ordered;
}

// Ensure word/numbering.xml exists
void docx_renderer_list::AddNumberingXml() const {
  if (is_json_valid_) {
    auto numbering_component = new numbering(path_extract_);
    numbering_component->AddNumberingXml(is_ordered_);

    delete numbering_component;
  }
}

// Ensure relation to word/numbering.xml in word/_rels/document.xml.rels
void docx_renderer_list::AddNumberingRels() const {
  if (!is_json_valid_) return;

  rels::GetRelationshipId(
      path_extract_,
      "numbering.xml",
      rels::RelationType::RelationType_Numbering);
}

bool docx_renderer_list::InitFromJson() {
  if (!helper::Json::IsJson(json_)
      || !docx_renderer::IsElementType(
          {ElementType_ListUnordered, ElementType_ListOrdered})) return false;

  items_.clear();

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      try {
        const std::string &key = it.key();

        if ("items" == key) {
          auto items = it.value();

          for (nlohmann::json::iterator it_items = items.begin();
               it_items != items.end();
               ++it_items)
            items_.push_back(it_items.value());
        } else if (key == "pre" || key == "post") {
          ExtractPreOrPostfix(it);
        }
      } catch (nlohmann::detail::invalid_iterator &e) {
        continue;
      }
    }
  }

  return items_.empty()
    ? docxbox::AppStatus::Error("Invalid markup: list contains no items.")
    : true;
}

std::string docx_renderer_list::Render(bool is_ordered) {
  SetIsOrdered(is_ordered);

  AddNumberingXml();
  AddNumberingRels();
  // TODO(kay): add numbering override(?)

  return Render();
}

std::string docx_renderer_list::Render() {
  if (!is_json_valid_) {
    wml_ = "";

    return wml_;
  }

  generic_root_tag_ = "w:r";

  wml_ = kWRunLhs;

  for (std::string& item : items_) {
    wml_ +=
        "<w:p>"
          "<w:pPr>"
            "<w:numPr>"
              "<w:ilvl w:val=\"0\"/>"
              "<w:numId w:val=\"1\"/>"
            "</w:numPr>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
          + RenderTextInRun(item)
        + "</w:p>";
  }

  wml_ += kWRunRhs;

  RenderPreAndPostFixAroundWml();

  return wml_;
}
