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
            std::string token = string.substr(start, end - start);
            if(!token.empty() && token != delimeter)
                result.push_back(token);
            start = end + delimeter.length();
            end = string.find(delimeter, start);
        }
        
        std::string token = string.substr(start, end);
        if(!token.empty() && token != delimeter)
            result.push_back(token);
        return result;
    }
}
