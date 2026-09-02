 #include "Voice.hpp"
  #include <iostream>
  #include <windows.h>
  #include <sapi.h>
  #include <spspeech.h>

  #pragma comment(lib, "sapi.lib")
  #pragma comment(lib, "ole32.lib")

  Voice::Voice() : currentType(VoiceType::LOCAL), isRunning(true) {
      std::cout << "[Voice] Advanced Audio System Initialized." << std::endl;

      if (FAILED(CoInitialize(NULL))) {
          std::cerr << "Voice Error: COM initialization failed!" << std::endl;
      }

      // Konuşmaları yöneten arka plan thread'ini başlat
      workerThread = std::thread(&Voice::processQueue, this);
  }

  Voice::~Voice() {
      isRunning = false;
      cv.notify_all(); // Thread'i uyandır ve kapat
      if (workerThread.joinable()) {
          workerThread.join();
      }
      CoUninitialize();
  }

  void Voice::speak(const std::string& text) {
      {
          std::lock_guard<std::mutex> lock(queueMutex);
          speechQueue.push(text);
      }
      cv.notify_one(); // İşçi thread'e "konuşacak bir şey var" diye haber ver
  }

  void Voice::processQueue() {
      while (isRunning) {
          std::string currentText;

          {
              std::unique_lock<std::mutex> lock(queueMutex);
              // Kuyruk boşsa veya sistem kapanmışsa bekle
              cv.wait(lock, [this] { return !speechQueue.empty() || !isRunning; });

              if (!isRunning) break;

              currentText = speechQueue.front();
              speechQueue.pop();
          }

          // Asıl konuşma işlemi burada yapılır
          if (currentType == VoiceType::LOCAL) {
              speakLocal(currentText);
          } else {
              speakPremium(currentText);
          }
      }
  }

  void Voice::speakLocal(const std::string& text) {
      ISpVoice* pVoice = NULL;
      if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice))) {
          return;
      }

      // Ses ayarlarını uygula
      pVoice->setVolume((LONG)volume * 100);
      pVoice->setRate(rate);

      std::wstring wideText(text.begin(), text.end());
      pVoice->Speak(wideText.c_str(), SPF_DEFAULT, NULL);
      pVoice->Release();
  }

  void Voice::stopAll() {
      std::lock_guard<std::mutex> lock(queueMutex);
      while (!speechQueue.empty()) speechQueue.pop();
      // Not: SAPI'yi anlık susturmak için ek API çağrıları gerekir.
      std::cout << "[Voice] All speech requests cancelled." << std::endl;
  }

  // Diğer set fonksiyonları basitçe değişkenleri günceller
  void Voice::setVoiceType(VoiceType type) { currentType = type; }
  void Voice::setVolume(float v) { volume = v; }
  void Voice::setRate(int r) { rate = r; }
  void Voice::speakPremium(const std::string& text) {
      std::cout << "[Premium] " << text << std::endl; // Buraya API entegrasyonu gelecek
  }
