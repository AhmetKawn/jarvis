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
