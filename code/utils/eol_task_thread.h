




#ifndef EOL_TASK_THREAD_H
#define EOL_TASK_THREAD_H

#include <future>
#include <thread>
#include <functional>
#include <condition_variable>

#include "eol_blocking_list.h"

namespace eol {

/**
 * @brief The EolTask class    后台线程执行的task基类，调用者可通过std::future获取task执行状态
 */
class EolTask
{
    friend class EolTaskThread;
public:
    EolTask(const std::function<void()>& runnable)
        : m_runnable(runnable) {}

    virtual ~EolTask() {}

    /**
     * @brief get_future 获取可运行对象的future,用于调用者同步和获取可运行对象异步执行结果
     * @return std::future对象，用于调用者同步异步调用和获取结果
     */
    std::future<void> get_future() { return m_promise.get_future(); }

    virtual std::string toString() { return "No task info"; }

protected:
    /**
     * @brief execute 执行可运行对象
     */
    virtual void execute() {
        m_runnable();
        m_promise.set_value();
    }

private:
    std::promise<void> m_promise;
    std::function<void()> m_runnable;
};

/**
 * @brief The EolTaskWrv class 执行完成后带返回结果的task，调用者可通过std::future的get()方法获取
 *                            task执行结果（Wrv=WithReturnValue）
 */
template<typename ReturnType>
class EolTaskWrv : public EolTask
{
    friend class EolTaskThread;
public:
    EolTaskWrv(const std::function<ReturnType()>& runnable)
        : EolTask(nullptr), m_runnable(runnable) {}

    virtual ~EolTaskWrv() {}

    std::future<ReturnType> get_future() { return m_promise.get_future(); }

protected:
    void execute() override {
        m_promise.set_value(m_runnable());
    }

private:
    std::promise<ReturnType> m_promise;
    std::function<ReturnType()> m_runnable;
};

/**
 * @brief The DlTaskThread class 后台task执行线程，接受并执行异步任务。
 */
class EolTaskThread
{
public:
    EolTaskThread(const std::string& name = "",
                 uint32_t max_task_count = DEFAULT_TASK_CAPACITY);
    virtual ~EolTaskThread();

    void startExecutor();

    bool isExecutorSetupDone();

    void waitExecutorSetupDone();

    void stopExecutor();

    bool sendTask(const std::shared_ptr<EolTask>& task, uint64_t time_out = 0);

protected:
    virtual void setup() {}
    virtual void update() {};
    virtual void cleanup() {}

private:
    void entrance() noexcept;
    void logDoneTasks(const std::list<std::shared_ptr<EolTask>>& tasks);

protected:
    static const uint32_t DEFAULT_TASK_CAPACITY  = 1024;

    bool m_thread_started = false;
    volatile bool m_thread_setup_done = false;

    std::function<void()> m_thread_entrance;
    EolBlockingList<std::shared_ptr<EolTask>> m_task_list;

private:
    std::string  m_thread_name;
    std::thread* m_thread = nullptr;
    unsigned int m_max_task_count = DEFAULT_TASK_CAPACITY;
};


} // namesapace dl


#endif // DL_TASK_THREAD_H
