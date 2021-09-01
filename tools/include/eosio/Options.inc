OPTION(prefix_4, &"-profile="[1], profile, Joined, INVALID, INVALID, nullptr, CC1Option, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, &"-abi-version="[1], abi_version, Joined, INVALID, INVALID, nullptr, CC1Option, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, &"-no-abigen"[1], no_abigen, Flag, INVALID, INVALID, nullptr, CC1Option, 0, nullptr, nullptr, nullptr) 
#ifndef ONLY_LD
OPTION(prefix_4, &"-contract"[1], contract, Separate, INVALID, INVALID, nullptr, CC1Option, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, &"-no-missing-ricardian-clause"[1], no_missing_ricardian_clause, Flag, INVALID, INVALID, nullptr, CC1Option, 0, nullptr, nullptr, nullptr)
#endif
