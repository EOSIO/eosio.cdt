from package_manager import package_manager

class yum(package_manager):
    def get_version(self, dep):
        eo, ee, ec = execute_cmd("brew info "+dep.package_name)
        if (eo and ec == 0):
            vers_str = eo.split()[2]
            return version(int(self.strip_suff(vers_str.split(".")[0])), int(self.strip_suff(vers_str.split(".")[1])))
        return None

    def check_dependency(self, dep):
        pass
    def install_dependency(self, dep):
        pass
