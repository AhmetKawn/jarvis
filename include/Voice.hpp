#ifndef VOICE_HPP
#define VOICE_HPP

#include <string>

enum class VoiceType {
    LOCAL,   // Windows SAPI (Robotik ama hızlı)
    PREMIUM  // ElevenLabs/Google (Gerçekçi ama bulut tabanlı)
};

class Voice {
public:
    Voice();
    ~Voice();

    void speak(const std::string& text);
    void setVoiceType(VoiceType type);

private:
    VoiceType currentType;

    // Yerel ses için yardımcı fonksiyon
    void speakLocal(const std::string& text);

    // Premium ses için yardımcı fonksiyon
    void speakPremium(const std::string& text);
};

#endif
