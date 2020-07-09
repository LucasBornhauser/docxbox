#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox zpc"

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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
UNZIPPED_DOCX_DIRECTORY="cp_table_unordered_list_images.docx-extracted"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Path of directory to be zipped"

  run ${DOCXBOX_BINARY} zpc
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} directory {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of docx to be created"

  run ${DOCXBOX_BINARY} zpc "${UNZIPPED_DOCX_DIRECTORY}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} directory /path-to-file/filename.docx\" zips directory into docx" {
  if [ ! -d "${UNZIPPED_DOCX_DIRECTORY}" ]; then
    ${DOCXBOX_BINARY} uzi "${PATH_DOCX}"
  fi

  path_new_docx="test/tmp/zp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} zpc "${UNZIPPED_DOCX_DIRECTORY}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ls test/tmp | grep -c zp_table_unordered_list_images.docx

  if [ -d "${UNZIPPED_DOCX_DIRECTORY}" ]; then
    rm --recursive "${UNZIPPED_DOCX_DIRECTORY}"
  fi
}
