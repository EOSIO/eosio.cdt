#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

struct kvtest_record {
    uint64_t id;
    std::string data;
};

class [[eosio::contract]] kvtest : public contract {
    struct [[eosio::table]] kvtest_table : eosio::kv::table<kvtest_record, "kvtest"_n> {
        KV_NAMED_INDEX("by.id", id);
        kvtest_table(name contract) { init(contract, id); }
    };

    public:
        using contract::contract;

        [[eosio::action]]
        void smalltest() {
            require_auth(_self);
            kvtest_table table{"kvtest"_n};

	    uint64_t id = 1;
            std::string data = "test";
        
            for (; id < 5000; ++id) {
                table.put({id, data + std::to_string(id)}, get_self());
            }
        	
            for (id = 1; id < 5000; ++id) {
                auto itr = table. id.find(id);
        	if (itr != table.id.end()) {
        	    std::string val = itr.value().data;   
        	    eosio::check(val.compare(data + std::to_string(id)), "The value for the key " + std::to_string(id) + " is " + val);
        	}
            }
        }

        [[eosio::action]]
        void largetest() {
            require_auth(_self);
            kvtest_table table{"kvtest"_n};
            
	    std::string data = std::string(8 * 1024 * 1024, 't');

            uint64_t id = 1;
            for (; id < 200; ++id)
        	table.put({id, std::to_string(id) + data}, get_self());
        	    
            for (id = 1; id < 200; ++id) {
                auto itr = table.id.find(id);
                if (itr != table.id.end()) {
        	    std::string val = itr.value().data;   
        	    eosio::check(val.compare(data + std::to_string(id)), "The value for the key " + std::to_string(id) + " is " + val);
        	}
            }
        }
};
