include "clang/Driver/Options.td"

def abi_version : Joined<["--"], "abi-version=">, Flags<[CC1Option]>,
    HelpText<"Which ABI version to generate">;
def contract : JoinedOrSeparate<["-","--"], "contract=">, Flags<[CC1Option]>,
    HelpText<"Contract name">;
def no_abigen : Flag<["--"], "no-abigen">, Flags<[CC1Option]>,
    HelpText<"Disable ABI file generation">;
def no_missing_ricardian_clause : Flag<["-","--"], "no-missing-ricardian-clause">, Flags<[CC1Option]>,
    HelpText<"Disable warnings for missing Ricardian clauses">;
def export_fn : JoinedOrSeparate<["--"], "export-fn=">, Flags<[CC1Option]>,
    HelpText<"Export this function, shortcut for combining --export with --only-export">;
def fcustom_opt : Flag<["-"], "fcustom_opt">, Flags<[CC1Option]>,
    HelpText<"Produce a wasm which isn't a contract">;
def ftester : Flag<["-"], "ftester">, Flags<[CC1Option]>,
    HelpText<"Produce binaries for eosio-tester">;
