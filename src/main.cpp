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
