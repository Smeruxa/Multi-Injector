#include "Config.h"

Config* g_Config;

Config::Config(const std::string& filename) : filename(filename) {
    if (std::filesystem::exists(filename))
        load();
    else {
        data["login"] = "";
        data["password"] = "";
        save();
    }
}

bool Config::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file >> data;

    file.close();
    return true;
}

bool Config::save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << std::setw(4) << data;

    file.close();
    return true;
}