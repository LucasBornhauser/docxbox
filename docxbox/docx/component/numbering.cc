// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/numbering.h>

// TODO(kay): transform into docx_renderer_numbering
numbering::numbering(std::string path_extract) {
  path_extract_ = std::move(path_extract);
}

bool numbering::AddNumberingXml(bool unordered) {
  // TODO(kay): enable having both (un- and ordered) style at the same time
  std::string path_numbering_xml = path_extract_ + "/word/numbering.xml";

  if (helper::File::FileExists(path_numbering_xml)) return true;

  std::string xml = RenderXml(unordered);

  return helper::File::WriteToNewFile(path_numbering_xml, xml);
}

std::string numbering::RenderXml(bool unordered) {
  if (unordered)
    return
      RenderXmlNamespaceTag() +
      RenderXmlAbstractNum0() +
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

  // TODO(kay): extract xml duplications of un/ordered numbering.xml

  return
      RenderXmlNamespaceTag() +
      RenderXmlAbstractNum0() +
      "<w:abstractNum w:abstractNumId=\"1\">"
        "<w:multiLevelType w:val=\"singleLevel\"/>"
        "<w:name w:val=\"Bullet 1\"/>"
        "<w:lvl w:ilvl=\"0\">"
          "<w:start w:val=\"1\"/>"
          "<w:numFmt w:val=\"decimal\"/>"
          "<w:lvlText w:val=\"%1.\"/>"
          "<w:lvlJc w:val=\"left\"/>"
          "<w:pPr>"
            "<w:tabs>"
              "<w:tab w:val=\"num\" w:pos=\"360\"/>"
            "</w:tabs>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
        "</w:lvl>"
      "</w:abstractNum>"
      "<w:num w:numId=\"1\">"
        "<w:abstractNumId w:val=\"1\"/>"
      "</w:num>"
      "<w:num w:numId=\"2\">"
          "<w:abstractNumId w:val=\"0\"/>"
      "</w:num>"
    "</w:numbering>";
}

std::string numbering::RenderXmlNamespaceTag() {
  const std::basic_string<char>
      &urlSchemaWord10 = std::string(kUrlSchemaMsOfficeWord2010);

  const std::basic_string<char>
      &urlSchemaOffice06 = std::string(kUrlSchemaOffice2006);

  return
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"
      "<w:numbering "
        "xmlns:wpc=\"" + urlSchemaWord10 + "wordprocessingCanvas\" "
        "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/"
        "2006\" "
        "xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
        "xmlns:r=\"" + urlSchemaOffice06 + "relationships\" "
        "xmlns:m=\"" + urlSchemaOffice06 + "math\" "
        "xmlns:v=\"urn:schemas-microsoft-com:vml\" "
        "xmlns:wp14=\"" + urlSchemaWord10 + "wordprocessingDrawing\" "
        "xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/"
        "wordprocessingDrawing\" "
        "xmlns:w10=\"urn:schemas-microsoft-com:office:word\" "
        "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/"
        "main\" "
        "xmlns:w14=\"" + urlSchemaWord10 + "wordml\" "
        "xmlns:wpg=\"" + urlSchemaWord10 + "wordprocessingGroup\" "
        "xmlns:wpi=\"" + urlSchemaWord10 + "wordprocessingInk\" "
        "xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" "
        "xmlns:wps=\"" + urlSchemaWord10 + "wordprocessingShape\" "
        "mc:Ignorable=\"w14 wp14\">";
}

std::string numbering::RenderXmlAbstractNum0() {
  return
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
      "</w:abstractNum>";
}
