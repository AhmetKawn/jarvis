 #include "Action.hpp"
  #include <iostream>
  #include <windows.h>
  #include <shellapi.h> // ShellExecute için şart

  #pragma comment(lib, "shell32.lib")

  Action::Action() {
      std::cout << "[Action] System actuators online. Ready to execute commands." << std::endl;

      // Uygulama kestirmelerini tanımla
      appMap["chrome"] = "chrome.exe";
      appMap["notepad"] = "notepad.exe";
      appMap["calc"] = "calc.exe";
      appMap["spotify"] = "spotify.exe";
      appMap["cmd"] = "cmd.exe";
  }

  Action::~Action() {}

  void Action::execute(const std::string& response) {
      // Jarvis'in cevabı içinde komut olup olmadığını kontrol et
      // Basit bir kontrol: "açıyorum", "başlatıyorum" gibi kelimeler geçiyor mu?

      if (response.find("açıyorum") != std::string::npos || response.find("başlatıyorum") != std::string::npos) {
          std::string app = parseCommand(response);
          if (!app.empty()) {
              openApplication(app);
          }
      }
      else if (response.find("gidiyorum") != std::string::npos || response.find("açıyorum") != std::string::npos &&
  response.find("http") != std::string::npos) {
          // Web sitesi açma mantığı
          openWebsite("https://www.google.com"); // Örnek
      }
      else if (response.find("kapatıyorum") != std::string::npos) {
          runSystemCommand("shutdown /s /t 60"); // 60 saniye sonra kapat
      }
  }

  std::string Action::parseCommand(const std::string& text) {
      // Cevap içinden uygulama ismini ayıkla
      // Örn: "Tabii efendim, Chrome'u açıyorum" -> "chrome"
      for (auto const& [key, val] : appMap) {
          if (text.find(key) != std::string::npos) {
              return key;
          }
      }
      return "";
  }

  void Action::openApplication(const std::string& appName) {
      if (appMap.find(appName) != appMap.end()) {
          std::string path = appMap[appName];
          std::cout << "[Action] Launching application: " << path << std::endl;

          // ShellExecute: Windows'ta bir programı veya dosyayı açmanın en etkili yolu
          ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
      } else {
          std::cerr << "[Action] Error: Application not found in map." << std::endl;
      }
  }

  void Action::openWebsite(const std::string& url) {
      std::cout << "[Action] Navigating to: " << url << std::endl;
      ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
  }

  void Action::runSystemCommand(const std::string& command) {
      std::cout << "[Action] Executing system command: " << command << std::endl;
      system(command.c_str());
  }
