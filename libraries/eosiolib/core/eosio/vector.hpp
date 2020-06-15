#pragma once
#include <algorithm>
#include <stdexcept>
#include <new>
#include <utility>

/*
 * TODO make everything const expr
 */

namespace eosio {
    template<class T>
    class vector {
    public:

        constexpr vector() : m_size(1), m_capacity(1) {
            m_data = std::unique_ptr<T[]>(new T[m_size]);
            m_data[0] = {};
        }

        constexpr vector(size_t size) : m_size(size), m_capacity(size) {
            m_data = std::unique_ptr<T[]>(new T[m_size]);
            for (auto i = 0; i < size; i++) {
                m_data[i] = {};
            }
        }

        constexpr vector(size_t size, const T& value) : m_size(size), m_capacity(size) {
            m_data = std::unique_ptr<T[]>(new T[m_size]);
            for(auto i = 0; i < size; i++) {
                m_data[i] = value;
            }
        }

        constexpr vector(const vector& v) {
            m_size = v.m_size;
            m_capacity = v.m_capacity;
            m_data = std::unique_ptr<T[]>(new T[m_size]);
            memcpy(m_data.get(), v.m_data.get(), sizeof(T) * m_size);
        }

        constexpr vector(vector&& v) {
            m_size = v.m_size;
            m_capacity = v.m_capacity;
            m_data = std::move(v.m_data);
        }

        // replace initializer list with VARGS - template parameter pack
        constexpr vector(std::initializer_list<T> list) {
            m_size = list.size();
            m_capacity = list.size();
            m_data = std::unique_ptr<T[]>(new T[m_size]);
            memcpy(m_data.get(), list.begin(), sizeof(T) * m_size);
        }

        constexpr vector& operator=(const vector& v) {
            reserve(v.m_size);
            m_size = v.m_size;
            m_capacity = v.m_capacity;
            memcpy(m_data.get(), v.m_data.get(), sizeof(T) * m_size);
            return *this;
        }

        constexpr void assign(size_t count, const T& value) {
            resize(count);
            for(auto i = 0; i < count; i++) {
                m_data[i] = value;
            }
        }

        constexpr T& at(uint64_t index) {
            eosio::check(index < m_size, "out_of_range");
            return m_data[index];
        }

        constexpr const T& at(uint64_t index) const {
            eosio::check(index < m_size, "out_of_range");
            return m_data[index];
        }

        constexpr T& operator[](uint64_t index) {
            return m_data[index];
        }

        constexpr const T& operator[](uint64_t index) const {
            return m_data[index];
        }

        constexpr T& front() {
            return m_data[0];
        }

        constexpr const T& front() const {
            return m_data[0];
        }

        constexpr T& back() {
            return m_data[m_size - 1];
        }

        constexpr const T& back() const {
            return m_data[m_size - 1];
        }

        constexpr T* data() {
            return m_data.get();
        }

        constexpr const T* data() const {
            return m_data.get();
        }

        using iterator = T*;
        using const_iterator = const T*;

        constexpr iterator begin() {
            return m_data.get();
        }

        constexpr const_iterator cbegin() const {
            return m_data.get();
        }

        constexpr iterator end() {
            return m_data.get() + m_size;
        }

        constexpr const_iterator cend() const {
            return m_data.get() + m_size;
        }

        constexpr iterator rbegin() {
            return m_data.get() + (m_size-1);
        }

        constexpr const_iterator crbegin() const {
            return m_data.get() + (m_size-1);
        }

        constexpr iterator rend() {
            return m_data.get() - 1;
        }

        constexpr const_iterator crend() const {
            return m_data.get() - 1;
        }

        constexpr const bool empty() const {
            return m_size == 0;
        }

        constexpr const size_t size() const {
            return m_size;
        }

        constexpr void reserve(size_t capacity) {
            if(capacity > m_capacity) {
                auto new_data = std::unique_ptr<T[]>(new T[capacity]);
                memcpy(new_data.get(), m_data.get(), sizeof(T) * m_size);
                m_data = std::move(new_data);
                m_capacity = capacity;
            }
        }

        constexpr const size_t capacity() {
            return m_capacity;
        }

        constexpr void shrink_to_fit() {
            if(m_capacity > m_size) {
                auto tmp = std::unique_ptr<T[]>(new T[m_size]);
                memcpy(tmp.get(), m_data.get(), sizeof(T) * m_size);
                m_capacity = m_size;
                m_data = std::move(tmp);
            }
        }

        constexpr void clear() {
            m_size = 0;
        }

        constexpr void insert(iterator position, const T& value) {
            auto index = position - begin();
            if(m_size >= m_capacity) {
                reserve(m_capacity * 2);
            }
            shift(index);
            m_data[index] = value;
            ++m_size;
        }

        template<typename... Args>
        constexpr void emplace(iterator position, Args&&... args) {
            auto index = position - begin();
            if(m_size >= m_capacity) {
                reserve(m_capacity * 2);
            }
            shift(index);
            new(begin()+index)T(std::forward<Args&&>(args)...);
            ++m_size;
        }

        constexpr void erase(iterator position) {
            --m_size;
            auto index = position - begin();
            auto tmp = std::unique_ptr<T[]>(new T[m_capacity]);
            memcpy(tmp.get(), begin(), sizeof(T) * index);
            for(auto i = index; i < m_size; i++) {
                tmp[i] = m_data[i+1];
            }
            m_data = std::move(tmp);
        }

        constexpr void push_back(const T& value) {
            if(m_size >= m_capacity) {
                reserve(m_capacity * 2);
            }
            m_data[m_size++] = value;
        }

        template<typename... Args>
        constexpr void emplace_back(Args&&... args) {
            if(m_size >= m_capacity) {
                reserve(m_capacity * 2);
            }
            new(end())T(std::forward<Args&&>(args)...);
            m_size++;
        }

        constexpr void pop_back() {
            --m_size;
        }

        constexpr void resize(size_t size) {
            reserve(size);
            m_size = size;
        }

        constexpr void swap(vector& v) {
            size_t tmp_size = m_size;
            size_t tmp_capacity = m_capacity;
            auto tmp_data = std::move(m_data);

            m_size = v.m_size;
            m_capacity = v.m_capacity;
            m_data = std::move(v.m_data);

            v.m_size = tmp_size;
            v.m_capacity = tmp_capacity;
            v.m_data = std::move(tmp_data);
        }

    private:
        constexpr void shift(uint64_t index) {
            auto tmp = std::unique_ptr<T[]>(new T[m_capacity]);
            memcpy(tmp.get(), begin(), sizeof(T) * index);
            for(auto i = index; i < m_size; i++) {
                tmp[i+1] = m_data[i];
            }
            tmp[index] = {};
            m_data = std::move(tmp);
        }

        size_t m_capacity{0};
        size_t m_size{0};
        std::unique_ptr<T[]> m_data;
    };
} // namespace eosio
