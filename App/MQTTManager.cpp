#include "MQTTManager.h"

// 📡 Configuration MQTT sécurisé
WiFiClientSecure secureClient;
PubSubClient client(secureClient);

// 📋 Configuration serveur MQTT
const char* mqtt_host = "b5f2bf8397624117be2142697084afa1.s1.eu.hivemq.cloud"; // Adresse du broker MQTT
const int mqtt_port = 8883; // Port sécurisé
const char* mqtt_username = "nicolas"; // Nom d'utilisateur
const char* mqtt_password = "abcd1234"; // Mot de passe

// 🔧 Configuration MQTT
void setupMQTT() {
    secureClient.setInsecure(); // Désactiver la vérification des certificats pour simplifier
    client.setServer(mqtt_host, mqtt_port);
    client.setCallback(mqttCallback);
}

// 🔄 Reconnexion MQTT
void mqttReconnect() {
    while (!client.connected()) {
        Serial.println("Connexion au serveur MQTT...");
        if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
            Serial.println("Connecté au serveur MQTT sécurisé !");
            // Abonnements aux topics des équipements
            for (int i = 0; i < numEquipments; i++) {
                client.subscribe(equipmentList[i].topic);
            }
            // Abonnement au topic des routines
            client.subscribe("TRIGGER/ROUTINE");
        } else {
            Serial.print("Échec de connexion, code : ");
            Serial.println(client.state());
            delay(5000); // Réessayer après 5 secondes
        }
    }
}

// 📩 Callback MQTT
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
    for (int i = 0; i < numEquipments; i++) {
        if (strcmp(topic, equipmentList[i].topic) == 0) {
        equipmentList[i].action(equipmentList[i].pin, message);
        // Envoi de confirmation via MQTT
        String confirmMessage = String("Equipment ") + equipmentList[i].topic + " set with payload: " + message;
        client.publish("CONFIRM", confirmMessage.c_str());
        break;
        }
    }
}
