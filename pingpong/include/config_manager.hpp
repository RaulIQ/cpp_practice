#pragma once
#include <fstream>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>


class ConfigManager {
public:
    explicit ConfigManager() {
        _o.open("logs.json", std::ios::app); 
    }

    ~ConfigManager() {
        if (_o.is_open()) {
            _o.close();
        }
    }

    void append(nlohmann::json json) {
        std::lock_guard<std::mutex> lock(mtx_);
        _o << json << std::endl;
    }

private:
    mutable std::mutex mtx_;
    std::ofstream _o;
};
