 cmake_minimum_required(VERSION 3.10)
  project(JarvisCPP)

  set(CMAKE_CXX_STANDARD 17)

  # Kütüphaneleri bul
  find_package(CURL REQUIRED)

  # Header dosyalarının yerini belirt
  include_directories(include)

  # Kaynak dosyaları listele
  set(SOURCES
      src/main.cpp
      src/Core.cpp
      src/Brain.cpp
      src/Perception.cpp
      src/Voice.cpp
      src/Action.cpp
  )

  # Çalıştırılabilir dosyayı oluştur
  add_executable(Jarvis ${SOURCES})

  # CURL kütüphanesini projeye bağla
  target_link_libraries(Jarvis PRIVATE CURL::libcurl)
  if(WIN32)
      target_link_libraries(Jarvis PRIVATE ole32 sapi)
  endif()
