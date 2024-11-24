#ifndef MQTTManager_h
#define MQTTManager_h

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "RoutineManager.h"
#include "EquipmentManager.h"

extern WiFiClientSecure secureClient;
extern PubSubClient client;

void setupMQTT();
void mqttReconnect();
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
