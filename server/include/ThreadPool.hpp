/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** ThreadPool.hpp
*/

#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>

namespace server {
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t thread_count);
        ThreadPool() = delete;
        ~ThreadPool();

        // Delete copy constructor and copy assignment operator
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;

        void enqueueTask(std::function<void()> task);

    private:
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;

        std::mutex _queue_mutex;
        std::condition_variable _condition;
        std::atomic<bool> _stop;

        void workerThread();
    };
}
