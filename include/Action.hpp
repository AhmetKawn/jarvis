 #ifndef ACTION_HPP
  #define ACTION_HPP

  #include <string>
  #include <vector>
  #include <map>

  class Action {
  public:
      Action();
      ~Action();

      void execute(const std::string& response); // Beyinden gelen cevabı işle

  private:
      // Uygulama ve Web açma fonksiyonları
      void openApplication(const std::string& appName);
      void openWebsite(const std::string& url);
      void runSystemCommand(const std::string& command);

      // Uygulama eşleşmeleri (Örn: "chrome" -> "chrome.exe")
      std::map<std::string, std::string> appMap;

      // Komut belirleme yardımcı fonksiyonu
      std::string parseCommand(const std::string& text);
  };

  #endif
