#include <eosio/eosio.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;

template <typename T, typename U>
struct baz {
   T val1;
   U val2;
};

template <typename S>
datastream<S>& operator >> ( datastream<S>& ds, baz<int, int>& b ) {
   ds >> b.val1 >> b.val2;
   return ds;
}

template <typename S>
datastream<S>& operator<< ( datastream<S>& ds, const baz<int, int>& b ) {
   ds << b.val1;
   ds << b.val2;
   return ds;
}

template <std::size_t Arg>
struct arg_val {
   int a;
};

template <typename S>
datastream<S>& operator >> ( datastream<S>& ds, arg_val<42>& b ) {
   ds >> b.a;
   return ds;
}

template <typename S>
datastream<S>& operator<< ( datastream<S>& ds, const arg_val<42>& b ) {
   ds << b.a;
   return ds;
}

struct foo {
   fixed_bytes<20> bar;
   baz<int, int>            bur;
   arg_val<42>              av;
};


CONTRACT template_gen_regression : public eosio::contract {
   public:
      using contract::contract;

      ACTION test( foo f ) {}
};
