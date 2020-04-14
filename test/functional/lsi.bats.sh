#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}
