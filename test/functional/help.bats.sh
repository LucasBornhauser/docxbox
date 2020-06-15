#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

# Meta commands
@test "Running w/o any command displays help" {
  "$BATS_TEST_DIRNAME"/docxbox | grep "Usage: docxbox <command> \[args\]"
}

regex_version_check="(^|\s)+(docxBox v)\K([0-9]|\.)*(?=\s|$)"
@test "Running w/o any command displays version" {
  "${docxbox}" | grep --perl-regexp --only-matching "${regex_version_check}"
}

@test "\"docxbox h\" displays help" {
  "${docxbox}" h | grep "Usage: docxbox <command> \[args\]"
}

@test "\"docxbox h\" displays version" {
  "${docxbox}" h | grep --perl-regexp --only-matching "${regex_version_check}"
}

@test "\"docxbox ?\" displays help" {
  "$BATS_TEST_DIRNAME"/docxbox ? | grep "Usage: docxbox <command> \[args\]"
}

@test "\"docxbox ?\" displays version" {
  "${docxbox}" ? | grep --perl-regexp --only-matching "${regex_version_check}"
}

# List DOCX contents:
@test "\"docxbox h ls\" displays help for ls command" {
  run "$BATS_TEST_DIRNAME"/docxbox h ls
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]
}

@test "\"docxbox h lsj\" displays help for lsj command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsj
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]
}

@test "\"docxbox h lsl\" displays help for lsl command" {
  pattern="List all files containing search-string or regular expression:"

  run "$BATS_TEST_DIRNAME"/docxbox h lsl
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"docxbox h lslj\" displays help for lslj command" {
  pattern="List all files containing search-string or regular expression:"

  run "$BATS_TEST_DIRNAME"/docxbox h lslj
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"docxbox h lsf\" displays help for lsf command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsf
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsfj\" displays help for lsfj command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsfj
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsd\" displays help for lsd command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsd
  [ "$status" -eq 0 ]
  [ "Command: lsd - List fields from DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsdj\" displays help for lsdj command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsdj
  [ "$status" -eq 0 ]
  [ "Command: lsd - List fields from DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsi\" displays help for lsi command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsi
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsij\" displays help for lsij command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsij
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsm\" displays help for lsm command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsm
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data of DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsmj\" displays help for lsmj command" {
  run "$BATS_TEST_DIRNAME"/docxbox h lsmj
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data of DOCX:" = "${lines[0]}" ]
}

# Manipulate DOCX document:
@test "\"docxbox h rpi\" displays help for rpi command" {
  run "$BATS_TEST_DIRNAME"/docxbox h rpi
  [ "$status" -eq 0 ]
  [ "Command: rpi - Replace image in DOCX document:" = "${lines[0]}" ]
}

@test "\"docxbox h rpt\" displays help for rpt command" {
  run "$BATS_TEST_DIRNAME"/docxbox h rpt
  [ "$status" -eq 0 ]
  [ "Command: rpt - Replace text in DOCX document:" = "${lines[0]}" ]
}

@test "\"docxbox h rmt\" displays help for rmt command" {
  pattern="Command: rmt - Remove DOCX content between given strings:"

  run "$BATS_TEST_DIRNAME"/docxbox h rmt
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"docxbox h mm\" displays help for mm command" {
  run "$BATS_TEST_DIRNAME"/docxbox h mm
  [ "$status" -eq 0 ]
  [ "Command: mm - Modify or set meta attribute in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h sfv\" displays help for sfv command" {
  run "$BATS_TEST_DIRNAME"/docxbox h sfv
  [ "$status" -eq 0 ]
  [ "Command: sfv - Set field value:" = "${lines[0]}" ]
}

# Convert DOCX:
@test "\"docxbox h txt\" displays help for txt command" {
  run "$BATS_TEST_DIRNAME"/docxbox h txt
  [ "$status" -eq 0 ]
  [ "Command: txt - Output plaintext from DOCX document:" = "${lines[0]}" ]
}

@test "\"docxbox h diff\" displays help for diff command" {
  pattern="Command diff - Side-by-side compare file from two DOCX archives:"

  run "$BATS_TEST_DIRNAME"/docxbox h diff
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

# Extract and create DOCX:
@test "\"docxbox h uz\" displays help for uz command" {
  run "$BATS_TEST_DIRNAME"/docxbox h uz
  [ "$status" -eq 0 ]
  [ "Command: uz - Unzip given DOCX file:" = "${lines[0]}" ]
}

@test "\"docxbox h uzi\" displays help for uzi command" {
  run "$BATS_TEST_DIRNAME"/docxbox h uzi
  [ "$status" -eq 0 ]
  [ "Command: uzi - Unzip DOCX and indent XML files:" = "${lines[0]}" ]
}

@test "\"docxbox h uzm\" displays help for uzm command" {
  run "$BATS_TEST_DIRNAME"/docxbox h uzm
  [ "$status" -eq 0 ]
  [ "Command: uzm - Unzip only media files DOCX file:" = "${lines[0]}" ]
}

@test "\"docxbox h zp\" displays help for zp command" {
  run "$BATS_TEST_DIRNAME"/docxbox h zp
  [ "$status" -eq 0 ]
  [ "Command: zp - Create (zip) DOCX from files:" = "${lines[0]}" ]
}

@test "\"docxbox h zpc\" displays help for zpc command" {
  pattern="Command: zpc - Compress XML, than create DOCX from files:"

  run "$BATS_TEST_DIRNAME"/docxbox h zpc
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}
