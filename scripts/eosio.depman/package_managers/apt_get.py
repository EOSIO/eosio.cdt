from package_manager import package_manager
from logger import err, warn, log
from util import execute_cmd
from dependency import version
import os

class apt_get(package_manager):
    def strip_pre(self, s):
        if (len(s.split(":")) > 1):
            return s.split(":")[1]
        return s

    def get_version(self, dep):
        eo, ee, ec = execute_cmd("apt-cache policy "+dep.package_name)
        if eo and ec == 0:
            vers_str = self.strip_pre(eo.split()[4])
            return version(int(self.strip_suff(vers_str.split(".")[0])), int(self.strip_suff(vers_str.split(".")[1])))
        return None
    
    def add_repo(self, repo):
        eo, ee, ec = execute_cmd("add-apt-repository ppa:"+repo)
        if eo and ec == 0:
            return True
        return False

    def prefix(self, dep):
        eo, ee, ec = execute_cmd("dpkg -L "+dep.package_name)
        if eo and ec == 0:
            return os.path.join(os.path.commonprefix(eo.split()[1:]), "lib", dep.package_name)
        return None

    def check_dependency(self, dep):
        eo, ee, ec = execute_cmd("apt-cache policy "+dep.package_name)
        vers = self.get_version(dep)
        if eo and ec == 0:
            ### not installed
            if eo.split()[2] == "(none)":
                if vers and vers.satisfies(dep.strict, dep.version):
                    return [self.not_installed, ""]
                else:
                    return [self.not_satisfiable, ""]
            elif vers and vers.satisfies(dep.strict, dep.version):
                return [self.installed, self.prefix( dep )]
            else:
                return self.installed_wrong
        return [self.not_satisfiable, ""]

    def install_dependency(self, dep):
        log.log("Installing "+dep.name+" "+dep.package_name)
        if (os.getuid() != 0):
            err.log("Installing via apt-get requires sudo!")
        eo, ee, ec = execute_cmd( "apt-get --yes install "+dep.package_name )
        return ec == 0

    def remove_dependency(self, dep):
        log.log("Removing "+dep.name+" "+dep.package_name)
        if (os.getuid() != 0):
            err.log("Removing via apt-get requires sudo!")
        eo, ee, ec = execute_cmd( "apt-get --yes remove "+dep.package_name )
        return ec == 0
