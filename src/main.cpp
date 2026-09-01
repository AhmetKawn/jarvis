 #include <iostream>
  #include "Core.hpp"

  int main() {
      std::cout << "--- JARVIS SYSTEM INITIALIZING ---" << std::endl;

      try {
          Core jarvis;
          jarvis.run();
      } catch (const std::exception& e) {
          std::cerr << "CRITICAL SYSTEM ERROR: " << e.what() << std::endl;
          return 1;
      }

      return 0;
  }

  ---

  ADIM 3: Çekirdek Mantık (Core Implementation)

  Burada Jarvis'in nasıl bir döngüyle çalışacağını tanımlıyoruz.

  src/Core.cpp
  #include "Core.hpp"
  #include <iostream>

  Core::Core() : isRunning(false) {
      // Modülleri başlat
      brain = std::make_unique<Brain>("YOUR_API_KEY_HERE");
      perception = std::make_unique<Perception>();
      voice = std::make_unique<Voice>();
      action = std::make_unique<Action>();
  }

  Core::~Core() {}

  void Core::run() {
      isRunning = true;
      std::cout << "Jarvis is now online. Listening..." << std::endl;

      while (isRunning) {
          processCycle();
      }
  }

  void Core::stop() {
      isRunning = false;
  }

  void Core::processCycle() {
      // 1. Algıla: Sesli girişi yazıya çevir
      std::string userInput = perception->listen();

      if (userInput.empty()) return;
      std::cout << "User: " << userInput << std::endl;

      // "Kapat" veya "Dur" denirse sistemi kapat
      if (userInput == "jarvis dur" || userInput == "sistemi kapat") {
          stop();
          return;
      }

      // 2. Düşün: Girdiyi zekaya gönder
      std::string response = brain->think(userInput);
      std::cout << "Jarvis: " << response << std::endl;

      // 3. Uygula: Eğer cevapta bir komut varsa (örn: "not defterini aç")
      action->execute(response);

      // 4. Konuş: Cevabı sesli olarak ilet
      voice->speak(response);
  }
