/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */

#include "crypto.hpp"
#include "datastream.hpp"

namespace eosio {

   void assert_sha256( const char* data, uint32_t length, const eosio::checksum256& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha256( data, length, reinterpret_cast<const ::capi_checksum256*>(hash_data.data()) );
   }

   void assert_sha1( const char* data, uint32_t length, const eosio::checksum160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha1( data, length, reinterpret_cast<const ::capi_checksum160*>(hash_data.data()) );
   }

   void assert_sha512( const char* data, uint32_t length, const eosio::checksum512& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha512( data, length, reinterpret_cast<const ::capi_checksum512*>(hash_data.data()) );
   }

   void assert_ripemd160( const char* data, uint32_t length, const eosio::checksum160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_ripemd160( data, length, reinterpret_cast<const ::capi_checksum160*>(hash_data.data()) );
   }

   eosio::checksum256 sha256( const char* data, uint32_t length ) {
      ::capi_checksum256 hash;
      ::sha256( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum160 sha1( const char* data, uint32_t length ) {
      ::capi_checksum160 hash;
      ::sha1( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum512 sha512( const char* data, uint32_t length ) {
      ::capi_checksum512 hash;
      ::sha512( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum160 ripemd160( const char* data, uint32_t length ) {
      ::capi_checksum160 hash;
      ::ripemd160( data, length, &hash );
      return {hash.hash};
   }

   eosio::public_key recover_key( const eosio::checksum256& digest, const eosio::signature& sig ) {
      auto digest_data = digest.extract_as_byte_array();

      char sig_data[70];
      eosio::datastream<char*> sig_ds( sig_data, sizeof(sig_data) );
      auto sig_begin = sig_ds.pos();
      sig_ds << sig;

      char pubkey_data[38];
      size_t pubkey_size = ::recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()),
                                          sig_begin, (sig_ds.pos() - sig_begin),
                                          pubkey_data, sizeof(pubkey_data) );
      eosio::datastream<char*> pubkey_ds( pubkey_data, pubkey_size );
      eosio::public_key pubkey;
      pubkey_ds >> pubkey;
      return pubkey;
   }

   std::string eosio::public_key::to_string() {
     // copy public key bytes
     std::array<char,33> datacopy = data;
     unsigned char to_encode[37];
     memcpy(to_encode, datacopy.data(), 33);

     // Calculate and concatenate checksum
     checksum160 checksum = ripemd160((const char *)datacopy.begin(), 33);
     memcpy(to_encode + 33, checksum.extract_as_byte_array().data(), 4);

     // convert to base58
     char b58[51];
     size_t b58sz = 51;
     b58enc(b58, &b58sz, (const uint8_t *)to_encode, 37);

     return std::string((const char *)b58);
   }

   void assert_recover_key( const eosio::checksum256& digest, const eosio::signature& sig, const eosio::public_key& pubkey ) {
      auto digest_data = digest.extract_as_byte_array();

      char sig_data[70];
      eosio::datastream<char*> sig_ds( sig_data, sizeof(sig_data) );
      auto sig_begin = sig_ds.pos();
      sig_ds << sig;

      char pubkey_data[38];
      eosio::datastream<char*> pubkey_ds( pubkey_data, sizeof(pubkey_data) );
      auto pubkey_begin = pubkey_ds.pos();
      pubkey_ds << pubkey;

      ::assert_recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()),
                            sig_begin, (sig_ds.pos() - sig_begin),
                            pubkey_begin, (pubkey_ds.pos() - pubkey_begin) );
   }


   bool b58enc(char *b58, size_t *b58sz, const uint8_t *bin, size_t binsz)
   {
       int carry;
       ssize_t i, j, high, zcount = 0;
       size_t size;

       while (zcount < (ssize_t)binsz && !bin[zcount])
           ++zcount;

       size = (binsz - zcount) * 138 / 100 + 1;
       uint8_t buf[size];
       memzero(buf, size);

       for (i = zcount, high = size - 1; i < (ssize_t)binsz; ++i, high = j)
       {
           for (carry = bin[i], j = size - 1; (j > high) || carry; --j)
           {
               carry += 256 * buf[j];
               buf[j] = carry % 58;
               carry /= 58;
           }
       }

       for (j = 0; j < (ssize_t)size && !buf[j]; ++j);

       if (*b58sz <= zcount + size - j)
       {
           *b58sz = zcount + size - j + 1;
           return false;
       }

       if (zcount)
           memset(b58, '1', zcount);
       for (i = zcount; j < (ssize_t)size; ++i, ++j)
           b58[i] = b58digits_ordered[buf[j]];
       b58[i] = '\0';
       *b58sz = i + 1;

       return true;
   }

   bool b58tobin(void *bin, size_t *binszp, const char *b58)
   {
       size_t binsz = *binszp;

       if (binsz == 0) {
           return false;
       }

       const unsigned char *b58u = (const unsigned char*)b58;
       unsigned char *binu = (unsigned char *)bin;
       size_t outisz = (binsz + 3) / 4;
       uint32_t outi[outisz];
       uint64_t t;
       uint32_t c;
       size_t i, j;
       uint8_t bytesleft = binsz % 4;
       uint32_t zeromask = bytesleft ? (0xffffffff << (bytesleft * 8)) : 0;
       unsigned zerocount = 0;
       size_t b58sz;

       b58sz = strlen(b58);

       memzero(outi, sizeof(outi));

       // Leading zeros, just count
       for (i = 0; i < b58sz && b58u[i] == '1'; ++i)
           ++zerocount;

       for ( ; i < b58sz; ++i)
       {
           if (b58u[i] & 0x80)
               // High-bit set on invalid digit
               return false;
           if (b58digits_map[b58u[i]] == -1)
               // Invalid base58 digit
               return false;
           c = (unsigned)b58digits_map[b58u[i]];
           for (j = outisz; j--; )
           {
               t = ((uint64_t)outi[j]) * 58 + c;
               c = (t & 0x3f00000000) >> 32;
               outi[j] = t & 0xffffffff;
           }
           if (c)
               // Output number too big (carry to the next int32)
               return false;
           if (outi[0] & zeromask)
               // Output number too big (last int32 filled too far)
               return false;
       }

       j = 0;
       switch (bytesleft) {
           case 3:
               *(binu++) = (outi[0] &   0xff0000) >> 16;
               //-fallthrough
           case 2:
               *(binu++) = (outi[0] &     0xff00) >>  8;
               //-fallthrough
           case 1:
               *(binu++) = (outi[0] &       0xff);
               ++j;
               //-fallthrough
           default:
               break;
       }

       for (; j < outisz; ++j)
       {
           *(binu++) = (outi[j] >> 0x18) & 0xff;
           *(binu++) = (outi[j] >> 0x10) & 0xff;
           *(binu++) = (outi[j] >>    8) & 0xff;
           *(binu++) = (outi[j] >>    0) & 0xff;
       }

       // Count canonical base58 byte count
       binu = (unsigned char *)bin;
       for (i = 0; i < binsz; ++i)
       {
           if (binu[i]) {
               if (zerocount > i) {
                   /* result too large */
                   return false;
               }
               break;
           }
           --*binszp;
       }
       *binszp += zerocount;

       return true;
   }

   void memzero(void *const pnt, const size_t len)
   {
   // I'm pretty sure none of the commented out stuff below
   // is needed for WASM but I've left it there in case
   //  - Kedar Iyer
   //#ifdef _WIN32
   //    SecureZeroMemory(pnt, len);
   //#elif defined(HAVE_MEMSET_S)
   //    memset_s(pnt, (rsize_t) len, 0, (rsize_t) len);
   //#elif defined(HAVE_EXPLICIT_BZERO)
   //    explicit_bzero(pnt, len);
   //#elif defined(HAVE_EXPLICIT_MEMSET)
   //    explicit_memset(pnt, 0, len);
   //#else
       volatile unsigned char *volatile pnt_ =
           (volatile unsigned char *volatile) pnt;
       size_t i = (size_t) 0U;

       while (i < len) {
           pnt_[i++] = 0U;
       }
   //#endif
   }

}
