#include "Perception.hpp"
#include <iostream>
#include <windows.h>
#include <mmsystem.h> // Windows Multimedia API
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#pragma comment(lib, "winmm.lib") // Ses kütüphanesini bağla

using json = nlohmann::json;

Perception::Perception() {
    std::cout << "[Perception] Audio sensors online. Listening for wake word..." << std::endl;
}

Perception::~Perception() {}

// Basit bir ses kayıt fonksiyonu (PCM formatında)
void Perception::recordAudio(const std::string& filename, int durationSeconds) {
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = 16000; // STT modelleri genelde 16kHz ister
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    HWAVEIN hWaveIn;
    MMRESULT result = waveInOpen(&hWaveIn, WAVE_MAPPER, &wfx, 0, 0, 0);
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "Perception Error: Could not open microphone." << std::endl;
        return;
    }

    // Kayıt tamponu (Buffer)
    const int bufferSize = 32000;
    char* buffer = new char[bufferSize];
    WAVEHDR header;
    memset(&header, 0, sizeof(WAVEHDR));
    header.lpData = buffer;
    header.dwBufferLength = bufferSize;

    waveInPrepareHeader(hWaveIn, &header, sizeof(WAVEHDR));
    waveInAddBuffer(hWaveIn, &header, sizeof(WAVEHDR));
    waveInStart(hWaveIn);

    std::ofstream outFile(filename, std::ios::binary);

    // Belirlenen süre kadar kayıt yap
    for (int i = 0; i < durationSeconds * 16; ++i) {
        while (!(header.dwFlags & WHDR_DONE)) { Sleep(10); }
        outFile.write(buffer, bufferSize);
        waveInPrepareHeader(hWaveIn, &header, sizeof(WAVEHDR));
        waveInAddBuffer(hWaveIn, &header, sizeof(WAVEHDR));
    }

    waveInStop(hWaveIn);
    waveInUnprepareHeader(hWaveIn, &header, sizeof(WAVEHDR));
    waveInClose(hWaveIn);
    delete[] buffer;
    outFile.close();
}

// Kaydedilen sesi metne çeviren fonksiyon (Whisper API örneği)
std::string Perception::transcribeAudio(const std::string& filename) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string readBuffer;

    // API'ye gönderilecek dosya (Multipart form data)
    curl_mime* mime = curl_mime_init(curl);
    curl_mimepart* part = curl_mime_addpart(mime);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, filename.c_str());

    curl_mimepart* part2 = curl_mime_addpart(mime);
    curl_mime_name(part2, "model");
    curl_mime_data(part2, "whisper-1", CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/audio/transcriptions");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // API Anahtarın (Brain'deki gibi eklenmeli)
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    try {
        auto j = json::parse(readBuffer);
        return j["text"].get<std::string>();
    } catch (...) {
        return "";
    }
}

std::string Perception::listen() {
    // 1. Sessizlik kontrolü yap (Sadece ses varsa kaydet)
    if (!detectVoiceActivity()) return "";

    std::cout << "[Perception] Voice detected. Recording..." << std::endl;

    std::string tempFile = "input.raw";
    recordAudio(tempFile, 4); // 4 saniyelik kayıt al

    std::string text = transcribeAudio(tempFile);
    return text;
}

bool Perception::detectVoiceActivity() {
    // Basitlik adına burada true dönüyoruz.
    // Gerçek projede burada mikrofonun RMS değerleri hesaplanıp
    // bir eşik değerin (threshold) üstü kontrol edilir.
    return true;
}

bool Perception::isWakeWordDetected() {
    // Gelen metin içinde "Jarvis" geçiyor mu kontrolü
    // Bu fonksiyon Core tarafından çağrılacak.
    return false; // Gelen metne göre dinamikleşecek
}
