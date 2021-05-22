#pragma once
#include "types.hpp"
#include "storage.hpp"

namespace cosmwasm {

   class contract {
   public:
      contract(env _env, message_info _info, storage _storage = {}): _env(_env), _info(_info) {}

   protected:
      env _env;
      message_info _info;
      storage _storage;
   };

}
