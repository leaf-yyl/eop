




#include <cassert>
#include <unistd.h>

#include "eol_task_thread.h"


namespace eol {


EolTaskThread::EolTaskThread(const std::string& name, uint32_t max_task_count)
    : m_task_list(max_task_count)
{
    m_thread_name = name;
    m_max_task_count = max_task_count;
    m_thread_entrance = std::bind(&EolTaskThread::entrance, this);
}

EolTaskThread::~EolTaskThread()
{
}

void EolTaskThread::startExecutor() {
    if(!m_thread_started) {
        m_thread_started = true;
        m_thread_setup_done = false;
        m_task_list.setEos(false);
        m_thread = new std::thread(m_thread_entrance);
    } else {
        DlLogW << "Background thread " << m_thread_name << " has been started!";
    }
}

bool EolTaskThread::isExecutorSetupDone()
{
    return m_thread_setup_done;
}

void EolTaskThread::waitExecutorSetupDone()
{
    while(!m_thread_setup_done) {
        usleep(10 * 1000);
    }
}

void EolTaskThread::stopExecutor()
{
    if(m_thread_started) {
        m_thread_started = false;
        m_task_list.setEos(true);
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    } else {
        DlLogW << "Background thread " << m_thread_name << " has been stopped or not been started!";
    }
}

bool EolTaskThread::sendTask(const std::shared_ptr<EolTask>& task, uint64_t time_out)
{
    return m_task_list.push_back(task, time_out);
}

void EolTaskThread::logDoneTasks(const std::list<std::shared_ptr<EolTask>>& tasks)
{
    /* TODO: 搞个统计类记录和输出线程信息，类似普罗米修斯那种 */
    DlLogD << "Thread name: " << m_thread_name;
    for(const auto& task : tasks) {
        DlLogD << task->toString();
    }
}

void EolTaskThread::entrance() noexcept
{
    /* 初始化线程运行资源 */
    setup();
    m_thread_setup_done = true;

    /* 等待并执行任务,设置batch=0来获取当前缓存的全部任务 */
    while(!m_task_list.getEos()) {
        auto task_list = m_task_list.pop_front();
        for(auto& task : task_list) {
            task->execute();
        }
        logDoneTasks(task_list);
        update();
    }

    /* 获取和执行可能尚未获取的任务 */
    auto task_list = m_task_list.flush();
    for(auto& task : task_list) {
        task->execute();
    }
    logDoneTasks(task_list);
    update();

    /* 释放线程资源 */
    cleanup();
}

} // namespace eol

