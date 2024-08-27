




#include "utils/eol_log.h"
#include "eol_op_manager.h"

namespace eol {


bool OpManager::isOpAvailable(const std::string& op_name, Backend backend)
{
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if(m_map_backend_op_allocator.find(backend) != m_map_backend_op_allocator.end()) {
        DlLogW << "Unsupported backend = " << backend;
        return false;
    }
    const auto& allocator_map = m_map_backend_op_allocator.at(backend);
    return allocator_map.find(op_name) != allocator_map.end();
}

Op* OpManager::createOp(const std::string& op_name, Backend backend)
{
    Op* op = nullptr;
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if(m_map_backend_op_allocator.find(backend) != m_map_backend_op_allocator.end()) {
        DlLogW << "Failed to find allocator map on backend = " << backend;
        return nullptr;
    }
    const auto& allocator_map = m_map_backend_op_allocator.at(backend);
    if(allocator_map.find(op_name) != allocator_map.end()) {
        op = allocator_map.at(op_name)->createOp();
        if(nullptr != op) {
            m_map_created_op_name[op]    = op_name;
            m_map_created_op_backend[op] = backend;
        }
    } else {
        DlLogW << "Failed to find allocator with name = " << op_name
               << " on backend = " << backend << ", to be implemented!";
    }

    return op;
}

void OpManager::releaseOp(Op* op)
{
    if(nullptr == op) {
        return;
    }

    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if(m_map_created_op_name.find(op) != m_map_created_op_name.end()
        && m_map_created_op_backend.find(op) != m_map_created_op_backend.end()) {
        auto name = m_map_created_op_name.at(op);
        auto backend = m_map_created_op_backend.at(op);
        m_map_backend_op_allocator.at(backend).at(name)->deleteOp(op);
        m_map_created_op_name.erase(op);
        m_map_created_op_backend.erase(op);
    } else {
        DlLogW << "Failed to find this op with address = " << op
               << ", name = " << op->getName()
               << ", double free or wild pointer detected!";
    }
}

void OpManager::registerOpAllocator(const std::string& op_name, Backend backend, OpAllocator* op_allocator)
{
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    auto& op_allocator_map = m_map_backend_op_allocator[backend];
    if(op_allocator_map.find(op_name) == op_allocator_map.end()) {
        op_allocator_map.insert(std::pair<std::string, OpAllocator*>(op_name, op_allocator));
    } else {
        DlLogW << "Op name = " << op_name << " has been registered, update allocator";
        op_allocator_map[op_name] = op_allocator;
    }
}

void OpManager::showConfig()
{
    DlLogI << "No available config for OpManager!";
}

void OpManager::updateConfig()
{
    /* do nothing */
}

} // namespace eol
