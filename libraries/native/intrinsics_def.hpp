#pragma once
#define INTRINSICS(intrinsic_macro) \
intrinsic_macro(intrinsic___ashlti3, ([](int128_t&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___ashrti3, ([](int128_t&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___lshlti3, ([](int128_t&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___lshrti3, ([](int128_t&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___divti3,  ([](int128_t&,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___udivti3, ([](int128_t&,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___modti3,  ([](int128_t&,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___umodti3, ([](int128_t&,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___multi3,  ([](int128_t&,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___addtf3,  ([](long double&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___subtf3,  ([](long double&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___multf3,  ([](long double&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___divtf3,  ([](long double&,uint64_t,uint64_t,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___eqtf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___netf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___getf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___gttf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___lttf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___letf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___cmptf2,  ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___unordtf2, ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___negtf2,   ([](uint64_t,uint64_t,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic___floatsitf,   ([](long double&, int32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___floatunsitf, ([](long double&, uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___floatditf,   ([](long double&, int64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___floatunditf, ([](long double&, uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___floattidf,   ([](uint64_t, uint64_t){eosio_assert(false, "unsupported intrinsic"); return (double)0;})) \
intrinsic_macro(intrinsic___floatuntidf, ([](uint64_t, uint64_t){eosio_assert(false, "unsupported intrinsic"); return (double)0;})) \
intrinsic_macro(intrinsic___floatsidf,   ([](int32_t){eosio_assert(false, "unsupported intrinsic"); return (double)0;})) \
intrinsic_macro(intrinsic___extendsftf2, ([](long double&,float){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___extenddftf2, ([](long double&,double){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixtfti,     ([](int128_t&,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixtfdi,     ([](int128_t&,double){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixtfsi,     ([](int128_t&,float){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixunstfti,  ([](uint128_t&,uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixunstfdi,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (uint64_t)0;})) \
intrinsic_macro(intrinsic___fixunstfsi,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (uint64_t)0;})) \
intrinsic_macro(intrinsic___fixsfti,     ([](long double&,float){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixdfti,     ([](long double&, double){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic___fixunssfti,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (float)0;})) \
intrinsic_macro(intrinsic___fixunsdfti,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (double)0;})) \
intrinsic_macro(intrinsic___trunctfdf2,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (double)0;})) \
intrinsic_macro(intrinsic___trunctfsf2,  ([](uint64_t,uint64_t){eosio_assert(false, "unsupported intrinsic"); return (float)0;})) \
intrinsic_macro(intrinsic_is_feature_active, ([](int64_t){eosio_assert(false, "unsupported intrinsic"); return true;})) \
intrinsic_macro(intrinsic_activate_feature,  ([](int64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_get_resource_limits, ([](capi_name,int64_t*,int64_t*,int64_t*){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_set_resource_limits, ([](capi_name,int64_t,int64_t,int64_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_set_proposed_producers, ([](char*,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_get_blockchain_parameters_packed, ([](char*,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_set_blockchain_parameters_packed, ([](char*,uint32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_is_privileged,  ([](capi_name){eosio_assert(false, "unsupported intrinsic"); return true;})) \
intrinsic_macro(intrinsic_set_privileged, ([](capi_name,bool){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_get_active_producers, ([](capi_name*,size_t){eosio_assert(false, "unsupported intrinsic"); return (int)0;})) \
intrinsic_macro(intrinsic_db_idx64_store, ([](uint64_t,capi_name,capi_name,uint64_t,const void*,int64_t){eosio_assert(false, "unsupported intrinsic"); return (int32_t)0;})) \
intrinsic_macro(intrinsic_db_idx64_remove, ([](int32_t){eosio_assert(false, "unsupported intrinsic");})) \
intrinsic_macro(intrinsic_db_idx64_update, ([](int32_t,capi_name,const uint64_t*){eosio_assert(false, "unsupported intrinsic");}))

#if 0
intrinsic_macro(db_idx64_find_primary)
intrinsic_macro(db_idx64_find_secondary)
intrinsic_macro(db_idx64_lowerbound)
intrinsic_macro(db_idx64_upperbound)
intrinsic_macro(db_idx64_end)
intrinsic_macro(db_idx64_next)
intrinsic_macro(db_idx64_previous)
intrinsic_macro(db_idx128_store)
intrinsic_macro(db_idx128_remove)
intrinsic_macro(db_idx128_update)
intrinsic_macro(db_idx128_find_primary)
intrinsic_macro(db_idx128_find_secondary)
intrinsic_macro(db_idx128_lowerbound)
intrinsic_macro(db_idx128_upperbound)
intrinsic_macro(db_idx128_end)
intrinsic_macro(db_idx128_next)
intrinsic_macro(db_idx128_previous)
intrinsic_macro(db_idx256_store)
intrinsic_macro(db_idx256_remove)
intrinsic_macro(db_idx256_update)
intrinsic_macro(db_idx256_find_primary)
intrinsic_macro(db_idx256_find_secondary)
intrinsic_macro(db_idx256_lowerbound)
intrinsic_macro(db_idx256_upperbound)
intrinsic_macro(db_idx256_end)
intrinsic_macro(db_idx256_next)
intrinsic_macro(db_idx256_previous)
intrinsic_macro(db_idx_double_store)
intrinsic_macro(db_idx_double_remove)
intrinsic_macro(db_idx_double_update)
intrinsic_macro(db_idx_double_find_primary)
intrinsic_macro(db_idx_double_find_secondary)
intrinsic_macro(db_idx_double_lowerbound)
intrinsic_macro(db_idx_double_upperbound)
intrinsic_macro(db_idx_double_end)
intrinsic_macro(db_idx_double_next)
intrinsic_macro(db_idx_double_previous)
intrinsic_macro(db_idx_long_double_store)
intrinsic_macro(db_idx_long_double_remove)
intrinsic_macro(db_idx_long_double_update)
intrinsic_macro(db_idx_long_double_find_primary)
intrinsic_macro(db_idx_long_double_find_secondary)
intrinsic_macro(db_idx_long_double_lowerbound)
intrinsic_macro(db_idx_long_double_upperbound)
intrinsic_macro(db_idx_long_double_end)
intrinsic_macro(db_idx_long_double_next)
intrinsic_macro(db_idx_long_double_previous)
intrinsic_macro(db_idx64_store)
intrinsic_macro(db_idx64_remove)
intrinsic_macro(db_idx64_update)
intrinsic_macro(db_idx64_find_primary)
intrinsic_macro(db_idx64_find_secondary)
intrinsic_macro(db_idx64_lowerbound)
intrinsic_macro(db_idx64_upperbound)
intrinsic_macro(db_idx64_end)
intrinsic_macro(db_idx64_next)
intrinsic_macro(db_idx64_previous)
intrinsic_macro(db_idx128_store)
intrinsic_macro(db_idx128_remove)
intrinsic_macro(db_idx128_update)
intrinsic_macro(db_idx128_find_primary)
intrinsic_macro(db_idx128_find_secondary)
intrinsic_macro(db_idx128_lowerbound)
intrinsic_macro(db_idx128_upperbound)
intrinsic_macro(db_idx128_end)
intrinsic_macro(db_idx128_next)
intrinsic_macro(db_idx128_previous)
intrinsic_macro(db_idx256_store)
intrinsic_macro(db_idx256_remove)
intrinsic_macro(db_idx256_update)
intrinsic_macro(db_idx256_find_primary)
intrinsic_macro(db_idx256_find_secondary)
intrinsic_macro(db_idx256_lowerbound)
intrinsic_macro(db_idx256_upperbound)
intrinsic_macro(db_idx256_end)
intrinsic_macro(db_idx256_next)
intrinsic_macro(db_idx256_previous)
intrinsic_macro(db_idx_double_store)
intrinsic_macro(db_idx_double_remove)
intrinsic_macro(db_idx_double_update)
intrinsic_macro(db_idx_double_find_primary)
intrinsic_macro(db_idx_double_find_secondary)
intrinsic_macro(db_idx_double_lowerbound)
intrinsic_macro(db_idx_double_upperbound)
intrinsic_macro(db_idx_double_end)
intrinsic_macro(db_idx_double_next)
intrinsic_macro(db_idx_double_previous)
intrinsic_macro(db_idx_long_double_store)
intrinsic_macro(db_idx_long_double_remove)
intrinsic_macro(db_idx_long_double_update)
intrinsic_macro(db_idx_long_double_find_primary)
intrinsic_macro(db_idx_long_double_find_secondary)
intrinsic_macro(db_idx_long_double_lowerbound)
intrinsic_macro(db_idx_long_double_upperbound)
intrinsic_macro(db_idx_long_double_end)
intrinsic_macro(db_idx_long_double_next)
intrinsic_macro(db_idx_long_double_previous)
intrinsic_macro(db_store_i64)
intrinsic_macro(db_update_i64)
intrinsic_macro(db_remove_i64)
intrinsic_macro(db_get_i64)
intrinsic_macro(db_next_i64)
intrinsic_macro(db_previous_i64)
intrinsic_macro(db_find_i64)
intrinsic_macro(db_lowerbound_i64)
intrinsic_macro(db_upperbound_i64)
intrinsic_macro(db_end_i64)
intrinsic_macro(assert_recover_key)
intrinsic_macro(recover_key)
intrinsic_macro(assert_sha256)
intrinsic_macro(assert_sha1)
intrinsic_macro(assert_sha512)
intrinsic_macro(assert_ripemd160)
intrinsic_macro(sha1)
intrinsic_macro(sha256)
intrinsic_macro(sha512)
intrinsic_macro(ripemd160)
intrinsic_macro(check_transaction_authorization)
intrinsic_macro(check_permission_authorization)
intrinsic_macro(get_permission_last_used)
intrinsic_macro(get_account_creation_time)
intrinsic_macro(current_time)
intrinsic_macro(publication_time)
intrinsic_macro(abort)
intrinsic_macro(eosio_assert)
intrinsic_macro(eosio_assert_message)
intrinsic_macro(eosio_assert_code)
intrinsic_macro(eosio_exit)
intrinsic_macro(read_action_data)
intrinsic_macro(action_data_size)
intrinsic_macro(current_receiver)
intrinsic_macro(require_recipient)
intrinsic_macro(require_auth)
intrinsic_macro(require_auth2)
intrinsic_macro(has_auth)
intrinsic_macro(is_account)
intrinsic_macro(prints)
intrinsic_macro(prints_l)
intrinsic_macro(printi)
intrinsic_macro(printui)
intrinsic_macro(printi128)
intrinsic_macro(printui128)
intrinsic_macro(printsf)
intrinsic_macro(printdf)
intrinsic_macro(printqf)
intrinsic_macro(printn)
intrinsic_macro(printhex)
intrinsic_macro(read_transaction)
intrinsic_macro(transaction_size)
intrinsic_macro(expiration)
intrinsic_macro(tapos_block_prefix)
intrinsic_macro(tapos_block_num)
intrinsic_macro(get_action)
intrinsic_macro(send_inline)
intrinsic_macro(send_context_free_inline)
intrinsic_macro(send_deferred)
intrinsic_macro(cancel_deferred)
intrinsic_macro(get_context_free_data)
intrinsic_macro(memcpy)
intrinsic_macro(memmove)
intrinsic_macro(memcmp)
intrinsic_macro(memset)
intrinsic_macro(_eosio_f32_add)
intrinsic_macro(_eosio_f32_sub)
intrinsic_macro(_eosio_f32_mul)
intrinsic_macro(_eosio_f32_div)
intrinsic_macro(_eosio_f32_min)
intrinsic_macro(_eosio_f32_max)
intrinsic_macro(_eosio_f32_copysign)
intrinsic_macro(_eosio_f32_abs)
intrinsic_macro(_eosio_f32_neg)
intrinsic_macro(_eosio_f32_sqrt)
intrinsic_macro(_eosio_f32_ceil)
intrinsic_macro(_eosio_f32_floor)
intrinsic_macro(_eosio_f32_trunc)
intrinsic_macro(_eosio_f32_nearest)
intrinsic_macro(_eosio_f32_eq)
intrinsic_macro(_eosio_f32_ne)
intrinsic_macro(_eosio_f32_lt)
intrinsic_macro(_eosio_f32_le)
intrinsic_macro(_eosio_f32_gt)
intrinsic_macro(_eosio_f32_ge)
intrinsic_macro(_eosio_f64_add)
intrinsic_macro(_eosio_f64_sub)
intrinsic_macro(_eosio_f64_mul)
intrinsic_macro(_eosio_f64_div)
intrinsic_macro(_eosio_f64_min)
intrinsic_macro(_eosio_f64_max)
intrinsic_macro(_eosio_f64_copysign)
intrinsic_macro(_eosio_f64_abs)
intrinsic_macro(_eosio_f64_neg)
intrinsic_macro(_eosio_f64_sqrt)
intrinsic_macro(_eosio_f64_ceil)
intrinsic_macro(_eosio_f64_floor)
intrinsic_macro(_eosio_f64_trunc)
intrinsic_macro(_eosio_f64_nearest)
intrinsic_macro(_eosio_f64_eq)
intrinsic_macro(_eosio_f64_ne)
intrinsic_macro(_eosio_f64_lt)
intrinsic_macro(_eosio_f64_le)
intrinsic_macro(_eosio_f64_gt)
intrinsic_macro(_eosio_f64_ge)
intrinsic_macro(_eosio_f32_promote)
intrinsic_macro(_eosio_f64_demote)
intrinsic_macro(_eosio_f32_trunc_i32s)
intrinsic_macro(_eosio_f64_trunc_i32s)
intrinsic_macro(_eosio_f32_trunc_i32u)
intrinsic_macro(_eosio_f64_trunc_i32u)
intrinsic_macro(_eosio_f32_trunc_i64s)
intrinsic_macro(_eosio_f64_trunc_i64s)
intrinsic_macro(_eosio_f32_trunc_i64u)
intrinsic_macro(_eosio_f64_trunc_i64u)
intrinsic_macro(_eosio_i32_to_f32)
intrinsic_macro(_eosio_i64_to_f32)
intrinsic_macro(_eosio_ui32_to_f32)
intrinsic_macro(_eosio_ui64_to_f32)
intrinsic_macro(_eosio_i32_to_f64)
intrinsic_macro(_eosio_i64_to_f64)
intrinsic_macro(_eosio_ui32_to_f64)
intrinsic_macro(_eosio_ui64_to_f64)
#endif

#define CREATE_STUB(name, func) \
   static constexpr auto ___stub ## name = func;

#define CREATE_ENUM(name, func) \
   name,

#define GET_TYPE(name, func) \
   decltype(___stub ## name),

#define REGISTER_INTRINSIC(name, func) \
   ___stub ## name,
