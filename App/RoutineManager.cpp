#include "RoutineManager.h"

Routine routines[] = {
    {
        "DAY_MODE", 
        {"SET/WINDOW", "SET/LED", "SET/FAN"}, 
        {"176", "HIGH", "176"}, 
        3,
    },
    {
        "NIGHT_MODE", 
        {"SET/WINDOW", "SET/LED", "SET/FAN"}, 
        {"0", "LOW", "0"}, 
        3,
    },
};

const int numRoutines = sizeof(routines) / sizeof(Routine);

void executeRoutine(const char* routineName, PubSubClient& client) {
  for (int i = 0; i < numRoutines; i++) {
    if (strcmp(routines[i].name, routineName) == 0) {
      for (int j = 0; j < routines[i].numActions; j++) {
        client.publish(routines[i].topics[j], routines[i].payloads[j]);
      }
      Serial.println(String("Routine ") + routineName + " exécutée.");
      break;
    }
  }
}
