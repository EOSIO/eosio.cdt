#pragma once
#include "stringize.hpp"
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

#define COSMWASM_DESERIALIZE_FIELD(r, data, elem) \
      out.elem = cosmwasm::json::from_json<decltype(elem)>(v.get(BOOST_PP_STRINGIZE(elem)));

#define COSMWASM_SERIALIZE_FIELD(r, data, elem) \
      out.insert_or_assign(BOOST_PP_STRINGIZE(elem), cosmwasm::json::to_json(v.elem));

#define COSMWASM_SERIALIZE(TYPE, FIELDS) \
   static TYPE from_json(const cosmwasm::json::value& v) { \
      TYPE out; \
      BOOST_PP_SEQ_FOR_EACH(COSMWASM_DESERIALIZE_FIELD, _, FIELDS) \
      return out; \
   } \
   static cosmwasm::json::value to_json(const TYPE& v) { \
      cosmwasm::json::value::object out; \
      BOOST_PP_SEQ_FOR_EACH(COSMWASM_SERIALIZE_FIELD, _, FIELDS) \
      return cosmwasm::json::value(out); \
   } \
   std::string to_string() { \
      return to_json(*this).serialize(); \
   } \
   explicit operator std::string() { \
      return to_string(); \
   }
