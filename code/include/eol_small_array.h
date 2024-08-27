




#ifndef EOL_SMALL_ARRAY_H
#define EOL_SMALL_ARRAY_H

#include <array>
#include <iostream>

#include "eol_op_global.h"

namespace eol {

template<typename T, size_t Capacity = 8>
class OP_EXPORT SmallArray {
public:
//    using value_type = T;
    using iterator = T *;
    using const_iterator = T *const;

    SmallArray(size_t size) noexcept {
        ASSERT_WITH_MSG(size <= Capacity, "Size of small array should be less then Capacity");
        m_size = size;
    }

    SmallArray(const std::initializer_list<T>& init_list) noexcept {
        m_size = init_list.size();
        ASSERT_WITH_MSG(m_size <= Capacity, "Size of small array should be less then Capacity");
        std::copy(init_list.begin(), init_list.end(), m_array.begin());
    }

    SmallArray(const SmallArray& other) noexcept {
        m_size = other.getSize();
        ASSERT_WITH_MSG(m_size <= Capacity, "Size of small array should be less then Capacity");
        std::copy(other.begin(), other.end(), m_array.begin());
    }

    SmallArray(SmallArray&& other) noexcept {
        m_size = other.getSize();
        ASSERT_WITH_MSG(m_size <= Capacity, "Size of small array should be less then Capacity");
        std::copy(other.begin(), other.end(), m_array.begin());
    }

    SmallArray& OP_API operator=(const SmallArray& other) {
        if(*this == other) {
            return *this;
        } else {
            m_size = other.getSize();
            ASSERT_WITH_MSG(m_size <= Capacity, "Size of small array should be less then Capacity");
            std::copy(other.begin(), other.end(), m_array.begin());
        }
    }

    SmallArray& OP_API operator=(SmallArray&& other) {
        if(*this == other) {
            return *this;
        } else {
            m_size = other.getSize();
            ASSERT_WITH_MSG(m_size <= Capacity, "Size of small array should be less then Capacity");
            std::copy(other.begin(), other.end(), m_array.begin());
        }
    }

    const T& OP_API at(int32_t idx) const {
        ASSERT_WITH_MSG(m_size > 0, "Small array is empty, get element failed!");
        ASSERT_WITH_MSG(idx < 0 || idx < m_size, "Index is out of range of small array, get element failed!");
        ASSERT_WITH_MSG(idx > 0 || idx + m_size >= 0, "Index is out of range of small array, get element failed!");
        return m_array.at(idx >= 0 ? idx : m_size + idx);
    }

    T& OP_API operator[](int32_t idx) {
        ASSERT_WITH_MSG(m_size > 0, "Small array is empty, get element failed!");
        ASSERT_WITH_MSG(idx < 0 || idx < m_size, "Index is out of range of small array, get element failed!");
        ASSERT_WITH_MSG(idx > 0 || idx + m_size >= 0, "Index is out of range of small array, get element failed!");
        return m_array[idx >= 0 ? idx : m_size + idx];
    }

    SmallArray& OP_API append(const T& t) {
        ASSERT_WITH_MSG(m_size + 1 <= Capacity, "Size of small array should be less then Capacity");
        m_array[m_size++] = t;
        return *this;
    }

    SmallArray& OP_API append(const SmallArray& other) {
        ASSERT_WITH_MSG(m_size + other.getSize() <= Capacity, "Size of small array should be less then Capacity");
        std::copy(other.begin(), other.end(), m_array.end());
        m_size += other.getSize();
        return *this;
    }

    uint32_t OP_API getSize() const {
        return m_size;
    }

    static constexpr uint32_t OP_API getCapacity() {
        return Capacity;
    }

    iterator OP_API begin() noexcept {
        return m_array.begin();
    }

    iterator OP_API end() noexcept {
        return m_array.begin() + m_size;
    }

    const_iterator OP_API begin() const noexcept {
        return m_array.begin();
    }

    const_iterator OP_API end() const noexcept {
        return m_array.end() + m_size;
    }

private:
    static void checkMaxSize() {
        static const size_t MAX_SIZE_OF_SMALL_ARRAY = 32;
        static_assert (Capacity <= MAX_SIZE_OF_SMALL_ARRAY,
                       "Capacity of fixed array should" \
                       " be less then MAX_SIZE_OF_SMALL_ARRAY, Capacity = " TOSTRING(Capacity));
    }

private:
    size_t m_size;
    std::array<T, Capacity> m_array;
};

}

#endif // EOL_SMALL_ARRAY_H
