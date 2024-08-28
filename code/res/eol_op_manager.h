




#ifndef EOL_OP_MANAGER_H
#define EOL_OP_MANAGER_H

#include <mutex>
#include <unordered_set>
#include <unordered_map>

#include "res/eol_res.h"
#include "include/eol_op.h"

namespace eol {

class OpAllocator {
public:
    virtual ~OpAllocator() {}
    virtual Op* createOp() const = 0;
    virtual void deleteOp(Op* op) const = 0;
};

template<typename DerivedOp>
class OpAllocatorImpl : public OpAllocator
{
public:
    Op* createOp() const override {
        return new DerivedOp();
    }

    void deleteOp(Op* op) const override {
        delete op;
    }
};

class OpManager : public EolRes
{
public:
    ~OpManager();

    static OpManager& getInstance() {
        static OpManager eol_op_manager;
        return eol_op_manager;
    }

    void showAvaiableOps();
    std::map<std::string, std::vector<Backend>> getAvaiableOps();

    Op* createOp(const std::string& op_name, Backend backend);
    void releaseOp(Op* op);

    bool isOpAvailable(const std::string& op_name, Backend backend);
    void registerOpAllocator(const std::string& op_name, Backend backend, OpAllocator* op_allocator);

private:
    OpManager();
    OpManager(OpManager&) = delete;
    OpManager& operator = (const OpManager&) = delete;

private:
    std::mutex m_mutex;
    std::unordered_map<Op*, Backend> m_map_created_op_backend;
    std::unordered_map<Op*, std::string> m_map_created_op_name;
    std::map<std::string, std::map<Backend, OpAllocator*>> m_map_backend_op_allocator;
};

#define REGISTER_OP_ALLOCATOR(OpName, Backend, Op) \
    __attribute__((constructor)) void RegisterOpAllocator##Backend##Op() { \
        OpManager::getInstance().registerOpAllocator(OpName, Backend, new OpAllocatorImpl<Op>()); \
    }

}

#endif
