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
