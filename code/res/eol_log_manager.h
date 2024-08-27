




#ifndef EOL_LOG_MANAGER_H
#define EOL_LOG_MANAGER_H

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "res/eol_res.h"
#include "utils/eol_log.h"
#include "utils/eol_task_thread.h"

namespace eol {

class EolLogManager : public EolRes
{
private:
    class EolLogThread : public EolTaskThread
    {
    public:
        EolLogThread(EolLogManager& log_manager) :
            EolTaskThread("EolLogThread"), m_log_manager(log_manager) {}
        ~EolLogThread() {}

        std::shared_ptr<EolTask> sendLogTask(const std::string& log);

    protected:
        void setup() override;
        void update() override;
        void cleanup() override;

        void openLogFile();
        void doLogTask(const std::string& log);

    private:
        uint64_t m_cur_log_file_size = 0;   /* 单位Byte */
        std::ofstream m_ofs;                /* 当前的日志文件句柄 */
        EolLogManager& m_log_manager;       /* 日志资源管理器 */
    };

public:
    ~EolLogManager();

    static EolLogManager& getInstance() {
        static EolLogManager eol_log_manager;
        return eol_log_manager;
    }

    bool isLogEnabled(const EolLogLevel level, const std::string& module);

    /* TODO:
     * 1: 添加日志级别，在命令行使能颜色
     * 2: 添加format参数,格式化日志输出(log_header,log_context,tab_count++/--)
     */
    std::shared_ptr<EolTask> sendLogTask(const std::string& log);

protected:
    void showConfig() override;
    void updateConfig() override;

private:
    EolLogManager();
    EolLogManager(EolLogManager&) = delete;
    EolLogManager& operator = (const EolLogManager&) = delete;

    std::string generateNewLogFile();
    bool reachLogFileMaxSize(uint64_t size);

private:
    EolLogLevel m_log_level = EolLogLevel_Info;     /* 日志级别 */
    std::unordered_set<std::string> m_log_module;   /* 日志模块 */
    std::string m_log_dir = "";                     /* 日志文件目录 */
    uint64_t m_log_interval = 10;                   /* 日志配置更新间隔，单位秒 */
    uint64_t m_log_file_max_size = 64;              /* 日志文件最大大小，单位MBytes */
    EolLogThread* m_log_thread = nullptr;
    static const std::unordered_map<std::string, std::string> s_config_map;
};

}

#endif  // EOL_LOG_MANAGER_H
