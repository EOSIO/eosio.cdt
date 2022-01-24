// copyright defined in LICENSE.txt

#pragma once
#include <eosio/datastream.hpp>
#include <string_view>

namespace eosio {

/// Tag objects which share memory with streams or with other things. These reduce
/// deserialization overhead, but require the source memory isn't freed and remains untouched.
template <typename T>
struct shared_memory {
   T value = {};

   T&       operator*() { return value; }
   const T& operator*() const { return value; }
   T*       operator->() { return &value; }
   const T* operator->() const { return &value; }
};

/// \exclude
template <typename Stream>
struct shared_memory<datastream<Stream>> {
   datastream<Stream> value = { nullptr, 0 };

   datastream<Stream>&       operator*() { return value; }
   const datastream<Stream>& operator*() const { return value; }
   datastream<Stream>*       operator->() { return &value; }
   const datastream<Stream>* operator->() const { return &value; }
};

template <typename Stream>
inline datastream<Stream>& operator>>(datastream<Stream>& ds, shared_memory<datastream<Stream>>& dest) {
   unsigned_int size;
   ds >> size;
   dest.value = datastream<Stream>{ ds.pos(), size };
   ds.skip(size);
   return ds;
}

template <typename Stream1, typename Stream2>
inline datastream<Stream1>& operator<<(datastream<Stream1>& ds, const shared_memory<datastream<Stream2>>& obj) {
   unsigned_int size = obj.value.remaining();
   ds << size;
   ds.write(obj.value.pos(), size);
   return ds;
}

template <typename Stream>
inline datastream<Stream>& operator>>(datastream<Stream>& ds, shared_memory<std::string_view>& dest) {
   unsigned_int size;
   ds >> size;
   dest.value = std::string_view{ ds.pos(), size };
   ds.skip(size);
   return ds;
}

template <typename Stream>
inline datastream<Stream>& operator<<(datastream<Stream>& ds, const shared_memory<std::string_view>& obj) {
   unsigned_int size = obj.value.size();
   ds << size;
   ds.write(obj.value.begin(), size);
   return ds;
}

}; // namespace eosio
