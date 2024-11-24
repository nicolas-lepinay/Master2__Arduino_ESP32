#include "APIManager.h"
#include "EquipmentManager.h"
#include <ArduinoJson.h>

// 📌 Fonction pour envoyer un événement à l'API
void sendEvent(const char* equipmentEsp32Id, const char* houseId, const bool state, const char* value, const char* unit) {
    WiFiClientSecure httpsClient;
    httpsClient.setInsecure(); // Désactiver la vérification du certificat pour simplifier

    HTTPClient http;
    http.begin(httpsClient, api_url);
    http.addHeader("Content-Type", "application/json");

    // Préparation du payload JSON
    DynamicJsonDocument doc(256);
    doc["equipmentEsp32Id"] = equipmentEsp32Id;
    doc["houseId"] = houseId;
    doc["state"] = state;
    doc["value"] = value;
    doc["unit"] = unit;

    String payload;
    serializeJson(doc, payload);

    // Envoi de la requête POST
    int httpResponseCode = http.POST(payload);

    // Gestion de la réponse
    if (httpResponseCode > 0) {
        Serial.println("Événement envoyé avec succès !");
        Serial.println("Réponse de l'API : " + http.getString());
    } else {
        Serial.println("Échec de l'envoi de l'événement.");
        Serial.println("Code d'erreur HTTP : " + String(httpResponseCode));
    }
    http.end(); // Libération des ressources
}

// 📌 Routine pour collecter et envoyer des données périodiques
void collectAndSendData() {
    const char* houseId = "KEVIN";

    /*
    // LED
    bool ledState = digitalRead(LED_PIN);
    sendEvent("LED", houseId, ledState, ledState ? "HIGH" : "LOW", nullptr);

    // Ventilateur
    int fanValue = analogRead(FAN_PIN);
    sendEvent("FAN", houseId, fanValue > 0, String(fanValue).c_str(), "%");

    // Servos
    int windowValue = windowServo.read();
    sendEvent("WINDOW_SERVO", houseId, windowValue > 0, String(windowValue).c_str(), "degrés");
    int doorValue = doorServo.read();
    sendEvent("DOOR_SERVO", houseId, doorValue > 0, String(doorValue).c_str(), "degrés");

    // LCD
    const char* lcdContent = getLCDContent();
    sendEvent("LCD", houseId, strlen(lcdContent) > 0, lcdContent, nullptr);

    // Buzzer
    sendEvent("BUZZER", houseId, currentBuzzerFrequency > 0, String(currentBuzzerFrequency).c_str(), "Hz");
    */

    // Capteur d'humidité
    int humidityValue = analogRead(HUMIDITY_SENSOR_PIN);
    sendEvent("HUMIDITY_SENSOR", houseId, true, String(humidityValue).c_str(), "%");

    // Capteur de température
    int temperatureValue = analogRead(TEMP_SENSOR_PIN);
    sendEvent("TEMP_SENSOR", houseId, true, String(temperatureValue).c_str(), "°C");

    // Capteur de gaz
    bool gasDetected = digitalRead(GAS_SENSOR_PIN);
    sendEvent("GAS_SENSOR", houseId, gasDetected, gasDetected ? "DETECTED" : "NOT_DETECTED", nullptr);

    // Détecteur de fumée
    bool smokeDetected = digitalRead(SMOKE_SENSOR_PIN);
    sendEvent("SMOKE_SENSOR", houseId, smokeDetected, smokeDetected ? "DETECTED" : "NOT_DETECTED", nullptr);

    // Détecteur de mouvements
    bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
    sendEvent("MOTION_SENSOR", houseId, motionDetected, motionDetected ? "DETECTED" : "NOT_DETECTED", nullptr);
}






