// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_batch.h>

docx_batch::docx_batch(std::string path_extract, std::string json)
    : path_extract_(std::move(path_extract)), json_(std::move(json)) {
  is_json_valid_ = InitFromJson();
}

// Extract batch commands and rel. arguments
bool docx_batch::InitFromJson() {
  if (!helper::Json::IsJson(json_)) return false;

  auto json_outer = nlohmann::json::parse(json_);

  for (auto json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      commands_.push_back(docxbox::AppCommands::ResolveCommandByName(it.key()));

      // note: dumped JSON string has associations alphabetically sorted,
      //       and not necessarily in same order as originally given
      arguments_json.push_back(it.value().dump());
    }
  }

  return !commands_.empty()
         && commands_.size() == arguments_json.size();
}

bool docx_batch::ProcessBatch() {
  if (!is_json_valid_)
    return docxbox::AppStatus::Error("Cannot process: Detected invalid JSON");

  for (int index = 0; index < commands_.size(); ++index)
    if (!ProcessStep(index)) return false;

  return true;
}

bool docx_batch::ProcessStep(int index) {
  switch (commands_[index]) {
    case docxbox::AppCommands::Command_Batch:
      return docxbox::AppStatus::Error(
          "Detected nested batch command (not supported)");
    case docxbox::AppCommands::Command_ExecuteUserCommand:
      return docxbox::AppStatus::Error(
          "cmd command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_LoremIpsum:
      return docxbox::AppStatus::Error(
          "lorem command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_ModifyMeta:
      return docxbox::AppStatus::Error(
          "mm command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_RemoveBetweenText:
      return docxbox::AppStatus::Error(
          "rmt command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_ReplaceImage:
      return docxbox::AppStatus::Error(
          "rpi command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_ReplaceText:
      return docxbox::AppStatus::Error(
          "rpt command is not yet implemented in batch processor");
    case docxbox::AppCommands::Command_SetFieldValue:
      return docxbox::AppStatus::Error(
          "sfv command is not yet implemented in batch processor");
      // Ignore non-modification commands
    case docxbox::AppCommands::Command_Cat:break;
    case docxbox::AppCommands::Command_FileDiff:break;
    case docxbox::AppCommands::Command_Help:break;
    case docxbox::AppCommands::Command_GetPlainText:break;
    case docxbox::AppCommands::Command_GetPlainTextSegments:break;
    case docxbox::AppCommands::Command_List:
    case docxbox::AppCommands::Command_ListAsJson:
    case docxbox::AppCommands::Command_ListImages:
    case docxbox::AppCommands::Command_ListImagesAsJson:
    case docxbox::AppCommands::Command_ListFonts:
    case docxbox::AppCommands::Command_ListFontsAsJson:
    case docxbox::AppCommands::Command_ListFields:
    case docxbox::AppCommands::Command_ListFieldsAsJson:
    case docxbox::AppCommands::Command_ListMeta:
    case docxbox::AppCommands::Command_ListMetaAsJson:
    case docxbox::AppCommands::Command_LocateFilesContaining:
    case docxbox::AppCommands::Command_LocateFilesContainingAsJson:
    case docxbox::AppCommands::Command_Unzip:
    case docxbox::AppCommands::Command_UnzipAndIndentXml:
    case docxbox::AppCommands::Command_UnzipMedia:
    case docxbox::AppCommands::Command_Version:
    case docxbox::AppCommands::Command_Zip:
    case docxbox::AppCommands::Command_ZipCompressed:
      return docxbox::AppStatus::Warning(
          "Found non-modification command in batch (ignored)");
    case docxbox::AppCommands::Command_Invalid:
    default:
      return docxbox::AppStatus::Error(
          "Detected invalid command in batch processor");
  }

  return false;
}
