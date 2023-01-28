// include guard
#ifndef MEAL_H
#define MEAL_H
#include "Self.h"
#include "../sinastd/Date.h"

typedef struct Meal
{
    int id;
    int selfId;
    int foodId;
    SelfMeal type;
    int count;
    Date *date;
} Meal;

Meal *MealCreate(int id, int selfId, int foodId, SelfMeal type, int count, Date *date);
void MealIntialise(Meal *this, int id, int selfId, int foodId, SelfMeal type, int count, Date *date);
void MealFree(Meal *this);
void MealFreeAll(Meal **meals);
void MealFreeAllFrom(Meal **meals, int from);

Meal **MealFind(const char *whereCols[], const char *whereVals[]);
Error *MealSave(Meal *this);
Error *MealValidate(Meal *this);

int MealCallback(void *data, int argc, char **argv, char **colName);

#endif