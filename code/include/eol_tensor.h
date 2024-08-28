




#ifndef EOL_TENSOR_H
#define EOL_TENSOR_H

#include <numeric>
#include <functional>

#include "eol_type.h"
#include "eol_small_array.h"

namespace eol {

class OP_EXPORT Tensor
{
public:
    virtual ~Tensor() {}

    DataType OP_API getDataType() const {
        return m_data_type;
    }

    Tensor& OP_API setDataType(DataType data_type) {
        m_data_type = data_type;
        return *this;
    }

    MemType OP_API getMemType() const {
        return m_mem_type;
    }

    Tensor& OP_API setMemType(MemType mem_type) {
        m_mem_type = mem_type;
        return *this;
    }

    Shape OP_API getShape() const {
        return m_shape;
    }

    Tensor& OP_API setShape(Shape shape) {
        m_shape = shape;
        return *this;
    }

    Shape OP_API getStride() const {
        return m_stride;
    }

    Tensor& OP_API setStride(Shape stride) {
        m_stride = stride;
        return *this;
    }

    DataLayout OP_API getDataLayout() const {
        return m_data_layout;
    }

    Tensor& OP_API setDataLayout(DataLayout data_layout) {
        m_data_layout = data_layout;
        return *this;
    }

    void* OP_API getBuffer() const {
        return m_buffer;
    }

    Tensor& OP_API setBuffer(void* buffer) {
        m_buffer = buffer;
        return *this;
    }

    uint64_t OP_API getBytes() {
        return std::accumulate(m_stride.dims, m_stride.dims + m_stride.nb_dim, 1, std::multiplies<uint64_t>());
    }

private:
    Shape m_shape;
    Shape m_stride;
    MemType  m_mem_type  = MemType_UnKnown;
    DataType m_data_type = DataType_UNKNOWN;
    DataLayout m_data_layout = DataLayout_UNKNOWN;
    void* m_buffer = nullptr;
};

typedef SmallArray<Tensor> Tensors;

}   // namespace eol

#endif  // EOL_TENSOR_H
