#include <eosio/action.hpp>
#include "intrinsics_def.hpp"

#pragma once

namespace eosio { namespace native {
   
   class intrinsics {
      public:
         static intrinsics& get() {
            static intrinsics inst;
            return inst;
         }

         enum intrinsic_name {
            INTRINSICS(CREATE_ENUM)
            INTRINSICS_SIZE
         };

         INTRINSICS(GENERATE_TYPE_MAPPING)
         std::tuple< INTRINSICS(GET_TYPE) std::function<void()> > funcs {
            INTRINSICS(REGISTER_INTRINSIC)
            std::function<void()>{[](){}}
         };

         template <intrinsic_name IN, typename... Args>
         auto call(Args... args) -> decltype(std::get<IN>(intrinsics::get().funcs)(args...)) {
            return std::get<IN>(intrinsics::get().funcs)(args...); 
         }

         template <intrinsic_name IN, typename F>
         static void set_intrinsic(F&& func) {
            auto& f = std::get<IN>(intrinsics::get().funcs);
            std::get<IN>(intrinsics::get().funcs) = typename std::remove_reference<decltype(f)>::type {func};
         }
         
         template <intrinsic_name IN>
         static auto get_intrinsic() 
               -> typename std::remove_reference<decltype(std::get<IN>(intrinsics::get().funcs))>::type {
            return std::get<IN>(intrinsics::get().funcs);
         }
   };

}} //ns eosio::native
