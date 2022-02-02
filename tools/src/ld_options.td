include "lld/wasm/Options.td"


def abi_version : Joined<["--"], "abi-version=">,
    HelpText<"Which ABI version to generate">;
def no_abigen : Flag<["--"], "no-abigen">,
    HelpText<"Disable ABI file generation">;
def headerpad_max_install_names : Flag<["-"], "headerpad_max_install_names">,
    HelpText<"Allocate extra space so all load-command paths can expand to MAXPATHLEN via install_name_tool">;
def search_paths_first : Flag<["-"], "search_paths_first">,
    HelpText<"Search for lib<name>.dylib and lib<name>.a at each step in traversing search path (default for Xcode 4 and later)">;
def keep_generated : Flag<["-","--"], "keep-generated">,
    HelpText<"Keep generated files">;
def show_commands : Flag<["-","--"], "show-commands">,
    HelpText<"Show commands to run">;