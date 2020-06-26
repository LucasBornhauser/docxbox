#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

path_docx="test/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsl filename.docx searchString"

description="lists all files containing given searchString"
regex_description="lists all files containing given regular expression"

regex="/[0-9A-Z]\{8\}/"

search_results=(
"word/document.xml"
"word/fontTable.xml"
"word/numbering.xml"
"word/styles.xml")

regex_result="docProps/core.xml"

@test "Exit code of \"${base_command}\" is zero" {
  run ${DOCXBOX_BINARY} lsl "${path_docx}" fonts
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsl {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsl
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

title="Output of \"docxbox lsl filename.docx {missing argument}\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - Missing argument: "
  pattern+="String or regular expression to be located"

  run ${DOCXBOX_BINARY} lsl "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"${base_command}\" ${description}" {
  for i in "${search_results[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${path_docx}" fonts | grep --count "${i}"
  done 
}

@test "\"docxbox ls filename.docx -l searchString\" ${description}" {
  for i in "${search_results[@]}"
  do
    ${DOCXBOX_BINARY} ls "${path_docx}" -l fonts | grep --count "${i}"
  done
}

@test "\"docxbox ls filename.docx --locate searchString\" ${description}" {
  for i in "${search_results[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${path_docx}" --locate fonts | grep --count "${i}"
  done
}

@test "With \"docxbox lsl filename.docx regex\" ${regex_description}" {
  ${DOCXBOX_BINARY} lsl "${path_docx}" "${regex}" | grep --count ${regex_result}
}

@test "With \"docxbox ls filename.docx -l regex\" ${regex_description}" {
  ${DOCXBOX_BINARY} ls "${path_docx}" -l "${regex}" | grep --count ${regex_result}
}

@test "With \"docxbox ls filename.docx --locate regex\" ${regex_description}" {
  ${DOCXBOX_BINARY} ls "${path_docx}" --locate "${regex}" | grep --count ${regex_result}
}

@test "Output of \"docxbox lsl nonexistent.docx searchString\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsl nonexistent.docx fonts
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsl nonexistent.docx fonts 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsl wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${i}" fonts 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
