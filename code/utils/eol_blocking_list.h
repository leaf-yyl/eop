




#ifndef EOL_BLOCKING_LIST_H
#define EOL_BLOCKING_LIST_H

#include <list>
#include <mutex>
#include <cstdint>
#include <condition_variable>

#include "utils/eol_log.h"

namespace eol {

/**
 *  EolBlockingList是一个先进先出(FIFO)的阻塞队列，用于多个生产者和消费者之间传递数据和同步，
 *      并支持按指定批次大小pop出数据。生产者在停止push数据时需要setEos(conditional notify方法的补充)
 *      来通知消费者，以规避可能的死锁。
 */
template<typename T>
class EolBlockingList
{
public:
    explicit EolBlockingList(uint32_t capacity) {
        m_capacity       = capacity;
        m_mutex          = new std::mutex();
        m_cond_consumed  = new std::condition_variable();
        m_cond_producted = new std::condition_variable();
    }

    ~EolBlockingList() {
        delete m_cond_consumed;
        delete m_cond_producted;
        delete m_mutex;
    }

    /**
     * @brief size  获取当前队列大小
     * @return      当前队列大小
     */
    uint32_t size() {
        return m_list.size();
    }

    /**
     * @brief push_back 在指定的时间内添加元素，若@param time_out为0,则一直等待直到添加成功
     * @param e         待添加的元素
     * @param time_out  添加元素时可等待的最长时间。若为0,则表示一直等待直到添加成功
     * @return          添加元素成功与否
     * @note            若@param time_out不为0且等待时间较长，此处的自旋锁会占用大量CPU资源，
     *                  可能导致CPU资源使用异常。建议设置为0，在软件逻辑上保证可以添加成功，
     *                  除非是和硬件相关的资源交互，可能会存在无法添加成功的情况，才设置相应的@param time_out
     */
    bool push_back(const T& e, uint64_t time_out = 0) {
        if(m_eos) {
            DlLogE << "Current blocking list has been setted eos, push_back is disabled!";
            return false;
        }

        std::unique_lock<std::mutex> lock(*m_mutex);
        while(m_list.size() >= m_capacity) {
            if(0 == time_out) {
                m_cond_consumed->wait(lock);
            } else {
                auto deadline = std::chrono::system_clock::now()
                                + std::chrono::milliseconds(time_out);
                if(std::cv_status::timeout == m_cond_consumed->wait_until(lock, deadline)) {
                    DlLogE << "Current blocking list is full, failed to push back element in "
                           << time_out << "ms, capacity = " << m_capacity;
                    return false;
                }
            }
        }
        m_list.push_back(e);
        m_cond_producted->notify_one();
        return true;
    }

    /**
     * @brief pop_front 在指定的时间内从队列前端获取batch大小的数据。
     *                  若@param batch为0且@param time_out为0，则获取当前队列的全部元素
     *                  若@param batch为0且@param time_out不为0，则等待指定的时间后获取队列的全部元素
     *                  若@param batch不为0且@param time_out为0，则获取当前队列的最多batch数量的元素
     *                  若@param batch不为0且@param time_out不为0，
     *                      若当前队列包含batch数量的元素，则获取当前队列的batch数量的元素；
     *                      若当前队列不包含batch数量的元素，则等待指定的时间后获取当前队列的不超过batch数量的元素
     * @param batch     获取队列中元素的最大大小，若为0，则表示获取队列中的全部元素;
     * @param time_out  等待的时间
     * @return          当前队列中获取的元素列表，可能为空列表
     * @note            若@param time_out不为0且等待时间较长，此处的自旋锁会占用大量CPU资源，
     *                  可能导致CPU资源使用异常。建议设置为0，在软件逻辑上保证可以添加成功，
     *                  除非是和硬件相关的资源交互，可能会存在无法添加成功的情况，才设置相应的@param time_out
     */
    std::list<T> pop_front(uint32_t batch = 0, uint64_t time_out = 0) {
        std::unique_lock<std::mutex> lock(*m_mutex);
        if(!m_eos && time_out && m_list.size() < batch) {
            auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(time_out);
            m_cond_producted->wait_until(lock, deadline);
        }

        std::list<T> tmp;
        if(0 == batch) {
            tmp.swap(m_list);
        } else {
            for(uint32_t i = 0; i < batch; ++i) {
                tmp.push_back(m_list.front());
                m_list.pop_front();
            }
        }

        m_cond_consumed->notify_all();
        return std::move(tmp);
    }

    /**
     * @brief setEos    设置Eos标志，表示生产者不再推送元素，通知消费者退出等待
     */
    void setEos(bool eos = true) {
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_eos = eos;
        m_cond_producted->notify_all();
    }

    /**
     * @brief getEos    获取队列的eos标志，以便消费者决定是否退出线程
     * @return          队列的eos标志
     */
    bool getEos() { return m_eos; }

    /**
     * @brief flush     获取队列中当前全部的元素，一般在生产者设置eos后调用
     * @return          队列中当前全部的元素
     */
    std::list<T> flush() {
        std::list<T> tmp;
        if(!m_eos) {
            DlLogW << "Current blocking list has not been setted eos, flushed is not recommended!";
        }

        std::lock_guard<std::mutex> lock(*m_mutex);
        tmp.swap(m_list);
        return std::move(tmp);
    }

private:
    volatile bool m_eos = false;
    std::list<T> m_list;
    uint32_t m_capacity = 0;

    std::mutex* m_mutex                       = nullptr;
    std::condition_variable* m_cond_consumed  = nullptr;
    std::condition_variable* m_cond_producted = nullptr;
};


} // namespace eol


#endif // EOL_BLOCKING_LIST_H
