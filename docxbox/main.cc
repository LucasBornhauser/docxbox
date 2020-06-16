// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app.h>
#include <docxbox/helper/helper_dateTime.h>

/**
 * @param argc Amount of arguments
 * @param argv Array of arguments, argv[0] is name and path of executable
 */
int main(int argc, char **argv) {
  docxbox::AppLog::LogStartUp(argc, argv);

  // Process command arguments, display help if no valid command given
  auto *app = new docxbox::App(argc, argv);

  bool success = argc > 1
      ? app->Process()
      : false;

  delete app;

  // TODO(kay): return comprehensible bash error codes instead of 125
  int return_signal = success ? 0 : 125;

  docxbox::AppLog::NotifyInfo(
      "docxBox finished w/ return code: " + std::to_string(return_signal),
      true);

  docxbox::AppLog::Output();

  return return_signal;
}
