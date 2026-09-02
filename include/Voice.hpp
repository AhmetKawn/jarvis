#ifndef VOICE_HPP
#define VOICE_HPP

#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

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
    void setVolume(float v);
    void setRate(int r);
    void stopAll();

private:
    VoiceType currentType;
    float volume = 1.0f;
    int rate = 0;
    
    // Thread ve senkronizasyon
    bool isRunning;
    std::thread workerThread;
    std::mutex queueMutex;
    std::queue<std::string> speechQueue;
    std::condition_variable cv;

    // Yerel ses için yardımcı fonksiyon
    void speakLocal(const std::string& text);

    // Premium ses için yardımcı fonksiyon
    void speakPremium(const std::string& text);
    
    // Kuyruk işlemci
    void processQueue();
};

#endif
