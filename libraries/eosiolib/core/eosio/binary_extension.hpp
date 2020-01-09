#pragma once

#include "check.hpp"

namespace eosio {
    /**
    *  @defgroup binary_extension Binary Extension
    *  @ingroup core
    *  @ingroup types
    *  @brief Container to hold a binary payload for an extension
    */

    /**
    *  Container to hold a binary payload for an extension
    *
    *  @ingroup binary_extension
    *  @tparam T - Contained typed
    */
   template <typename T>
   class binary_extension {
      public:
         using value_type = T;

         constexpr binary_extension() {}
         constexpr binary_extension( const T& ext )
         :_has_value(true)
         {
            ::new (&_data) T(ext);
         }
         constexpr binary_extension( T&& ext )
         :_has_value(true)
         {
            ::new (&_data) T(std::move(ext));
         }
          /** construct contained type in place */
         template <typename... Args>
         constexpr binary_extension( std::in_place_t, Args&&... args )
         :_has_value(true)
         {
            ::new (&_data) T(std::forward<Args>(args)...);
         }

         ~binary_extension() { reset(); }

         constexpr binary_extension( const binary_extension& other )
         :_has_value(other._has_value)
         {
            if( other._has_value ) ::new (&_data) T( *other );
         }

         constexpr binary_extension( binary_extension&& other )
         :_has_value(other._has_value)
         {
            if( other._has_value ) {
               ::new (&_data) T( *std::move(other) );
               other._has_value = false;
            }
         }

         /** test if container is holding a value */
         constexpr explicit operator bool()const { return _has_value; }
         /** test if container is holding a value */
         constexpr bool has_value()const { return _has_value; }

          /** get the contained value */
         constexpr T& value()& {
            if (!_has_value) {
               check(false, "cannot get value of empty binary_extension");
            }
            return _get();
         }

         /// @cond INTERNAL

          /** get the contained value */
         constexpr const T& value()const & {
            if (!_has_value) {
               check(false, "cannot get value of empty binary_extension");
            }
            return _get();
         }

          /** get the contained value or a user specified default
          * @pre def should be convertible to type T
          * */
         template <typename U>
         constexpr auto value_or( U&& def ) -> std::enable_if_t<std::is_convertible<U, T>::value, T&>& {
            if (_has_value)
               return _get();
            return def;
         }
         constexpr T&& value_or()&& {
            if (!_has_value)
               return std::move(T());
            _has_value = false;
            return std::move(_get());
         }
         constexpr const T&& value_or()const&& {
            if (!_has_value)
               return std::move(T());
            _has_value = false;
            return std::move(_get());
         }
         constexpr T value_or()& {
            if (!_has_value)
               return T();
            return _get();
         }
         constexpr T value_or()const& {
            if (!_has_value)
               return T();
            return _get();
         }

         constexpr T* operator->() {
            return &_get();
         }
         constexpr const T* operator->()const {
            return &_get();
         }

         constexpr T& operator*()& {
            return _get();
         }
         constexpr const T& operator*()const& {
            return _get();
         }
         constexpr const T&& operator*()const&& {
            return std::move(_get());
         }
         constexpr T&& operator*()&& {
            return std::move(_get());
         }

         template<typename ...Args>
         T& emplace(Args&& ... args)& {
            if (_has_value) {
               reset();
            }

            ::new (&_data) T( std::forward<Args>(args)... );
            _has_value = true;

            return _get();
         }

         void reset() {
            if( _has_value ) {
               _get().~value_type();
               _has_value = false;
            }
         }

         /// @endcond

       private:
         bool _has_value = false;
         typename std::aligned_storage<sizeof(T), alignof(T)>::type _data;

         constexpr T& _get() {
            return *reinterpret_cast<T*>(&_data);
         }

         constexpr const T& _get()const {
            return *reinterpret_cast<const T*>(&_data);
         }
   };

   /// @cond IMPLEMENTATIONS

   /**
    *  Serialize a binary_extension into a stream
    *
    *  @ingroup binary_extension
    *  @brief Serialize a binary_extension
    *  @param ds - The stream to write
    *  @param opt - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream, typename T>
   inline DataStream& operator<<(DataStream& ds, const eosio::binary_extension<T>& be) {
     ds << be.value_or();
     return ds;
   }

   /**
    *  Deserialize a binary_extension from a stream
    *
    *  @ingroup binary_extension
    *  @brief Deserialize a binary_extension
    *  @param ds - The stream to read
    *  @param opt - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream, typename T>
   inline DataStream& operator>>(DataStream& ds, eosio::binary_extension<T>& be) {
     if( ds.remaining() ) {
        T val;
        ds >> val;
        be.emplace(val);
     }
     return ds;
   }

   /// @endcond

} // namespace eosio
