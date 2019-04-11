import os, re
from util import execute_cmd_dump_output, execute_cmd

class dependency:
    def __init__(self, nm, vs, st, pn, en, bs, su, bu, pbc, bc, ic):
        self.name         = nm
        self.version      = vs
        self.strict       = st
        self.build_sys    = bs
        self.source_url   = su
        self.package_name = pn
        self.exe_name     = en
        self.bin_url      = bu
        self.pre_build_cmds = pbc
        self.build_cmds   = bc
        self.install_cmds = ic

    def __init__(self, nm, vs, t, bs):
        self.name         = nm
        self.version      = vs
        self.type         = t
        self.build_sys    = bs

    def is_library(self):
        return self.type == "lib"
    
    def is_executable(self):
        return self.type == "exe"
    
    def find_executable(self):
        eo, ee, ec = execute_cmd("which "+self.name)
        if eo and ec == 0:
            neo, ee, ec = execute_cmd(self.name+" --version")
            if neo and ec == 0:
                vers_str = re.findall("\d+.\d+", neo)[0]
                dep = dependency(self.name, version(vers_str.split(".")[0], vers_str.split(".")[1]), "exe", "none")
                installed_dep = installed_dependency( dep, False, os.path.dirname(eo), list() )
                return installed_dep
        return None

    name             = "***"
    package_name     = "***"
    exe_name         = "***"
    strict           = False
    version          = ""
    type             = ""
    build_sys        = ""
    source_url       = ""
    bin_url          = ""
    build_cmds       = ""
    pre_build_cmds   = ""
    install_cmds     = ""

class installed_dependency:
    def __init__(self, d, prov, p, fs):
        self.dep      = d
        self.provided = prov
        self.path     = p
        self.files    = fs
    
    def execute(self, *cmds):
        cmd_str = ""
        for cmd in cmds:
            cmd_str += " "+cmd
        _bin = ""
        if os.path.isfile(os.path.join(self.path, self.dep.name)):
            _bin = os.path.join(self.path, self.dep.name)
        else:
            _bin = os.path.join(self.path, "bin", self.dep.name)
        return execute_cmd_dump_output(_bin+cmd_str)

    dep      = None
    provided = False
    path     = ""
    files    = list()

class version:
    def __init__(self, major, minor, patch):
        self.major = int(major)
        self.minor = int(minor)
        self.patch = int(patch)

    def __init__(self, major, minor):
        self.major = int(major)
        self.minor = int(minor)
    
    def satisfies(self, is_strict, v):
        if self.major == -1 and self.minor == -1:
            return True
        if v.major == -1 and v.minor == -1:
            return True
        if is_strict:
            return self.major == v.major and self.minor == v.minor
        else:
            return self.major >= v.major and self.minor >= v.minor

    def ge(self, v):
        if self.major == -1 and self.minor == -1:
            return True
        if v.major == -1 and v.minor == -1:
            return True
        return self.major >= v.major and self.minor >= v.minor

    def eq(self, v):
        if self.major == -1 and self.minor == -1:
            return True
        if v.major == -1 and v.minor == -1:
            return True
        return self.major == v.major and self.minor == v.minor

    def to_string(self):
        if self.major == -1 and self.minor == -1:
            return "any"
        return str(self.major)+"."+str(self.minor)

    major = 0
    minor = 0
    patch = -1
