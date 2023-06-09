#include "fepch.h"
#include "StringUitls.h"

namespace FusionEngine
{
    std::vector<std::string> SplitString(const std::string& string, const std::string& delimeter)
    {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = string.find(delimeter);
        while (end != std::string::npos)
        {
            if(string[start] != delimeter[0])
                result.push_back(string.substr(start, end - start));
            start = end + delimeter.length();
            end = string.find(delimeter, start);
        }
        result.push_back(string.substr(start, end));
        return result;
    }
}
