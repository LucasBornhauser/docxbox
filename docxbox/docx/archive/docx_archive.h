// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_H_
#define DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/app/app_error.h>
#include <docxbox/docx/archive/docx_diff.h>
#include <docxbox/docx/component/fontTable.h>
#include <docxbox/docx/component/meta.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_table.h>
#include <docxbox/docx/xml/contentType/docx_xml_field.h>
#include <docxbox/docx/xml/visitor/docx_xml_lorem.h>
#include <docxbox/docx/xml/visitor/docx_xml_remove.h>
#include <docxbox/docx/xml/visitor/docx_xml_replace.h>
#include <docxbox/docx/xml/visitor/docx_xml_to_plaintext.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include <docxbox/ext/ext_miniz_cpp.hpp>

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

  virtual ~docx_archive();

  std::string UnzipDocx(
      const std::string &path_docx,
      const std::string &path_extract_appendix = "",
      const std::string &path_extract_prefix = "",
      bool is_temporary = true);

  // Unzip all files of DOCX file w/ file argument taken from argv_
  bool UnzipDocxByArgv(bool is_temporary = false,
                       const std::string &directory_appendix = "",
                       bool ensure_is_docx = true,
                       bool format_xml_files = false);

  static bool IsZipArchive(const std::string& path_file);

  // Check formal structure of DOCX archive - mandatory files given?
  bool IsUnzippedDocx();

  bool UnzipMedia();

  bool CreateDocxFromExtract(
      const std::string &path_docx_out, bool overwrite_source_docx);

  // Zip files into given path into DOCX of given filename
  // Optionally update "creation" and "modified" meta attributes (core.xml)
  // to current date-time value
  bool Zip(
      bool compress_xml = false,
      std::string path_directory = "",
      std::string path_docx_result = "",
      bool set_date_created_to_now = false,
      bool set_date_modified_to_now = false,
      const std::string& date_created = "",
      const std::string& date_modified = "");

  bool GetText(bool newline_at_segments);

  bool ExecuteUserCommand(std::string command = "");
  bool CatFile();
  bool ViewFilesDiff();

  bool ModifyMeta();

  void RemoveTemporaryFiles();

 protected:
  int argc_;
  char **argv_;

  std::string path_working_directory_;

  // DOCX to be read or manipulated
  std::string path_docx_in_;

  // Directory containing extracted XML files of DOCX
  std::string path_extract_;

  // Path(s) of temp. extracted DOCX archives, to be removed at end of docxBox.
  // 1. Paths are added during initialization in InitExtractionPath()
  // 2. Before ending docxBox, RemoveTemporaryFiles() must be called to remove
  //    the no longer needed files
  std::vector<std::string> paths_temporary_;

  bool InitPathDocxByArgV(int index_path_argument);

  std::string InitExtractionPath(
      const std::string &path_docx,
      const std::string &path_extract_appendix,
      bool is_temporary = false,
      const std::string &path_extract_prefix = "");

  void RememberTemporaryExtractionPath(const std::string& path);

  std::string ParseFileWildcard(int index_argument) const;

 private:
  // Update given meta date attribute and immediately save updated core.xml
  bool UpdateCoreXmlDate(
      meta::Attribute attribute,
      const std::string& value = "");

  // TODO(kay): unused for now, ms word refuses to open the file.
  //  check: needs to use other compression algorithm (zip base64)?
  void ZipUsingMinizCpp(bool compress_xml,
                        const std::string &path_directory,
                        const std::string &path_docx_result) const;

  // TODO(kay): remove workaround when minizCpp-compressed DOCX is valid
  //  also according to MS word
  void ZipUsingCLi(const std::string &path_directory,
                   const std::string &path_docx_result,
                   bool compress_xml = false) const;

  void CompressXmlFiles(const std::string &path_directory) const;
};

#endif  // DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_H_
