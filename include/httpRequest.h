#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <ctime>
#include <algorithm>
#include "utils.h"
#include "curl/curl.h"

enum Candle
{
    R0,
    R1,
    R2,
    R3,
    R4,
    G0,
    G1,
    G2,
    G3,
    G4,
    F0,
    F1,
    F2,
    F3,
    F4
};

struct DateData
{
    double Opening;
    double Highest;
    double Lower;
    double Price;
    double Closed;
    long Volume;
    std::tm Date;

    bool isUp() const
    {
        return Price > Opening;
    }

    bool isFlat() const
    {
        return Price == Opening;
    }

    bool empty() const
    {
        return Date.tm_mday == 0;
    }

    Candle getCandle()
    {
        if (isUp())
        {
            if (Highest < Closed)
                return Candle::R0;
            if (Highest >= Closed && Price < Closed)
                return Candle::R1;
            if (Price >= Closed && Opening < Closed)
                return Candle::R2;
            if (Opening >= Closed && Lower < Closed)
                return Candle::R3;
            else
                return Candle::R4;
        }
        else if (isFlat())
        {
            if (Highest < Closed)
                return Candle::F0;
            if (Highest >= Closed && Price < Closed)
                return Candle::F1;
            if (Price == Closed)
                return Candle::F2;
            if (Price > Closed && Lower < Closed)
                return Candle::F3;
            else
                return Candle::F4;
        }
        else
        {
            if (Highest < Closed)
                return Candle::G0;
            if (Highest >= Closed && Opening < Closed)
                return Candle::G1;
            if (Opening >= Closed && Price < Closed)
                return Candle::G2;
            if (Price >= Closed && Lower < Closed)
                return Candle::G3;
            else
                return Candle::G4;
        }
    }
};

#pragma region 请求数据
std::string dataRequest(const std::string &);
#pragma endregion

#pragma region 处理数据
std::vector<DateData> dataRespond(const std::string &);
#pragma endregion
