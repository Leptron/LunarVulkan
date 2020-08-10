#include "../../public/StaticHolder.h"

namespace LunarUtils {
    StaticHolder* StaticHolder::instance = nullptr;

    StaticHolder::StaticHolder() {

    }

    StaticHolder* StaticHolder::getInstance() {
        if(instance == nullptr)
            instance = new StaticHolder;

        return instance;
    }

    void StaticHolder::SetValue(std::string id, float inital_value) {
        std::tuple<std::string, float> m_tup = std::make_tuple(id, inital_value);
        staticFloats.push_back(m_tup);
    }

    void StaticHolder::SetValue(std::string id, int inital_value) {
        std::tuple<std::string, int> m_tup = std::make_tuple(id, inital_value);
        staticInts.push_back(m_tup);
    }

    void StaticHolder::SetValue(std::string id, std::string inital_value) {
        std::tuple<std::string, std::string> m_tup = std::make_tuple(id, inital_value);
        staticStrings.push_back(m_tup);
    }

    float StaticHolder::GetValue(std::string id) {
        for(auto m_tup : staticFloats) {
            std::string pID = std::get<0>(m_tup);
            if(pID == id)
                return std::get<1>(m_tup);
        }
    }

    int StaticHolder::GetValue(std::string id, int dVal) {
        for(auto m_tup : staticInts) {
            std::string pID = std::get<0>(m_tup);
            if(pID == id)
                return std::get<1>(m_tup);
        }
    }

    std::string StaticHolder::GetValue(std::string id, bool str) {
        for(auto m_tup : staticStrings) {
            std::string pID = std::get<0>(m_tup);
            if(pID == id)
                return std::get<1>(m_tup);
        }
    }
}