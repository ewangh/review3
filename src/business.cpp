#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include "business.h"

int MarkDataPO::beginHour;
int MarkDataPO::beginMin;

bool getMarkCode(std::vector<DateData> vec1,const std::string& lastDay)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    std::sort(vec1.begin(), vec1.end(), [](DateData data1, DateData data2)
              { return std::mktime(&data1.Date) < std::mktime(&data2.Date); });

    int last_candle;
    std::tm last_day = convertToDate(lastDay);
    DateData* prevInfo{nullptr};

    for (DateData &obj : vec1)
    {
        if (prevInfo->empty())
        {
            prevInfo = &obj;
            continue;
        }

        obj.Closed = prevInfo->Price;
        
        if(std::mktime(&obj.Date) == std::mktime(&last_day))
        {
            last_candle = obj.getCandle();
        }

        prevInfo = &obj;
    }
    
    if(vec1.back().getCandle() == last_candle)
    {
        return true;
    }

    return false;
}

std::string setPreCode(const std::string &str)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    const std::string pre_sh = "sh";
    const std::string pre_sz = "sz";

    if (str.size() >= 8 && (str.substr(0, 2) == pre_sh || str.substr(0, 2) == pre_sz))
    {
        return str;
    }
    else if (str.size() >= 6 && (str.substr(0, 1) == "6"))
    {
        return pre_sh + str;
    }
    else
    {
        return pre_sz + str;
    }
}
