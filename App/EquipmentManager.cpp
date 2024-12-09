#include "EquipmentManager.h"

// 🎯 Initialisation des équipements
Servo windowServo;
Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(HUMIDITY_SENSOR_PIN, DHT11);
Adafruit_NeoPixel pixels(4, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

Equipment equipmentList[] = {
    { LED_PIN, "SET/LED", setDigitalOutput },
    { FAN_PIN, "SET/FAN", setAnalogOutput },
    { BUZZER_PIN, "SET/BUZZER", setBuzzerState },
    { -1, "SET/WINDOW_SERVO", [](int, const char* payload) { setServoPosition(windowServo, payload); }},
    { -1, "SET/DOOR_SERVO", [](int, const char* payload) { setServoPosition(doorServo, payload); }},
    { -1, "SET/LCD_DISPLAY", setLCDContent },
    { -1, "SET/RGB_LED", setRGBColor },
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

    // Servos
    windowServo.attach(WINDOW_SERVO_PIN);
    doorServo.attach(DOOR_SERVO_PIN);

    // LCD Display
    lcd.init();
    lcd.backlight();

    // Temperature & Humidity sensors
    dht.begin();

    // RGB LED
    pixels.begin(); // Initialize NeoPixel strip object
    pixels.show(); // Turn off all pixels
    pixels.setBrightness(75); // Set brightness to 75 out of 255
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

void setRGBColor(int, const char* payload) {
    // Le payload doit être sous forme "R,G,B" (ex: "255,0,127")
    int r, g, b;
    if (sscanf(payload, "%d,%d,%d", &r, &g, &b) == 3) { // Parse le payload
        if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            for (int i = 0; i < pixels.numPixels(); i++) {
                pixels.setPixelColor(i, pixels.Color(r, g, b)); // Applique la couleur
            }
            pixels.show(); // Met à jour la LED RGB
            delay(50);
        } else {
            Serial.println("Valeurs RGB hors des limites (0-255) !");
        }
    } else {
        Serial.println("Format du payload RGB invalide ! Utiliser 'R,G,B'.");
    }
}
