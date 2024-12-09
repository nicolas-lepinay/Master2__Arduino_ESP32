#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_eap_client.h>
#include "Env.h"
#include "EquipmentManager.h"
#include "MQTTManager.h"

// 🔄 Timer pour les données périodiques
unsigned long lastUpdate = 0;
const unsigned long interval = 2000; // (2000 = 2 secondes, 60000 = 60 secondes)
const char* SSID = "WiFi@YNOV";

WiFiClientSecure secureClient;
PubSubClient client(secureClient);

// ⚙️ Setup
void setup() {
    Serial.begin(115200);
    setupWifi();
    secureClient.setInsecure();
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
        publishData(); // Publications des données des capteurs sur MQTT
    }
}

void setupWifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to network with client: ");
  esp_eap_client_set_identity((const unsigned char*)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_username((const unsigned char*)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_password((const unsigned char*)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wifi_sta_enterprise_enable();

  WiFi.begin(SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
