




#include "eol_res.h"

namespace eol {

void EolRes::showConfig()
{
    printf("No config for res %s\n", m_res_name.c_str());
}

void EolRes::updateConfig()
{
    printf("No config for res %s, do nothing!\n", m_res_name.c_str());
}

}
