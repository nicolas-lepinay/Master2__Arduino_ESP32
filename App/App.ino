#include <WiFi.h>
#include "PinList.h"
#include "EquipmentManager.h"
#include "MQTTManager.h"
#include "APIManager.h"

// 📡 Configuration WiFi
const char* WIFI_SSID = "nicolas";
const char* WIFI_PASSWORD = "abcd1234";

// 🔄 Timer pour les données périodiques
unsigned long lastUpdate = 0;
const unsigned long interval = 60000; // 1 minute

// ⚙️ Setup
void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connexion au WiFi...");
        delay(1000);
    }
    Serial.println("Connecté au WiFi.");

    setupEquipments();
    setupMQTT();
}

// 🔄 Loop
void loop() {
    if (!client.connected()) {
        mqttReconnect();
    }
    client.loop();

    if (millis() - lastUpdate >= interval) {
        lastUpdate = millis();
        collectAndSendData(); // Appel pour envoyer des données à l'API
    }
}
