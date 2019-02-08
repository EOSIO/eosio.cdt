#pragma once
#include "crypto.hpp"

#warning "<eosiolib/public_key.hpp> is deprecated use <eosio/crypto.hpp>"
// This file only exists so that existing contracts that include eosiolib/public_key.hpp do not break.
// Going forward contracts should instead just include eosiolib/crypto.hpp
