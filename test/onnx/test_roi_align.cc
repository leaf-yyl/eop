




#include <iostream>

#include "eol_op.h"
#include "gtest/gtest.h"

namespace onnx {


class RoiAlignTest
{
public:
    bool func() {
        auto op = eol::createOpByName("roi_align", eol::Backend_Denglin);
//        op->setInputTensors();
        eol::releaseOp(op);
        return true;
    }

    void perf() {

    }
};

TEST(onnx, RoiAlignTest)
{
    RoiAlignTest test;
    EXPECT_TRUE(test.func());
}

}
