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

BASE_COMMAND="\"docxbox ls filename.docx"
PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_NEW_DOCX="test/tmp/changedFile.docx"

ERR_LOG="test/tmp/err.log"

@test "Exit code of ${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} ls
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Output of ${BASE_COMMAND}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributs[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Output of ${BASE_COMMAND}\" is contained files" {
search_values=(
"[Content_Types].xml"
"_rels/.rels"
"word/_rels/document.xml.rels"
"word/document.xml"
"word/theme/theme1.xml"
"word/media/image1.png"
"word/media/image2.jpeg"
"word/settings.xml"
"word/webSettings.xml"
"word/styles.xml"
"word/numbering.xml"
"docProps/core.xml"
"word/fontTable.xml"
"docProps/app.xml")

  for i in "${search_values[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Output of ${BASE_COMMAND}\" contains amount of contained files" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count '14 files'

  check_for_valgrind_error
}

@test "Output of ${BASE_COMMAND}\" contains files' date and time" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "7/3/2020"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "7/3/2020"

  check_for_valgrind_error
}

long_description="contains files with the given file ending"
@test "Output of ${BASE_COMMAND} *.file-ending\" ${long_description}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" *.jpeg | grep --count "image2.jpeg"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" *.xml | grep --count "10 files"

  check_for_valgrind_error
}

@test "With \"${BASE_COMMAND} changedFile.docx\" a side-by-side comparison is displayed" {
  run ${DOCXBOX_BINARY} lorem "${PATH_DOCX}" "${PATH_NEW_DOCX}"

  amount_chars_base=$(${DOCXBOX_BINARY} ls "${PATH_DOCX}" | wc --bytes)
  amount_chars_diff=$(${DOCXBOX_BINARY} ls "${PATH_DOCX}" "${PATH_NEW_DOCX}" | wc --bytes)

  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" "${PATH_NEW_DOCX}" | (( ${amount_chars_base} < ${amount_chars_diff} ))

  check_for_valgrind_error
}

@test "Output of ${BASE_COMMAND} nonexistent.docx\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox ls nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} ls nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"

  check_for_valgrind_error
}

@test "Output of ${BASE_COMMAND} wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} ls "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
