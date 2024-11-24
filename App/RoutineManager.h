#ifndef RoutineManager_h
#define RoutineManager_h

#include <PubSubClient.h>

struct Routine {
  const char* name;
  const char* topics[10];
  const char* payloads[10];
  int numActions;
};

extern Routine routines[];
extern const int numRoutines;

void executeRoutine(const char* routineName, PubSubClient& client);

#endif
