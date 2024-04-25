#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include "utils.h"
#include "httpRequest.h"

#define D_LASTS 19800 //9.30-15.00
#define D_MAXTS 14400 //9.30-11.30 13.00-15.00
#define D_PMTS 12600 //9.30-13.00
#define D_STOPTS 5400 //11.30-13.00
#define D_AMTS 7200 //9.30-11.30

struct MarkDataPO
{
    static int beginHour;
    static int beginMin;
};

bool getMarkCode(std::vector<DateData>,const std::string &);

std::string setPreCode(const std::string &);