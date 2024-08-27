




#include "roi_align.h"
#include "res/eol_op_manager.h"

namespace eol {
namespace onnx {

RoiAlignAttr::RoiAlignAttr()
{

}

bool RoiAlignAttr::isKeyValid(const std::string& key)
{
    return true;
}

std::string RoiAlignAttr::showValidKeys()
{
    return "";
}

const std::string RoiAlignOp::OpName = "roi_align";
std::string RoiAlignOp::getName()
{
    return OpName;
}

bool RoiAlignOp::inferType()
{
    return true;
}

bool RoiAlignOp::inferStride()
{
    return true;
}

bool RoiAlignOp::run()
{
    return true;
}

uint64_t RoiAlignOp::queryWorkspaceSize(MemType mem_type)
{
    return 0;
}

REGISTER_OP_ALLOCATOR(RoiAlignOp::OpName, Backend_Nvidia, RoiAlignOp);
REGISTER_OP_ALLOCATOR(RoiAlignOp::OpName, Backend_Denglin, RoiAlignOp);

}
}
