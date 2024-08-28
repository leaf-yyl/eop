




#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

#include "eol_log.h"
#include "res/eol_log_manager.h"

namespace eol {

static const std::map<EolLogLevel, std::string> g_logger_serverity_map = {
    {EolLogLevel_Debug, "[D]"},
    {EolLogLevel_Info, "[I]"},
    {EolLogLevel_Warning, "[W]"},
    {EolLogLevel_Error, "[E]"},
    {EolLogLevel_Fatal,"[F]"}
};


EolLog::EolLog(const char* file, uint32_t line, EolLogLevel severity, const std::string& module)
{
    auto last_slash = strrchr(file, '/');
    if(nullptr != last_slash) {
        m_file = last_slash + 1;
    } else {
        m_file = file;
    }
    m_line      = line;
    m_module    = module;
    m_severity  = severity;
}

EolLog::~EolLog()
{
    if(EolLogManager::getInstance().isLogEnabled(m_severity, m_module)) {
        auto task = EolLogManager::getInstance().sendLogTask(formatLogHeader() + m_log_context.str());
        if(EolLogLevel_Fatal == m_severity) {
            task->get_future().get();
            abort();
        }
    }
}

std::ostream& EolLog::log()
{
    return m_log_context;
}

std::string EolLog::formatLogHeader()
{
    std::ostringstream ss_header;
    if(m_module.empty()) {
        ss_header << m_file << ":" << m_line << " ";
    } else {
        /* TODO: 格式化文件名-行号输出，文件名过长时截断文件名前面部分 */
        ss_header << g_logger_serverity_map.at(m_severity)
                       << "[" << m_module << "]: ";
    }

    return ss_header.str();
}


} // namespace eol
