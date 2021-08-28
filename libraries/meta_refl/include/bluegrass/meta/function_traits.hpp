#pragma once

#include "utility.hpp"

#include <type_traits>
#include <tuple>

#define BLUEGRASS_META_HAS_MEMBER_GENERATOR(NAME, MEMBER)                       \
   template <typename T, typename E>                                            \
   struct has_member_ ## NAME {                                                 \
      template <typename U, E (U::*)() const> struct sfinae {};                 \
      template <typename U> constexpr static char test(sfinae<U, &U::MEMBER>*); \
      template <typename U> constexpr static int test(...);                     \
      constexpr static const bool value = sizeof(test<T>(0)) == sizeof(char);   \
   };

#define BLUEGRASS_HAS_MEMBER(ARG, NAME)                        \
   bluegrass::meta::overloaded {                       \
        [](auto&& f, std::enable_if_t<std::is_class_v<std::decay_t<decltype(f)>> && \
                        bluegrass::meta::detail::pass_type<    \
                            decltype(&std::decay_t<decltype(f)>::type::NAME)>(), int> = 0) constexpr { \
                            return true;                       \
       }, [](...) constexpr { return false; }                  \
    }(bluegrass::meta::detail::wrapper_t<decltype(ARG)>{})

#define BLUEGRASS_HAS_MEMBER_TY(TY, NAME)                         \
   bluegrass::meta::overloaded {                             \
        [](auto&& f, std::enable_if_t<std::is_class_v<TY> &&   \
                        bluegrass::meta::detail::pass_type<          \
                            decltype(&std::decay_t<decltype(f)>::type::NAME)>(), int> = 0) constexpr { \
                            return true;                       \
       }, [](...) constexpr { return false; }                  \
    }(bluegrass::meta::detail::wrapper_t<TY>{})

#define BLUEGRASS_HAS_TEMPLATE_MEMBER(ARG, NAME)                  \
   bluegrass::meta::overloaded {                             \
        [&](auto&& f, std::enable_if_t<std::is_class_v<std::decay_t<decltype(f)>> && \
                        bluegrass::meta::detail::pass_type<          \
                            decltype(&std::decay_t<decltype(f)>::type::template NAME)>(), int> = 0) constexpr { \
                            return true;                       \
       }, [](...) constexpr { return false; }                  \
    }(bluegrass::meta::detail::wrapper_t<decltype(ARG)>{})

#define BLUEGRASS_HAS_TEMPLATE_MEMBER_TY(TY, NAME)                \
   bluegrass::meta::overloaded {                             \
        [](auto&& f, std::enable_if_t<std::is_class_v<TY> &&   \
                        bluegrass::meta::detail::pass_type<          \
                            decltype(&std::decay_t<decltype(f)>::type::template NAME)>(), int> = 0) constexpr { \
                            return true;                       \
       }, [](...) constexpr { return false; }                  \
    }(bluegrass::meta::detail::wrapper_t<TY>{})


// Workaround for compiler bug handling C++g17 auto template parameters.
// The parameter is not treated as being type-dependent in all contexts,
// causing early evaluation of the containing expression.
// Tested at Apple LLVM version 10.0.1 (clang-1001.0.46.4)
#define AUTO_PARAM_WORKAROUND(X) bluegrass::meta::detail::make_dependent<decltype(X)>(X)

namespace bluegrass { namespace meta {

   struct freestanding {};

   namespace detail {
      template <typename T>
      constexpr bool pass_type() { return true; }

      template <typename>
      constexpr bool is_callable_impl(...) {
        return false;
      }

      template <typename T>
      struct wrapper_t {
        using type = T;
        constexpr wrapper_t() {}
        constexpr wrapper_t(T&&) {}
      };

      template <typename T, typename U>
      inline constexpr U&& make_dependent(U&& u) { return static_cast<U&&>(u); }
   }

   template <auto FN>
   inline constexpr static bool is_callable_v = BLUEGRASS_HAS_MEMBER(AUTO_PARAM_WORKAROUND(FN), operator());

   template <typename F>
   constexpr bool is_callable(F&& fn) { return BLUEGRASS_HAS_MEMBER(fn, operator()); }

   namespace detail {
      template <bool Decay, typename R, typename... Args>
      constexpr auto get_types(R(Args...)) -> std::tuple<R, freestanding,
                                              std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)) -> std::tuple<R, Cls,
                                                       std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)const) -> std::tuple<R, Cls,
                                                            std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)&) -> std::tuple<R, Cls,
                                                        std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)&&) -> std::tuple<R, Cls,
                                                         std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)const &) -> std::tuple<R, Cls,
                                                              std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename R, typename Cls, typename... Args>
      constexpr auto get_types(R (Cls::*)(Args...)const &&) -> std::tuple<R, Cls,
                                                               std::tuple<std::conditional_t<Decay, std::decay_t<Args>, Args>...>>;
      template <bool Decay, typename F>
      constexpr auto get_types(F&& fn) {
         if constexpr (is_callable_v<fn>)
            return get_types<Decay>(&F::operator());
         else
            return get_types<Decay>(fn);
      }

      template <bool Decay, typename F>
      using get_types_t = decltype(get_types<Decay>(std::declval<F>()));

      template <std::size_t Sz, std::size_t N, std::size_t I, typename... Args>
      struct pack_from;

      template <std::size_t Sz, std::size_t N, std::size_t I, typename Arg, typename... Args>
      struct pack_from<Sz, N, I, Arg, Args...> {
          static_assert(Sz > N, "index out of range");
          using type = typename pack_from<Sz, N, I+1, Args...>::type;
      };

      template <std::size_t Sz, std::size_t N, typename Arg, typename... Args>
      struct pack_from<Sz, N, N, Arg, Args...> {
          using type = std::tuple<Arg, Args...>;
      };

      template <std::size_t N, typename... Args>
      using pack_from_t = typename pack_from<sizeof...(Args), N, 0, Args...>::type;

      template <std::size_t N, typename R, typename... Args>
      constexpr auto parameters_from_impl(R(Args...)) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)const) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)&) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)&&) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)const &) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename R, typename Cls, typename... Args>
      constexpr auto parameters_from_impl(R(Cls::*)(Args...)const &&) ->  pack_from_t<N, Args...>;
      template <std::size_t N, typename F>
      constexpr auto parameters_from_impl(F&& fn) {
         if constexpr (is_callable_v<fn>)
            return parameters_from_impl<N>(&F::operator());
         else
            return parameters_from_impl<N>(fn);
      }

      template <std::size_t N, typename F>
      using parameters_from_impl_t = decltype(parameters_from_impl<N>(std::declval<F>()));
   } // ns bluegrass::meta::detail

   template <typename R, typename... Args>
   constexpr bool is_function(R(*)(Args...)) { return true; }

   template <typename F>
   constexpr bool is_function(F&&) { return false; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)) { return true; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)const ) { return true; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)& ) { return true; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)&& ) { return true; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)const & ) { return true; }

   template <typename R, typename Cls, typename... Args>
   constexpr bool is_member_function(R(Cls::*)(Args...)const && ) { return true; }

   template <typename F>
   constexpr bool is_member_function(F&&) { return false; }

   template <auto FN>
   inline constexpr static bool is_function_v = is_function(AUTO_PARAM_WORKAROUND(FN));

   template <auto FN>
   inline constexpr static bool is_member_function_v = is_member_function(AUTO_PARAM_WORKAROUND(FN));

   template <typename F>
   constexpr bool is_class(F&&) { return std::is_class_v<F>; }

   template <typename F>
   constexpr auto return_type(F&& fn) -> std::tuple_element_t<0, detail::get_types_t<false, F>>;

   template <auto FN>
   using return_type_t = decltype(return_type(AUTO_PARAM_WORKAROUND(FN)));

   template <typename F>
   constexpr auto class_from_member(F&& fn) -> std::tuple_element_t<1, detail::get_types_t<false, F>>;

   template <auto FN>
   using class_from_member_t = decltype(class_from_member(AUTO_PARAM_WORKAROUND(FN)));

   template <typename F>
   constexpr auto flatten_parameters(F&& fn) -> std::tuple_element_t<2, detail::get_types_t<false, F>>;

   template <auto FN>
   using flatten_parameters_t = decltype(flatten_parameters(AUTO_PARAM_WORKAROUND(FN)));

   template <typename F>
   constexpr auto decayed_flatten_parameters(F&& fn) -> std::tuple_element_t<2, detail::get_types_t<true, F>>;

   template <auto FN>
   using decayed_flatten_parameters_t = decltype(decayed_flatten_parameters(AUTO_PARAM_WORKAROUND(FN)));

   template <std::size_t N, typename F>
   constexpr auto parameter_at(F&& fn) -> std::tuple_element_t<N,
                                             decltype(flatten_parameters(std::declval<F>()))>;

   template <std::size_t N, auto FN>
   using parameter_at_t = decltype(parameter_at<N>(AUTO_PARAM_WORKAROUND(FN)));

   template <std::size_t N, typename F>
   constexpr auto parameters_from(F&& fn) -> detail::parameters_from_impl_t<N, F>;

   template <std::size_t N, auto FN>
   using parameters_from_t = decltype(parameters_from<N>(AUTO_PARAM_WORKAROUND(FN)));

   template <typename F>
   inline constexpr static std::size_t arity(F&& fn) { return std::tuple_size_v<decltype(flatten_parameters(fn))>; }

   template <auto FN>
   inline constexpr static std::size_t arity_v = arity(FN);
}}
