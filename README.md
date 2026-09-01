 🤖 JarvisCPP: Advanced AI Personal Assistant

  JarvisCPP is a high-performance, modular personal assistant built from the ground up using C++17. It mimics the core
  functionality of the fictional J.A.R.V.I.S. by integrating state-of-the-art Large Language Models (LLMs),
  Speech-to-Text (STT), and Text-to-Speech (TTS) engines, all while maintaining direct control over the host Windows
  environment.

  Unlike simple chatbots, JarvisCPP is designed as a system of interconnected "organs" (modules) that handle perception,
  cognition, and action in a continuous loop.

  ---

  🏗 System Architecture

  The project follows a modular design pattern to ensure scalability and maintainability:

  - Core: The orchestrator. It manages the "Listen $\rightarrow$ Think $\rightarrow$ Act $\rightarrow$ Speak" cycle.
  - Brain: The cognitive engine. It handles API communication with LLMs (Claude/OpenAI) and manages both short-term
    conversation history and long-term memory.
  - Perception: The auditory system. It captures raw PCM audio from the microphone and utilizes OpenAI Whisper for
    high-accuracy transcription.
  - Voice: The vocal apparatus. It implements a hybrid TTS system, supporting both local Windows SAPI (for speed) and
    cloud-based premium voices (for realism).
  - Action: The physical interface. It interacts with the Windows Shell API to launch applications, navigate the web,
    and execute system-level commands.

  ---

  🚀 Key Features

  - ✅ Cognitive Intelligence: Integrated with Claude 3 / GPT-4 for human-like reasoning and a sarcastic, professional
    personality.
  - ✅ Voice Command Recognition: Real-time audio capture and transcription.
  - ✅ OS Automation: Ability to launch apps (Chrome, Spotify, etc.) and control system functions via natural language.
  - ✅ Hybrid Memory: Combines session-based context with a key-value long-term memory store.
  - ✅ Low-Level Optimization: Written in C++ for minimal latency and direct hardware access.

  ---

  🛠 Tech Stack & Requirements

  Languages & Tools

  - Language: C++17
  - Build System: CMake 3.10+
  - OS: Windows 10/11

  Dependencies

  - libcurl: For handling HTTP requests to AI APIs.
  - nlohmann/json: For parsing and constructing JSON payloads.
  - Windows SAPI: For local text-to-speech.
  - WinMM: For low-level audio recording.
  - Shell32: For system automation.

  ---

  📦 Installation & Setup

  1. Clone the Repository

  git clone https://github.com/your-username/JarvisCPP.git
  cd JarvisCPP

  2. Install Dependencies

  Make sure you have libcurl and nlohmann/json installed on your system. If using vcpkg:
  vcpkg install curl nlohmann-json

  3. Configure API Keys

  Open src/Brain.cpp and src/Perception.cpp and replace the placeholders with your actual API keys:
  // In Brain.cpp
  std::string apiKey = "YOUR_CLAUDE_OR_OPENAI_KEY";

  // In Perception.cpp
  headers = curl_slist_append(headers, "Authorization: Bearer YOUR_OPENAI_KEY");

  4. Build the Project

  mkdir build
  cd build
  cmake ..
  cmake --build .

  5. Run Jarvis

  ./Jarvis.exe

  ---

  🗺 Roadmap

  - [ ] Visual Interface: Integrating an OpenGL or Qt-based holographic UI.
  - [ ] Local LLM: Support for Llama 3 via llama.cpp to remove API dependency.
  - [ ] Computer Vision: Adding OpenCV for facial recognition and gesture control.
  - [ ] Home Automation: Integrating MQTT for smart light and device control.

  ⚠️ Disclaimer

  This project is for educational purposes. Use of APIs may incur costs depending on your provider's pricing model.
  Please handle your API keys securely and do not commit them to public repositories.

  ---

  Developed with ❤️ by [Ahmet Kaan]
