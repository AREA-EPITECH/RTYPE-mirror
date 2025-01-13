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
#include <spdlog/spdlog.h>

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
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void ThreadPool::enqueueTask(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(_queue_mutex);
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
            try {
                task();
            } catch (const std::exception &e) {
                spdlog::error("Exception in task: {}", e.what());
            } catch (...) {
                spdlog::error("Unknown exception in task.");
            }
        }
    }
} // namespace server
