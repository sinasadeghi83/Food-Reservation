// include guard for Self.h
#ifndef SELF_H
#define SELF_H

#include <stdlib.h>
#include <stdbool.h>
#include "../sinastd/TimePeriod.h"
#include "../sinastd/Error.h"

typedef struct Self Self;
typedef enum SelfType SelfType;
typedef enum SelfMeal SelfMeal;

enum SelfType
{
    SelfType_Boyish,
    SelfType_Girlish,
    SelfType_Mixed,
    SelfType_INVALID
};

enum SelfMeal
{
    SelfMeal_Breakfast,
    SelfMeal_Lunch,
    SelfMeal_Dinner,
    SelfMeal_Both,
    SelfMeal_INVALID
};

struct Self
{
    int id;
    char *name;
    char *location;
    int capacity;
    SelfType type;
    SelfMeal meal;
    TimePeriod *lunchTime;
    TimePeriod *dinnerTime;
};

// self initialiser
void SelfIntialise(Self *this, int id, char *name, char *location, int capacity, SelfType type, SelfMeal meal, TimePeriod *lunchTime, TimePeriod *dinnerTime);
Self *SelfCreate(int id, char *name, char *location, int capacity, SelfType type, SelfMeal meal, TimePeriod *lunchTime, TimePeriod *dinnerTime);
void SelfFree(Self *this);
void SelfFreeAll(Self **meals);
void SelfFreeAllFrom(Self **meals, int from);
SelfType SelfTypeFromString(char *type);
char *SelfTypeToString(SelfType type);
SelfMeal SelfMealFromString(char *meal);
char *SelfMealToString(SelfMeal meal);
Error *SelfValidate(Self *this);
int SelfCallback(void *selves, int argc, char **argv, char **colName);
Self **SelfFind(const char *whereCols[], const char *whereVals[]);
Error *SelfSave(Self *this);

#endif