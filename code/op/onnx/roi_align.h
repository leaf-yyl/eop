




#ifndef ROI_ALIGN_H
#define ROI_ALIGN_H

#include "include/eol_op.h"

namespace eol {
namespace onnx {

class RoiAlignAttr : public OpAttrs
{
public:
    RoiAlignAttr();

    bool isKeyValid(const std::string& key) override;
    std::string showValidKeys() override;
};

class RoiAlignOp : public Op
{
public:
    std::string OP_API getName() override;
    bool OP_API inferType() override;
    bool OP_API inferStride() override;
    bool OP_API run() override;
    uint64_t OP_API queryWorkspaceSize(MemType mem_type) override;

    static const std::string OpName;
};

}   // namespace onnx
}   // namespace eol

#endif
