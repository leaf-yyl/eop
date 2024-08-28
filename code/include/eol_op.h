




#ifndef EOL_OP_H
#define EOL_OP_H

#include "eol_tensor.h"

namespace eol {

typedef enum enBackend {
    Backend_CPU,
    Backend_Nvidia,
    Backend_Denglin
} Backend;

class OP_EXPORT OpHandle
{
public:
    OpHandle() {}
    virtual ~OpHandle() {}

    /* TODO: 如何实现类型检查？ */
    void OP_API setOpScheduler(void* stream);
};

class OP_EXPORT Op
{
public:
    Op() {}
    virtual ~Op() {}

    OpAttrs* OP_API getAttributes();
    Op* OP_API setInputTensors(const Tensor& in_tensor);
    Op* OP_API setInputTensors(const Tensors& in_tensors);
    Op* OP_API setInputTensors(const std::initializer_list<Tensor>& in_tensors);
    Op* OP_API setOutputTensors(const Tensor& out_tensors);
    Op* OP_API setOutputTensors(const Tensors& out_tensors);
    Op* OP_API setOutputTensors(const std::initializer_list<Tensor>& out_tensors);

    virtual std::string OP_API getName() = 0;
    virtual bool OP_API inferType() = 0;
    virtual bool OP_API inferStride() = 0;
    virtual uint64_t OP_API queryWorkspaceSize(MemType mem_type) = 0;
    virtual bool OP_API run() = 0;

protected:
    OpAttrs* m_attributes = nullptr;
    Tensors  m_in_tensors;
    Tensors  m_out_tensors;

private:
    Op(const Op&) = delete;
    Op& operator=(const Op&) = delete;
};

std::string OP_API getBackendName(Backend backend);
Op* OP_API createOpByName(const std::string& op_name, Backend backend);
void OP_API releaseOp(Op* op);

}   // namespace eol

#endif // EOL_OP_H
