#pragma once
 #include <eosiolib/print.hpp>
 namespace eosio {
    /**
    *  @defgroup binary_extension Binary Extension 
    *  @brief container to hold a binary payload for an extension 
    *  @ingroup types
    *  @{
    */
     /**
    *  Binary Extension 
    *  @brief container to hold a binary payload for an extension 
    *  @tparam T - Contained typed
    *  @ingroup types
    */
   template <typename T>
   class binary_extension {
      public:
         constexpr binary_extension() {}
         constexpr binary_extension( const T& ext )
            : _has_value(true) {
            ::new (&_data) T(ext);
         }
         constexpr binary_extension( T&& ext )
            : _has_value(true) {
            ::new (&_data) T(std::move(ext));
         }
          /** construct contained type in place */
         template <typename... Args>
         constexpr binary_extension( std::in_place_t, Args&&... args )
            : _has_value(true) {
            ::new (&_data) T(std::forward<Args>(args)...);
         }
         
         /** test if container is holding a value */ 
         constexpr bool has_value()const { return _has_value; }
          /** get the contained value */ 
         constexpr T& value()& { return *reinterpret_cast<T*>(&_data); }
          /** get the contained value */ 
         constexpr const T& value()const & { return *reinterpret_cast<const T*>(&_data); }
          /** get the contained value or a user specified default 
          * @pre def should be convertible to type T
          * */ 
         template <typename U>
         constexpr auto value_or( U&& def ) -> std::enable_if_t<std::is_convertible<U, T>::value, T&>& {
            if (_has_value)
               return value();
            return def;
         }
          constexpr T&& value_or() {
            if (!_has_value)
               return std::move(T());
            return value();
         }
          constexpr T const& value_or()const {
            if (!_has_value)
               return std::move(T());
            return value();
         }
         
         constexpr T* operator->() {
            return &value();
         }
          constexpr T* operator->()const {
            return &value();
         }
        
         constexpr T& operator*()& {
            return value();
         }
          constexpr T& operator*()const& {
            return value();
         }
          constexpr T&& operator*()const&& {
            return std::move(value());
         }
          constexpr T&& operator*()&& {
            return std::move(value());
         }
         
         constexpr void set( const T& ext ) {
            _has_value = true;
            ::new (&_data) T(ext);
         }
          constexpr void set( T&& ext ) {
            _has_value = true;
            ::new (&_data) T(std::move(ext));
         }
       private:
         bool _has_value = false;
         typename std::aligned_storage<sizeof(T), alignof(T)>::type _data;
   };
} // namespace eosio
