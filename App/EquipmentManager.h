#ifndef EquipmentManager_h
#define EquipmentManager_h

#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include "PinList.h"
#include "RoutineManager.h"

// 📋 Structure pour les équipements
struct Equipment {
    byte pin; // Numéro du pin
    const char* topic; // Topic MQTT associé
    void (*action)(short int, const char*); // Fonction de gestion
};

// 📋 Déclaration des équipements (variables globales)
extern Equipment equipmentList[];
extern const short int numEquipments;
extern Servo windowServo;               
extern Servo doorServo;
extern LiquidCrystal_I2C lcd;
extern DHT dht;
extern Adafruit_NeoPixel pixels;

// 📋 Prototypes des fonctions
void setupEquipments();
void setDigitalOutput(short int pin, const char* payload);
void setAnalogOutput(short int pin, const char* payload);
void setServoPosition(Servo& servo, const char* payload);
void setLCDContent(short int, const char* payload);
const char* getLCDContent();
void setBuzzerState(short int pin, const char* payload);
void setRGBColor(short int, const char* payload);

#endif
