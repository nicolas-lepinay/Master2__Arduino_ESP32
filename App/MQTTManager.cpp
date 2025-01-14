#include <ArduinoJson.h>
#include "MQTTManager.h"
#include "EquipmentManager.h" // Inclure pour accéder à 'dht'

// 📋 Configuration serveur MQTT
const char* mqtt_host = "b5f2bf8397624117be2142697084afa1.s1.eu.hivemq.cloud"; // Adresse du broker MQTT
const int mqtt_port = 8883; // Port sécurisé
const char* mqtt_username = "nicolas";
const char* mqtt_password = "Abcd1234";

// 🔧 Configuration MQTT
void setupMQTT() {
    secureClient.setInsecure(); // Désactiver la vérification des certificats pour simplifier
    client.setServer(mqtt_host, mqtt_port);
    client.setCallback(mqttCallback);
}

// 🔄 Reconnexion MQTT
void mqttReconnect() {
    while (!client.connected()) {
        Serial.println(F("Connexion au serveur MQTT..."));
        if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
            Serial.println(F("Connecté au serveur MQTT sécurisé !"));
            // Abonnements aux topics des équipements
            for (short int i = 0; i < numEquipments; i++) {
                client.subscribe(equipmentList[i].topic);
            }
            // Abonnement au topic des routines
            client.subscribe("TRIGGER/ROUTINE");
        } else {
            Serial.print(F("Échec de connexion, code : "));
            Serial.println(client.state());
            delay(5000); // Réessayer après 5 secondes
        }
    }
}

// 📩 Callback MQTT (traitement des messages reçus)
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0'; // Null-terminate le message pour traitement

    // 📋 Gestion des routines
    if (strcmp(topic, "TRIGGER/ROUTINE") == 0) {
        executeRoutine(message, client);
        return;
    }

    // 📋 Gestion des équipements individuels
    for (short int i = 0; i < numEquipments; i++) {
        if (strcmp(topic, equipmentList[i].topic) == 0) {
            equipmentList[i].action(equipmentList[i].pin, message);
            // Envoi de confirmation via MQTT
            char confirmMessage[100];
            snprintf(confirmMessage, sizeof(confirmMessage), "Equipment %s set with payload: %s", equipmentList[i].topic, message);
            client.publish("CONFIRM", confirmMessage);
            break;
        }
    }
}

// 📌 Publication d'un événement sur MQTT
void publishEvent(const char* equipmentEsp32Id, const char* houseId, bool state, const char* value, const char* unit) {
    // Préparation du payload JSON
    StaticJsonDocument<128> doc; // Use StaticJsonDocument
    doc["equipmentEsp32Id"] = equipmentEsp32Id;
    doc["houseId"] = houseId;
    doc["state"] = state;
    doc["value"] = value;
    doc["unit"] = unit;

    char payload[128];
    serializeJson(doc, payload);

    const char* topic = "ADD/EVENT";
    if (client.publish(topic, payload)) {
        Serial.println(F("Événement publié avec succès sur MQTT !"));
    } else {
        Serial.println(F("Échec de la publication MQTT !"));
    }
}

// 📌 Collecte et publication des données des capteurs
void publishData() {
    const char houseId[] PROGMEM = "KEVIN";

    // Capteur d'humidité
    int humidityValue = dht.readHumidity() * 100;
    char humidityStr[10];
    snprintf(humidityStr, sizeof(humidityStr), "%d", humidityValue / 100);
    publishEvent("HUMIDITY_SENSOR", houseId, true, humidityStr, "%");

    // Capteur de température
    int temperatureValue = dht.readTemperature() * 100;
    char temperatureStr[10];
    snprintf(temperatureStr, sizeof(temperatureStr), "%d", temperatureValue / 100);
    publishEvent("TEMP_SENSOR", houseId, true, temperatureStr, "°C");

    // Capteur de gaz
    bool gasDetected = digitalRead(GAS_SENSOR_PIN);
    publishEvent("GAS_SENSOR", houseId, gasDetected, gasDetected ? "DETECTED" : "NOT_DETECTED", nullptr);

    // Détecteur de fumée
    bool steamDetected = digitalRead(STEAM_SENSOR_PIN);
    publishEvent("STEAM_SENSOR", houseId, steamDetected, steamDetected ? "DETECTED" : "NOT_DETECTED", nullptr);

    // Détecteur de mouvements
    bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
    publishEvent("MOTION_SENSOR", houseId, motionDetected, motionDetected ? "DETECTED" : "NOT_DETECTED", nullptr);
}