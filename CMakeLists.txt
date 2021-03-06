cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.14)

project(docxbox)

set (VERSION_MAJOR "0")
set (VERSION_MINOR "0")
set (VERSION_PATCH "1")

# configure config.h to be generated from config.h.in, with injected version number
configure_file ("docxbox/config.h.in" "docxbox/config.h")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/linux)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -pedantic")

add_executable(docxbox
        docxbox/main.cc

        docxbox/app/app.cc
        docxbox/app/app_argument.cc
        docxbox/app/app_command.cc
        docxbox/app/app_help.cc
        docxbox/app/app_help.h

        docxbox/docx/docx_archive.cc
        docxbox/docx/docx_fontTable.cc
        docxbox/docx/docx_meta.cc

        docxbox/docx/docx_xml.cc
        docxbox/docx/docx_xml_fields.cc
        docxbox/docx/docx_xml_lorem.cc
        docxbox/docx/docx_xml_to_plaintext.cc
        docxbox/docx/docx_xml_replace.cc

        docxbox/helper/helper_file.cc
        docxbox/helper/helper_string.cc
        docxbox/helper/helper_numeric.cc

        vendor/tinyxml2/tinyxml2.cpp)
