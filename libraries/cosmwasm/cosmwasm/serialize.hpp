#pragma once
#include <boost/preprocessor/seq/for_each.hpp>

#define COSMWASM_SERIALIZE_FIELD(r, data, elem) \
      out.elem = cosmwasm::json::from_json<decltype(elem)>(v.get(#elem));

#define COSMWASM_SERIALIZE(TYPE, FIELDS) \
   static TYPE from_json(const cosmwasm::json::value& v) { \
      TYPE out; \
      BOOST_PP_SEQ_FOR_EACH(COSMWASM_SERIALIZE_FIELD, _, FIELDS) \
      return out; \
   }
