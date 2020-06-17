#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox="$BATS_TEST_DIRNAME/docxbox"
path_docx="test/functional/tmp/cp_bio_assay.docx"

unzipped_folder="cp_bio_assay.docx-extracted"

@test "Output of \"docxbox uzi {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" uzi
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox uzi nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" uzi nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" uzi nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox uzi wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" uzi "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}

@test "With of \"docxbox uzi filename.docx\" all files are unzipped" {
  run "${docxbox}" uzi "${path_docx}"
  [ "$status" -eq 0 ]

  cat "${unzipped_folder}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Unzipped files are located in project root" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}

@test "With of \"docxbox uz filename.docx -i\" all files are unzipped" {
  run "${docxbox}" uz "${path_docx}" -i
  [ "$status" -eq 0 ]

  cat "${unzipped_folder}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Unzipped files are located in project root after running uz -i" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}

@test "With of \"docxbox uz filename.docx --indent\" all files are unzipped" {
  run "${docxbox}" uz "${path_docx}" --indent
  [ "$status" -eq 0 ]

  cat "${unzipped_folder}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Unzipped files are located in project root after running uz --indent" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}
