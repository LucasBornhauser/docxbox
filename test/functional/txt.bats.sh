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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox txt filename.docx"
APPENDIX="is the segmented plain text from given file"

@test "Output of \"docxbox txt {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} txt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND}\" is the the plain text from given file" {
  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "Officia"
}

@test "Output of \"${BASE_COMMAND} -s \" ${APPENDIX}" {
  segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" -s | wc --lines)
  non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  (( ${segmented} > ${non_segmented} ))
}

@test "Output of \"${BASE_COMMAND} --segments \" ${APPENDIX}" {
  segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" --segments | wc --lines)
  non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  (( ${segmented} > ${non_segmented} ))
}

@test "Output of \"docxbox txt nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} txt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} txt nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox txt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} txt "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
