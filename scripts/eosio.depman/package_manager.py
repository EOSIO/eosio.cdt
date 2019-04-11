import os, shutil, imp
from logger import err, warn, log

### interface for package managers 
class package_manager:
    installed = 1
    installed_wrong = -2
    not_installed = 0
    not_satisfiable = -1

    def strip_suff(self, s):
        for i in range(0, len(s)): 
            if not s[i].isdigit(): 
                return s[0:i] 
        return s

    def get_version(self, dep):
        pass
    def prefix(self, dep):
        pass
    def add_repo(self, repo):
        pass
    def check_dependency(self, dep):
        pass
    def install_dependency(self, dep):
        pass
    def remove_dependency(self, dep):
        pass

def import_package_managers():
    old_cwd = os.getcwd()
    cur_pm = ""
    try:
        prefix = os.path.dirname(os.path.realpath(__file__))+"/package_managers"
        try:
            shutil.rmtree(prefix+"/__pycache__")
        except:
            pass
        package_managers = os.listdir(prefix)
        try:
            for pm in package_managers:
                cur_pm = pm
                f, p, d = imp.find_module(pm[:-3], [prefix])
                imp.load_module(pm[:-3], f, p, d)
        except Exception as ex:
            warn.log(str(ex))
            raise ex
            err.log("Failed to import package manager <"+cur_pm+">")
    except Exception as ex:
        warn.log(str(ex))
        raise ex
        err.log("Failed to open package managers directory")
