// include guard for Self_internal
#ifndef SELF_INTERNAL_H
#define SELF_INTERNAL_H

#include "Self.h"

enum SelfType
{
    SelfType_Boyish,
    SelfType_Girlish,
};

enum SelfMeal
{
    SelfMeal_Breakfast,
    SelfMeal_Lunch,
    SelfMeal_Dinner,
}

struct Self
{
    char *name;
    char *location;
    int capacity;
    SelfType type;
    SelfMeal meal;
};

// self initialiser
void SelfIntialise(Self *this, char *name, char *location, int capacity, SelfType type, SelfMeal meal);
#endif