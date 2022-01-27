include "clang/Driver/Options.td"

def abi_version : Joined<["--"], "abi-version=">, Flags<[CC1Option]>,
    HelpText<"Which ABI version to generate">;
def contract : JoinedOrSeparate<["-","--"], "contract=">, Flags<[CC1Option]>,
    HelpText<"Contract name">;
def no_abigen : Flag<["--"], "no-abigen">, Flags<[CC1Option]>,
    HelpText<"Disable ABI file generation">;
def no_missing_ricardian_clause : Flag<["-","--"], "no-missing-ricardian-clause">, Flags<[CC1Option]>,
    HelpText<"Disable warnings for missing Ricardian clauses">;
