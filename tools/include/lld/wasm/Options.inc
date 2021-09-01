/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Option Parsing Definitions                                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

/////////
// Prefixes

#ifdef PREFIX
#define COMMA ,
PREFIX(prefix_0, {nullptr})
PREFIX(prefix_2, {"-" COMMA nullptr})
PREFIX(prefix_3, {"--" COMMA nullptr})
PREFIX(prefix_1, {"--" COMMA "-" COMMA nullptr})
#undef COMMA
#endif // PREFIX

/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, &"<input>"[0], INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, &"<unknown>"[0], UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--allow-undefined-file="[2], allow_undefined_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Allow symbols listed in <file> to be undefined in linked binary", nullptr, nullptr)
OPTION(prefix_2, &"-allow-undefined-file"[1], allow_undefined_file_s, Separate, INVALID, allow_undefined_file, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--allow-undefined"[2], allow_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow undefined symbols in linked binary", nullptr, nullptr)
OPTION(prefix_1, &"--Bsymbolic"[2], Bsymbolic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind defined symbols locally", nullptr, nullptr)
OPTION(prefix_1, &"--check-features"[2], check_features, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Check feature compatibility of linked objects (default)", nullptr, nullptr)
OPTION(prefix_1, &"--color-diagnostics="[2], color_diagnostics_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_1, &"--color-diagnostics"[2], color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_1, &"--compress-relocations"[2], compress_relocations, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Compress the relocation targets in the code section.", nullptr, nullptr)
OPTION(prefix_1, &"--demangle"[2], demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names", nullptr, nullptr)
OPTION(prefix_1, &"--disable-verify"[2], disable_verify, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--emit-relocs"[2], emit_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate relocations in output", nullptr, nullptr)
OPTION(prefix_1, &"--entry="[2], anonymous_1, Joined, INVALID, entry, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--entry"[2], entry, Separate, INVALID, INVALID, nullptr, 0, 0,
       "Name of entry point symbol", "<entry>", nullptr)
OPTION(prefix_1, &"--error-limit="[2], error_limit, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_1, &"--error-unresolved-symbols"[2], error_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as errors", nullptr, nullptr)
OPTION(prefix_1, &"--experimental-pic"[2], experimental_pic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable Experimental PIC", nullptr, nullptr)
OPTION(prefix_1, &"--export-all"[2], export_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Export all symbols (normally combined with --no-gc-sections)", nullptr, nullptr)
OPTION(prefix_1, &"--export-dynamic"[2], export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Put symbols in the dynamic symbol table", nullptr, nullptr)
OPTION(prefix_1, &"--export-table"[2], export_table, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Export function table to the environment", nullptr, nullptr)
OPTION(prefix_1, &"--export="[2], export_eq, Joined, INVALID, export, nullptr, 0, 0,
       "Force a symbol to be exported", nullptr, nullptr)
OPTION(prefix_1, &"--export"[2], export, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"-E"[1], anonymous_2, Flag, INVALID, export_dynamic, nullptr, 0, 0,
       "Alias for --export-dynamic", nullptr, nullptr)
OPTION(prefix_2, &"-e"[1], anonymous_0, JoinedOrSeparate, INVALID, entry, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--fatal-warnings"[2], fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_1, &"--features="[2], features, CommaJoined, INVALID, INVALID, nullptr, 0, 0,
       "Comma-separated used features, inferred from input objects by default.", nullptr, nullptr)
OPTION(prefix_1, &"--gc-sections"[2], gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable garbage collection of unused sections", nullptr, nullptr)
OPTION(prefix_1, &"--global-base="[2], global_base, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Where to start to place global data", nullptr, nullptr)
OPTION(prefix_1, &"--growable-table"[2], growable_table, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove maximum size from function table, allowing table to grow", nullptr, nullptr)
OPTION(prefix_1, &"--help"[2], help, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print option help", nullptr, nullptr)
OPTION(prefix_1, &"--import-memory"[2], import_memory, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Import memory from the environment", nullptr, nullptr)
OPTION(prefix_1, &"--import-table"[2], import_table, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Import function table from the environment", nullptr, nullptr)
OPTION(prefix_1, &"--initial-memory="[2], initial_memory, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Initial size of the linear memory", nullptr, nullptr)
OPTION(prefix_2, &"-i"[1], anonymous_3, Flag, INVALID, initial_memory, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--lto-debug-pass-manager"[2], lto_debug_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Debug new pass manager", nullptr, nullptr)
OPTION(prefix_3, &"--lto-legacy-pass-manager"[2], lto_legacy_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use legacy pass manager", nullptr, nullptr)
OPTION(prefix_1, &"--lto-O"[2], lto_O, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Optimization level for LTO", "<opt-level>", nullptr)
OPTION(prefix_1, &"--lto-partitions="[2], lto_partitions, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of LTO codegen partitions", nullptr, nullptr)
OPTION(prefix_2, &"-L"[1], L, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Add a directory to the library search path", "<dir>", nullptr)
OPTION(prefix_2, &"-l"[1], l, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Root name of library to use", "<libName>", nullptr)
OPTION(prefix_1, &"--Map="[2], Map_eq, Joined, INVALID, Map, nullptr, 0, 0,
       "Print a link map to the specified file", nullptr, nullptr)
OPTION(prefix_1, &"--Map"[2], Map, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--max-memory="[2], max_memory, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Maximum size of the linear memory", nullptr, nullptr)
OPTION(prefix_1, &"--merge-data-segments"[2], merge_data_segments, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable merging data segments", nullptr, nullptr)
OPTION(prefix_1, &"--mllvm"[2], mllvm, Separate, INVALID, INVALID, nullptr, 0, 0,
       "Options to pass to LLVM", nullptr, nullptr)
OPTION(prefix_2, &"-M"[1], anonymous_4, Flag, INVALID, print_map, nullptr, 0, 0,
       "Alias for --print-map", nullptr, nullptr)
OPTION(prefix_2, &"-m"[1], m, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Set target emulation", nullptr, nullptr)
OPTION(prefix_1, &"--no-check-features"[2], no_check_features, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignore feature compatibility of linked objects", nullptr, nullptr)
OPTION(prefix_1, &"--no-color-diagnostics"[2], no_color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_1, &"--no-demangle"[2], no_demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbol names", nullptr, nullptr)
OPTION(prefix_1, &"--no-entry"[2], no_entry, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not output any entry point", nullptr, nullptr)
OPTION(prefix_1, &"--no-export-dynamic"[2], no_export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not put symbols in the dynamic symbol table (default)", nullptr, nullptr)
OPTION(prefix_1, &"--no-fatal-warnings"[2], no_fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--no-gc-sections"[2], no_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable garbage collection of unused sections", nullptr, nullptr)
OPTION(prefix_3, &"--no-lto-legacy-pass-manager"[2], no_lto_legacy_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use new pass manager", nullptr, nullptr)
OPTION(prefix_1, &"--no-merge-data-segments"[2], no_merge_data_segments, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable merging data segments", nullptr, nullptr)
OPTION(prefix_1, &"--no-pie"[2], no_pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not create a position independent executable (default)", nullptr, nullptr)
OPTION(prefix_1, &"--no-print-gc-sections"[2], no_print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not list removed unused sections", nullptr, nullptr)
OPTION(prefix_1, &"--no-whole-archive"[2], no_whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not force load of all members in a static library (default)", nullptr, nullptr)
OPTION(prefix_2, &"-O"[1], O, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Optimize output file size", nullptr, nullptr)
OPTION(prefix_2, &"-o"[1], o, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", "<path>", nullptr)
OPTION(prefix_1, &"--pie"[2], pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create a position independent executable", nullptr, nullptr)
OPTION(prefix_1, &"--print-gc-sections"[2], print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "List removed unused sections", nullptr, nullptr)
OPTION(prefix_1, &"--print-map"[2], print_map, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print a link map to the standard output", nullptr, nullptr)
OPTION(prefix_1, &"--relocatable"[2], relocatable, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create relocatable object file", nullptr, nullptr)
OPTION(prefix_1, &"--reproduce="[2], reproduce_eq, Joined, INVALID, reproduce, nullptr, 0, 0,
       "Dump linker invocation and input files for debugging", nullptr, nullptr)
OPTION(prefix_1, &"--reproduce"[2], reproduce, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--rsp-quoting="[2], rsp_quoting_eq, Joined, INVALID, rsp_quoting, nullptr, 0, 0,
       "Quoting style for response files", "[posix,windows]", nullptr)
OPTION(prefix_1, &"--rsp-quoting"[2], rsp_quoting, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[posix,windows]", nullptr)
OPTION(prefix_2, &"-r"[1], anonymous_5, Flag, INVALID, relocatable, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--save-temps"[2], save_temps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_1, &"--shared-memory"[2], shared_memory, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use shared linear memory", nullptr, nullptr)
OPTION(prefix_1, &"--shared"[2], shared, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Build a shared object", nullptr, nullptr)
OPTION(prefix_1, &"--stack-first"[2], stack_first, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Place stack at start of linear memory rather than after data", nullptr, nullptr)
OPTION(prefix_1, &"--strip-all"[2], strip_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip all symbols", nullptr, nullptr)
OPTION(prefix_1, &"--strip-debug"[2], strip_debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip debugging information", nullptr, nullptr)
OPTION(prefix_2, &"-S"[1], anonymous_7, Flag, INVALID, strip_debug, nullptr, 0, 0,
       "Alias for --strip-debug", nullptr, nullptr)
OPTION(prefix_2, &"-s"[1], anonymous_6, Flag, INVALID, strip_all, nullptr, 0, 0,
       "Alias for --strip-all", nullptr, nullptr)
OPTION(prefix_1, &"--thinlto-cache-dir="[2], thinlto_cache_dir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_1, &"--thinlto-cache-policy="[2], thinlto_cache_policy_eq, Joined, INVALID, thinlto_cache_policy, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_1, &"--thinlto-cache-policy"[2], thinlto_cache_policy, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--thinlto-jobs="[2], thinlto_jobs, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of ThinLTO jobs. Default to --threads=", nullptr, nullptr)
OPTION(prefix_1, &"--threads="[2], threads_eq, Joined, INVALID, threads, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_1, &"--threads"[2], threads, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--trace-symbol="[2], trace_symbol_eq, Joined, INVALID, trace_symbol, nullptr, 0, 0,
       "Trace references to symbols", nullptr, nullptr)
OPTION(prefix_1, &"--trace-symbol"[2], trace_symbol, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--trace"[2], trace, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print the names of the input files", nullptr, nullptr)
OPTION(prefix_2, &"-t"[1], anonymous_8, Flag, INVALID, trace, nullptr, 0, 0,
       "Alias for --trace", nullptr, nullptr)
OPTION(prefix_1, &"--undefined="[2], undefined_eq, Joined, INVALID, undefined, nullptr, 0, 0,
       "Force undefined symbol during linking", nullptr, nullptr)
OPTION(prefix_1, &"--undefined"[2], undefined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--unresolved-symbols="[2], unresolved_symbols_eq, Joined, INVALID, unresolved_symbols, nullptr, 0, 0,
       "Determine how to handle unresolved symbols", nullptr, nullptr)
OPTION(prefix_1, &"--unresolved-symbols"[2], unresolved_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"-u"[1], anonymous_10, JoinedOrSeparate, INVALID, undefined, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"--verbose"[2], verbose, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Verbose mode", nullptr, nullptr)
OPTION(prefix_1, &"--version"[2], version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_2, &"-v"[1], v, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number", nullptr, nullptr)
OPTION(prefix_1, &"--warn-unresolved-symbols"[2], warn_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as warnings", nullptr, nullptr)
OPTION(prefix_1, &"--whole-archive"[2], whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Force load of all members in a static library", nullptr, nullptr)
OPTION(prefix_1, &"--wrap="[2], wrap_eq, Joined, INVALID, wrap, nullptr, 0, 0,
       "Use wrapper functions for symbol", "<symbol>=<symbol>", nullptr)
OPTION(prefix_1, &"--wrap"[2], wrap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>=<symbol>", nullptr)
OPTION(prefix_2, &"-y"[1], anonymous_9, JoinedOrSeparate, INVALID, trace_symbol, nullptr, 0, 0,
       "Alias for --trace-symbol", nullptr, nullptr)
OPTION(prefix_2, &"-z"[1], z, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Linker option extensions", "<option>", nullptr)
#endif // OPTION

#ifdef SIMPLE_ENUM_VALUE_TABLE

struct SimpleEnumValue {
  const char *Name;
  unsigned Value;
};

struct SimpleEnumValueTable {
  const SimpleEnumValue *Table;
  unsigned Size;
};
static const SimpleEnumValueTable SimpleEnumValueTables[] = {};
static const unsigned SimpleEnumValueTablesSize = sizeof(SimpleEnumValueTables) / sizeof(SimpleEnumValueTable);
#endif // SIMPLE_ENUM_VALUE_TABLE


#ifdef OPTTABLE_ARG_INIT
//////////
// Option Values


#endif // OPTTABLE_ARG_INIT
