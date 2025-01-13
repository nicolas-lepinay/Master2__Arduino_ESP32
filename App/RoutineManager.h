#ifndef RoutineManager_h
#define RoutineManager_h

#include <PubSubClient.h>

struct Routine {
  const char* name;
  const char* topics[10];
  const char* payloads[10];
  short int numActions;
};

extern Routine routines[];
extern const short int numRoutines;

void executeRoutine(const char* routineName, PubSubClient& client);

#endif
