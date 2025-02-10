#pragma once 

#include <fstream>
#include <string>
#include "Json/json.hpp"

class Config {
public:
    Config(const std::string& filename);

    bool load();
    bool save();

    nlohmann::json data;
private:
    std::string filename;
};

extern Config* g_Config;