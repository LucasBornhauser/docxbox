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

PATH_DOCX="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

UNZIPPED_FOLDER="cp_bio_assay.docx-extracted"

@test "Case 1: Output of \"docxbox uzi {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} uzi
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox uzi nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} uzi nonexistent.docx
  [ "$status" -ne 0 ]
  check_for_valgrind_error

  ${DOCXBOX_BINARY} uzi nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 3: Output of \"docxbox uzi wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} uzi "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "Case 4: With of \"docxbox uzi filename.docx\" all files are unzipped" {
  run ${DOCXBOX_BINARY} uzi "${PATH_DOCX}"
  [ "$status" -eq 0 ]
  check_for_valgrind_error

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Case 5: Unzipped files are located in project root" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}

@test "Case 6: With of \"docxbox uz filename.docx -i\" all files are unzipped" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" -i
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Case 7: Unzipped files are located in project root after running uz -i" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}

@test "Case 8: With of \"docxbox uz filename.docx --indent\" all files are unzipped" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" --indent
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "Case 9: Unzipped files are located in project root after running uz --indent" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
