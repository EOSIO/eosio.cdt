import os, shutil, imp
from logger import err, warn, log

### interface for build systems
class build_system:
    def pre_build(self, dep, cmds):
        pass
    def build(self, dep, cmds):
        pass
    def install(self, dep, cmds):
        pass

### implementation for pre-built sources
class noop_build_system:
    def pre_build(self, dep, cmds):
        pass
    def build(self, dep, cmds):
        pass
    def install(self, dep, cmds):
        log.log("install step for "+dep.name)

def import_build_systems():
    old_cwd = os.getcwd()
    cur_bs = ""
    try:
        prefix = os.path.dirname(os.path.realpath(__file__))+"/build_systems"
        try:
            shutil.rmtree(prefix+"/__pycache__")
        except:
            pass
        build_systems = os.listdir(prefix)
        try:
            for bs in build_systems:
                cur_bs = bs 
                f, p, d = imp.find_module(bs[:-3], [prefix])
                imp.load_module(bs[:-3], f, p, d)
        except Exception as ex:
            warn.log(str(ex))
            raise ex
            err.log("Failed to import build system <"+cur_bs+">")
    except Exception as ex:
        warn.log(str(ex))
        raise ex
        err.log("Failed to open package managers directory")
