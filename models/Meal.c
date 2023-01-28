#include "Meal.h"
#include "Self.h"
#include "Food.h"
#include "../sinastd/Parser.h"
#include "../sinastd/Error.h"
#include "../sinastd/Date.h"
#include "../database/DbManager.h"
#include <stdlib.h>
#include <string.h>

#define MEAL_MAX_FETCH 100
#define MEAL_TABLE "meal"

const char *MEAL_COLS[7] = {"id", "self_id", "food_id", "type", "count", "date", NULL};

static int mealsIndex = 0;

// Meal Creator
Meal *MealCreate(int id, int selfId, int foodId, SelfMeal type, int count, Date *date)
{
    Meal *this = malloc(sizeof(Meal));
    MealIntialise(this, id, selfId, foodId, type, count, date);
    return this;
}

void MealIntialise(Meal *this, int id, int selfId, int foodId, SelfMeal type, int count, Date *date)
{
    this->id = id;
    this->selfId = selfId;
    this->foodId = foodId;
    this->type = type;
    this->count = count;
    this->date = malloc(sizeof(Date));
    memcpy(this->date, date, sizeof(Date));
}

void MealFree(Meal *this)
{
    DateFree(this->date);
    free(this);
}

void MealFreeAll(Meal **meals)
{
    MealFreeAllFrom(meals, 0);
}

void MealFreeAllFrom(Meal **meals, int from)
{
    for (int i = from; meals[i] != NULL; i++)
    {
        MealFree(meals[i]);
    }
    free(meals);
}

// Meal Validator
Error *MealValidate(Meal *this)
{
    if (this->selfId <= 0)
    {
        return ErrorCreate(true, "Self ID must be greater than 0", ERR_INVALID);
    }
    char *selfIdStr = parseInt(this->selfId);
    Self **selves = SelfFind((const char *[]){"id", NULL}, (const char *[]){selfIdStr, NULL});
    free(selfIdStr);
    if (selves == NULL)
    {
        return ErrorCreate(true, "Self ID not found", ERR_404);
    }
    else
    {
        SelfFreeAll(selves);
    }
    if (this->foodId <= 0)
    {
        return ErrorCreate(true, "Food ID must be greater than 0", ERR_INVALID);
    }
    if (this->type == SelfMeal_INVALID)
    {
        return ErrorCreate(true, "Type is invalid", ERR_INVALID);
    }
    if (this->count <= 0)
    {
        return ErrorCreate(true, "Meal count must be greater than 0", ERR_INVALID);
    }
    if (this->date == NULL)
    {
        return ErrorCreate(true, "Date must not be empty", ERR_INVALID);
    }
    if (DateIsValid(this->date) == false)
    {
        return ErrorCreate(true, "Date is invalid", ERR_INVALID);
    }
    return ErrorCreate(false, NULL, NULL);
}

// Meal Callback
int MealCallback(void *data, int argc, char **argv, char **colName)
{
    Meal **meals = (Meal **)data;
    int id = atoi(argv[0]);
    int selfId = atoi(argv[1]);
    int foodId = atoi(argv[2]);
    SelfMeal type = SelfMealFromString(argv[3]);
    int count = atoi(argv[4]);
    Date *date = CreateDateFromString(argv[5]);
    meals[mealsIndex++] = MealCreate(id, selfId, foodId, type, count, date);
    meals[mealsIndex] = NULL;
    return 0;
}

// Meal Find
Meal **MealFind(const char *whereCols[], const char *whereValues[])
{
    // Create a new array of Meals
    Meal **meals = calloc(MEAL_MAX_FETCH, sizeof(Meal *));
    meals[0] = NULL;
    bool status = DbSelect(MEAL_TABLE, whereCols, whereValues, MealCallback, (void *)meals);
    mealsIndex = 0;
    if (!status || meals[0] == NULL)
    {
        free(meals);
        return NULL;
    }
    return meals;
}

// Meal Save
Error *MealSave(Meal *this)
{
    Error *err = MealValidate(this);
    if (err->isAny)
    {
        return err;
    }
    char *idStr = parseInt(this->id);
    char *selfIdStr = parseInt(this->selfId);
    char *foodIdStr = parseInt(this->foodId);
    char *typeStr = SelfMealToString(this->type);
    char *countStr = parseInt(this->count);
    char *dateStr = DateToString(this->date);
    const char *vals[6] = {selfIdStr, foodIdStr, typeStr, countStr, dateStr, NULL};
    bool status = false;
    if (this->id <= 0)
    {
        status = DbInsert(MEAL_TABLE, MEAL_COLS + 1, vals);
    }
    else
    {
        const char *whereCols[2] = {"id", NULL};
        const char *whereVals[2] = {idStr, NULL};
        status = DbUpdate(MEAL_TABLE, MEAL_COLS + 1, vals, whereCols, whereVals);
    }
    free(idStr);
    free(selfIdStr);
    free(foodIdStr);
    free(countStr);
    free(dateStr);
    if (!status)
    {
        return ErrorCreate(true, "Failed to save meal", ERR_INVALID);
    }
    return ErrorCreate(false, NULL, NULL);
}