// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_numbering.h>

docx_numbering::docx_numbering(std::string path_extract) {
  path_extract_ = std::move(path_extract);
}

bool docx_numbering::AddNumberingXml() {
  std::string path_numbering_xml = path_extract_ + "/word/numbering.xml";
  
  if (helper::File::FileExists(path_numbering_xml)) return true;
  
  std::string xml =
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"
      "<w:numbering "
        "xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" "
        "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" "
        "xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
        "xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" "
        "xmlns:v=\"urn:schemas-microsoft-com:vml\" "
        "xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" "
        "xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" "
        "xmlns:w10=\"urn:schemas-microsoft-com:office:word\" "
        "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "
        "xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" "
        "xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" "
        "xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" "
        "xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" "
        "xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" "
        "mc:Ignorable=\"w14 wp14\">"
      "<w:abstractNum w:abstractNumId=\"0\">"
        "<w:multiLevelType w:val=\"hybridMultilevel\"/>"
        "<w:tmNoNumList/>"
        "<w:lvl w:ilvl=\"0\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"1\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"2\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"3\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"4\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"5\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"6\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"7\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "<w:lvl w:ilvl=\"8\">"
          "<w:numFmt w:val=\"none\"/>"
          "<w:lvlText w:val=\"\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
        "</w:abstractNum>"
        "<w:abstractNum w:abstractNumId=\"1\">"
          "<w:multiLevelType w:val=\"singleLevel\"/>"
          "<w:name w:val=\"Bullet 1\"/>"
          "<w:lvl w:ilvl=\"0\">"
            "<w:numFmt w:val=\"bullet\"/>"
            "<w:lvlText w:val=\"\uF09F\"/>"
            "<w:lvlJc w:val=\"left\"/>"
            "<w:pPr>"
              "<w:tabs>"
                "<w:tab w:val=\"num\" w:pos=\"360\"/>"
              "</w:tabs>"
              "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
            "</w:pPr>"
            "<w:rPr>"
              "<w:rFonts "
                "w:ascii=\"Wingdings\" "
                "w:hAnsi=\"Wingdings\" "
                "w:eastAsia=\"Wingdings\" "
                "w:cs=\"Wingdings\"/>"
            "</w:rPr>"
          "</w:lvl>"
        "</w:abstractNum>"
        "<w:num w:numId=\"1\">"
          "<w:abstractNumId w:val=\"1\"/>"
        "</w:num>"
        "<w:num w:numId=\"2\">"
          "<w:abstractNumId w:val=\"0\"/>"
        "</w:num>"
      "</w:numbering>";

  return helper::File::WriteToNewFile(path_numbering_xml, xml);
}