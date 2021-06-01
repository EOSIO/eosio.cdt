#pragma once
#include "builtins.hpp"
#include <memory>
#include <vector>
#include <algorithm>

namespace cosmwasm {

   std::unique_ptr<region> build_region_from_components(uint32_t offset, uint32_t capacity, uint32_t length) {
      return std::make_unique<region>(offset, capacity, length);
   }

   region* alloc(size_t size) {
      void* data = malloc(size);
      return build_region_from_components((uint32_t)data, (uint32_t)size, 0).release();
   }

   std::unique_ptr<region> build_region(char* data, uint32_t length) {
      return build_region_from_components((uint32_t)data, length, length);
   }

   std::unique_ptr<region> build_region(const char* data, uint32_t length) {
      return build_region_from_components((uint32_t)data, length, length);
   }

   std::unique_ptr<region> build_region_dup(char* data, uint32_t length) {
      void* dup = malloc(length);
      std::copy_n(data, length, (char*)dup);
      return build_region_from_components((uint32_t)dup, length, length);
   }

   std::unique_ptr<region> build_region_dup(const char* data, uint32_t length) {
      void* dup = malloc(length);
      std::copy_n(data, length, (char*)dup);
      return build_region_from_components((uint32_t)dup, length, length);
   }

   std::unique_ptr<region> build_region_dup(std::vector<char>& data) {
      return build_region_dup(data.data(), data.size());
   }

   std::unique_ptr<region> build_region_dup(const std::vector<char>& data) {
      return build_region_dup(data.data(), data.size());
   }

   std::vector<char> consume_region(region* ptr) {
      if (!ptr)
         abort();

      std::unique_ptr<region> reg{ptr};
      if (!reg->offset)
         abort();

      std::vector<char> data(reg->length);
      std::copy_n((char*)reg->offset, reg->length, data.begin());
      data.reserve(reg->capacity);

      return data;
   }

} /// namespace cosmwasm
