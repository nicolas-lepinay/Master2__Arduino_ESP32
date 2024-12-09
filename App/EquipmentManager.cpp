#include "EquipmentManager.h"

// 🎯 Initialisation des équipements
Servo windowServo;
Servo doorServo;
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

Equipment equipmentList[] = {
    { LED_PIN, "SET/LED", setDigitalOutput },
    { FAN_PIN, "SET/FAN", setAnalogOutput },
    { BUZZER_PIN, "SET/BUZZER", setBuzzerState },
    { -1, "SET/WINDOW", [](int, const char* payload) { setServoPosition(windowServo, payload); }},
    { -1, "SET/DOOR", [](int, const char* payload) { setServoPosition(doorServo, payload); }},
    { -1, "SET/LCD", setLCDContent },
};

const int numEquipments = sizeof(equipmentList) / sizeof(Equipment);
int currentBuzzerFrequency = 0; // Stocke la fréquence du buzzer
char lcdCurrentContent[33] = ""; // Stocke le texte actuel du LCD (16 colonnes x 2 lignes max)

void setupEquipments() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(MOTION_SENSOR_PIN, OUTPUT);
    pinMode(LEFT_BTN_PIN, OUTPUT);
    pinMode(HUMIDITY_SENSOR_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(GAS_SENSOR_PIN, OUTPUT);  
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RGB_LED_PIN, OUTPUT);
    pinMode(RIGHT_BTN_PIN, OUTPUT);
    pinMode(SMOKE_SENSOR_PIN, OUTPUT);

    windowServo.attach(WINDOW_SERVO_PIN);
    doorServo.attach(DOOR_SERVO_PIN);

    lcd.init();
    lcd.backlight();
}

// 🎯 Gestion des équipements numériques (digitalWrite)
void setDigitalOutput(int pin, const char* payload) {
    if (strcmp(payload, "HIGH") == 0) {
        digitalWrite(pin, HIGH); // Active le pin
    } else if (strcmp(payload, "LOW") == 0) {
        digitalWrite(pin, LOW); // Désactive le pin
    }
}

// 🎯 Gestion des équipements analogiques (analogWrite)
void setAnalogOutput(int pin, const char* payload) {
    int value = atoi(payload); // Convertit le payload en entier
    if (value >= 0 && value <= 255) {
        analogWrite(pin, value); // Définit la sortie PWM
    } else {
        Serial.println("Valeur analogique invalide !");
    }
}

// 🎯 Gestion des servos (position)
void setServoPosition(Servo& servo, const char* payload) {
    int angle = atoi(payload); // Convertit le payload en angle
    if (angle >= 0 && angle <= 180) {
        servo.write(angle); // Définit la position du servo
    } else {
        Serial.println("Angle de servo invalide !");
    }
}

// 🎯 Gestion du LCD
void setLCDContent(int, const char* payload) {
    if (payload == nullptr || strcmp(payload, "") == 0) {
        lcd.clear(); // Efface l'écran si le payload est vide ou null
        strcpy(lcdCurrentContent, ""); // Réinitialise le contenu local
    } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(payload); // Affiche le texte sur le LCD
        // Met à jour le contenu actuel
        strncpy(lcdCurrentContent, payload, sizeof(lcdCurrentContent) - 1);
        lcdCurrentContent[sizeof(lcdCurrentContent) - 1] = '\0'; // Sécurise la chaîne
    }
}

// 📋 Lecture du contenu actuel du LCD
const char* getLCDContent() {
    return lcdCurrentContent;
}

// 🎯 Gestion du buzzer
void setBuzzerState(int pin, const char* payload) {
    int frequency = atoi(payload); // Convertit le payload en fréquence
    if (frequency > 0) {
        currentBuzzerFrequency = frequency;
        tone(pin, frequency); // Active le buzzer avec la fréquence donnée
    } else {
        currentBuzzerFrequency = 0;
        noTone(pin); // Désactive le buzzer
    }
}
