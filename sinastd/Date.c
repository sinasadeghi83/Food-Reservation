#include "Date.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Parser.h"

// Implementation of Date.h
Date *CreateDate(int year, int month, int day)
{
    Date *this = malloc(sizeof(Date));
    initialiseDate(this, year, month, day);
    return this;
}

Date *CreateDateFromString(char *str)
{
    int year, month, day;
    sscanf(str, "%d-%d-%d", &year, &month, &day);
    if (year == 0)
    {
        sscanf(str, "%d/%d/%d", &year, &month, &day);
    }
    return CreateDate(year, month, day);
}

void DateFree(Date *this)
{
    free(this);
}

// Implementation of Date_internal.h
void initialiseDate(Date *this, int year, int month, int day)
{
    this->year = year;
    this->month = month;
    this->day = day;
}

// Returns Date *this as a string
char *DateToString(Date *this)
{
    char *result = malloc(11);
    sprintf(result, "%d-", this->year);
    if (this->month < 10)
    {
        sprintf(result, "%s0%d-", result, this->month);
    }
    else
    {
        sprintf(result, "%s%d-", result, this->month);
    }

    if (this->day < 10)
    {
        sprintf(result, "%s0%d", result, this->day);
    }
    else
    {
        sprintf(result, "%s%d", result, this->day);
    }
    return result;
}

// Checks if the given date is valid according to the Jalali calendar
bool DateIsValid(Date *this)
{
    if (this->year < 0 || this->month < 0 || this->day < 0)
    {
        return false;
    }
    if (this->month > 12)
    {
        return false;
    }
    if (this->day > 31)
    {
        return false;
    }
    // Check for leap year
    if (this->month == 12)
    {
        if (this->day > 30)
        {
            return false;
        }
        if (this->day == 30)
        {
            if (this->year % 4 != 3)
            {
                return false;
            }
        }
    }
    if (this->month > 6)
    {
        if (this->day > 30)
        {
            return false;
        }
    }
    return true;
}