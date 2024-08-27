




#include "eol_log.h"
#include "eol_config.h"

#include <regex>
#include <fstream>

namespace eol {

static std::string g_config_path = "eol_config.h";
void setConfigPath(const std::string& path)
{
    g_config_path = path;
}

template<typename T>
T getConfigValue(const std::string& key)
{
    DlLogF << "Not implement config type for " << key
           << ", value is not fetched!";
    return T();
}

template<>
uint64_t getConfigValue(const std::string& str)
{
    return atoll(str.c_str());
}

template<>
float getConfigValue(const std::string& str)
{
    return atof(str.c_str());
}

template<>
std::string getConfigValue(const std::string& str)
{
    return str;
}


template<typename T>
T getConfigValueWithDefaultValue(const std::string& config_key, const T& default_value)
{
    std::ifstream config_file(g_config_path);
    if(!config_file.is_open()) {
        DlLogW << "Failed to open config file, path = " << g_config_path;
        return default_value;
    }

    std::string line;
    while(std::getline(config_file, line)) {
        if(line.find(config_key) != line.npos && '#' != line.at(0)) {
            DlLogD << "Find config of item: " << line;
            std::regex pattern("(\\w+):\\s(\\w|\\d+)");
            std::sregex_iterator iter(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            while(iter != end) {
                auto match = *iter;
                auto reg_key = match[1].str();
                auto reg_value = match[2].str();
                if(0 == config_key.compare(reg_key)) {
                    config_file.close();
                    DlLogD << "Success to find config with key = " << config_key << ", value = " << reg_value;
                    return getConfigValue<T>(reg_value);
                }
                ++iter;
            }
        }
    }

    config_file.close();
    DlLogD << "Failed to find config key = " << config_key << " in config file"
           << ", return default value = " << default_value;
    return default_value;
}

template uint64_t getConfigValueWithDefaultValue(const std::string& config_key, const uint64_t& value);
template float getConfigValueWithDefaultValue(const std::string& config_key, const float& value);
template std::string getConfigValueWithDefaultValue(const std::string& config_key, const std::string& value);

template<typename T>
std::unordered_set<T> collectConfigValue(const std::string& config_key)
{
    std::unordered_set<T> config_set;
    std::ifstream config_file(g_config_path);
    if(!config_file.is_open()) {
        DlLogW << "Failed to open config file, path = " << g_config_path;
        return config_set;
    }

    std::string line;
    while(std::getline(config_file, line)) {
        if(line.find(config_key) != line.npos && '#' != line.at(0)) {
            DlLogD << "Find config of item: " << line;
            std::regex pattern("(\\w+):\\s(\\w|\\d+)");
            std::sregex_iterator iter(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            while(iter != end) {
                auto match = *iter;
                auto reg_key = match[1].str();
                auto reg_value = match[2].str();
                if(0 == config_key.compare(reg_key)) {
                    DlLogD << "Success to find config with key = " << config_key << ", value = " << reg_value;
                    config_set.insert(getConfigValue<T>(reg_value));
                }
                ++iter;
            }
        }
    }

    config_file.close();
    return config_set;
}

template std::unordered_set<std::string> collectConfigValue(const std::string& config_key);

}

