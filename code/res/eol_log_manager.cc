




#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

#include "utils/eol_log.h"
#include "utils/eol_config.h"
#include "res/eol_log_manager.h"


namespace eol {

std::shared_ptr<EolTask> EolLogManager::EolLogThread::sendLogTask(const std::string& log) {
    auto runnable = std::bind(&EolLogThread::doLogTask, this, log);
    auto task = std::make_shared<EolTask>(std::move(runnable));
    sendTask(task);
    return task;
}

void EolLogManager::EolLogThread::openLogFile()
{
    auto log_file_name = m_log_manager.generateNewLogFile();
    if(!log_file_name.empty()) {
        m_ofs.open(log_file_name);
        m_cur_log_file_size = 0;
    }
}

void EolLogManager::EolLogThread::doLogTask(const std::string& log) {
    std::cout << log << std::endl;
    if(m_ofs.is_open() && m_log_manager.reachLogFileMaxSize(m_cur_log_file_size + log.size())) {
        /* 日志文件大小已达到上限，关闭当前日志文件 */
        m_ofs.close();
    }

    if(!m_ofs.is_open()) {
        /* 日志文件未创建或者文件大小达到上限被关闭，尝试打开新的日志文件 */
        openLogFile();
    }

    if(m_ofs.is_open()) {
        m_ofs << log << std::endl;
        m_cur_log_file_size += log.size();
    }
}

void EolLogManager::EolLogThread::setup() {
    m_log_manager.updateConfig();
}

void EolLogManager::EolLogThread::update() {
    m_log_manager.updateConfig();
}

void EolLogManager::EolLogThread::cleanup() {
    if(m_ofs.is_open()) {
        m_ofs.close();
    }
}


const std::unordered_map<std::string, std::string> EolLogManager::s_config_map = {
        {"EOL_LOG_LEVEL", "整数类型,日志级别,[0,1,2,3,4]分别代表[debug,info,warning,error,fatal]，默认值为1"},
        {"EOL_LOG_MODULE", "字符串类型，使能的日志模块，可配置多条来使能多个日志模块，默认值为使能所有模块"},
        {"EOL_LOG_DIR", "字符串类型，日志文件存储目录，若为空，则只输出到终端，不保存日志文件，默认值为空"},
        {"EOL_LOG_INTERVAL", "整数类型,日志文件配置更新间隔，单位秒，默认值为10"},
        {"EOL_LOG_FILE_SIZE", "整数类型，单个日志文件最大大小，单位MBytes，默认值为64"}
   };

void EolLogManager::showConfig()
{
    sendLogTask("Available config of log manager:");
    for(const auto& [key, description] : s_config_map) {
        sendLogTask(key + ": " + description);
    }
}

void EolLogManager::updateConfig()
{
    static auto last_time = std::chrono::system_clock::now();
    auto cur_time = std::chrono::system_clock::now();
    if(cur_time - last_time > std::chrono::milliseconds(m_log_interval * 1000)) {
        last_time = cur_time;
        m_log_level = (EolLogLevel)getConfigValueWithDefaultValue<uint64_t>("EOL_LOG_LEVEL", EolLogLevel_Info);
        m_log_module = collectConfigValue<std::string>("EOL_LOG_MODULE");
        m_log_dir = getConfigValueWithDefaultValue<std::string>("EOL_LOG_DIR", "");
        m_log_interval = getConfigValueWithDefaultValue("EOL_LOG_INTERVAL", 10ull);
        m_log_file_max_size = getConfigValueWithDefaultValue("EOL_LOG_FILE_SIZE", 64ull);
    }
}

bool EolLogManager::isLogEnabled(const EolLogLevel level, const std::string& module)
{
    if(level < m_log_level) {
        return false;
    }

    if(!module.empty() && !m_log_module.empty()) {
        if(m_log_module.find(module) == m_log_module.end()) {
            return false;
        }
    }

    return true;
}

std::shared_ptr<EolTask> EolLogManager::sendLogTask(const std::string& log) {
    return m_log_thread->sendLogTask(log);
}

std::string EolLogManager::generateNewLogFile() {
    std::stringstream ss;
    if(!m_log_dir.empty()) {
        auto now    = std::chrono::system_clock::now();
        auto t_now  = std::chrono::system_clock::to_time_t(now);
        auto tm_now = std::localtime(&t_now);
        ss << m_log_dir << "/eol_" << std::put_time(tm_now, "%Y-%m-%d-%H-%M-%S.log");
    }
    return ss.str();
}

bool EolLogManager::reachLogFileMaxSize(uint64_t size) {
    return size <= m_log_file_max_size * 1024 * 1024;
}

EolLogManager::EolLogManager() {
    m_log_thread = new EolLogThread(*this);
    m_log_thread->startExecutor();

    /* 显示日志资源的配置选项 */
    showConfig();
}

EolLogManager::~EolLogManager() {
    m_log_thread->stopExecutor();
    delete m_log_thread;
}

} // namespace eol
