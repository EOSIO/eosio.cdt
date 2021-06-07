#pragma once
#include "stringize.hpp"
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/stringize.hpp>

#define COSMWASM_DESERIALIZE_FIELD(r, data, elem) \
      out.elem = cosmwasm::json::from_json<decltype(elem)>(v.get(BOOST_PP_STRINGIZE(elem)));

#define COSMWASM_SERIALIZE_FIELD(r, data, elem) \
      out.insert_or_assign(BOOST_PP_STRINGIZE(elem), cosmwasm::json::to_json(v.elem));

#define COSMWASM_SERIALIZE2(TYPE, TYPENAME, ...) \
   static constexpr const char* __typename = TYPENAME; \
   static TYPE from_json(const cosmwasm::json::value& v) { \
      TYPE out; \
      __VA_OPT__(BOOST_PP_SEQ_FOR_EACH(COSMWASM_DESERIALIZE_FIELD, _, __VA_ARGS__)) \
      return out; \
   } \
   static cosmwasm::json::value to_json(const TYPE& v) { \
      cosmwasm::json::value::object out; \
      __VA_OPT__(BOOST_PP_SEQ_FOR_EACH(COSMWASM_SERIALIZE_FIELD, _, __VA_ARGS__)) \
      return cosmwasm::json::value(out); \
   } \
   std::string to_string() { \
      return to_json(*this).serialize(); \
   } \
   const std::string to_string() const { \
      return to_json(*this).serialize(); \
   } \
   explicit operator std::string() { \
      return to_string(); \
   } \
   cosmwasm::bytes as_bytes() { \
      std::string s = to_string(); \
      cosmwasm::bytes out(s.size()); \
      std::copy_n(s.data(), s.size(), out.data()); \
      return out; \
   } \
   const cosmwasm::bytes as_bytes() const { \
      std::string s = to_string(); \
      cosmwasm::bytes out(s.size()); \
      std::copy_n(s.data(), s.size(), out.data()); \
      return out; \
   } \
   auto to_tuple() { \
      return std::make_tuple( __VA_OPT__(BOOST_PP_SEQ_ENUM(__VA_ARGS__)) ); \
   } \
   const auto to_tuple() const { \
      return std::make_tuple( __VA_OPT__(BOOST_PP_SEQ_ENUM(__VA_ARGS__)) ); \
   }

#define COSMWASM_SERIALIZE(TYPE, ...) \
   COSMWASM_SERIALIZE2(TYPE, #TYPE, __VA_ARGS__)
