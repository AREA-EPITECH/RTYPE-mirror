/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** PeerWrapper.hpp
*/

#include <any>
#include <enet/enet.h>
#include <memory>

namespace network
{
    class PeerWrapper
    {
    private:
        std::shared_ptr<ENetPeer> _peer;
        std::any _data;

    public:
        PeerWrapper(ENetPeer *rawPeer) :
            _peer(rawPeer, [](ENetPeer *p) {}) {};

        std::shared_ptr<ENetPeer> getPeer() const { return _peer; }

        template <typename T>
        void setData(T&& data) {
            _data = std::forward<T>(data);
        }

        template <typename T>
        T getData() const {
            return std::any_cast<T>(_data);
        }

        bool hasData() const { return _data.has_value(); }
    };

} // namespace network
