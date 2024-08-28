




#ifndef EOL_RES_H
#define EOL_RES_H

#include <iostream>

namespace eol {

class EolRes
{
public:
    EolRes(std:: string res_name) { m_res_name = res_name; }
    virtual ~EolRes() {}

protected:
    std::string m_res_name;

protected:
    virtual void showConfig();
    virtual void updateConfig();
};

}

#endif // EOL_RES_H
