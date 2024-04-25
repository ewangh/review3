#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>
#include <regex>
#include "business.h"

#define D_PATH "../file/"
#define D_REGEX R"(\d{6})" //"\\d{6}"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || _WIN64
#define D_TXT_EXTENSION ".txt"
#else
#define D_TXT_EXTENSION
#endif

std::vector<std::string> getMatchingElements(const std::string &filePath, const std::string &regexPattern)
{
    std::vector<std::string> result;

    std::ifstream file(filePath);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return result;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // 读取文件内容到字符串
    std::regex pattern(regexPattern);
    std::smatch matches;

    while (std::regex_search(content, matches, pattern))
    {
        for (auto match : matches)
        {
            std::string element;
            std::istringstream(match) >> element;
            result.push_back(element);
        }
        content = matches.suffix().str(); // 继续匹配剩余的内容
    }

    file.close();
    return result;
}

std::vector<std::string> getQueue(const std::string &file)
{
    std::vector<std::string> tempq;

    std::string fileName = file;

    if (file.size() <= 4 || file.substr(file.length() - 4) != D_TXT_EXTENSION)
    {
        fileName += D_TXT_EXTENSION;
    }

    tempq = getMatchingElements(D_PATH + fileName, D_REGEX);

    return tempq;
}

void run(const std::string &code, const std::string &lastDay)
{
    std::time_t currentTime = std::time(nullptr);
    std::tm* currentTM = std::localtime(&currentTime);

    std::tm last_day = convertToDate(lastDay);
    std::time_t time1 = std::mktime(const_cast<std::tm*>(&last_day));
    std::time_t time2 = std::mktime(currentTM);

    double difference = std::difftime(time2, time1);
    int diffDay = static_cast<int>(difference / (60 * 60 * 24) + 3); // 转换为天数

    std::string url = R"(https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?_var=kline_dayqfq&param=)" + code + R"(,day,,,)" + std::to_string(diffDay) + R"(,qfq)";
    auto response = dataRequest(url);

    if (response.empty())
    {
        std::cout << "ERROR REQUEST!!!" << std::endl;
        return;
    }

    try
    {
        std::vector<DateData> list = dataRespond(response);

        if (list.empty())
        {
            std::cout << "HAS EMPTY QUEUE!!!" << std::endl;
            std::cout << url << std::endl;
            return;
        }

        std::cout << code << (getMarkCode(list, lastDay) ? " ✔" : " ✘") << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char *argv[])
{
    // for (int i = 0;i < argc;i ++)
    //     std::cout << argv[i] << std::endl;
    SetConsoleOutputCP(CP_UTF8);

    MarkDataPO::beginHour = 9;
    MarkDataPO::beginMin = 30;

    if (argc >= 2)
    {
        MarkDataPO::beginHour = std::stoi(argv[1]);
    }

    if (argc >= 3)
    {
        MarkDataPO::beginMin = std::stoi(argv[2]);
    }

    while (1)
    {
        std::string input;
        std::string i_day;

        std::cout << "File :";
        std::getline(std::cin, input);
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

        if (input.empty())
        {
            continue;
        }

        do
        {
            std::cout << "Last day:";
            std::getline(std::cin, i_day);
            i_day.erase(std::remove_if(i_day.begin(), i_day.end(), ::isspace), i_day.end());
        } while (!canConvertToDate(i_day));

        std::vector<std::string> queue = getQueue(input);

        for (std::string& obj : queue)
        {
            auto code = setPreCode(obj);
            run(code, i_day);
            // 阻塞5秒
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    return 0;
}
