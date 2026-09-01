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

  Şimdi src/Voice.cpp dosyasını yazalım. Burada Windows'un COM (Component Object Model) yapısını kullanarak SAPI'ye
  erişeceğiz. Bu kısım C++'ın en "zor" ve "alt seviye" kısımlarından biridir çünkü Windows'un derinliklerine ineriz.

  src/Voice.cpp
  #include "Voice.hpp"
  #include <iostream>
  #include <windows.h>
  #include <sapi.h> // Windows Speech API
  #include <spspeech.h>

  // COM kütüphaneleri için gerekli linkleme
  #pragma comment(lib, "sapi.lib")
  #pragma comment(lib, "ole32.lib")

  Voice::Voice() : currentType(VoiceType::LOCAL) {
      std::cout << "[Voice] Audio systems initialized. Voice: LOCAL" << std::endl;

      // COM kütüphanesini başlat (Windows SAPI için şarttır)
      if (FAILED(CoInitialize(NULL))) {
          std::cerr << "Voice Error: COM initialization failed!" << std::endl;
      }
  }

  Voice::~Voice() {
      // COM kütüphanesini serbest bırak
      CoUninitialize();
  }

  void Voice::setVoiceType(VoiceType type) {
      currentType = type;
  }

  void Voice::speak(const std::string& text) {
      if (currentType == VoiceType::LOCAL) {
          speakLocal(text);
      } else {
          speakPremium(text);
      }
  }

  void Voice::speakLocal(const std::string& text) {
      // ISpVoice arayüzünden bir nesne oluştur
      ISpVoice* pVoice = NULL;

      if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice))) {
          std::cerr << "Voice Error: Could not create SAPI voice instance." << std::endl;
          return;
      }

      // Metni geniş karakterlere (Wide String) çevir (SAPI bunu ister)
      std::wstring wideText = std::wstring(text.begin(), text.end());

      // Konuşmayı başlat
      pVoice->Speak(wideText.c_str(), SPF_DEFAULT, NULL);

      // Kaynakları serbest bırak
      pVoice->Release();
  }

  void Voice::speakPremium(const std::string& text) {
      // BURASI GELECEK GELİŞTİRME ALANI
      // Burada Brain.cpp'deki gibi bir API isteği atılacak.
      // API'den gelen .mp3 dosyası geçici olarak kaydedilecek.
      // Windows 'ShellExecute' veya bir ses kütüphanesi (SFML/Bass) ile oynatılacak.

      std::cout << "[Premium Voice] Requesting high-quality audio for: " << text << std::endl;
      std::cout << "[System] Premium Voice API integration is in progress..." << std::endl;

      // Şimdilik premium seçilirse de local çalışsın ki sistem çökmesin
      speakLocal(text);
  }
