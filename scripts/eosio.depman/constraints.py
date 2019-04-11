import util
### utility functions for "contraints" in dependency file

def get_os_type():
    return get_os()[0]

def get_os_name():
    return get_os()[1]

def get_os_version():
    return get_os()[2]

def is_c_compiler_apple_clang():
    eo, ee, ec = execute_cmd("cc -v")
    if eo and ec == 0:
        return eo.split()[0] == "Apple"
    else:
        eo, ee, ec = execute_cmd("CC -v")
        if eo and ec == 0:
            return eo.split()[0] == "Apple"

def is_cpp_compiler_apple_clang():
    eo, ee, ec = execute_cmd("c++ -v")
    if eo and ec == 0:
        return eo.split()[0] == "Apple"
    else:
        eo, ee, ec = execute_cmd("C++ -v")
        if eo and ec == 0:
            return eo.split()[0] == "Apple"

def get_c_compiler():
    if is_c_compiler_apple_clang():
        return "apple clang"

