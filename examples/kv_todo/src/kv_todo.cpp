#include <kv_todo.hpp>

// example) cleos push action todo getbyaccname '["todo"]' -p todo@active
[[eosio::action]]
std::vector<todo_entry> kv_todo::getbyaccname(name account_name) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.account_name.range({account_name, min_uuid}, {account_name, max_uuid});
}

// example) cleos push action todo cntbyaccname '["todo"]' -p todo@active
[[eosio::action]]
size_t kv_todo::cntbyaccname(name account_name) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.account_name.range({account_name, min_uuid}, {account_name, max_uuid}).size();
}

// example) cleos push action todo getbytask '["Start Blockchain"]' -p todo@active
[[eosio::action]]
std::vector<todo_entry> kv_todo::getbytask(std::string task) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.task.range({task, min_uuid}, {task, max_uuid});
}

// example) cleos push action todo cntbytask '["Start Blockchain"]' -p todo@active
[[eosio::action]]
size_t kv_todo::cntbytask(std::string task) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.task.range({task, min_uuid}, {task, max_uuid}).size();
}

// example) cleos push action todo getbychecked '["false"]' -p todo@active
[[eosio::action]]
std::vector<todo_entry> kv_todo::getbychecked(bool checked) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.checked.range({checked, min_uuid}, {checked, max_uuid});
}

// example) cleos push action todo cntbychecked '["false"]' -p todo@active
[[eosio::action]]
size_t kv_todo::cntbychecked(bool checked) {
   std::string min_uuid = "00000000-0000-0000-0000-000000000000";
   std::string max_uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";

   return todo_entries.checked.range({checked, min_uuid}, {checked, max_uuid}).size();
}

/**
examples)
cleos push action todo upsert '["ac8acfe7-cd4e-4d22-8400-218b697a4517", "todo", "Deploy Hello World Contract", false]' -p todo@active
cleos push action todo upsert '["b7b0d09d-a82b-44d9-b067-3bae2d02917e", "todo", "Start Blockchain", false]' -p todo@active
cleos push action todo upsert '["bf581bee-9f2c-447b-94ad-78e4984b6f51", "todo", "Write Hello World Contract", false]' -p todo@active
*/
[[eosio::action]]
todo_entry kv_todo::upsert(const std::string& uuid,
                           eosio::name account_name,
                           const std::string& task,
                           bool checked) {

   require_auth(account_name);

   auto itr = todo_entries.uuid.find(uuid);
   if (itr != todo_entries.uuid.end()) {
      check(account_name == itr.value().get_account_name(), "Unauthorized");
      todo_entry todo_entry_update = {itr.value().get_uuid(), {itr.value().get_account_name(), uuid}, {task, uuid}, {checked, uuid}, {itr.value().get_created(), uuid}};
      todo_entries.put(todo_entry_update, get_self());
   } else {
      uint32_t created = eosio::current_time_point().sec_since_epoch();
      todo_entry todo_entry_insert = {uuid, {account_name, uuid}, {task,uuid}, {checked, uuid}, {created, uuid}};
      todo_entries.put(todo_entry_insert, get_self());
   }

   itr = todo_entries.uuid.find(uuid);
   return itr.value();
}

[[eosio::action]]
void kv_todo::del(const std::string& uuid) {
   auto itr = todo_entries.uuid.find(uuid);

   if (itr != todo_entries.uuid.end()) {
      todo_entries.erase(itr.value());
      eosio::print_f("todo_entry was successfully deleted from table.");
   } else {
      eosio::print_f("todo_entry not found in table.");
   }
}
