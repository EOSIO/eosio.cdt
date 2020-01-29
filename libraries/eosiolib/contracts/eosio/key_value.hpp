#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/print.hpp"
#include "../../core/eosio/utility.hpp"

#include <algorithm>
#include <cctype>
#include <functional>

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/pfr.hpp>

#define EOSIO_CDT_KV_INDEX_nullptr

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

#define EOSIO_CDT_KV_FIX_INDEX_TYPE_0(index_name) kv_index
#define EOSIO_CDT_KV_FIX_INDEX_TYPE_1(index_name) null_kv_index
#define EOSIO_CDT_KV_FIX_INDEX_TYPE(iskeyword, garbage) EOSIO_CDT_KV_FIX_INDEX_TYPE_ ## iskeyword

#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_0(value_class, index_name) {&value_class::index_name}
#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_1(value_class, index_name)
#define EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT(iskeyword, garbage) EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT_ ## iskeyword

#define EOSIO_CDT_KV_INDEX_NAME(index_name, i)                                                                         \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_NAME,                                                                        \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX_ ## index_name ()))))(index_name, i)

#define EOSIO_CDT_KV_INDEX_TYPE(index_name)                                                                            \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_TYPE,                                                                        \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX_ ## index_name ()))))(index_name)

#define EOSIO_CDT_KV_INDEX_CONSTRUCT(value_class, index_name)                                                          \
   EOSIO_CDT_APPLY(EOSIO_CDT_KV_FIX_INDEX_CONSTRUCT,                                                                   \
      (EOSIO_CDT_CAT(EOSIO_CDT_KV_INDEX_TEST_,                                                                         \
           EOSIO_CDT_EXPAND(EOSIO_CDT_KV_INDEX_TEST EOSIO_CDT_KV_INDEX_ ## index_name ()))))(value_class, index_name)


#define CREATE_KV_INDEX(r, value_class, i, index_name)                                                                 \
   EOSIO_CDT_KV_INDEX_TYPE(index_name) EOSIO_CDT_KV_INDEX_NAME(index_name, i) EOSIO_CDT_KV_INDEX_CONSTRUCT(value_class, index_name);

#define LIST_INDICES(value_class, ...)                                                                                 \
   BOOST_PP_SEQ_FOR_EACH_I(CREATE_KV_INDEX, value_class, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define TABLE_INHERITANCE(table_class, value_class, table_name, db_name)                                               \
   eosio::kv_table<table_class, value_class, table_name##_n, db_name##_n>

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
 *
 * Example:
 *
 * @code
 * struct myrecord {
 *    std::string                primary_key;
 *    uint64_t                   secondary_1;
 *    std::tuple<uint32_t, bool> secondary_2;
 * }
 *
 * DEFINE_TABLE(mytable, myrecord, "testtable",
 *    primary_key,
 *    secondary_1,
 *    secondary_2
 * )
 *
 * // The above macro results in the following class.
 *
 * struct mytable : kv_table<mytable, myrecord, "testtable"_n> {
 *    struct {
 *       kv_index primary_key{&myrecord::primary_key};
 *       kv_index secondary_1{&myrecord::secondary_1};
 *       kv_index secondary_2{&myrecord::secondary_2};
 *    } index;
 *
 *    mytable(eosio::name contract_name) {
 *       init(contract_name, &index);
 *    }
 * }
 * @endcode
 */
#define DEFINE_TABLE(table_class, value_class, table_name, db_name, /*indices*/...)                                    \
   struct table_class : TABLE_INHERITANCE(table_class, value_class, table_name, db_name) {                             \
      struct {                                                                                                         \
         LIST_INDICES(value_class, __VA_ARGS__)                                                                        \
      } index;                                                                                                         \
                                                                                                                       \
      table_class(eosio::name contract_name) {                                                                         \
         init(contract_name, &index);                                                                                  \
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

/**
 * The key_type struct is used to store the binary representation of a key.
 */
struct key_type : std::string {
   using std::string::string;
};

namespace detail {
   constexpr inline size_t max_stack_buffer_size = 512;
}

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
   size_t buffer_size = key.size() + prefix.size();
   void* buffer = buffer_size > detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);

   memcpy(buffer, prefix.data(), prefix.size());
   memcpy(((char*)buffer) + prefix.size(), key.data(), key.size());

   key_type s((const char*)buffer, buffer_size);

   if (buffer_size > detail::max_stack_buffer_size) {
      free(buffer);
   }

   return s;
}

template <typename I>
inline I flip_msb(I val) {
   constexpr static size_t bits = sizeof(I) * 8;
   return val ^ (static_cast<I>(1) << (bits - 1));
}

template <typename I>
inline I get_msb(I val) {
   constexpr static size_t bits = sizeof(I) * 8;
   constexpr static I mask = static_cast<I>(0x08) << (bits - 4);
   I masked = val & mask;
   return masked >> (bits - 1);
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

template <typename F, typename std::enable_if_t<std::is_floating_point<F>::value, int> = 0>
inline key_type make_key(F val) {
   static_assert(sizeof(F) != sizeof(long double), "long doubles are currently not supported by make_key");

   if (val == -0) {
      val = +0;
   }

   if (sizeof(F) == sizeof(float)) {
      return make_floating_key<uint32_t>(val);
   }
   else {
      return make_floating_key<uint64_t>(val);
   }
}

inline key_type make_key(const char* str, size_t size, bool case_insensitive=false) {
   size_t data_size = size + 3;
   void* data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

   if (case_insensitive) {
      std::transform(str, str + size, (char*)data_buffer, [](unsigned char c) -> unsigned char { return std::toupper(c); });
   } else {
      memcpy(data_buffer, str, size);
   }

   ((char*)data_buffer)[data_size - 3] = 0x01;
   ((char*)data_buffer)[data_size - 2] = 0x00;
   ((char*)data_buffer)[data_size - 1] = 0x00;

   key_type s((const char*)data_buffer, data_size);

   if (data_size > detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return s;
}

inline key_type make_key(const std::string& val, bool case_insensitive=false) {
   return make_key(val.data(), val.size(), case_insensitive);
}

inline key_type make_key(const char* str, bool case_insensitive=false) {
   return make_key(std::string{str}, case_insensitive);
}

inline key_type make_key(eosio::name n) {
   return make_key(n.value);
}

inline key_type make_key(key_type&& val) {
   return val;
}

template <typename S, typename std::enable_if_t<std::is_class<S>::value, int> = 0>
inline key_type make_key(S val) {
   size_t data_size = 0;
   size_t pos = 0;
   void* data_buffer;

   boost::pfr::for_each_field(val, [&](auto& field) {
      data_size += pack_size(field);
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
inline key_type make_key(std::tuple<Args...> val) {
   size_t data_size = 0;
   size_t pos = 0;
   void* data_buffer;

   boost::fusion::for_each(val, [&](auto& field) {
      data_size += pack_size(field);
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
 *
 * Example:
 *
 * @code
 * struct location {
 *    std::string city;
 *    std::string state;
 *    uint64_t zip_code;
 *
 *    auto icity() { return eosio::make_insensitive(city); }
 *    auto istate() { return eosio::make_insensitive(state); }
 * }
 *
 * DEFINE_TABLE(my_table, location, "testtable", "eosio.kvram",
 *    icity,
 *    istate,
 *    zip_code
 * )
 * @endcode
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
 *
 * Example:
 *
 * @code
 * #include <eosiolib/eosio.hpp>
 * using namespace eosio;
 *
 * struct address {
 *    uint64_t account_name;
 *    string first_name;
 *    string last_name;
 *    string street;
 *    string city;
 *    string state;
 *
 *    auto full_name() { return first_name + " " + last_name; }
 * }
 *
 * struct address_table : kv_table<address_table, myrecord, "testtable"_n> {
 *    struct {
 *       kv_index account_name{&address::account_name};
 *       kv_index full_name{&address::full_name};
 *    } index;
 *
 *    addresses(eosio::name contract_name) {
 *       init(contract_name, &index);
 *    }
 * }
 *
 * class addressbook : contract {
 *    public:
 *       void myaction() {
 *          auto addresses = address_table::open("mycontract"_n);
 *       }
 * }
 * @endcode
 */
template<typename Class, typename T, eosio::name::raw TableName, eosio::name::raw DbName = eosio::name{"eosio.kvram"}>
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
         iterator(eosio::name contract_name, uint32_t itr, kv_it_stat itr_stat, size_t data_size, kv_index* idx) :
                  contract_name{contract_name}, itr{itr}, itr_stat{itr_stat}, data_size{data_size}, idx{idx} {}

         /**
          * Returns the value that the iterator points to.
          * @ingroup keyvalue
          *
          * @return The value that the iterator points to.
          *
          * Example:
          * @code
          * // This assumes the code from the class example.
          *
          *    void myaction() {
          *       // add dan account to table           - see put example
          *
          *       auto dan = addresses.index.full_name.find("Dan Larimer");
          *       eosio::check(dan != addresses.index.full_name.end(), "Dan Larimer is not in the table");
          *       eosio::check(dan.value().city == "Blacksburg", "Got the wrong value");
          *    }
          * @endcode
          */
         T value() const {
            using namespace detail;

            eosio::check(itr_stat != kv_it_stat::iterator_end, "Cannot read end iterator");
            eosio::check(data_size > 0, "Cannot read a value of size 0");

            uint32_t actual_value_size;
            uint32_t offset = 0;

            void* buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);
            auto stat = internal_use_do_not_use::kv_it_value(itr, offset, (char*)buffer, data_size, actual_value_size);

            eosio::check(static_cast<kv_it_stat>(stat) != kv_it_stat::iterator_end, "Error reading value");

            if (idx->name == idx->tbl->primary_index->name) {
               datastream<const char*> ds((char*)buffer, actual_value_size);

               T val;
               ds >> val;
               if (data_size > detail::max_stack_buffer_size) {
                  free(buffer);
               }
               return val;
            } else {
               uint32_t actual_data_size;
               auto success = internal_use_do_not_use::kv_get(db, contract_name.value, (const char*)buffer, actual_value_size, actual_data_size);
               eosio::check(success, "failure getting primary key");

               void* pk_buffer = actual_data_size > detail::max_stack_buffer_size ? malloc(actual_data_size) : alloca(actual_data_size);
               auto copy_size = internal_use_do_not_use::kv_get_data(db, 0, (char*)pk_buffer, actual_data_size);

               eosio::check(copy_size > 0, "failure getting primary index data");

               datastream<const char*> ds((char*)pk_buffer, copy_size);

               T val;
               ds >> val;

               if (actual_data_size > detail::max_stack_buffer_size) {
                  free(pk_buffer);
               }
               if (data_size > detail::max_stack_buffer_size) {
                  free(buffer);
               }
               return val;
            }
         }

         iterator& operator++() {
            eosio::check(itr_stat != kv_it_stat::iterator_end, "cannot increment end iterator");
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_next(itr));
            return *this;
         }

         iterator& operator--() {
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_prev(itr));
            eosio::check(itr_stat != kv_it_stat::iterator_end, "incremented past the beginning");
            return *this;
         }

         bool operator==(const iterator& b) const {
            auto cmp = internal_use_do_not_use::kv_it_compare(itr, b.itr);
            return cmp == 0;
         }

         bool operator!=(const iterator& b) const {
            return !(*this == b);
         }

         bool operator<(const iterator& b) const {
            auto cmp = internal_use_do_not_use::kv_it_compare(itr, b.itr);
            return cmp == -1;
         }

         bool operator<=(const iterator& b) const {
            auto cmp = internal_use_do_not_use::kv_it_compare(itr, b.itr);
            return cmp <= 0;
         }

         bool operator>(const iterator& b) const {
            auto cmp = internal_use_do_not_use::kv_it_compare(itr, b.itr);
            return cmp == 1;
         }

         bool operator>=(const iterator& b) const {
            auto cmp = internal_use_do_not_use::kv_it_compare(itr, b.itr);
            return cmp >= 0;
         }

      private:
         eosio::name contract_name;
         const kv_index* idx;

         size_t data_size;

         uint32_t itr;
         kv_it_stat itr_stat;

         key_type key() const {
            return idx->get_key(value());
         }
      };

   public:
      eosio::name name;
      eosio::name table_name;
      eosio::name contract_name;

      kv_table* tbl;

      kv_index() = default;

      template <typename KF>
      kv_index(KF&& kf) {
         key_function = [=](const T& t) {
            return make_key(std::invoke(kf, &t));
         };
      }

      /**
       * Search for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @tparam K - The type of the key. This will be auto-deduced by the key param.
       *
       * @param key - The key to search for.
       * @return An iterator to the found object OR the `end` iterator if the given key was not found.
       *
       * Example:
       *
       * @code
       * // This assumes the code from the class example.
       *
       *     void myaction() {
       *       // add dan account to table           - see put example
       *
       *       auto itr = addresses.index.account_name.find("dan"_n);
       *       eosio::check(itr != addresses.index.account_name.end(), "Couldn't get him.");
       *
       *       auto itr = addresses.index.full_name.find("Dan Larimer");
       *       eosio::check(itr != addresses.index.full_name.end(), "Couldn't get him.");
       *     }
       * @endcode
       */
      template <typename K>
      iterator find(K key) {
         uint32_t value_size;

         auto prefix = make_prefix(table_name, name);
         auto t_key = table_key(prefix, make_key(key));

         auto success = internal_use_do_not_use::kv_get(db, contract_name.value, t_key.data(), t_key.size(), value_size);
         if (!success) {
            return end();
         }

         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         auto cmp = internal_use_do_not_use::kv_it_key_compare(itr, t_key.data(), t_key.size());

         if (cmp != 0) {
            return end();
         }

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), value_size, this};
      }

      /**
       * Returns an iterator pointing to the element with the lowest key greater than or equal to the given key.
       * @ingroup keyvalue
       *
       * @return An iterator pointing to the element with the lowest key greater than or equal to the given key.
       */
      template <typename K>
      iterator lower_bound(K key) {
         auto prefix = make_prefix(table_name, name);
         auto t_key = table_key(prefix, make_key(key));

         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         if (static_cast<kv_it_stat>(itr_stat) == kv_it_stat::iterator_end) {
            return end();
         }

         uint32_t value_size;

         // kv_it_value is just used to get the value_size
         void* buffer = alloca(1);
         internal_use_do_not_use::kv_it_value(itr, 0, (char*)buffer, 0, value_size);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), value_size, this};
      }

      /**
       * Returns an iterator referring to the `past-the-end` element. It does not point to any element, therefore `value` should not be called on it.
       * @ingroup keyvalue
       *
       * @return An iterator referring to the `past-the-end` element.
       *
       * Example:
       *
       * @code
       * // This assumes the code from the class example.
       *
       *     void myaction() {
       *       // add dan account to table           - see put example
       *
       *       auto itr = addresses.index.account_name.find("brendan"_n);
       *       eosio::check(itr == addresses.index.account_name.end(), "brendan should not be in the table.");
       *
       *       auto itr = addresses.index.full_name.find("Brendan Blumer");
       *       eosio::check(itr == addresses.index.full_name.end(), "Brendan Blumer should not be in the table.");
       *     }
       * @endcode
       */
      iterator end() {
         auto prefix = make_prefix(table_name, name);
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_move_to_end(itr);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), 0, this};
      }

      /**
       * Returns an iterator to the object with the lowest key (by this index) in the table.
       * @ingroup keyvalue
       *
       * @return An iterator to the object with the lowest key (by this index) in the table.
       *
       * Example:
       *
       * @code
       * // This assumes the code from the class example.
       *
       *     void myaction() {
       *       // add dan account to table           - see put example
       *
       *       auto itr = addresses.index.account_name.find("dan"_n);
       *       eosio::check(itr == addresses.index.account_name.begin(), "dan should be at the beginning.");
       *
       *       auto itr = addresses.index.full_name.find("Dan Larimer");
       *       eosio::check(itr == addresses.index.full_name.begin(), "Dan Larimer should be at the beginning.");
       *     }
       * @endcode
       */
      iterator begin() {
         auto prefix = make_prefix(table_name, name);
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, "", 0);

         uint32_t value_size;

         // kv_it_value is just used to get the value_size
         void* buffer = alloca(1);
         internal_use_do_not_use::kv_it_value(itr, 0, (char*)buffer, 0, value_size);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), value_size, this};
      }

      /**
       * Returns a vector of objects that fall between the specifed range. The range is inclusive on both ends.
       * @ingroup keyvalue
       *
       * @tparam K - The type of the key. This will be auto-deduced by the key param.
       *
       * @param begin - The beginning of the range.
       * @param end - The end of the range.
       * @return A vector containing all the objects that fall between the range.
       *
       * Example:
       *
       * @code
       * // This assumes the code from the class example.
       *
       *     void myaction() {
       *       // add dan account to table           - see put example
       *       // add brendan account to table       - see put example
       *       // add john account to table          - see put example
       *
       *       address dan = {...}
       *       address brendan = {...}
       *       address john = {...}
       *
       *       std::vector expected_values = {brendan, dan, john};
       *       auto values = addresses.index.account_name.range("brendan"_n, "john"_n);
       *       eosio::check(values == expected_values, "Did not get the expected values");
       *
       *       std::vector expected_values = {dan};
       *       auto values = addresses.index.account_name.range("dan"_n, "dan"_n);
       *       eosio::check(values == expected_values, "Did not get the expected values");
       *     }
       * @endcode
       */
      template <typename K>
      std::vector<T> range(K begin, K end) {
         auto begin_itr = find(begin);
         auto end_itr = find(end);
         eosio::check(begin_itr != this->end(), "beginning of range is not in table");
         eosio::check(end_itr != this->end(), "end of range is not in table");

         eosio::check(begin <= end, "Beginning of range should be less than or equal to end");

         if (begin == end) {
            std::vector<T> t;
            t.push_back(find(begin).value());
            return t;
         }

         std::vector<T> return_values;
         return_values.push_back(begin_itr.value());

         iterator itr = begin_itr;
         while(itr != end_itr) {
            ++itr;
            return_values.push_back(itr.value());
         }

         return return_values;
      }

      key_type get_key(const T& inst) const { return key_function(inst); }

   private:
      std::function<key_type(const T&)> key_function;
   };

   /**
    * @ingroup keyvalue
    *
    * @brief Defines a deleted index on an EOSIO Key Value Table
    * @details Due to the way indexes are named, when deleting an index a "placeholder" index needs to be created instead.
    * A null_kv_index should be created in this case. If using DEFINE_TABLE, just passing in nullptr will handle this.
    *
    * Example:
    * @code
    * // Original table:
    * DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
    *    primary_key,
    *    secondary_1,
    *    secondary_2
    * )
    *
    * struct my_table : kv_table<my_table, my_struct, "testtable"_n, "eosio.kvram"_n> {
    *   struct {
    *       kv_index primary_key ...;
    *       kv_index secondary_1 ...;
    *       kv_index secondary_2 ...;
    *   } index;
    *
    *   my_table(eosio::name contract_name) {
    *       init(contract_name, &index);
    *   }
    * }
    *
    * // Table with secondary_1 deleted:
    * DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
    *    primary_key,
    *    nullptr
    *    secondary_2
    * )
    *
    * struct my_table : kv_table<my_table, my_struct, "testtable"_n, "eosio.kvram"_n> {
    *   struct {
    *       kv_index primary_key ...;
    *       null_kv_index nullptr1 ...;
    *       kv_index secondary_2 ...;
    *   } index;
    *
    *   my_table(eosio::name contract_name) {
    *       init(contract_name, &index);
    *   }
    * }
    * @endcode
    */
   class null_kv_index : public kv_index {};

   /**
    * Puts a value into the table. If the value already exists, it updates the existing entry.
    * The key is determined from the defined primary index.
    * @ingroup keyvalue
    *
    * @param value - The entry to be stored in the table.
    *
    * Example:
    * @code
    * // This assumes the code from the class example.
    *
    *    void myaction() {
    *       auto addresses = address_table::open("mycontract"_n);
    *       addresses.put({
    *          .account_name = "dan"_n,
    *          .first_name = "Daniel",
    *          .last_name = "Larimer",
    *          .street = "1 EOS Way",
    *          .city = "Blacksburg",
    *          .state = "VA"
    *       });
    *    }
    * @endcode
    */
   void put(const T& value) {
      using namespace detail;

      auto t_key = table_key(make_prefix(table_name, primary_index->name), primary_index->get_key(value));

      size_t data_size = pack_size(value);
      void* data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);
      datastream<char*> data_ds((char*)data_buffer, data_size);
      data_ds << value;

      internal_use_do_not_use::kv_set(db, contract_name.value, t_key.data(), t_key.size(), (const char*)data_buffer, data_size);

      for (auto& idx : secondary_indices) {
         auto st_key = table_key(make_prefix(table_name, idx->name), idx->get_key(value));
         internal_use_do_not_use::kv_set(db, contract_name.value, st_key.data(), st_key.size(), t_key.data(), t_key.size());
      }
      
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
    *
    * Example:
    * // This assumes the code from the class example.
    * @code
    *    void myaction() {
    *       auto addresses = address_table::open("mycontract"_n);
    *
    *       auto itr = addresses.index.account_name.find("dan"_n);
    *       eosio::check(itr != addresses.index.account_name.end());
    *       addresses.erase("dan"_n);
    *    }
    * @endcode
    */
   template <typename K>
   void erase(K key) {
      auto primary_itr = primary_index->find(key);

      eosio::check(primary_itr != primary_index->end(), "Attempted to erase non-existent key");

      T val = primary_itr.value();

      auto k = table_key(make_prefix(table_name, primary_index->name), make_key(key));
      internal_use_do_not_use::kv_erase(db, contract_name.value, k.data(), k.size());

      for (auto& idx : secondary_indices) {
         auto skey = table_key(make_prefix(table_name, idx->name), idx->get_key(val));
         internal_use_do_not_use::kv_erase(db, contract_name.value, skey.data(), skey.size());
      }
   }

   /**
    * Initializes and returns an instance of the Key Value table scoped to the contract name.
    *
    * @param contract_name - The name of the contract.
    * @return An initialized instance of the user specifed Key Value table class.
    *
    * Example:
    *
    * @code
    * struct myrecord {
    *    std::string                primary_key;
    * }
    *
    * DEFINE_TABLE(mytable, myrecord, "testtable", primary_key)
    *
    * class mycontract : contract {
    *    public:
    *       void myaction() {
    *          auto t = mytable::open("mycontract"_n);
    *       }
    * }
    *
    * @endcode
    */
   static Class open(eosio::name contract_name) {
      Class c = Class(contract_name);
      return c;
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
   void init(eosio::name contract, Indices indices) {
      contract_name = contract;
      uint64_t index_name = 1;

      auto& primary = get<0>(*indices);

      primary_index = &primary;
      primary_index->name = eosio::name{index_name};
      primary_index->contract_name = contract_name;
      primary_index->table_name = table_name;
      primary_index->tbl = this;

      ++index_name;

      for_each_field(*indices, [&](auto& idx) {
         if (idx.name != primary.name) {
            kv_index* si = &idx;
            si->name = eosio::name{index_name};
            si->contract_name = contract_name;
            si->table_name = table_name;
            si->tbl = this;
            secondary_indices.push_back(si);
            ++index_name;
         }
      });

      // Makes sure the indexes are run in the correct order.
      // This is mainly useful for debugging, this probably could be deleted.
      std::reverse(std::begin(secondary_indices), std::end(secondary_indices));
   }

private:
   constexpr static uint64_t db = static_cast<uint64_t>(DbName);
   constexpr static eosio::name table_name = static_cast<eosio::name>(TableName);

   eosio::name contract_name;

   kv_index* primary_index;
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
};
} // eosio
