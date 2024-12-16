/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** MessageQueue.hpp
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

namespace client
{
    template <typename T>
    class MessageQueue
    {
    private:
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;

    public:
        void push(const T& message)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                queue.push(message);
            }
            cv.notify_one();
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return !queue.empty(); });
            T message = queue.front();
            queue.pop();
            return message;
        }

        bool empty()
        {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.empty();
        }
    };
} // namespace client
