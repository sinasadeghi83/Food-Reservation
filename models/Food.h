// include guard for Food.h
#ifndef FOOD_H
#define FOOD_H

#include "../sinastd/Error.h"

typedef struct Food Food;
typedef enum FoodType FoodType;

enum FoodType
{
    FOOD_TYPE_FOOD,
    FOOD_TYPE_DESSERT,
    FOOD_TYPE_INVALID
};

struct Food
{
    int id;
    char *name;
    FoodType type;
    int price;
};

Food *FoodCreate(int id, char *name, FoodType type, int price);
void FoodIntialise(Food *this, int id, char *name, FoodType type, int price);
void FoodFree(Food *this);
void FoodFreeAll(Food **foods);
void FoodFreeAllFrom(Food **foods, int from);
FoodType FoodTypeFromString(char *type);
char *FoodTypeToString(FoodType type);

Food **FoodFind(const char *whereCols[], const char *whereVals[]);
Error *FoodSave(Food *this);
Error *FoodValidate(Food *this);

int FoodCallback(void *data, int argc, char **argv, char **colName);

#endif