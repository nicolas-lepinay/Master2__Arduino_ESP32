#ifndef EquipmentManager_h
#define EquipmentManager_h

#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "PinList.h"

// 📋 Structure pour les équipements
struct Equipment {
    int pin; // Numéro du pin
    const char* topic; // Topic MQTT associé
    void (*action)(int, const char*); // Fonction de gestion
};

// 📋 Déclaration des équipements
extern Equipment equipmentList[];
extern const int numEquipments;

// 📋 Prototypes des fonctions
void setupEquipments();
void setDigitalOutput(int pin, const char* payload);
void setAnalogOutput(int pin, const char* payload);
void setServoPosition(Servo& servo, const char* payload);
void setLCDContent(int, const char* payload);
const char* getLCDContent();
void setBuzzerState(int pin, const char* payload);

#endif
