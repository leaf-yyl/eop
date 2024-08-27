




#ifndef EOL_LOG_H
#define EOL_LOG_H

#include <sstream>

namespace eol {


typedef enum enEolLogLevel {
    EolLogLevel_Debug     = 0x00000000,
    EolLogLevel_Info      = 0x00000001,
    EolLogLevel_Warning   = 0x00000002,
    EolLogLevel_Error     = 0x00000003,
    EolLogLevel_Fatal     = 0x00000004
} EolLogLevel;


#define DlLogD EolLog(__FILE__, __LINE__, EolLogLevel_Debug).log()
#define DlLogI EolLog(__FILE__, __LINE__, EolLogLevel_Info).log()
#define DlLogW EolLog(__FILE__, __LINE__, EolLogLevel_Warning).log()
#define DlLogE EolLog(__FILE__, __LINE__, EolLogLevel_Error).log()
#define DlLogF EolLog(__FILE__, __LINE__, EolLogLevel_Fatal).log()

#define EolLogModuleD(module) EolLog(__FILE__, __LINE__, EolLogLevel_Debug, module).log()
#define EolLogModuleI(module) EolLog(__FILE__, __LINE__, EolLogLevel_Info, module).log()
#define EolLogModuleW(module) EolLog(__FILE__, __LINE__, EolLogLevel_Warning, module).log()
#define EolLogModuleE(module) EolLog(__FILE__, __LINE__, EolLogLevel_Error, module).log()
#define EolLogModuleF(module) EolLog(__FILE__, __LINE__, EolLogLevel_Fatal, module).log()

class EolLog
{
public:
    explicit EolLog(const char* file, uint32_t line, EolLogLevel severity, const std::string& module = "");
    ~EolLog();

    std::ostream& log();

protected:
    std::string formatLogHeader();

private:
    uint32_t m_line;
    const char* m_file = nullptr;
    EolLogLevel m_severity;
    std::string m_module;
    std::ostringstream m_log_context;
};


} // namespace eol


#endif // EOL_LOG_H
