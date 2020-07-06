#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [0-9] errors from [0-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsm filename.docx"

@test "Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsm "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Output of \"docxbox lsm {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Output of \"${BASE_COMMAND}\" contains information about the xml schema" {
  xml_schema="xmlSchema: http://schemas.openxmlformats.org/officeDocument/2006"

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${xml_schema}"

  check_for_valgrind_error
}

title="Output of \"${BASE_COMMAND}\" "
title+="contains information about the creation time and date"
@test "${title}" {
  created="created: 2020-06-18T10:30:11Z"

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${created}"

  check_for_valgrind_error
}

@test "Output of \"${BASE_COMMAND}\" contains language information" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "language: en-US"

  check_for_valgrind_error
}

@test "Output of \"${BASE_COMMAND}\" contains information about the revision" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "revision: 2"

  check_for_valgrind_error
}

@test "Output of \"docxbox lsm nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsm nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsm nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsm wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsm "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
