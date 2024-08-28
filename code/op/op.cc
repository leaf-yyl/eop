




#include "utils/eol_log.h"
#include "include/eol_op.h"
#include "res/eol_op_manager.h"

namespace eol {

OpAttrs* OP_API Op::getAttributes()
{
    return m_attributes;
}

Op* OP_API Op::setInputTensors(const Tensor& in_tensor)
{
    m_in_tensors = in_tensor;
    return this;
}

Op* OP_API Op::setInputTensors(const Tensors& in_tensors)
{
    m_in_tensors = in_tensors;
    return this;
}

Op* OP_API Op::setInputTensors(const std::initializer_list<Tensor>& in_tensors)
{
    m_in_tensors = in_tensors;
    return this;
}

Op* OP_API Op::setOutputTensors(const Tensor& out_tensor)
{
    m_out_tensors = out_tensor;
    return this;
}

Op* OP_API Op::setOutputTensors(const Tensors& out_tensors)
{
    m_out_tensors = out_tensors;
    return this;
}

Op* OP_API Op::setOutputTensors(const std::initializer_list<Tensor>& out_tensors)
{
    m_out_tensors = out_tensors;
    return this;
}

std::string OP_API getBackendName(Backend backend)
{
    switch (backend) {
    case Backend_CPU:
        return "CPU";
    case Backend_Nvidia:
        return "Nvidia";
    case Backend_Denglin:
        return "denglin";
    default:
        DlLogF << "Unknown backend";
        return "";
    }
}

Op* OP_API createOpByName(const std::string& op_name, Backend backend)
{
    Op* op = nullptr;
    if(!OpManager::getInstance().isOpAvailable(op_name, backend)) {
        DlLogW << "Op " << op_name << " is not available!";
    } else {
        op = OpManager::getInstance().createOp(op_name, backend);
    }

    return op;
}

void OP_API releaseOp(Op* op)
{
    OpManager::getInstance().releaseOp(op);
}

}
