#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lorem"

ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: Exit code of \"${CMD} filename.docx\" is zero" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"

  run "${DOCXBOX_BINARY}" lorem "${path_docx}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "$BATS_TEST_NUMBER: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${DOCXBOX_BINARY}" lorem
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx\" replaces all text by dummy text" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"
  local pattern="Culpa ad eiusmod"

  local before_lorem=$(${DOCXBOX_BINARY} txt "${path_docx}" | grep --count "Culpa ad eiusmod")

  run "${DOCXBOX_BINARY}" lorem "${path_docx}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  local after_lorem=$(${DOCXBOX_BINARY} txt "${path_docx}" | grep --count "Culpa ad eiusmod")

  (( before_lorem > after_lorem ))
}

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx newFilename.docx\" replaces all text by dummy text and saves it to new file" {
  local path_docx_1="test/tmp/cp_table_unordered_list_images.docx"
  local path_docx_2="test/tmp/lorem.docx"

  ${DOCXBOX_BINARY} lorem "${path_docx_1}" "${path_docx_2}"

  source ./test/functional/_check_for_valgrind_errors.sh

  ls test/tmp | grep -c lorem.docx
}

@test "$BATS_TEST_NUMBER: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" lorem nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lorem nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "$BATS_TEST_NUMBER: \"${CMD} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
