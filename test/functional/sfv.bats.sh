#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [1-9] errors from [1-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox sfv"

MERGEFIELD="MERGEFIELD  Mergefield_One"
MERGEFIELD_HEADER="MERGEFIELD  Mergefield_Header"
MERGEFIELD_FOOTER="MERGEFIELD  Mergefield_Footer"

ARGUMENTS="filename.docx fieldIdentifier fieldValue"

@test "Case 1: Output of \"${BASE_COMMAND} {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} sfv
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

missing_arguments="filename.docx {missing argument}"
@test "Case 2: Output of \"${BASE_COMMAND} ${missing_arguments}\" is an error message" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Field identifier" = "${lines[0]}" ]

  check_for_valgrind_error
}

missing_value="filename.docx fieldIdentifier {missing argument}"
@test "Case 3: Output of \"${BASE_COMMAND} ${missing_value}\" is an error message" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Value to be set" = "${lines[0]}" ]

  check_for_valgrind_error
}

appendix=" the value of the given field is changed"
@test "Case 4: With \"${BASE_COMMAND} ${ARGUMENTS}\" ${appendix}" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD}" foobar
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

title_header="Case 5: With \"${BASE_COMMAND} ${ARGUMENTS}\" the value of the \
MERGEFIELD in the header gets changed"
@test "${title_header}" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD_HEADER}" foobar
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

title_footer="Case 6: With \"${BASE_COMMAND} ${ARGUMENTS}\" the value of the \
MERGEFIELD in the footer gets changed"
@test "${title_footer}" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD_FOOTER}" foobar
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

@test "Case 7: Output of \"docxbox sfv nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} sfv nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} sfv nonexistent.docx "${MERGEFIELD}" foobar 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

title="Case 8: Output of \"docxbox fieldIdentifier fieldValue wrong_file_type\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" "${MERGEFIELD}" foobar 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
