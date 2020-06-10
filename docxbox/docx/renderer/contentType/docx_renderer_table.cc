// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_table.h>

// Constructor
docx_renderer_table::docx_renderer_table(
    std::string path_extract, const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = helper::Json::IsJson(json);

  if (is_json_valid_) InitFromJson();
}

bool docx_renderer_table::InitFromJson() {
  if (!docx_renderer::IsElementType(ElementType_Table)) return false;

  amount_columns_ = 0;
  amount_rows_ = 0;

  auto json_outer = nlohmann::json::parse(json_);

  for (auto& json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string& key = it.key();

      if (key == "columns") {
        amount_columns_ = it.value();
      } else if (key == "rows") {
        amount_rows_ = it.value();
      } else if (key == "header") {
        has_column_headers = true;
        amount_columns_ = 0;

        auto headers = it.value();

        for (nlohmann::json::iterator it_header = headers.begin();
             it_header != headers.end();
             ++it_header) {
          column_headers_.push_back(it_header.value());

          ++amount_columns_;
        }
      } else if (key == "content") {
        amount_rows_ = 0;

        auto rows = it.value();

        for (nlohmann::json::iterator it_row = rows.begin();
             it_row != rows.end();
             ++it_row) {
          auto row = it_row.value();

          for (nlohmann::json::iterator it_column = row.begin();
               it_column != row.end();
               ++it_column) {
            cell_content_.push_back(it_column.value());
          }

          ++amount_rows_;
        }
      } else if (key == "pre" || key == "post") {
        ExtractPreOrPostfix(it);
      }
    }
  }

  return amount_columns_ == 0
             || amount_rows_ == 0
         ? docxbox::AppStatus::Error(
          "Invalid table config: must contain column(s) / row(s)")
         : true;
}

// @see http://officeopenxml.com/WPtable.php
std::string docx_renderer_table::Render() {
  if (!is_json_valid_) throw "Failed render table markup.\n";

  generic_root_tag_ = "w:r";

  wml_ = std::string(kWRunLhs);

  wml_ += std::string(kWTableLhs);
  wml_ += RenderTableProperties();
  wml_ += RenderTableGrid();
  // TODO(kay): add rendering of header
  wml_ += RenderTableRowsAndCells();
  wml_ += std::string(kWTableRhs);

  wml_ += std::string(kWRunRhs);

  RenderPreAndPostFixAroundWml();

  return wml_;
}

std::string docx_renderer_table::RenderTableProperties() {
  return
    "<w:tblPr>"
      "<w:tblStyle w:val=\"TableGrid\"/>"
      "<w:tblW w:w=\"" + std::to_string(table_width_) + "\" w:type=\"pct\"/>"
    "</w:tblPr>";
}

std::string docx_renderer_table::RenderTableGrid() {
  std::string markup = "<w:tblGrid>";

  for (int i = 0; i < amount_columns_; i++) {
    markup += "<w:gridCol w:w=\"" + std::to_string(col_width_) + "\"/>";
  }

  return markup + "</w:tblGrid>";
}

std::string docx_renderer_table::RenderTableRowsAndCells() {
  std::string markup;

  int index_cell = 0;

  for (int i = 0; i < amount_rows_; i++) {
    markup += "<w:tr>";

    for (int j = 0; j < amount_columns_; j++) {
      markup += RenderTableCell(index_cell);

      index_cell++;
    }

    markup += "</w:tr>";
  }

  return markup;
}

std::string docx_renderer_table::RenderTableCell(int index_cell) {
  return
      "<w:tc>"
        "<w:tcPr>"
          "<w:tcW w:w=\"" + std::to_string(col_width_) + "\" w:type=\"dxa\"/>"
        "</w:tcPr>"
        + RenderTextInRunInParagraph(cell_content_[index_cell])
    + "</w:tc>";
}
