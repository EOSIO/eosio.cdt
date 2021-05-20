#pragma once
#include "../../core/eosio/context.hpp"
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/varint.hpp"

#include "../../core/eosio/key_utils.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <string_view>

/**
 * @defgroup keyvaluemap Key Value Map
 * @ingroup contracts
 */

namespace eosio::kv {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         int64_t kv_erase(uint64_t contract, const char* key, uint32_t key_size);

         __attribute__((eosio_wasm_import))
         int64_t kv_set(uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size, uint64_t payer);

         __attribute__((eosio_wasm_import))
         bool kv_get(uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_get_data(uint32_t offset, char* data, uint32_t data_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_it_create(uint64_t contract, const char* prefix, uint32_t size);

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
         int32_t kv_it_next(uint32_t itr, uint32_t& found_key_size, uint32_t& found_value_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_prev(uint32_t itr, uint32_t& found_key_size, uint32_t& found_value_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_lower_bound(uint32_t itr, const char* key, uint32_t size, uint32_t& found_key_size, uint32_t& found_value_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_value(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);
      }
   }

   // tag used by some of the types to delineate overloads between key_type (std::string) and the map's key type
   struct packed_tag {};

   namespace detail {
      static inline void increment_bytes(key_type& kt) {
         // this will terminate without have to lengthen by one byte given the way prefixes are formed
         // need to re-evaluate if that changes
         for (std::size_t i=kt.size()-1; i >= 0; i--) {
            uint16_t v = kt[i]+1;
            kt[i] = v;
            if (v <= 0xFF)
               break;
         }
      }

      struct packed_view {
         packed_view() = default;
         constexpr inline packed_view(char* p, std::size_t s)
            : ptr(p), sz(s) {}

         using iterator = char*;
         using const_iterator = const char*;

         constexpr inline char* data() { return ptr; }
         constexpr inline const char* data() const { return ptr; }
         constexpr inline std::size_t size() const { return sz; }
         constexpr inline const_iterator    begin() const { return data(); }
         constexpr inline const_iterator    end() const   { return data() + size(); }
         char* ptr;
         std::size_t sz;
      };

      inline uint32_t itr_create(name contract, std::string_view prefix) {
         return internal_use_do_not_use::kv_it_create(contract.value, prefix.data(), prefix.size());
      }

      inline void    itr_destroy(uint32_t itr) { internal_use_do_not_use::kv_it_destroy(itr); }
      inline int32_t itr_status(uint32_t itr) { return internal_use_do_not_use::kv_it_status(itr); }
      inline int32_t itr_compare(uint32_t a, uint32_t b) { return internal_use_do_not_use::kv_it_compare(a, b); }
      inline int32_t itr_key_compare(uint32_t itr, std::string_view k) { return internal_use_do_not_use::kv_it_key_compare(itr, k.data(), k.size()); }
      inline int32_t itr_move_to_end(uint32_t itr) { return internal_use_do_not_use::kv_it_move_to_end(itr); }
      inline int32_t itr_key(uint32_t itr, char* dest, uint32_t size, uint32_t& actual_size) { return internal_use_do_not_use::kv_it_key(itr, 0, dest, size, actual_size); }
      inline int32_t itr_value(uint32_t itr, char* dest, uint32_t size, uint32_t& actual_size) { return internal_use_do_not_use::kv_it_value(itr, 0, dest, size, actual_size); }

      inline int32_t itr_next(uint32_t itr, uint32_t& key_size, uint32_t& val_size) {
         return internal_use_do_not_use::kv_it_next(itr, key_size, val_size);
      }
      inline int32_t itr_next(uint32_t itr) {
         uint32_t k,v;
         return internal_use_do_not_use::kv_it_next(itr, k, v);
      }
      inline int32_t itr_prev(uint32_t itr, uint32_t& key_size, uint32_t& val_size) {
         return internal_use_do_not_use::kv_it_prev(itr, key_size, val_size);
      }
      inline int32_t itr_prev(uint32_t itr) {
         uint32_t k,v;
         return internal_use_do_not_use::kv_it_prev(itr, k, v);
      }
      inline int32_t itr_lower_bound(uint32_t itr, std::string_view k, uint32_t& key_size, uint32_t& val_size) {
         return internal_use_do_not_use::kv_it_lower_bound(itr, k.data(), k.size(), key_size, val_size);
      }
      inline int32_t itr_lower_bound(uint32_t itr, std::string_view k) {
         uint32_t _k,_v;
         return internal_use_do_not_use::kv_it_lower_bound(itr, k.data(), k.size(), _k, _v);
      }
      inline int32_t itr_lower_bound(uint32_t itr) {
         uint32_t _k,_v;
         return internal_use_do_not_use::kv_it_lower_bound(itr, "", 0, _k, _v);
      }

      /**
       * @ingroup keyvaluemap
       * 
       * @brief This struct represents the data type stored in map.
       * @details You will use the set of functions and operations associated 
       * with this type to access the items stored in the map.
       * 
       * Note that this type in `eosio::kv::map` is defined as `eosio::kv::map::elem_t`.
       */
      template <typename KV>
      struct elem {
         using key_t   = typename KV::key_t;
         using value_t = typename KV::value_t;
         elem() = default;
         constexpr inline elem(const key_t& k, value_t v, name p=current_context_contract())
            : key(KV::full_key(k)), value(std::move(v)), payer(p) {}

         inline elem(key_type k, value_t v, name p, packed_tag)
            : key(std::move(k)), value(std::move(v)), payer(p) {}

         constexpr inline bool operator==(const elem& e) const {
            return std::tie(key, value) == std::tie(e.key, e.value);
         }

         key_type& first() { return key; }
         const key_type& first() const { return key; }
         value_t& second() { return value; }
         const value_t& second() const { return value; }

         key_type key;
         value_t  value;
         name     payer;
      };

      /**
       * @ingroup keyvaluemap
       * 
       * @brief This struct represents the iterator for the `eosio::kv::map` data type.
       * @details You will use the set of functions and operations associated with 
       * this type to iterate through values in the map and reference them.
       * 
       * Note that this iterator type in `eosio::kv::map` is defined as `eosio::kv::map::iterator_t`.
       * There is also a reverse iterator available, it is defined as `eosio::kv::map::reverse_iterator_t`.
       * The only difference between iterator_t and reverse_iterator_t is the direction the ++ and -- operators go.
       */
      template <bool Reverse, typename KV>
      struct iterator {
         using elem_t = elem<KV>;
         using value_t = typename KV::value_t;
         constexpr static inline uint32_t invalidated_iterator = std::numeric_limits<uint32_t>::max();

         enum class status { ok = 0, erased = -1, end = -2 };

         template <status Stat>
         constexpr inline static bool query_status(status stat) {
            using namespace internal_use_do_not_use;
            return Stat == stat;
         }
         template <status Stat>
         constexpr inline static bool query_status(int32_t stat) {
            using namespace internal_use_do_not_use;
            return Stat == static_cast<status>(stat);
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Constructor for the iterator type.
          * @param owner This is the owner of the table object.
          */
         inline iterator(name owner)
            : element(),
              handle(itr_create(owner, {KV::prefix().data(), KV::prefix().size()})) {}

         iterator(const iterator&) = delete;
         iterator& operator=(const iterator&) = delete;

         inline iterator(iterator&& o)
            : element(o.element),
              handle(std::exchange(o.handle, invalidated_iterator)),
              current_status(o.current_status) {}

         inline iterator(iterator<!Reverse, KV>&& o)
            : element(o.element),
              handle(std::exchange(o.handle, invalidated_iterator)),
              current_status(o.current_status) {}

         inline iterator& operator=(iterator&& o) {
            element = o.element;
            if (handle != invalidated_iterator)
               itr_destroy(handle);
            handle = std::exchange(o.handle, invalidated_iterator);
            current_status = o.current_status;
            return *this;
         }

         inline iterator& operator=(iterator<!Reverse, KV>&& o) {
            element = o.element;
            if (handle != invalidated_iterator)
               itr_destroy(handle);
            handle = std::exchange(o.handle, invalidated_iterator);
            current_status = o.current_status;
            return *this;
         }

         ~iterator() {
            if (handle != invalidated_iterator)
               itr_destroy(handle);
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Utility function which returns true if iterator is valid or false 
          * if iterator is not valid. An iterator is not valid if is pointing at 
          * `end` or one past the last or first element.
          */
         inline bool is_valid() const { return query_status<status::ok>(current_status); }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to advance the iterator to the beginning of the map's key value pairs.
          */
         iterator& seek_to_begin() {
            current_status = static_cast<status>(itr_lower_bound(handle));
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to advance the iterator to the last element of the map's key value pairs.
          */
         iterator& seek_to_last() {
            current_status = static_cast<status>(itr_move_to_end(handle));
            current_status = static_cast<status>(itr_prev(handle));
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to advance the iterator to the element past the last element of the map's key value pairs.
          */
         iterator& seek_to_end() {
            current_status = static_cast<status>(itr_move_to_end(handle));
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief This will take a `key_type` key and find the value that is equal to or greater than that key.
          * @details If no element is greater than or equal to the key the iterator will now hold `end`.
          * @param key This is the key which you wish to query with.
          */
         iterator& lower_bound(const key_type& k) {
            current_status = static_cast<status>(itr_lower_bound(handle, {k.data(), k.size()}));
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief This will take a `key_type` key and find the value that exactly matches that key.
          * @details If no element is equal to the key the iterator will now hold `end`.
          * @param key This is the key which you wish to query with.
          */
         iterator& find(const key_type& k) {
            lower_bound(k);
            if (itr_key_compare(handle, {k.data(), k.size()}) != 0)
               seek_to_end();
            return *this;
         }

         inline elem_t& operator*() {
            materialize();
            return element;
         }
         inline const elem_t& operator*() const {
            materialize();
            return element;
         }
         inline elem_t* operator->() {
            materialize();
            return &element;
         }
         inline const elem_t* operator->() const {
            materialize();
            return &element;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to increment the iterator to the next element (sorted in lexicographic order).
          * @details Note this is the prefix operator, i.e. `++it`, the `it++` operator is explicitly missing because
          * of performance issues. If you increment past the last element this iterator will then be invalid and point to `end`.
          */
         iterator& operator++() {
            if constexpr (Reverse) {
               current_status = static_cast<status>(itr_prev(handle));
               check(query_status<status::ok>(current_status), "incrementing past end or an erased iterator");
            } else {
               check(query_status<status::ok>(current_status), "incrementing past end or an erased iterator");
               current_status = static_cast<status>(itr_next(handle));
            }
            if (query_status<status::end>(current_status))
               seek_to_end();
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to decrement the iterator to the next element (sorted in lexicographic order).
          * @details Note this is the prefix operator, i.e. `--it`, the `it--` operator is explicitly missing because
          * of performance issues. If you decrement past the first element this iterator will then be invalid and point to `end`.
          */
         iterator& operator--() {
            if constexpr (Reverse) {
               check(query_status<status::ok>(current_status), "decrementing past end or an erased iterator");
               current_status = static_cast<status>(itr_next(handle));
            } else {
               current_status = static_cast<status>(itr_prev(handle));
               check(query_status<status::ok>(current_status), "decrementing past end or an erased iterator");
            }
            if (query_status<status::end>(current_status))
               seek_to_end();
            return *this;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to test equality.
          */
         inline bool operator==(const iterator& o) const {
            // ignoring key_size and value_size as they shouldn't play a role in equality
            return (std::tie(handle, current_status) == std::tie(o.handle, o.current_status)) ||
                   (query_status<status::end>(current_status) && query_status<status::end>(o.current_status)) ||
                   !itr_compare(handle, o.handle);
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief Function to test inequality.
          */
         inline bool operator!=(const iterator& o) const { return !((*this) == o); }

         void materialize() const {
            using namespace internal_use_do_not_use;
            uint32_t sz;
            itr_key(handle, nullptr, 0, sz);
            element.key.resize(sz);
            check(query_status<status::ok>(itr_key(handle, element.key.data(), element.key.size(), sz)), "failure getting key");

            itr_value(handle, nullptr, 0, sz);
            auto val_bytes = KV::get_tmp_buffer(sz);
            auto status = itr_value(handle, val_bytes.data(), val_bytes.size(), sz);
            check(query_status<status::ok>(status), "failure getting value");
            unpack<value_t>(element.value, val_bytes.data(), val_bytes.size());
         }

         mutable elem_t element;
         uint32_t       handle;
         status         current_status = status::ok;
      };
   } // namespace eosio::kv::detail

   /**
    * @ingroup keyvaluemap
    *
    * @brief Defines an EOSIO Key Value Map
    * @details EOSIO Key Value API provides a C++ interface to the EOSIO Key Value database.
    * The Key Value Map offered by the KV API serves as a storage location which is organized 
    * as a sorted associative container that contains key-value pairs with unique keys.
    * Keys are sorted lexicographically. Search, removal, and insertion operations have
    * logarithmic complexity, O(log(n)). 'KV Map' is designed to offer a comparable 
    * interface to std::map template class.
    *
    * @tparam K         - the type of the data stored as the key of the map
    * @tparam V         - the type of the data stored as the value of the map
    */
   template <eosio::name::raw TableName, typename K, typename V, eosio::name::raw IndexName="map.index"_n>
   class [[eosio::table]] map {
      public:
         constexpr static inline uint8_t magic = 1;
         constexpr static inline name    table_name = name{static_cast<uint64_t>(TableName)};
         using key_t   = K;
         using value_t = V;
         using self_t  = map<TableName, K, V>;

         static const key_type& prefix() {
            static key_type prfx = eosio::detail::const_pack(magic, table_name, index_name);
            return prfx;
         }

         static key_type full_key(const key_t& k) { return prefix() + convert_to_key(k); }

         using elem_t = detail::elem<self_t>;
         using iterator_t = detail::iterator<false, self_t>;
         using reverse_iterator_t = detail::iterator<true, self_t>;

         struct writable_wrapper {
            writable_wrapper(key_type k, value_t v, name p, name o=current_context_contract())
               : element(std::move(k), std::move(v), p, packed_tag{}), owner(o) {}

            explicit operator value_t&() { return element.value; }
            operator value_t() const { return element.value; }

            writable_wrapper& operator=(const value_t& o) {
               map{owner}.set(element.key, o, element.payer, packed_tag{});
               element.value = o;
               return *this;
            }

            writable_wrapper& operator=(value_t&& o) {
               map{owner}.set(element.key, o, element.payer, packed_tag{});
               element.value = std::move(o);
               return *this;
            }

            elem_t element;
            name owner;
         };

         inline map(name owner=current_context_contract())
            : owner(owner) {}

         /**
          * Basic constructor of N elements.  This will bill the implicit owner of the table by default.
          */
         inline map(std::initializer_list<elem_t> l) {
            for ( const auto& e : l ) {
               set(e.key, e.value, e.payer, packed_tag{});
            }
         }

         /**
          * Basic constructor of N elements.  This will bill the owner explicitly passed in.
          */
         inline map(name owner, std::initializer_list<elem_t> l)
            : owner(owner) {
            for ( const auto& e : l ) {
               set(e.key, e.value, e.payer, packed_tag{});
            }
         }

         writable_wrapper operator[](const std::pair<key_t, name>& key_payer) {
            auto v = get(key_payer.first);
            if (std::get<0>(v))
               return {std::get<1>(v), *std::get<0>(v), key_payer.second, owner};

            set(std::get<1>(v), value_t{}, owner, packed_tag{});
            return {std::move(std::get<1>(v)), value_t{}, key_payer.second, owner};
         }

         writable_wrapper operator[](const key_t& k) {
            return (*this)[std::pair{k, owner}];
         }

         template <typename Key>
         writable_wrapper at(Key&& k, name payer=current_context_contract()) {
            auto v = get(std::forward<Key>(k));
            check(std::get<0>(v), "key not found");
            return {std::get<1>(v), *std::get<0>(v), payer, owner};
         }

         inline iterator_t begin() const {
            iterator_t it = {owner};
            it.seek_to_begin();
            return it;
         }

         inline iterator_t end() const {
            iterator_t it = {owner};
            it.seek_to_end();
            return it;
         }

         inline reverse_iterator_t rbegin() const {
            reverse_iterator_t it = {owner};
            it.seek_to_last();
            return it;
         }

         inline reverse_iterator_t rend() const {
            return end();
         }

         inline bool empty() const {
            iterator_t it = {owner};
            it.lower_bound(key_type{""});
            return it == end();
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief This will take a `key_t` key and find the value that exactly matches that key.
          * @details If no element is equal to the key the iterator will now hold `end`.
          * @param key This is the key which you wish to query with.
          */
         iterator_t find(const key_t& k) const {
            auto fk = full_key(k);
            iterator_t it = {owner};
            it.find(fk);
            return it;
         }

         void erase(const key_t& k) {
            using namespace internal_use_do_not_use;
            auto fk = full_key(k);
            kv_erase(owner.value, fk.data(), fk.size());
         }

         iterator_t erase(const iterator_t& it) {
            using namespace internal_use_do_not_use;
            auto key = *it.key;
            ++it;
            kv_erase(owner, key.data(), key.size());
            return it;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief This will take a `key_t` key and find the value that is equal to or greater than that key.
          * @details If no element is greater than or equal to the key the iterator will now hold `end`.
          * @param key This is the key which you wish to query with.
          */
         inline iterator_t lower_bound(const key_t& k) const {
            iterator_t it = {owner};
            it.lower_bound(full_key(k));
            return it;
         }

         /**
          * @ingroup keyvaluemap
          *
          * @brief This will take a `key_t` key and find the value that is strictly greater than that key.
          * @details If no element is strictly greater than the key the iterator will now hold `end`.
          * @param key This is the key which you wish to query with.
          */
         inline iterator_t upper_bound(const key_t& k) const {
            auto fk = full_key(k);
            detail::increment_bytes(fk); // add '1' to the last byte, this should get us to the next value up
            iterator_t it = {owner};
            it.lower_bound(fk);
            return it;
         }

         std::pair<iterator_t, iterator_t> equal_range(const key_t& k) const { return {lower_bound(k), upper_bound(k)}; }

         std::vector<elem_t> ranged_slice(const key_t& l, const key_t& h) {
            std::vector<elem_t> ret;

            for (auto itr = lower_bound(l); itr != lower_bound(h); ++itr) {
               ret.emplace_back(std::move(*itr));
            }

            return ret;
         }

         inline bool contains(const key_t& k) const {
            using namespace internal_use_do_not_use;
            uint32_t _vs;
            auto fk = full_key(k);
            return static_cast<bool>(kv_get(owner.value, fk.data(), fk.size(), _vs));
         }

         bool raw_write(const key_t& k, std::string_view bytes, name payer=current_context_contract()) const {
            using namespace internal_use_do_not_use;
            const auto& fk = full_key(k);
            auto wrote = kv_set(owner.value, fk.data(), fk.size(), bytes.data(), bytes.size(), payer.value);
            return wrote == bytes.size();
         }

         friend iterator_t;

         static detail::packed_view get_tmp_buffer(std::size_t size_needed=0) {
            constexpr std::size_t max_size = 512;
            static char static_data[max_size];
            static std::vector<char> dynamic_data = {0};
            if (size_needed > max_size) {
               if (dynamic_data.size() < size_needed)
                  dynamic_data.resize(size_needed);
               return {dynamic_data.data(), size_needed};
            } else {
               return {&static_data[0], size_needed};
            }
         }

      protected:
         constexpr static inline name index_name = name{IndexName};

         writable_wrapper at(const key_type& bytes, name payer=current_context_contract()) {
            auto v = get(bytes, packed_tag{});
            check(std::get<0>(v), "key not found");
            return {std::get<1>(v), *std::get<0>(v), payer, owner};
         }

         std::tuple<value_t*, key_type> get(key_type k, packed_tag) {
            using namespace internal_use_do_not_use;
            uint32_t sz;
            if (!kv_get(owner.value, k.data(), k.size(), sz))
               return {nullptr, std::move(k)};

            auto val_bytes = get_tmp_buffer(sz);
            check(kv_get_data(0, val_bytes.data(), val_bytes.size()) == val_bytes.size(), "kv get internal failure");
            temp = unpack<value_t>(val_bytes.data(), val_bytes.size());
            return {&temp, std::move(k)};
         }

         std::tuple<value_t*, key_type> get(const key_t& k) {
            return get(full_key(k), packed_tag{});
         }

         inline bool set(const key_type& k, const value_t& v, name payer, packed_tag) const {
            using namespace internal_use_do_not_use;
            const auto& packed_value = pack_value(v);
            auto wrote = kv_set(owner.value, k.data(), k.size(), packed_value.data(), packed_value.size(), payer.value);
            return wrote == packed_value.size();
         }

         inline bool set(const key_t& k, const value_t& v, name payer) const {
            return set(full_key(k), v, payer, packed_tag{});
         }

         template <typename Value>
         inline detail::packed_view pack_value(Value&& v) const {
            auto pv = get_tmp_buffer(pack_size(v));
            datastream<char*> ds(pv.data(), pv.size());
            ds << std::forward<Value>(v);
            return pv;
         }

      private:
         name    owner = current_context_contract();
         value_t temp; // used for

         CDT_REFLECT(owner, temp);
   };

} // namespace eosio::kv
