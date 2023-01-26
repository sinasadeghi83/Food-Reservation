#include "Self_internal.h"

// Self Creator
Self *SelfCreate(char *name, char *location, int capacity, SelfType type, SelfMeal meal)
{
    Self *self = malloc(sizeof(Self));
    SelfIntialise(self, name, location, capacity, type, meal);
    return self;
}

// Self initiliser
void SelfIntialise(Self *this, char *name, char *location, int capacity, SelfType type, SelfMeal meal)
{
    // Copying string variables to this struct
    this->name = malloc(strlen(name) + 1);
    strcpy(this->name, name);
    this->location = malloc(strlen(location) + 1);
    strcpy(this->location, location);
    this->capacity = capacity;
    this->type = type;
    this->meal = meal;
}

void SelfFree(Self *this)
{
    free(this->name);
    free(this->location);
    free(this);
}