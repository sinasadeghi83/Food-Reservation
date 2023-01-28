#include "Self.h"
#include <stdlib.h>
#include <string.h>
#include "../sinastd/Error.h"
#include "../sinastd/TimePeriod.h"
#include "../database/DbManager.h"
#include "../sinastd/Parser.h"

#define SELF_MAX_FETCH 100
#define SELF_TABLE "self"

static int selvesIndex = 0;
const char *SELF_COLS[9] = {"id", "name", "location", "capacity", "type", "meal", "lunch_time", "dinner_time", NULL};

// Self Creator
Self *SelfCreate(int id, char *name, char *location, int capacity, SelfType type, SelfMeal meal, TimePeriod *lunchTime, TimePeriod *dinnerTime)
{
    Self *self = malloc(sizeof(Self));
    SelfIntialise(self, id, name, location, capacity, type, meal, lunchTime, dinnerTime);
    return self;
}

// Self initiliser
void SelfIntialise(Self *this, int id, char *name, char *location, int capacity, SelfType type, SelfMeal meal, TimePeriod *lunchTime, TimePeriod *dinnerTime)
{
    this->id = id;
    // Copying string variables to this struct
    this->name = malloc(strlen(name) + 1);
    strcpy(this->name, name);
    this->location = malloc(strlen(location) + 1);
    strcpy(this->location, location);
    this->capacity = capacity;
    this->type = type;
    this->meal = meal;
    this->lunchTime = malloc(sizeof(TimePeriod));
    memcpy(this->lunchTime, lunchTime, sizeof(TimePeriod));
    this->dinnerTime = malloc(sizeof(TimePeriod));
    memcpy(this->dinnerTime, dinnerTime, sizeof(TimePeriod));
}

// Self destructor
void SelfFree(Self *this)
{
    if (this == NULL)
        return;
    if (this->lunchTime != NULL)
        TimePeriodFree(this->lunchTime);
    if (this->dinnerTime != NULL)
        TimePeriodFree(this->dinnerTime);
    if (this->name != NULL)
        free(this->name);
    if (this->location != NULL)
        free(this->location);
    free(this);
}

// SelfType from string
SelfType SelfTypeFromString(char *type)
{
    if (strcmp(type, "boyish") == 0)
        return SelfType_Boyish;
    if (strcmp(type, "girlish") == 0)
        return SelfType_Girlish;
    if (strcmp(type, "mixed") == 0)
        return SelfType_Mixed;
    return SelfType_INVALID;
}

// SelfType to string
char *SelfTypeToString(SelfType type)
{
    switch (type)
    {
    case SelfType_Boyish:
        return "boyish";
    case SelfType_Girlish:
        return "girlish";
    case SelfType_Mixed:
        return "mixed";
    default:
        return ERR_INVALID;
    }
}

// SelfMeal from string
SelfMeal SelfMealFromString(char *meal)
{
    // if (strcmp(meal, "breakfast") == 0)
    //     return SelfMeal_Breakfast;
    if (strcmp(meal, "lunch") == 0)
        return SelfMeal_Lunch;
    if (strcmp(meal, "dinner") == 0)
        return SelfMeal_Dinner;
    if (strcmp(meal, "both") == 0)
        return SelfMeal_Both;
    return SelfMeal_INVALID;
}

// SelfMeal to string
char *SelfMealToString(SelfMeal meal)
{
    switch (meal)
    {
    // case SelfMeal_Breakfast:
    //     return "breakfast";
    case SelfMeal_Lunch:
        return "lunch";
    case SelfMeal_Dinner:
        return "dinner";
    case SelfMeal_Both:
        return "both";
    default:
        return ERR_INVALID;
    }
}

// Self FreeAll
void SelfFreeAll(Self **selves)
{
    if (selves == NULL)
        return;
    for (int i = 0; selves[i] != NULL; i++)
    {
        SelfFree(selves[i]);
    }
    free(selves);
}

// Self Free all from
void SelfFreeAllFrom(Self **selves, int from)
{
    if (selves == NULL)
        return;
    for (int i = 0; selves[i] != NULL; i++)
    {
        if (i < from)
        {
            continue;
        }
        SelfFree(selves[i]);
    }
    free(selves);
}

// Self validator
Error *SelfValidate(Self *this)
{
    // Create Error
    Error *error = ErrorCreate(false, NULL, NULL);
    // Check if id is less than or equal to 0
    if (this->id <= 0)
    {
        error->isAny = true;
        error->msg = "Id is invalid";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if name is empty
    if (this->name == NULL || strlen(this->name) == 0)
    {
        error->isAny = true;
        error->msg = "Name cannot be empty";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // // Check if name isn't alphanumeric
    // if (!isAlphaNumeric(this->name))
    // {
    //     error->isAny = true;
    //     error->msg = "Name should be alphanumeric";
    //     error->testMsg = ERR_INVALID;
    //     return error;
    // }
    // Check if location is empty
    if (this->location == NULL || strlen(this->location) == 0)
    {
        error->isAny = true;
        error->msg = "Location cannot be empty";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // // Check if location isn't alphanumeric
    // if (!isAlphaNumeric(this->location))
    // {
    //     error->isAny = true;
    //     error->msg = "Location should be alphanumeric";
    //     error->testMsg = ERR_INVALID;
    //     return error;
    // }
    // Check if capacity is less than or equal to 0
    if (this->capacity <= 0)
    {
        error->isAny = true;
        error->msg = "Capacity cannot be less than 0";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if type is invalid
    if (this->type == SelfType_INVALID)
    {
        error->isAny = true;
        error->msg = "Type cannot be invalid";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if meal is invalid
    if (this->meal == SelfMeal_INVALID)
    {
        error->isAny = true;
        error->msg = "Meal cannot be invalid";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if lunchTime is invalid
    if (this->lunchTime != NULL && !TimePeriodValidate(this->lunchTime))
    {
        error->isAny = true;
        error->msg = "LunchTime cannot be invalid";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if dinnerTime is invalid
    if (this->dinnerTime != NULL && !TimePeriodValidate(this->dinnerTime))
    {
        error->isAny = true;
        error->msg = "DinnerTime cannot be invalid";
        error->testMsg = ERR_INVALID;
        return error;
    }
    // Check if both lunchTime and dinnerTime are NULL
    if (this->lunchTime == NULL && this->dinnerTime == NULL)
    {
        error->isAny = true;
        error->msg = "LunchTime and DinnerTime cannot both be NULL";
        error->testMsg = ERR_INVALID;
        return error;
    }
    return error;
}

// Self Callback to store selves from database
int SelfCallback(void *data, int argc, char **argv, char **colName)
{
    // Cast selves to Self **
    Self **selves = (Self **)data;
    // Check if selves is full
    if (selves[SELF_MAX_FETCH - 1] != NULL)
        return 0;
    // Create Self
    Self *self = SelfCreate(atoi(argv[0]), argv[1], argv[2], atoi(argv[3]), SelfTypeFromString(argv[4]), SelfMealFromString(argv[5]), TimePeriodFromString(argv[6]), TimePeriodFromString(argv[7]));
    // Store Self in selves
    selves[selvesIndex++] = self;
    selves[selvesIndex] = NULL;
    return 0;
}

// Find Selves from database using DbManager
Self **SelfFind(const char *whereCols[], const char *whereVals[])
{
    Self **selves = calloc(SELF_MAX_FETCH, sizeof(Self *));
    selves[0] = NULL;
    bool status = DbSelect(SELF_TABLE, whereCols, whereVals, SelfCallback, (void *)selves);
    selvesIndex = 0;
    // Free selves if status is false
    if (!status || selves[0] == NULL)
    {
        free(selves);
        return NULL;
    }
    return selves;
}

// Save new Self to database using DbManager
Error *SelfSave(Self *this)
{
    // Check if there is an existing Self with same id
    char *idStr = parseInt(this->id);
    Self **selves = SelfFind((const char *[]){"id", NULL}, (const char *[]){idStr, NULL});
    if (selves != NULL)
    {
        Error *error = ErrorCreate(true, "Self with same id already exists", ERR_INVALID);
        free(idStr);
        SelfFreeAll(selves);
        return error;
    }
    free(idStr);
    // Validate Self
    Error *error = SelfValidate(this);
    if (error->isAny)
        return error;
    // Convert TimePeriods to string
    char *lunchTime = TimePeriodToString(this->lunchTime);
    char *dinnerTime = TimePeriodToString(this->dinnerTime);
    // Convert capacity to string
    char *capacity = parseInt(this->capacity);
    // Create columns and values
    const char *vals[9] = {
        parseInt(this->id), this->name, this->location, capacity, SelfTypeToString(this->type), SelfMealToString(this->meal), lunchTime, dinnerTime, NULL};
    // Insert Self to database
    bool status = DbInsert(SELF_TABLE, SELF_COLS, vals);
    // Free lunchTime and dinnerTime if they are not NULL
    if (lunchTime != NULL)
        free(lunchTime);
    if (dinnerTime != NULL)
        free(dinnerTime);
    if (capacity != NULL)
        free(capacity);
    // Check if insert is successful
    if (!status)
    {
        error->isAny = true;
        error->msg = "Failed to save Self";
        error->testMsg = ERR_INVALID;
    }
    return error;
}