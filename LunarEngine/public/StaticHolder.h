#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace LunarUtils {
    class StaticHolder {
    private:
        static StaticHolder* instance;

        StaticHolder();

    public:
        static StaticHolder* getInstance();

        void SetValue(std::string id, float inital_value);
        void SetValue(std::string id, int inital_value);
        void SetValue(std::string id, std::string inital_value);

        float GetValue(std::string id);
        int GetValue(std::string id, int dVal = 0);
        std::string GetValue(std::string id, bool str = true);

        std::vector<std::tuple<std::string, float>> staticFloats;
        std::vector<std::tuple<std::string, int>> staticInts;
        std::vector<std::tuple<std::string, std::string>> staticStrings;
    };
}