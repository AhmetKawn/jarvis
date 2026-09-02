#ifndef VOICE_HPP
  #define VOICE_HPP

  #include <string>
  #include <queue>
  #include <mutex>
  #include <condition_variable>
  #include <thread>
  #include <atomic>

  enum class VoiceType {
      LOCAL,   // Windows SAPI
      PREMIUM  // ElevenLabs / Google Cloud
  };

  class Voice {
  public:
      Voice();
      ~Voice();

      // Konuşma isteğini kuyruğa ekler (Asenkrondur, programı dondurmaz)
      void speak(const std::string& text);

      void setVoiceType(VoiceType type);
      void setVolume(float volume); // 0.0f ile 1.0f arası
      void setRate(int rate);       // Konuşma hızı (-10 ile 10 arası)

      // Acil durum: Tüm konuşmaları durdur ve kuyruğu temizle
      void stopAll();

  private:
      // Ses ayarları
      VoiceType currentType;
      float volume = 1.0f;
      int rate = 0;

      // Asenkron yönetim için gerekli bileşenler
      std::queue<std::string> speechQueue;    // Konuşma sırası
      std::mutex queueMutex;                  // Kuyruk güvenliği (Thread-safe)
      std::condition_variable cv;              // Thread uyandırma sinyali
      std::thread workerThread;               // Arka plan konuşma işçisi
      std::atomic<bool> isRunning;            // Sistem çalışma durumu

      // İç işleyiş fonksiyonları
      void processQueue(); // Arka planda çalışan döngü
      void speakLocal(const std::string& text);
      void speakPremium(const std::string& text);
  };

  #endif
