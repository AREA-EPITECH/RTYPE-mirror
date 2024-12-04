/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ThreadPool.cpp
*/

#include "ThreadPool.hpp"
#include <utility>
#include <functional>
#include <mutex>
#include <cstddef>


namespace server {
    ThreadPool::ThreadPool(size_t thread_count) : _stop(false) {
        for (size_t i = 0; i < thread_count; ++i) {
            _workers.emplace_back(&ThreadPool::workerThread, this);
        }
    }

    ThreadPool::~ThreadPool() {
        {
            const std::lock_guard lock(_queue_mutex);
            _stop = true;
        }
        _condition.notify_all();
        for (auto& worker : _workers) {
            worker.join();
        }
    }

    void ThreadPool::enqueueTask(std::function<void()> task) {
        {
            std::lock_guard const lock(_queue_mutex);
            _tasks.push(std::move(task));
        }
        _condition.notify_one();
    }

    void ThreadPool::workerThread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(_queue_mutex);
                _condition.wait(lock, [this] { return _stop || !_tasks.empty(); });
                if (_stop && _tasks.empty()) {
                    return;
                }
                task = std::move(_tasks.front());
                _tasks.pop();
            }
            task();
        }
    }
} // namespace server
