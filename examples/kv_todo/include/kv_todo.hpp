#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
using namespace eosio;

struct todo_entry {
  std::string uuid;
  eosio::non_unique<eosio::name, std::string> account_name;
  eosio::non_unique<std::string, std::string> task;
  eosio::non_unique<bool, std::string> checked;
  eosio::non_unique<uint32_t, std::string> created;

  std::string get_uuid() const { return uuid; }
  eosio::name get_account_name() const { return std::get<0>(account_name); }
  std::string get_task() const { return std::get<0>(task); }
  bool get_checked() const { return std::get<0>(checked); }
  int get_created() const { return std::get<0>(created); }
};

class [[eosio::contract]] kv_todo : public contract {
  struct [[eosio::table]] todo_table : eosio::kv::table<todo_entry, "todo"_n> {
    KV_NAMED_INDEX("uuid"_n, uuid);
    KV_NAMED_INDEX("accname"_n, account_name);
    KV_NAMED_INDEX("task"_n, task)
    KV_NAMED_INDEX("checked"_n, checked)
    KV_NAMED_INDEX("created"_n, created)

    // constructor for our `kvtodo` table to setup and initialize it
    todo_table(eosio::name contract_name) { init(contract_name, uuid, account_name, task, checked, created); }
  };
  public:
      using contract::contract;

      [[eosio::action]]
      std::vector<todo_entry> getbyaccname(name account_name);

      [[eosio::action]]
      size_t cntbyaccname(name account_name);

      [[eosio::action]]
      std::vector<todo_entry> getbytask(std::string task);

      [[eosio::action]]
      size_t cntbytask(std::string name);

      [[eosio::action]]
      std::vector<todo_entry> getbychecked(bool value);

      [[eosio::action]]
      size_t cntbychecked(bool checked);

      [[eosio::action]]
      todo_entry upsert(const std::string& uuid,
                        eosio::name account_name,
                        const std::string& task,
                        bool checked);

      [[eosio::action]]
      void del(const std::string& uuid);

      using get_by_account_name_action = action_wrapper<"getbyaccname"_n, &kv_todo::getbyaccname>;
      using cnt_by_account_name_action = action_wrapper<"cntbyaccname"_n, &kv_todo::cntbyaccname>;
      using get_by_task_action = action_wrapper<"getbytask"_n, &kv_todo::getbytask>;
      using cnt_by_task_action = action_wrapper<"cntbytask"_n, &kv_todo::cntbytask>;
      using get_by_checked_action = action_wrapper<"getbychecked"_n, &kv_todo::getbychecked>;
      using cnt_by_checked_action = action_wrapper<"cntbychecked"_n, &kv_todo::cntbychecked>;

      using upsert_action = action_wrapper<"upsert"_n, &kv_todo::upsert>;
      using del_action = action_wrapper<"del"_n, &kv_todo::del>;

  private:
    todo_table todo_entries{"todo"_n};
};
