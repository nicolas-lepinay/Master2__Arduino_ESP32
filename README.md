# Master2__Arduino_ESP32

Ce projet utilise une carte ESP32 pour gérer divers équipements via MQTT. Il inclut la gestion des capteurs, des actionneurs et des routines prédéfinies.

## Structure du projet

```
App/
├── .idea/
│   ├── .gitignore
│   ├── App.iml
│   ├── editor.xml
│   ├── modules.xml
│   ├── vcs.xml
├── App.ino
├── Env.h
├── EquipmentManager.cpp
├── EquipmentManager.h
├── MQTTManager.cpp
├── MQTTManager.h
├── PinList.h
├── RoutineManager.cpp
├── RoutineManager.h
├── README.md
```

## Fichiers principaux

- **App.ino** : Fichier principal de l'application Arduino.
- **Env.h** : Fichier d'environnement contenant les identifiants et mots de passe.
- **EquipmentManager.cpp/h** : Gestion des équipements (capteurs et actionneurs).
- **MQTTManager.cpp/h** : Gestion de la connexion et des communications MQTT.
- **RoutineManager.cpp/h** : Gestion des routines prédéfinies.
- **PinList.h** : Définition des pins utilisés par les équipements.

## Fonctionnalités

### Connexion WiFi

La fonction `setupWifi()` dans [App.ino](App/App.ino) configure la connexion WiFi en utilisant les identifiants définis dans `Env.h`.

### Gestion des équipements

Les équipements sont gérés dans [EquipmentManager.cpp](App/EquipmentManager.cpp). Les fonctions principales incluent :
- `setDigitalOutput()`
- `setAnalogOutput()`
- `setServoPosition()`
- `setLCDContent()`
- `setBuzzerState()`
- `setRGBColor()`

### Communication MQTT

La communication MQTT est gérée dans [MQTTManager.cpp](App/MQTTManager.cpp). Les fonctions principales incluent :
- `setupMQTT()`
- `mqttReconnect()`
- `mqttCallback()`
- `publishEvent()`
- `publishData()`

### Gestion des routines

Les routines prédéfinies sont gérées dans [RoutineManager.cpp](App/RoutineManager.cpp). La fonction principale est `executeRoutine()`.

## Configuration

Assurez-vous de configurer les identifiants WiFi et MQTT dans `Env.h` avant de téléverser le code sur l'ESP32.

## Installation

1. Clonez le dépôt.
2. Ouvrez le projet dans l'IDE Arduino.
3. Téléversez le code sur votre carte ESP32.

## Utilisation

Une fois le code téléversé, l'ESP32 se connectera au WiFi et au serveur MQTT. Vous pouvez alors envoyer des commandes MQTT pour contrôler les équipements et exécuter des routines.

## Auteurs

- Nicolas
- Kevin
- Quentin

## Licence

Ce projet est sous licence MIT.