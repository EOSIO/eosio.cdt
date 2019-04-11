import os
from logger import err, warn, log
from util import execute_cmd_dump_output, get_temp_dir
from build_system import build_system

### concrete implementation for autoconf
class autoconf_bootstrap(build_system):
    def pre_build(self, installed, dep):
        log.log("autoconf : pre-build step for "+dep.name)
        os.chdir("..")
        return execute_cmd_dump_output("./bootstrap --prefix="+get_temp_dir()+"/"+dep.name+".tmp "+dep.pre_build_cmds)

    def build(self, installed, dep):
        log.log("autoconf : build step for "+dep.name)
        return installed["make"].execute("-j ", dep.build_cmds)

    def install(self, installed, dep):
        log.log("autoconf : install step for "+dep.name)
        return installed["make"].execute("install", dep.install_cmds)
