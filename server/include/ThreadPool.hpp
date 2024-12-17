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

namespace server {
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t thread_count);
        ~ThreadPool();

        void enqueueTask(std::function<void()> task);

    private:
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;

        std::mutex _queue_mutex;
        std::condition_variable _condition;
        bool _stop;

        void workerThread();
    };
}
