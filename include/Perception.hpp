#ifndef PERCEPTION_HPP
#define PERCEPTION_HPP

#include <string>
#include <vector>

class Perception {
public:
    Perception();
    ~Perception();

    std::string listen(); // Mikrofonu dinle ve metne çevir
    bool isWakeWordDetected(); // "Jarvis" dendi mi?

private:
    // Ses kaydı için yardımcı fonksiyonlar
    void recordAudio(const std::string& filename, int durationSeconds);
    std::string transcribeAudio(const std::string& filename);

    // Ses eşiği kontrolü (Sessizliği algılamak için)
    bool detectVoiceActivity();
};

#endif
