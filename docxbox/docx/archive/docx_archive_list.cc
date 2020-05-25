// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/archive/docx_archive_list.h>

docx_archive_list::docx_archive_list(
    int argc,
    char **argv) : docx_archive(argc, argv) {}

// List files inside DOCX archive and their attributes
bool docx_archive_list::ListFilesInDocx(bool as_json, bool images_only) {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX filename"))
    return false;

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    return docxbox::AppError::Output(message);
  }

  if (!IsZipArchive(path_docx_in_)) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  std::string file_ending = ParseFileWildcard(3);

  bool compare_two_docx = file_ending.empty()
      && argc_ >= 4
      && helper::String::EndsWith(argv_[3], ".docx")
      && helper::File::FileExists(argv_[3]);

  auto miniz_ext = new miniz_cpp_ext();

  std::string files_list = miniz_ext->PrintDir(
      docx_file,
      as_json, images_only, file_ending, {},
      false, compare_two_docx);

  if (compare_two_docx) {
    std::string kSummary1 = miniz_ext->GetSummary();

    ListFilesInDocxCompare(
        as_json, images_only, file_ending, &files_list, &kSummary1);
  } else {
    // Output files list of single DOCX
    std::cout << files_list;
  }

  delete miniz_ext;

  return true;
}

// Output two DOCX file lists side-by-side
void docx_archive_list::ListFilesInDocxCompare(bool as_json,
                                               bool images_only,
                                               const std::string &file_ending,
                                               std::string *file_list_1,
                                               std::string *summary_1) {
  const std::string &path_docx_in_2 =
      docxbox::AppArguments::ResolvePathFromArgument(
          path_working_directory_, argc_, argv_, 4);

  miniz_cpp::zip_file docx_file_2(path_docx_in_2);

  auto miniz_ext = new miniz_cpp_ext();

  std::string file_list_2 = miniz_ext->PrintDir(
      docx_file_2,
      as_json, images_only, file_ending, {},
      false, true);

  std::string summary_2 = miniz_ext->GetSummary();

  auto renderer = new docx_fileListCompare(
      *file_list_1, *summary_1,
      file_list_2, summary_2,
      path_docx_in_, path_docx_in_2);

  renderer->Output();

  delete renderer;
  delete miniz_ext;
}

// List contained images and their attributes
bool docx_archive_list::ListImageFilesInDocx(bool as_json) {
  if (as_json) return ListFilesInDocx(as_json, true);

  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto miniz_ext = new miniz_cpp_ext();

  std::cout << miniz_ext->PrintDir(docx_file, false, true);

  delete miniz_ext;

  return true;
}

bool docx_archive_list::LocateFilesContainingString(bool as_json) {
  std::string needle;

  if (!InitLocateFilesContaining(&as_json, needle)) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string grep = "grep -iRl \"" + needle + "\" " + path_extract_;

  auto files_located = helper::Cli::GetExecutionResponse(grep.c_str());
  helper::String::ReplaceAll(files_located, path_extract_ + "/", "");

  auto filenames = helper::String::Explode(files_located, '\n');

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();

  if (!filenames.empty()) {
    auto miniz_ext = new miniz_cpp_ext();

    std::cout << miniz_ext->PrintDir(docx_file, as_json, false, "", filenames);

    delete miniz_ext;
  }

  return true;
}

bool docx_archive_list::InitLocateFilesContaining(
  bool *as_json, std::string &needle) const {
  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      2, "DOCX filename")) return false;

  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      3, "String or regular expression to be located")) return false;

  needle = argv_[3];

  if (needle == "-l" || needle == "--locate") {
    if (!docxbox::AppArguments::IsArgumentGiven(
        argc_,
        4, "String or regular expression to be located")) return false;

    needle = argv_[4];

    if (needle == "-j" || needle == "--json") {
      if (!docxbox::AppArguments::IsArgumentGiven(
          argc_,
          5, "String or regular expression to be located")) return false;

      *as_json = true;
      needle = argv_[5];
    }

    return  true;
  }

  if (needle == "-lj") {
    if (!docxbox::AppArguments::IsArgumentGiven(
        argc_,
        4, "String or regular expression to be located")) return false;

    needle = argv_[4];
    *as_json = true;
  }

  if (needle == "-j" || needle == "--json") {
    if (!docxbox::AppArguments::IsArgumentGiven(
        argc_,
        4, "String or regular expression to be located")) return false;

    *as_json = true;
    needle = argv_[4];
  }

  if (needle == "-lj") {
    if (!docxbox::AppArguments::IsArgumentGiven(
        argc_,
        4, "String or regular expression to be located")) return false;

    needle = argv_[4];
  }

  return true;
}

// Output meta data from within given DOCX file:
// Creation date, revision, title, language, used fonts, contained media files
bool docx_archive_list::ListMetaFromXmls(bool as_json) {
  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto *meta_component = new meta(argc_, argv_);
  meta_component->SetPathExtract(path_extract_);

  if (as_json) meta_component->SetOutputAsJson(true);

  int index_app = 0;
  int index_core = 0;

  for (const auto &file_in_zip : file_list) {
    const char *path_file_within_docx = file_in_zip.filename.c_str();

    std::string
        path_file_absolute = path_extract_ + "/" + path_file_within_docx;

    // Extract meta data from app.xml and core.xml,
    // data is output when both are collected
    // or another of the same kind is found (e.g. merged documents)
    if (helper::String::EndsWith(file_in_zip.filename, "app.xml")) {
      meta_component->CollectFromAppXml(
          file_in_zip.filename,
          helper::File::GetFileContents(path_file_absolute));

      ++index_app;
    } else if (helper::String::EndsWith(file_in_zip.filename, "core.xml")) {
      meta_component->LoadCoreXml(path_file_absolute);
      meta_component->CollectFromCoreXml(file_in_zip.filename);

      ++index_core;
    }
  }

  meta_component->Output();  // Output anything that hasn't been yet

  delete meta_component;

  return true;
}

// List referenced fonts and their metrics
bool docx_archive_list::ListReferencedFonts(bool as_json) {
  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto *fontTable_component = new fontTable();

  if (as_json) std::cout << "[";

  int index_font = 0;

  for (const auto &file_in_zip : file_list) {
    if (helper::String::EndsWith(file_in_zip.filename, "fontTable.xml")) {
      // Extract fonts data from all fontTable.xml files
      const char *path_file_within_docx = file_in_zip.filename.c_str();

      std::string path_file_absolute =
          path_extract_ + "/" + path_file_within_docx;

      fontTable_component->CollectFontsMetrics(
          helper::File::GetFileContents(path_file_absolute));

      if (as_json) {
        if (index_font > 0) std::cout << ",";

        fontTable_component->OutputAsJson(file_in_zip.filename);
      } else {
        fontTable_component->OutputPlain(file_in_zip.filename);
      }

      fontTable_component->Clear();

      ++index_font;
    }
  }

  delete fontTable_component;

  if (as_json) std::cout << "]";

  return true;
}

bool docx_archive_list::ListFieldsFromXmls(bool as_json) {
  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_field(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    // TODO(kay): fetch from all textual XML files, instead only document.xml

    if (!helper::String::EndsWith(file_in_zip.filename, "word/document.xml"))
      continue;

    parser->CollectFields(path_extract_ + "/" + file_in_zip.filename);
  }

  parser->Output(as_json);

  delete parser;

  return true;
}
