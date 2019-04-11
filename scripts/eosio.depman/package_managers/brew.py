import re
from package_manager import package_manager
from util import execute_cmd
from dependency import version
from logger import err, warn, log

class brew(package_manager):
    def get_version(self, dep):
        eo, ee, ec = execute_cmd("brew info "+dep.package_name)
        if eo and ec == 0:
            vers_str = eo.split()[2]
            ver = version(int(self.strip_suff(vers_str.split(".")[0])), int(self.strip_suff(vers_str.split(".")[1])))
            return ver
        return None
    
    def prefix(self, dep):
        cellar_re = re.compile("/usr/local/Cellar/.*")
        eo, ee, ec = execute_cmd("brew info "+dep.package_name)
        if eo and ec == 0:
            for tok in eo.split():
                if cellar_re.match(tok):
                    return tok
        return None

    def check_dependency(self, dep):
        vers = self.get_version(dep)
        eo, ee, ec = execute_cmd("brew list --versions "+dep.package_name)
        ### installed
        if vers and eo and ec == 0:
            if vers.satisfies(dep.strict, dep.version):
                return [self.installed, self.prefix( dep )]
            else:
                return [self.installed_wrong, ""]
        elif vers:
            if vers.satisfies(dep.strict, dep.version):
                return [self.not_installed, ""]
        return [self.not_satisfiable, ""]

    def install_dependency(self, dep):
        log.log("Installing "+dep.name+" "+dep.package_name)
        eo, ee, ec = execute_cmd("brew install "+dep.package_name)
        return eo and ec == 0

    def remove_dependency(self, dep):
        log.log("Removing "+dep.name+" "+dep.package_name)
        eo, ee, ec = execute_cmd("brew remove "+dep.package_name)
        return eo and ec == 0
