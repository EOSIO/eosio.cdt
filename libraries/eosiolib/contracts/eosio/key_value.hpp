#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/print.hpp"
#include "../../core/eosio/utility.hpp"
#include "../../core/eosio/varint.hpp"

#include <algorithm>
#include <cctype>
#include <functional>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/pfr.hpp>

#define EOSIO_CDT_KV_INDEXnullptr

#define EOSIO_CDT_KV_INDEX_TEST() 1
#define EOSIO_CDT_KV_INDEX_TEST_EOSIO_CDT_KV_INDEX_TEST 0,
#define EOSIO_CDT_KV_INDEX_TEST_1 1, ignore
#define EOSIO_CDT_EXPAND(x) x
#define EOSIO_CDT_CAT2(x, y) x ## y
#define EOSIO_CDT_CAT(x, y) EOSIO_CDT_CAT2(x, y)
#define EOSIO_CDT_APPLY(f, args) f args

#define EOSIO_CDT_KV_FIX_INDEX_NAME_0(index_name, i) index_name
#define EOSIO_CDT_KV_FIX_INDEX_NAME_1(index_name, i) index_name ## i
#define EOSIO_CDT_KV_FIX_INDEX_NAME(x, i) EOSIO_CDT_KV_FIX_INDEX_NAME_ ## x

#define EOSIO_CDT_KV_FIX_INDEX_TYPE_0(index_name) kv_non_unique_index
#define EOSIO_CDT_KV_FIX_INDEX_TYPE_1(index_name) null_kv_index
#define EOSIO_CDT_KV_FIX_INDEX_TYPE(iskeyword, garbage) EOSIO_CDT_KV_FIX_INDEX_TYPE_ ## iskeyword

#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_0(value_class, index_name) {&value_class::index_name}
#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_1(value_class, index_name)
#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT(iskeyword, garbage) EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_ ## iskeyword

#define EOSIO_CDT_KV_INDEX_NAME(index_name, i)                                                                         \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_NAME,                                                                        \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX ## index_name ()))))(index_name, i)

#define EOSIO_CDT_KV_INDEX_TYPE(index_name)                                                                            \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_TYPE,                                                                        \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX ## index_name ()))))(index_name)

#define EOSIO_CDT_KV_INDEX_CONSTRUCT(value_class, index_name)                                                          \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT,                                                                   \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX ## index_name ()))))(value_class, index_name)


#define EOSIO_CDT_CREATE_KV_NON_UNIQUE_INDEX(r, value_class, i, index_name)                                            \
   EOSIO_CDT_KV_INDEX_TYPE(index_name) EOSIO_CDT_KV_INDEX_NAME(index_name, i) EOSIO_CDT_KV_INDEX_CONSTRUCT(value_class, index_name);

#define EOSIO_CDT_CREATE_KV_UNIQUE_INDEX(r, value_class, i, index_name)                                                \
   kv_unique_index EOSIO_CDT_KV_INDEX_NAME(index_name, i) EOSIO_CDT_KV_INDEX_CONSTRUCT(value_class, index_name);

#define EOSIO_CDT_CREATE_KV_INDEX(r, value_class, i, index_name)                                                       \
   BOOST_PP_IF(i, EOSIO_CDT_CREATE_KV_NON_UNIQUE_INDEX, EOSIO_CDT_CREATE_KV_UNIQUE_INDEX)(r, value_class, i, index_name)

#define EOSIO_CDT_LIST_INDICES(value_class, ...)                                                                       \
   BOOST_PP_SEQ_FOR_EACH_I(EOSIO_CDT_CREATE_KV_INDEX, value_class, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

/**
 * @brief Macro to define a table.
 * @details The resulting table will have a member `index` that has fields on it that match 1-1 with the names of the
 * fields passed into the list. See example for further clarification.
 *
 * @param table_class - The name of the class of the user defined table that inherits from eosio::kv_table
 * @param value_class - The name of the class of the data stored as the value of the table
 * @param table_name  - The name of the table
 * @param db_name     - The type of the EOSIO Key Value database. Defaulted to eosio.kvram
 * @param ...         - A variadic list of 1 or more indexes to define on the table.
 */
#define DEFINE_TABLE(table_class, value_class, table_name, db_name, /*indices*/...)                                    \
   struct table_class : eosio::kv_table<value_class> {                                                                 \
      struct {                                                                                                         \
         EOSIO_CDT_LIST_INDICES(value_class, __VA_ARGS__)                                                              \
      } index;                                                                                                         \
                                                                                                                       \
      table_class(eosio::name contract_name) {                                                                         \
         init(contract_name, table_name##_n, db_name##_n, &index);                                                     \
      }                                                                                                                \
   };

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         void kv_erase(uint64_t db, uint64_t contract, const char* key, uint32_t key_size);

         __attribute__((eosio_wasm_import))
         void kv_set(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size);

         __attribute__((eosio_wasm_import))
         bool kv_get(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_get_data(uint64_t db, uint32_t offset, char* data, uint32_t data_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_it_create(uint64_t db, uint64_t contract, const char* prefix, uint32_t size);

         __attribute__((eosio_wasm_import))
         void kv_it_destroy(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_status(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_compare(uint32_t itr_a, uint32_t itr_b);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key_compare(uint32_t itr, const char* key, uint32_t size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_move_to_end(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_next(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_prev(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_lower_bound(uint32_t itr, const char* key, uint32_t size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_value(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);
      }
   }

namespace detail {
   constexpr inline size_t max_stack_buffer_size = 512;
}

/**
 * The key_type struct is used to store the binary representation of a key.
 */
struct key_type : private std::string {
   key_type() : std::string() {}
   key_type(const char* c, size_t s) : std::string(c, s) {}


   key_type operator+(const key_type& b) const {
      size_t buffer_size = size() + b.size();
      void* buffer = buffer_size > detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);

      memcpy(buffer, data(), size());
      memcpy(((char*)buffer) + size(), b.data(), b.size());
      return {(char*)buffer, buffer_size};
   }

   bool operator==(const key_type& b) const {
      if (size() != b.size()) {
         return false;
      }
      return memcmp(data(), b.data(), b.size()) == 0;
   }

   using std::string::data;
   using std::string::size;
};

/* @cond PRIVATE */
inline key_type make_prefix(eosio::name table_name, eosio::name index_name, uint8_t status = 1) {
   auto bige_table = swap_endian<uint64_t>(table_name.value);
   auto bige_index = swap_endian<uint64_t>(index_name.value);

   size_t size_name = sizeof(index_name);

   size_t buffer_size = (2 * size_name) + sizeof(status);
   void* buffer = buffer_size > detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);

   memcpy(buffer, &status, sizeof(status));
   memcpy(((char*)buffer) + sizeof(status), &bige_table, size_name);
   memcpy(((char*)buffer) + sizeof(status) + size_name, &bige_index, size_name);

   key_type s((const char*)buffer, buffer_size);

   if (buffer_size > detail::max_stack_buffer_size) {
      free(buffer);
   }

   return s;
}

inline key_type table_key(const key_type& prefix, const key_type& key) {
   return prefix + key;
}

template <typename I>
inline I flip_msb(I val) {
   constexpr static size_t bits = sizeof(I) * 8;
   return val ^ (static_cast<I>(1) << (bits - 1));
}

template <typename I>
inline I get_msb(I val) {
   constexpr static size_t bits = sizeof(I) * 8;
   return val >> (bits - 1);
}

template <typename I, typename std::enable_if_t<std::is_integral<I>::value, int> = 0>
inline key_type make_key(I val) {
   if (std::is_signed<I>::value) {
      val = flip_msb(val);
   }

   auto big_endian = swap_endian<I>(val);

   const char* bytes = reinterpret_cast<char*>(&big_endian);
   constexpr size_t size = sizeof(big_endian);
   key_type s(bytes, size);
   return s;
}

template <typename I, typename F>
inline key_type make_floating_key(F val) {
   if (val == -0) {
      val = +0;
   }

   auto* ival = reinterpret_cast<I*>(&val);
   I bit_val;
   auto msb = get_msb(*ival);
   if (msb) {
      // invert all the bits
      bit_val = ~(*ival);
   } else {
      // invert just msb
      bit_val = flip_msb(*ival);
   }

   auto big_endian = swap_endian<I>(bit_val);

   const char* bytes = reinterpret_cast<char*>(&big_endian);
   constexpr size_t size = sizeof(big_endian);
   key_type s(bytes, size);
   return s;
}

inline key_type make_key(float val) {
   return make_floating_key<uint32_t>(val);
}

inline key_type make_key(double val) {
   return make_floating_key<uint64_t>(val);
}

inline key_type make_key(const char* val, size_t size, bool case_insensitive=false) {
   size_t num_zeroes = std::count(val, val+size, 0);

   size_t data_size = size + num_zeroes + 2;
   char* data_buffer = data_size > detail::max_stack_buffer_size ? (char*)malloc(data_size) : (char*)alloca(data_size);

   size_t j = 0;
   for(size_t i = 0; i < size; ++i) {
      if (val[i] == 0x00) {
         data_buffer[j++] = 0x00;
         data_buffer[j++] = 0x01;
      } else {
         if(case_insensitive) {
            data_buffer[j++] = std::toupper(val[i]);
         } else {
            data_buffer[j++] = val[i];
         }
      }
   }

   data_buffer[data_size - 2] = 0x00;
   data_buffer[data_size - 1] = 0x00;

   key_type s(data_buffer, data_size);

   if (data_size > detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return s;
}

inline key_type make_key(const std::string& val, bool case_insensitive=false) {
   return make_key(val.data(), val.size(), case_insensitive);
}

inline key_type make_key(const std::string_view& val, bool case_insensitive=false) {
   return make_key(val.data(), val.size(), case_insensitive);
}

inline key_type make_key(const char* str, bool case_insensitive=false) {
   return make_key(std::string_view{str}, case_insensitive);
}

inline key_type make_key(eosio::name n) {
   return make_key(n.value);
}

inline key_type make_key(key_type&& val) {
   return val;
}

inline key_type make_key(const key_type& val) {
   return val;
}

template <typename S, typename std::enable_if_t<std::is_class<S>::value, int> = 0>
inline key_type make_key(const S& val) {
   size_t data_size = 0;
   size_t pos = 0;
   void* data_buffer;

   boost::pfr::for_each_field(val, [&](auto& field) {
      data_size += sizeof(field);
   });

   data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

   boost::pfr::for_each_field(val, [&](auto& field) {
      auto kt = make_key(field);
      memcpy((char*)data_buffer + pos, kt.data(), kt.size());
      pos += kt.size();
   });

   key_type s((const char*)data_buffer, data_size);

   if (data_size > detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return s;
}

template <typename... Args>
inline key_type make_key(const std::tuple<Args...>& val) {
   size_t data_size = 0;
   size_t pos = 0;
   void* data_buffer;

   boost::fusion::for_each(val, [&](auto& field) {
      data_size += sizeof(field);
   });

   data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

   boost::fusion::for_each(val, [&](auto& field) {
      auto kt = make_key(field);
      memcpy((char*)data_buffer + pos, kt.data(), kt.size());
      pos += kt.size();
   });

   key_type s((const char*)data_buffer, data_size);

   if (data_size > detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return s;
}
/* @endcond */

// This is the "best" way to document a function that does not technically exist using Doxygen.
#if EOSIO_CDT_DOXYGEN
/**
 * @brief A function for converting types to the appropriate binary representation for the EOSIO Key Value database.
 * @details The CDT provides implementations of this function for many of the common primitives and for structs/tuples.
 * If sticking with standard types, contract developers should not need to interact with this function.
 * If doing something more advanced, contract developers may need to provide their own implementation for a special type.
 */
template <typename T>
inline key_type make_key(T val) {
   return {};
}
#endif


/**
 * Used to return the appropriate representation of a case insensitive string for the EOSIO Key Value database.
 *
 * @param val - The string to be made case-insensitive
 * @return The binary representation of the case-insensitive string
 */
inline key_type make_insensitive(const std::string& val) {
   return make_key(val, true);
}


/**
 * @defgroup keyvalue Key Value Table
 * @ingroup contracts
 *
 * @brief Defines an EOSIO Key Value Table
 * @details EOSIO Key Value API provides a C++ interface to the EOSIO Key Value database.
 * Key Value Tables require 1 primary index, of any type that can be serialized to a binary representation.
 * Key Value Tables support 0 or more secondary index, of any type that can be serialized to a binary representation.
 * Indexes must be a member variable or a member function.
 *
 * @tparam Class     - the name of the class of the user defined table that inherits from eosio::kv_table
 * @tparam T         - the type of the data stored as the value of the table
 * @tparam TableName - the name of the table
 * @tparam DbName    - the type of the EOSIO Key Value database. Defaulted to eosio.kvram
  */
template<typename T>
class kv_table {

   enum class kv_it_stat {
      iterator_ok     = 0,  // Iterator is positioned at a key-value pair
      iterator_erased = -1, // The key-value pair that the iterator used to be positioned at was erased
      iterator_end    = -2, // Iterator is out-of-bounds
   };

public:
   /**
    * @ingroup keyvalue
    *
    * @brief Defines an index on an EOSIO Key Value Table
    * @details A Key Value Index allows a user of the table to search based on a given field.
    * The only restrictions on that field are that it is serializable to a binary representation.
    * Convenience functions exist to handle most of the primitive types as well as some more complex types, and are
    * used automatically where possible.
    */
   class kv_index {
      class iterator {
      public:
         iterator(eosio::name contract_name, uint32_t itr, kv_it_stat itr_stat, kv_index* idx) :
                  contract_name{contract_name}, itr{itr}, itr_stat{itr_stat}, idx{idx} {}

         iterator(iterator&& other) :
            contract_name(std::move(other.contract_name)),
            itr(std::exchange(other.itr, 0)),
            itr_stat(std::move(other.itr_stat)),
            idx(std::exchange(other.idx, nullptr))
         {}

         ~iterator() {
            if (itr) {
               internal_use_do_not_use::kv_it_destroy(itr);
            }
         }

         iterator& operator=(iterator&& other) {
            contract_name = std::move(other.contract_name);
            itr = std::exchange(other.itr, itr);
            itr_stat = std::move(other.itr_stat);
            idx = std::exchange(other.idx, nullptr);

            return *this;
         }

         /**
          * Returns the value that the iterator points to.
          * @ingroup keyvalue
          *
          * @return The value that the iterator points to.
          */
         T value() const {
            using namespace detail;

            eosio::check(itr_stat != kv_it_stat::iterator_end, "Cannot read end iterator");

            uint32_t value_size;
            uint32_t actual_value_size;
            uint32_t offset = 0;

            // call once to get the value_size
            internal_use_do_not_use::kv_it_value(itr, 0, (char*)nullptr, 0, value_size);

            void* buffer = value_size > detail::max_stack_buffer_size ? malloc(value_size) : alloca(value_size);
            auto stat = internal_use_do_not_use::kv_it_value(itr, offset, (char*)buffer, value_size, actual_value_size);

            eosio::check(static_cast<kv_it_stat>(stat) != kv_it_stat::iterator_end, "Error reading value");

            void* deserialize_buffer = buffer;
            size_t deserialize_size = actual_value_size;

            if (idx->name != idx->tbl->primary_index->name) {
               uint32_t actual_data_size;
               auto success = internal_use_do_not_use::kv_get(idx->tbl->db_name, contract_name.value, (char*)buffer, actual_value_size, actual_data_size);
               eosio::check(success, "failure getting primary key");

               void* pk_buffer = actual_data_size > detail::max_stack_buffer_size ? malloc(actual_data_size) : alloca(actual_data_size);
               internal_use_do_not_use::kv_get_data(idx->tbl->db_name, 0, (char*)pk_buffer, actual_data_size);

               deserialize_buffer = pk_buffer;
               deserialize_size = actual_data_size;
            }

            T val;
            deserialize(val, deserialize_buffer, deserialize_size);
            return val;
         }

         iterator& operator++() {
            eosio::check(itr_stat != kv_it_stat::iterator_end, "cannot increment end iterator");
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_next(itr));
            return *this;
         }

         iterator& operator--() {
            if (!itr) {
               itr = internal_use_do_not_use::kv_it_create(idx->tbl->db_name, contract_name.value, idx->prefix.data(), idx->prefix.size());
            }
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_prev(itr));
            eosio::check(itr_stat != kv_it_stat::iterator_end, "decremented past the beginning");
            return *this;
         }

         bool operator==(const iterator& b) const {
            return compare(b) == 0;
         }

         bool operator!=(const iterator& b) const {
            return compare(b) != 0;
         }

         bool operator<(const iterator& b) const {
            return compare(b) < 0;
         }

         bool operator<=(const iterator& b) const {
            return compare(b) <= 0;
         }

         bool operator>(const iterator& b) const {
            return compare(b) > 0;
         }

         bool operator>=(const iterator& b) const {
            return compare(b) >= 0;
         }

      private:
         eosio::name contract_name;
         const kv_index* idx;

         uint32_t itr;
         kv_it_stat itr_stat;

         key_type key() const {
            return idx->get_key(value());
         }

         int compare(const iterator& b) const {
            bool a_is_end = !itr || itr_stat == kv_it_stat::iterator_end;
            bool b_is_end = !b.itr || b.itr_stat == kv_it_stat::iterator_end;
            if (a_is_end && b_is_end) {
               return 0;
            } else if (a_is_end && b.itr) {
               return 1;
            } else if (itr && b_is_end) {
               return -1;
            } else {
               return internal_use_do_not_use::kv_it_compare(itr, b.itr);
            }
         }

         friend kv_index;
      };

   public:
      eosio::name name{0};
      eosio::name table_name;
      eosio::name contract_name;

      kv_index() = default;

      template <typename KF>
      kv_index(KF&& kf) {
         key_function = [=](const T& t) {
            return make_key(std::invoke(kf, &t));
         };
      }

      template <typename KF>
      kv_index(eosio::name name, KF&& kf) : name{name} {
         key_function = [=](const T& t) {
            return make_key(std::invoke(kf, &t));
         };
      }

      /**
       * Returns an iterator pointing to the element with the lowest key greater than or equal to the given key.
       * @ingroup keyvalue
       *
       * @return An iterator pointing to the element with the lowest key greater than or equal to the given key.
       */
      template <typename K>
      iterator lower_bound(K&& key) {
         auto t_key = table_key(prefix, make_key(std::forward<K>(key)));

         uint32_t itr = internal_use_do_not_use::kv_it_create(tbl->db_name, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         if (static_cast<kv_it_stat>(itr_stat) == kv_it_stat::iterator_end) {
            internal_use_do_not_use::kv_it_destroy(itr);
            return end();
         }

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), this};
      }

      /**
       * Returns an iterator pointing to the first element greater than the given key.
       * @ingroup keyvalue
       *
       * @return An iterator pointing to the element with the highest key less than or equal to the given key.
       */
      template <typename K>
      iterator upper_bound(K&& key) {
         auto t_key = table_key(prefix, make_key(std::forward<K>(key)));

         uint32_t itr = internal_use_do_not_use::kv_it_create(tbl->db_name, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         iterator it{contract_name, itr, static_cast<kv_it_stat>(itr_stat), this};

         auto cmp = internal_use_do_not_use::kv_it_key_compare(it.itr, t_key.data(), t_key.size());
         if (cmp == 0) {
            ++it;
         }

         return it;
      }

      /**
       * Returns an iterator referring to the `past-the-end` element. It does not point to any element, therefore `value` should not be called on it.
       * @ingroup keyvalue
       *
       * @return An iterator referring to the `past-the-end` element.
       */
      iterator end() {
         return {contract_name, 0, kv_it_stat::iterator_end, this};
      }

      /**
       * Returns an iterator to the object with the lowest key (by this index) in the table.
       * @ingroup keyvalue
       *
       * @return An iterator to the object with the lowest key (by this index) in the table.
       */
      iterator begin() {
         uint32_t itr = internal_use_do_not_use::kv_it_create(tbl->db_name, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, "", 0);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), this};
      }

      /**
       * Returns a vector of objects that fall between the specifed range. The range is inclusive, exclusive.
       * @ingroup keyvalue
       *
       * @tparam K - The type of the key. This will be auto-deduced by the key param.
       *
       * @param begin - The beginning of the range.
       * @param end - The end of the range.
       * @return A vector containing all the objects that fall between the range.
       */
      template <typename K>
      std::vector<T> range(K&& b, K&& e) {
         auto begin_itr = lower_bound(std::forward<K>(b));
         auto end_itr = lower_bound(std::forward<K>(e));

         if (begin_itr == end_itr || begin_itr > end_itr) {
            return {};
         }

         std::vector<T> return_values;

         iterator itr = std::move(begin_itr);
         while(itr < end_itr) {
            return_values.push_back(itr.value());
            ++itr;
         }

         return return_values;
      }

      virtual bool is_unique() = 0;

   protected:
      key_type get_key(const T& inst) const { return key_function(inst); }
      kv_table* tbl;
      key_type prefix;

   private:
      friend kv_table;

      std::function<key_type(const T&)> key_function;

      void set_prefix() {
         prefix = make_prefix(table_name, name);
      }
   };

   using iterator = typename kv_index::iterator;

   class kv_unique_index : public kv_index {
   public:
      kv_unique_index() = default;

      template <typename KF>
      kv_unique_index(KF&& kf): kv_index{kf} {}

      template <typename KF>
      kv_unique_index(eosio::name name, KF&& kf) : kv_index{name, kf} {}

      /**
       * Search for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @tparam K - The type of the key. This will be auto-deduced by the key param.
       *
       * @param key - The key to search for.
       * @return An iterator to the found object OR the `end` iterator if the given key was not found.
       */
      template <typename K>
      iterator find(K&& key) {
         auto t_key = table_key(prefix, make_key(std::forward<K>(key)));

         uint32_t itr = internal_use_do_not_use::kv_it_create(tbl->db_name, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         auto cmp = internal_use_do_not_use::kv_it_key_compare(itr, t_key.data(), t_key.size());

         if (cmp != 0) {
            internal_use_do_not_use::kv_it_destroy(itr);
            return this->end();
         }

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), this};
      }

      /**
       * Get the value for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @tparam K - The type of the key. This will be auto-deduced by the key param.
       *
       * @param key - The key to search for.
       * @return A std::optional of the value corresponding to the key.
       */
      template <typename K>
      std::optional<T> get(K&& key) {
         uint32_t value_size;
         std::optional<T> ret_val;

         auto t_key = table_key(prefix, make_key(std::forward<K>(key)));

         auto success = internal_use_do_not_use::kv_get(tbl->db_name, contract_name.value, t_key.data(), t_key.size(), value_size);
         if (!success) {
            return ret_val;
         }

         void* buffer = value_size > detail::max_stack_buffer_size ? malloc(value_size) : alloca(value_size);
         auto copy_size = internal_use_do_not_use::kv_get_data(tbl->db_name, 0, (char*)buffer, value_size);

         void* deserialize_buffer = buffer;
         size_t deserialize_size = copy_size;

         if (this->name != tbl->primary_index->name) {
            uint32_t actual_data_size;
            auto success = internal_use_do_not_use::kv_get(tbl->db_name, contract_name.value, (char*)buffer, copy_size, actual_data_size);
            eosio::check(success, "failure getting primary key");

            void* pk_buffer = actual_data_size > detail::max_stack_buffer_size ? malloc(actual_data_size) : alloca(actual_data_size);
            auto pk_copy_size = internal_use_do_not_use::kv_get_data(tbl->db_name, 0, (char*)pk_buffer, actual_data_size);

            deserialize_buffer = pk_buffer;
            deserialize_size = pk_copy_size;
         }

         ret_val.emplace();
         deserialize(*ret_val, deserialize_buffer, deserialize_size);

         if (value_size > detail::max_stack_buffer_size) {
            free(buffer);
         }

         return ret_val;
      }

      bool is_unique() override {
         return true;
      }

      using kv_table<T>::kv_index::tbl;
      using kv_table<T>::kv_index::contract_name;
      using kv_table<T>::kv_index::prefix;
   };

   class kv_non_unique_index : public kv_index {
   public:
      kv_non_unique_index() = default;

      template <typename KF>
      kv_non_unique_index(KF&& kf): kv_index{kf} {}

      template <typename KF>
      kv_non_unique_index(eosio::name name, KF&& kf) : kv_index{name, kf} {}

      using kv_table<T>::kv_index::tbl;

      bool is_unique() override {
         return false;
      }
   };


   /**
    * @ingroup keyvalue
    *
    * @brief Defines a deleted index on an EOSIO Key Value Table
    * @details Due to the way indexes are named, when deleting an index a "placeholder" index needs to be created instead.
    * A null_kv_index should be created in this case. If using DEFINE_TABLE, just passing in nullptr will handle this.
    */
   class null_kv_index : public kv_index {
   public:
      template <typename KF>
      null_kv_index(KF&& kf): kv_index{kf} {}

      template <typename KF>
      null_kv_index(eosio::name name, KF&& kf) : kv_index{name, kf} {}

      bool is_unique() override {
         return false;
      }
   };

   /**
    * Puts a value into the table. If the value already exists, it updates the existing entry.
    * The key is determined from the defined primary index.
    * @ingroup keyvalue
    *
    * @param value - The entry to be stored in the table.
    */
   void put(const T& value) {
      uint32_t value_size;
      T old_value;

      auto primary_key = primary_index->get_key(value);
      auto tbl_key = table_key(make_prefix(table_name, primary_index->name), primary_key);
      auto primary_key_found = internal_use_do_not_use::kv_get(db_name, contract_name.value, tbl_key.data(), tbl_key.size(), value_size);

      if (primary_key_found) {
         void* buffer = value_size > detail::max_stack_buffer_size ? malloc(value_size) : alloca(value_size);
         auto copy_size = internal_use_do_not_use::kv_get_data(db_name, 0, (char*)buffer, value_size);

         deserialize(old_value, buffer, copy_size);

         if (value_size > detail::max_stack_buffer_size) {
            free(buffer);
         }
      }

      for (const auto& idx : secondary_indices) {
         key_type sec_key;

         if (idx->is_unique()) {
            sec_key = idx->get_key(value);
            auto sec_tbl_key = table_key(make_prefix(table_name, idx->name), sec_key);
            auto sec_found = internal_use_do_not_use::kv_get(db_name, contract_name.value, sec_tbl_key.data(), sec_tbl_key.size(), value_size);

            if (!primary_key_found) {
               eosio::check(!sec_found, "Attempted to store an existing unique secondary index.");
            } else if (sec_found) {
               void* buffer = value_size > detail::max_stack_buffer_size ? malloc(value_size) : alloca(value_size);
               auto copy_size = internal_use_do_not_use::kv_get_data(db_name, 0, (char*)buffer, value_size);

               eosio::check(copy_size == tbl_key.size(), "Attempted to update an existing unique secondary index.");
               auto res = memcmp(buffer, tbl_key.data(), copy_size);
               eosio::check(res == 0, "Attempted to update an existing unique secondary index.");

               if (copy_size > detail::max_stack_buffer_size) {
                  free(buffer);
               }
            }

            if (primary_key_found) {
               auto old_sec_key = table_key(make_prefix(table_name, idx->name), idx->get_key(old_value));
               internal_use_do_not_use::kv_erase(db_name, contract_name.value, old_sec_key.data(), old_sec_key.size());
            }
         } else {
            if (primary_key_found) {
               auto old_sec_key = table_key(make_prefix(table_name, idx->name), idx->get_key(old_value) + primary_index->get_key(old_value));
               internal_use_do_not_use::kv_erase(db_name, contract_name.value, old_sec_key.data(), old_sec_key.size());
            }

            sec_key = idx->get_key(value) + primary_index->get_key(value);
         }

         auto sec_tbl_key = table_key(make_prefix(table_name, idx->name), sec_key);
         internal_use_do_not_use::kv_set(db_name, contract_name.value, sec_tbl_key.data(), sec_tbl_key.size(), tbl_key.data(), tbl_key.size());
      }

      size_t data_size = get_size(value);
      void* data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

      serialize(value, data_buffer, data_size);

      internal_use_do_not_use::kv_set(db_name, contract_name.value, tbl_key.data(), tbl_key.size(), (const char*)data_buffer, data_size);

      if (data_size > detail::max_stack_buffer_size) {
         free(data_buffer);
      }
   }

   /**
    * Removes a value from the table.
    * @ingroup keyvalue
    *
    * @tparam K - The type of the key. This will be auto-deduced through the key parameter.
    *
    * @param key - The key of the value to be removed.
    */
   template <typename K>
   void erase(const K& key) {
      auto primary_value = primary_index->get(key);

      if (!primary_value) {
         return;
      }

      auto k = table_key(make_prefix(table_name, primary_index->name), make_key(key));
      internal_use_do_not_use::kv_erase(db_name, contract_name.value, k.data(), k.size());

      for (auto& idx : secondary_indices) {
         key_type sk;

         if (idx->is_unique()) {
            sk = idx->get_key(*primary_value);
         } else {
            sk = idx->get_key(*primary_value) + make_key(key);
         }

         auto skey = table_key(make_prefix(table_name, idx->name), sk);
         internal_use_do_not_use::kv_erase(db_name, contract_name.value, skey.data(), skey.size());
      }
   }

protected:
   kv_table() = default;

   /**
    * Initializes a key value table. This method is intended to be called in the constructor of the user defined table class.
    * If using the DEFINE_TABLE macro, this is handled for the developer.
    * @ingroup keyvalue
    *
    * @tparam Indices - a list of types of the indices. This will be auto-deduced through the indices parameter.
    *
    * @param contract - the name of the contract this table is associated with
    * @param indices - a list of 1 or more indices to add to the table
    */
   template <typename Indices>
   void init(eosio::name contract, eosio::name table, eosio::name db, Indices indices) {
      contract_name = contract;
      table_name = table;
      db_name = db.value;

      bool is_named = false;
      uint64_t index_name = 1;

      auto& primary = get<0>(*indices);

      eosio::check(primary.is_unique(), "primary index should be kv_unique_index");

      primary_index = (kv_unique_index*)&primary;
      primary_index->contract_name = contract_name;
      primary_index->table_name = table_name;
      primary_index->tbl = this;

      if (primary_index->name.value > 0) {
         is_named = true;
      } else {
         primary_index->name = eosio::name{index_name};
      }

      primary_index->set_prefix();
      ++index_name;

      for_each_field(*indices, [&](auto& idx) {
         if (idx.name != primary.name) {
            kv_index* si = &idx;
            si->contract_name = contract_name;
            si->table_name = table_name;
            si->tbl = this;

            if (is_named) {
               eosio::check(si->name.value > 0, "All indices must be named if one is named.");
            } else {
               eosio::check(si->name.value <= 0, "All indices must be named if one is named.");
               si->name = eosio::name{index_name};
            }
            si->set_prefix();
            secondary_indices.push_back(si);
            ++index_name;
         }
      });

      // Makes sure the indexes are run in the correct order.
      // This is mainly useful for debugging, this probably could be deleted.
      std::reverse(std::begin(secondary_indices), std::end(secondary_indices));
   }

private:
   eosio::name contract_name;
   eosio::name table_name;
   uint64_t db_name;


   kv_unique_index* primary_index;
   std::vector<kv_index*> secondary_indices;

   template <size_t I, typename U>
   constexpr static auto& get(U& u) {
      constexpr size_t kv_index_size = sizeof(kv_index);
      static_assert(sizeof(U) % kv_index_size == 0);
      kv_index* indices = (kv_index*)(&u);
      return indices[I];
   }

   template <size_t S, typename U, typename F>
   constexpr static void for_each_field(U& u, F&& f) {
      f(get<S>(u));
      if constexpr (S <= 0) {
         return;
      } else {
         for_each_field<S-1>(u, f);
      }
   }

   template <typename U, typename F>
   constexpr static void for_each_field(U& u, F&& f) {
      constexpr size_t kv_index_size = sizeof(kv_index);
      static_assert(sizeof(U) % kv_index_size == 0);
      constexpr size_t num_elems = (sizeof(U) / sizeof(kv_index)) - 1;
      for_each_field<num_elems>(u, f);
   }

   template <typename V>
   static void serialize(const V& value, void* buffer, size_t size) {
      datastream<char*> ds((char*)buffer, size);
      unsigned_int i{0};
      ds << i;
      ds << value;
   }

   template <typename... Vs>
   static void serialize(const std::variant<Vs...>& value, void* buffer, size_t size) {
      datastream<char*> ds((char*)buffer, size);
      ds << value;
   }

   template <typename V>
   static void deserialize(V& value, void* buffer, size_t size) {
      unsigned_int idx;
      datastream<const char*> ds((char*)buffer, size);

      ds >> idx;
      eosio::check(idx==unsigned_int(0), "there was an error deserializing this value.");
      ds >> value;
   }

   template <typename... Vs>
   static void deserialize(std::variant<Vs...>& value, void* buffer, size_t size) {
      datastream<const char*> ds((char*)buffer, size);
      ds >> value;
   }

   template <typename V>
   static size_t get_size(const V& value) {
      auto size = pack_size(value);
      return size + 1;
   }

   template <typename... Vs>
   static size_t get_size(const std::variant<Vs...>& value) {
      auto size = pack_size(value);
      return size;
   }
};
} // eosio
