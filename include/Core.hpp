  #ifndef CORE_HPP
  #define CORE_HPP

  #include <string>
  #include <memory>
  #include "Brain.hpp"
  #include "Perception.hpp"
  #include "Voice.hpp"
  #include "Action.hpp"

  class Core {
  public:
      Core();
      ~Core();
      void run(); // Ana döngü
      void stop();

  private:
      bool isRunning;
      std::unique_ptr<Brain> brain;
      std::unique_ptr<Perception> perception;
      std::unique_ptr<Voice> voice;
      std::unique_ptr<Action> action;

      void processCycle(); // Algıla -> Düşün -> Uygula -> Konuş döngüsü
  };

  #endif

  include/Brain.hpp (Zeka kısmı)
  #ifndef BRAIN_HPP
  #define BRAIN_HPP

  #include <string>
  #include <vector>

  class Brain {
  public:
      Brain(const std::string& apiKey);
      std::string think(const std::string& input); // API'ye sor ve cevap al
      void remember(const std::string& key, const std::string& value);
      std::string recall(const std::string& key);

  private:
      std::string apiKey;
      std::vector<std::pair<std::string, std::string>> context; // Kısa süreli bellek
  };

  #endif
