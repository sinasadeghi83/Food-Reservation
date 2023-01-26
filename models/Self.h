// include guard for Self.h
#ifndef SELF_H
#define SELF_H

#include <stdlib.h>

typedef struct Self Self;
typedef enum SelfType SelfType;
typedef enum SelfMeal SelfMeal;

void SelfCreate(char *name, char *location, int capacity, SelfType type, SelfMeal meal);
void SelfFree(Self *this);

#endif