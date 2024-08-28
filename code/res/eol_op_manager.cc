




#include "utils/eol_log.h"
#include "eol_op_manager.h"

namespace eol {

OpManager::OpManager()
    : EolRes("OpManager")
{
    showConfig();
}

OpManager::~OpManager()
{
    if(!m_map_created_op_name.empty()) {
        DlLogW << "Ops are created but not released, count = " << m_map_created_op_name.size();
        auto name_iter = m_map_created_op_name.begin();
        auto backend_iter = m_map_created_op_backend.begin();
        while(name_iter != m_map_created_op_name.end()) {
            DlLogW << "Op name = " << name_iter->second << ", backend = " << backend_iter->second;
            releaseOp(name_iter->first);
            name_iter++;
            backend_iter++;
        }
        m_map_created_op_name.clear();
        m_map_created_op_backend.clear();
    }
    m_map_backend_op_allocator.clear();
}

bool OpManager::isOpAvailable(const std::string& op_name, Backend backend)
{
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if(m_map_backend_op_allocator.find(op_name) == m_map_backend_op_allocator.end()) {
        DlLogW << "Op name = " << op_name << " is not supported yet, to be implemented!";
        return false;
    }
    const auto& allocator_map = m_map_backend_op_allocator.at(op_name);
    return allocator_map.find(backend) != allocator_map.end();
}

Op* OpManager::createOp(const std::string& op_name, Backend backend)
{
    Op* op = nullptr;
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if(m_map_backend_op_allocator.find(op_name) == m_map_backend_op_allocator.end()) {
        DlLogW << "Failed to find allocator map of op name = " << op_name;
        return nullptr;
    }
    const auto& allocator_map = m_map_backend_op_allocator.at(op_name);
    if(allocator_map.find(backend) != allocator_map.end()) {
        op = allocator_map.at(backend)->createOp();
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
        m_map_backend_op_allocator.at(name).at(backend)->deleteOp(op);
        m_map_created_op_name.erase(op);
        m_map_created_op_backend.erase(op);
    } else {
        DlLogW << "Failed to find this op with address = " << op
               << ", name = " << op->getName()
               << ", double free or wild pointer detected!";
    }
}


std::map<std::string, std::vector<Backend>> OpManager::getAvaiableOps()
{
    std::map<std::string, std::vector<Backend>> available_ops;
    for(const auto& [op_name, allocator_map] : m_map_backend_op_allocator) {
        std::vector<Backend> backends;
        for(const auto& iter : allocator_map) {
            backends.push_back(iter.first);
        }
        available_ops.insert(std::pair<std::string, std::vector<Backend>>(op_name, backends));
    }

    return available_ops;
}

void OpManager::showAvaiableOps()
{
    DlLogI << "Available op now:";
    for(const auto& [op_name, allocator_map] : m_map_backend_op_allocator) {
        std::string backends;
        for(const auto& iter : allocator_map) {
            backends.append(getBackendName(iter.first) + ",");
        }
        backends.pop_back();
        DlLogI << "\t" << op_name << ", backends = " << backends;
    }
}

void OpManager::registerOpAllocator(const std::string& op_name, Backend backend, OpAllocator* op_allocator)
{
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    auto& op_allocator_map = m_map_backend_op_allocator[op_name];
    if(op_allocator_map.find(backend) == op_allocator_map.end()) {
        op_allocator_map.insert(std::pair<Backend, OpAllocator*>(backend, op_allocator));
        printf("Register op name = %s, backend = %s\n", op_name.c_str(), getBackendName(backend).c_str());
    } else {
        op_allocator_map[backend] = op_allocator;
        printf("Op name = %s has been registered, update allocator\n", op_name.c_str());
    }
}

} // namespace eol
