#ifndef APIManager_h
#define APIManager_h

#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// 📋 URL de base de l'API
const char* api_url = "https://iot-data-engineers-server.onrender.com/api/events"; 

// 📌 Prototypes des fonctions
void sendEvent(const char* equipmentEsp32Id, const char* houseId, const bool state, const char* value = nullptr, const char* unit = nullptr);
void collectAndSendData();

#endif
