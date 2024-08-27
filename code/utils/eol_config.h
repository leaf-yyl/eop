




#ifndef EOL_CONFIG_H
#define EOL_CONFIG_H

#include <iostream>
#include <unordered_set>

namespace eol {

void setConfigPath(const std::string& path);

template<typename T>
T getConfigValueWithDefaultValue(const std::string& config_key, const T& default_value);
extern template uint64_t getConfigValueWithDefaultValue(const std::string& config_key, const uint64_t& value);
extern template float getConfigValueWithDefaultValue(const std::string& config_key, const float& value);
extern template std::string getConfigValueWithDefaultValue(const std::string& config_key, const std::string& value);

template<typename T>
std::unordered_set<T> collectConfigValue(const std::string& config_key);
extern template std::unordered_set<std::string> collectConfigValue(const std::string& config_key);

}

#endif // EOL_CONFIG_H
