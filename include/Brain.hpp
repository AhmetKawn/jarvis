 #ifndef BRAIN_HPP
  #define BRAIN_HPP

  #include <string>
  #include <vector>
  #include <map>

  class Brain {
  public:
      Brain(const std::string& apiKey);
      std::string think(const std::string& input);
      void remember(const std::string& key, const std::string& value);
      std::string recall(const std::string& key);

  private:
      std::string apiKey;
      std::string apiUrl = "https://api.anthropic.com/v1/messages"; // Örn: Claude API
      std::map<std::string, std::string> longTermMemory; // Kalıcı bellek
      std::vector<std::pair<std::string, std::string>> conversationHistory; // Sohbet geçmişi

      // libcurl için yardımcı fonksiyon
      static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
  };

  #endif

  Şimdi ise gerçek "canavar" olan Brain.cpp dosyasına geçiyoruz. Burada hem HTTP protokolünü yönetiyoruz hem de Jarvis'e
  bir "kişilik" yüklüyoruz.

  src/Brain.cpp
  #include "Brain.hpp"
  #include <iostream>
  #include <curl/curl.h>
  #include <nlohmann/json.hpp>

  using json = nlohmann::json;

  Brain::Brain(const std::string& key) : apiKey(key) {
      std::cout << "[Brain] Neural networks initialized. API Key loaded." << std::endl;
  }

  // libcurl'den gelen verileri bir string'e toplamak için gerekli fonksiyon
  size_t Brain::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
      ((std::string*)userp)->append((char*)contents, size * nmemb);
      return size * nmemb;
  }

  std::string Brain::think(const std::string& input) {
      CURL* curl;
      CURLcode res;
      std::string readBuffer;

      curl = curl_easy_init();
      if (!curl) return "Sistem Hatası: CURL başlatılamadı.";

      // 1. Jarvis Kişiliği ve Sistem Komutu (System Prompt)
      // Burada Jarvis'in nasıl davranacağını belirliyoruz.
      std::string systemPrompt = "Sen Jarvis'sin. Tony Stark'ın asistanısın. Kibar, hafif sarkastik, çok zeki ve
  sadıksın. "
                                 "Yanıtların kısa, öz ve profesyonel olmalı. Kullanıcıya 'Efendim' diye hitap et.";

      // 2. Sohbet Geçmişini JSON'a ekle (Bellek yönetimi)
      json messages = json::array();
      for (auto& pair : conversationHistory) {
          messages.push_back({{"role", pair.first}, {"content", pair.second}});
      }
      messages.push_back({{"role", "user"}, {"content", input}});

      // 3. API İstek Gövdesi (Payload)
      json payload;
      payload["model"] = "claude-3-sonnet-20240229"; // Model seçimi
      payload["system"] = systemPrompt;
      payload["messages"] = messages;
      payload["max_tokens"] = 500;

      std::string jsonString = payload.dump();

      // 4. HTTP Başlıkları (Headers)
      struct curl_slist* headers = NULL;
      headers = curl_slist_append(headers, ("x-api-key: " + apiKey).c_str());
      headers = curl_slist_append(headers, "anthropic-version: 2023-06-01");
      headers = curl_slist_append(headers, "Content-Type: application/json");

      // 5. CURL Ayarları
      curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

      // İstek Gönder
      res = curl_easy_perform(curl);

      if (res != CURLE_OK) {
          std::cerr << "API Connection Error: " << curl_easy_strerror(res) << std::endl;
          return "Bağlantı hatası oluştu efendim.";
      }

      // Temizlik
      curl_easy_cleanup(curl);
      curl_slist_free_all(headers);

      try {
          // 6. Yanıtı Ayrıştır (Parse)
          auto responseJson = json::parse(readBuffer);
          std::string botResponse = responseJson["content"][0]["text"];

          // Geçmişe ekle
          conversationHistory.push_back({"user", input});
          conversationHistory.push_back({"assistant", botResponse});

          return botResponse;
      } catch (json::exception& e) {
          std::cerr << "JSON Parse Error: " << e.what() << std::endl;
          return "Yanıtı anlamlandıramadım efendim.";
      }
  }

  void Brain::remember(const std::string& key, const std::string& value) {
      longTermMemory[key] = value;
      std::cout << "[Memory] Saved: " << key << " -> " << value << std::endl;
  }

  std::string Brain::recall(const std::string& key) {
      if (longTermMemory.find(key) != longTermMemory.end()) {
          return longTermMemory[key];
      }
      return "Bu bilgiye sahip değilim efendim.";
  }
