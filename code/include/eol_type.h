




#ifndef EOL_TYPE_H
#define EOL_TYPE_H

#include <map>
#include <variant>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "eol_op_global.h"

namespace eol {

typedef enum enDataType {
    DataType_INT8,
    DataType_UINT8,
    DataType_INT16,
    DataType_UINT16,
    DataType_INT32,
    DataType_UINT32,
    DataType_INT64,
    DataType_UINT64,
    DataType_FLOAT16,
    DataType_FLOAT32,
    DataType_UNKNOWN
}DataType;

typedef enum enDataLayout {
    /* 1 dimension */
    DataLayout_W,
    DataLayout_H,
    DataLayout_C,
    DataLayout_UNKNOWN
}DataLayout;

template<DataType T>
constexpr const char* OP_API DataTypeToString() {
    switch (T) {
    case DataType_INT8: return "DataType_INT8";
    case DataType_UINT8: return "DataType_UINT8";
    case DataType_INT16: return "DataType_INT16";
    case DataType_UINT16: return "DataType_UINT16";
    case DataType_INT32: return "DataType_INT32";
    case DataType_UINT32: return "DataType_UINT32";
    case DataType_INT64: return "DataType_INT64";
    case DataType_UINT64: return "DataType_UINT64";
    case DataType_FLOAT16: return "DataType_FLOAT16";
    case DataType_FLOAT32: return "DataType_FLOAT32";
    default: return "DataType_UNKNOWN";
    }
}

template<DataType T>
uint32_t OP_API getDataSizeByType()
{
    switch (T) {
    case DataType_INT8:
    case DataType_UINT8:
        return 1;
    case DataType_INT16:
    case DataType_UINT16:
    case DataType_FLOAT16:
        return 2;
    case DataType_INT32:
    case DataType_UINT32:
    case DataType_FLOAT32:
        return 4;
    case DataType_INT64:
    case DataType_UINT64:
        return 8;
    default:
        static_assert(T >= DataType_INT8 && T < DataType_UNKNOWN, \
                      "Invalid datatype supported, data type = " TOSTRING(T));
        return 0;
    }

    return 0;
}

typedef enum enMemType {
    MemType_Host,       // memory on host cpu
    MemType_Device,     // memory on device gpu
    MemType_Others,     // memory on high-speed cache and so on on device
    MemType_UnKnown,
}MemType;

typedef struct stShape
{
#define MAX_DIM_NUMBER 8
    uint32_t nb_dim = 0;
    uint32_t dims[MAX_DIM_NUMBER] = {0};
}Shape;

class OP_EXPORT OpAttrs {
public:
    using Attribute = std::variant<bool, int32_t, uint32_t, float, void*, std::string>;
    virtual ~OpAttrs() {}

    void OP_API setAttribute(const std::string& key, const Attribute& value) {
        if(isKeyValid(key)) {
            m_attributes[key] = value;
        } else {
            std::stringstream ss;
            ss << "Invalid key for this op, key = " << key << "\n";
            ss << showValidKeys();
            throw std::invalid_argument(ss.str());
        }
    }

    void OP_API removeAttribute(const std::string& key) {
        m_attributes.erase(key);
    }

    void OP_API clearAttribute() {
        m_attributes.clear();
    }

    bool OP_API hasAttribute(const std::string& key) {
        return m_attributes.find(key) != m_attributes.end();
    }

    Attribute OP_API getAttribute(const std::string& key) {
        auto it = m_attributes.find(key);
        if(it != m_attributes.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Attribute not found!");
        }
    }

    std::string OP_API toString() {
        std::stringstream ss;
        for(const auto& [key, value] : m_attributes) {
            ss << "Key: " << key << ", Value: ";
            std::visit([](std::stringstream& ss, auto&& arg) {
                            ss << arg << "\n";
                       }, ss, value);
        }
        return ss.str();
    }

    virtual bool isKeyValid(const std::string& key) = 0;

    virtual std::string showValidKeys() = 0;

protected:
    std::unordered_map<std::string, Attribute> m_attributes;
};

}

#endif // EOL_TYPE_H
