
#ifndef CLASS_DOCXBOX_APP_HELP
#define CLASS_DOCXBOX_APP_HELP

#include <string>
#include <iostream>

#include "../../config.h"
#include "app.h"


namespace docxbox {

class AppHelp {
 public:
  static bool PrintVersion();

  static bool PrintHelp(bool with_title, AppCommands::Command command);
  static bool PrintOverview(bool with_title);
  static bool PrintHelpOnList();
  static bool PrintHelpOnListImages(bool with_title = true);
  static bool PrintHelpOnListFonts(bool with_title = true);
  static bool PrintHelpOnListMeta(bool with_title = true);
  static bool PrintHelpOnModifyMeta();
  static bool PrintHelpOnReplaceImage();
  static bool PrintHelpOnGetPlainText();
  static bool PrintHelpOnUnzip(bool with_uz = true);
  static bool PrintHelpOnVersion();
  static bool PrintHelpOnZip();

  static void PrintUnknownArgumentMessage(const char *arg);

 private:

};

} // namespace docxbox

#endif // CLASS_DOCXBOX_APP_HELP