




#ifndef EOL_RES_H
#define EOL_RES_H

#include <iostream>

namespace eol {

class EolRes
{
public:
    virtual ~EolRes() {}

protected:
    virtual void showConfig() = 0;
    virtual void updateConfig() = 0;
};

}

#endif // EOL_RES_H
