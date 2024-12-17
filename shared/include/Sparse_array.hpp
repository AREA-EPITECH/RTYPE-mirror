#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <utility>

template <typename Component>
class Sparse_array {
public:
    using value_type = std::optional<Component>;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

public:
    Sparse_array() = default;
    Sparse_array(Sparse_array const&) = default;
    Sparse_array(Sparse_array&&) noexcept = default;
    ~Sparse_array() = default;
    Sparse_array& operator=(Sparse_array const&) = default;
    Sparse_array& operator=(Sparse_array&&) noexcept = default;

    reference_type operator[](size_t idx) {
        resize(idx);
        return _data[idx];
    }

    const_reference_type operator[](size_t idx) const {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return _data[idx];
    }

    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }
    size_type size() const { return _data.size(); }

    reference_type insert_at(size_type pos, Component const& value) {
        resize(pos);
        _data[pos] = value;
        return _data[pos];
    }

    reference_type insert_at(size_type pos, Component&& value) {
        resize(pos);
        _data[pos] = std::move(value);
        return _data[pos];
    }

    template <class... Params>
    reference_type emplace_at(size_type pos, Params&&... params) {
        resize(pos);
        _data[pos].emplace(std::forward<Params>(params)...);
        return _data[pos];
    }

    void erase(size_type pos) {
        if (pos >= _data.size()) {
            return;
        }
        _data[pos].reset();
    }

    size_type get_index(value_type const& value) const {
        for (size_t i = 0; i < _data.size(); ++i) {
            if (_data[i] == value) {
                return i;
            }
        }
        throw std::runtime_error("Value not found");
    }

    void clear() {
        _data.clear();
    }

private:
    container_t _data;

    void resize(size_type pos) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
    }
};