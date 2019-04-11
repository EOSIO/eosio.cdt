#! /usr/bin/env python3

import sys, os, subprocess, platform, distro, pickle, atexit
import urllib.request, shutil, argparse, json, re
from logger import err, warn, log, verbose_log, set_log_colorize
from util import strip, str_to_class, get_os, get_temp_dir, get_install_dir, get_file_dir, get_home_dir, get_package_manager_name, is_owner_for_dir, get_original_uid
from dependency import dependency, installed_dependency, version
from package_manager import package_manager, import_package_managers
from build_system import build_system, import_build_systems
from source_builder import source_builder
import parser

class dependency_handler:
    comment_re = re.compile("#")
    tag_re     = re.compile("\[.*\]")
    dependencies      = list()
    tagged_deps       = dict()
    deps_dict         = dict()
    temp_dir          = get_temp_dir()
    prefix            = ""
    installed_deps    = dict()
    check_only        = False
    source_only       = False
    deps_to_install   = ""
    repos             = list()
    local_deps        = dict()
    system_deps       = dict()

    def download_dependency_and_unpack( self, dep, use_bin ):
        log.log("Downloading "+dep.name)
        url = ""
        if use_bin:
            url = dep.bin_url
        else:
            url = dep.source_url
        if not url:
            err.log("Can't do a source build for dependency ("+dep.name+")")
        base_name = os.path.basename(url)
        urllib.request.urlretrieve( url, os.path.join(self.temp_dir, base_name) )
        shutil.unpack_archive( os.path.join(self.temp_dir, base_name), os.path.join(self.temp_dir, dep.name) )

        log.log("Unpacked "+base_name)
        os.remove( os.path.join(self.temp_dir, base_name) )
    
    def set_source_only( self, is_source_only ):
        self.source_only = is_source_only

    def check_dependency( self, dep ):
        packman = str_to_class(get_package_manager_name())()
        res, path = packman.check_dependency( dep )
        if res == package_manager.installed:
            log.log("Dependency ("+dep.name+") found!")
            self.installed_deps[dep.name] = installed_dependency(dep, False, path, None)
            return True
        elif res == package_manager.not_installed:
            warn.log( "Dependency ("+dep.name+" : "+dep.version.to_string()+") not found!" )
            if (self.source_only or not packman.install_dependency( dep ) and not self.check_only):
                    ### fallback
                    self.download_dependency_and_unpack( dep, len(dep.bin_url) > 0 )
                    sb = source_builder()
                    installed = None
                    if dep.bin_url:
                        installed = sb.install_binary( self.installed_deps, dep, self.prefix )
                    else:
                        sb.build( self.installed_deps, dep )
                        installed = sb.install( self.installed_deps, dep, self.prefix )
                    self.installed_deps[dep.name] = installed
            else:
                self.installed_deps[dep.name] = installed_dependency(dep, True, packman.prefix(dep), "") 
            return True
        elif res == package_manager.not_satisfiable:
            if (not self.check_only):
                warn.log( "Dependency ("+dep.name+" : "+dep.version.to_string()+") not satisfiable, doing a source install!" )
                self.download_dependency_and_unpack( dep, len(dep.bin_url) > 0 )
                sb = source_builder()
                installed = None
                if dep.bin_url:
                    installed = sb.install_binary( self.installed_deps, dep, self.prefix )
                else:
                    sb.build( self.installed_deps, dep )
                    installed = sb.install( self.installed_deps, dep, self.prefix )
                self.installed_deps[dep.name] = installed
                return True
            else:
                warn.log( "Dependency ("+dep.name+" : "+dep.version.to_string()+") not satisfiable!" )
            return False
        else:
            err.log("Dependency ("+dep.name+") installed but version is too low ("+packman.get_version(dep).to_string()+")")
            return False

    def check_dependencies_helper( self, dep ):
        dep_check = True 
        log.log("Checking dep "+dep.name)
        if dep.name in self.installed_deps and self.installed_deps[dep.name].provided == True:
            if self.installed_deps[dep.name].dep.version.eq(self.deps_dict[dep.name].version):
                log.log("Dependency ("+dep.name+") found!")
                return dep_check
            else:
                err.log("Dependency ("+dep.name+") installed but dependency needs a different version!")

        if dep.is_executable():
            installed_dep = dep.find_executable()
            if installed_dep:
                if installed_dep.dep.version.eq(dep.version):
                    log.log("Dependency ("+dep.name+") found!")
                    self.installed_deps[dep.name] = dep.find_executable()
                else:
                    res = self.check_dependency( dep )
                    dep_check = dep_check and res
            else:
                res = self.check_dependency( dep )
                dep_check = dep_check and res
        else:
            res = self.check_dependency( dep )
            dep_check = dep_check and res
        return dep_check

    def check_dependencies( self, group ):
        dep_check = True
        for g, dl in self.tagged_deps.items():
            if group == "default" or group == g:
                for d in dl:
                    if not g.startswith("optional:"):
                        dep_check = dep_check and self.check_dependencies_helper( self.deps_dict[d] )
            elif group == "all":
                for d in dl:
                    dep_check = dep_check and self.check_dependencies_helper( self.deps_dict[d] )
        return dep_check

    def remove_dependency(self, dep_name, strict):
        packman = str_to_class(get_package_manager_name())()
        if not dep_name in self.installed_deps:
            err.log("Dependency ("+dep_name+") not installed")
        dep = self.installed_deps[dep_name].dep
        if self.installed_deps[dep_name].files:
            log.log("Removing "+dep.name+" "+dep.package_name)
            for f in self.installed_deps[dep_name].files:
                try:
                    os.remove(f)
                    os.rmdir(os.path.dirname(f))
                except:
                    pass
        elif self.installed_deps[dep_name].provided:
            packman.remove_dependency(dep)
        else:
            if strict:
                err.log("Dependency ("+dep_name+") was not installed via eosio.depman")
        self.installed_deps.pop(dep.name, None)

    def get_prefix(self, dep_name):
        if dep_name in self.installed_deps:
            return self.installed_deps[dep_name].path
        else:
            err.log("Dependency ("+dep_name+") not available")

    def check_if_uid_satisfied(self, pfx):
        self.prefix = os.path.abspath(os.path.realpath(os.path.expanduser(pfx)))
        if not is_owner_for_dir(self.prefix):
            err.log("Prefix for installation <"+self.prefix+"> needs root access, use sudo")
    
    def add_repos(self):
        packman = str_to_class(get_package_manager_name())()
        for repo in self.repos:
            if not packman.add_repo(repo):
                err.log("Repo ("+repo+") not available")

    def write_installed_deps_file( self ):
        if not len(self.installed_deps) > 0:
           return
        deps_file = open(os.path.join(get_home_dir(), ".eosio.install.deps"), "wb")
        pickle.dump( self.installed_deps, deps_file )
        deps_file.close()
        os.chown(os.path.join(get_home_dir(), ".eosio.install.deps"), get_original_uid()[0], get_original_uid()[1])

        if is_owner_for_dir(get_file_dir()):
            deps_file = open(os.path.join(get_file_dir(), ".eosio.install.deps"), "wb")
            pickle.dump( self.installed_deps, deps_file )
            deps_file.close()
   
    def read_installed_deps_file( self ):
        try:
            deps_file = open(os.path.join(get_home_dir(), ".eosio.install.deps"), "rb")
            self.local_deps = pickle.load(deps_file)
            deps_file.close()
            self.installed_deps = self.local_deps.copy()

            if is_owner_for_dir(get_file_dir()):
                deps_file = open(os.path.join(get_file_dir(), ".eosio.install.deps"), "rb")
                self.system_deps = pickle.load(deps_file)
                deps_file.close()
                self.installed_deps.update(self.system_deps)

        except Exception as Ex:
            pass

    def read_dependency_file(self, fname):
        dep_file_parser = parser.parser()
        self.deps_dict, self.tagged_deps, self.repos = dep_file_parser.parse( fname )

        for k, v in self.deps_dict.items():
            self.dependencies.append(v)

    def __init__(self, pfx, install):
        import_package_managers()
        self.deps_to_install = install
        self.prefix = os.path.expanduser(pfx)

if __name__ == "__main__":
    arg_parser = argparse.ArgumentParser(description="Manager for dependencies")
    arg_parser.add_argument('--verbose', dest='verbose', action="store_true", default=False)
    arg_parser.add_argument('--no-color', dest='no_color', action="store_false", default=True)
    arg_parser.add_argument('--prefix', type=str, dest='prefix', default="/usr/local")
    arg_parser.add_argument('--query', type=str, dest='query', default="")
    arg_parser.add_argument('--path', type=str, dest='path', default="")
    arg_parser.add_argument('--check', dest='check', action='store_true', default=False)
    arg_parser.add_argument('--install-group', type=str, dest='install_group', default="default")
    arg_parser.add_argument('--install', type=str, dest='install', default="")
    arg_parser.add_argument('--remove', type=str, dest='remove', default="")
    arg_parser.add_argument('--remove-all', dest='remove_all', action="store_true", default=False)
    arg_parser.add_argument('--install-dir', type=str, dest='install_dir', default="")
    arg_parser.add_argument('--source-only', dest='source_only', action='store_true', default=False)
    arg_parser.add_argument('--list', dest='list', action='store_true', default=False)
    arg_parser.add_argument('file', type=str)

    args = arg_parser.parse_args()
    try:
        handler = dependency_handler( args.prefix, args.install )
        deps_filename = "eosio.deps"
        atexit.register( lambda : handler.write_installed_deps_file() )
        if args.verbose:
            verbose_log.silence = False
        set_log_colorize(args.no_color)
        if not args.file:
           err.log("Must specify dependency file")
        deps_filename = args.file
        
        handler.read_dependency_file( deps_filename )
        handler.read_installed_deps_file()

        if args.check:
            handler.check_only = True
            if not handler.check_dependencies(args.install_group):
                exit(-1)
            exit(0)
        if args.query:
            log.log(args.query+" : version ("+handler.deps_dict[args.query].version.to_string()+") -- installed at ("+handler.get_prefix(strip(args.query))+")")
            exit(0)
        if args.install_dir:
            log.log(handler.get_prefix(strip(args.install_dir)))
            exit(0)
        if args.list:
            for k in handler.installed_deps.copy():
                log.log(k)
            exit(0)
        if args.remove:
            handler.remove_dependency(args.remove, True)
            exit(0)
        if args.remove_all:
            for k in handler.installed_deps.copy():
                handler.remove_dependency(k, False)
            exit(0)
        if args.source_only:
            handler.set_source_only(True)

        handler.add_repos()

        if args.install:
            handler.check_dependencies_helper(handler.deps_dict[args.install])
        else:
            handler.check_if_uid_satisfied( args.prefix )
            handler.check_dependencies(args.install_group)
    except Exception as ex:
        try:
           handler.write_installed_deps_file()
        except:
            pass
        warn.log(str(ex))
        err.log("Critical failure")
