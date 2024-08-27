



#include "utils/eol_log.h"
#include "include/eol_op.h"
#include "res/eol_op_manager.h"

namespace eol {

Op* OP_API createOpByName(const std::string& op_name, Backend backend)
{
    Op* op = nullptr;
    if(OpManager::getInstance().isOpAvailable(op_name)) {
        DlLogW << "Op " << op_name << " is not available!";
    } else {
        op = OpManager::getInstance().createOp(op_name);
    }

    return op;
}

void OP_API releaseOp(Op* op)
{
    OpManager::getInstance().releaseOp(op);
}

}
