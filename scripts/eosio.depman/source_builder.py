import os, shutil, json, pickle, atexit
from pathlib import Path
from logger import err, warn, log
from util import str_to_class, get_temp_dir, get_home_dir, get_original_uid
from dependency import dependency, installed_dependency, version
from build_system import build_system, noop_build_system, import_build_systems

def cleanup_tmp(d):
    try:
        print("Cleanup "+d)
        shutil.rmtree(os.path.join(d))
        shutil.rmtree(d+".tmp")
    except:
        pass

class source_builder:
    def __init__(self):
        import_build_systems()
    
    def check_install(self, prefix, installed_files):
        for f in installed_files:
            if os.path.isfile(f) and \
               not ( os.path.commonprefix([os.path.dirname(f), os.path.join(prefix, "share")]) == os.path.join(prefix, "share") or \
               os.path.commonprefix([os.path.dirname(f), os.path.join(prefix, "doc")]) == os.path.join(prefix, "doc") ):
                return True
        return False
        
    def build(self, installed, dep):
        old_cwd = os.getcwd()
        tmpd = os.path.join(get_temp_dir(), dep.name)
        atexit.register( lambda : cleanup_tmp(tmpd) )
        full_dir = os.path.join(tmpd, os.listdir(tmpd)[0])
        os.chdir( full_dir )
        try:
            os.mkdir( os.path.join(full_dir, "build") )
        except:
            pass
        os.chdir( "./build" )
        builder = None
        if len(dep.bin_url) > 0:
            builder = noop_build_system()
        else:
            builder = str_to_class(dep.build_sys)()
        
        if not builder.pre_build(installed, dep):
            err.log("Pre-build stage for "+dep.name+" failed!")
        if not builder.build(installed, dep):
            err.log("Build stage for "+dep.name+" failed!")

        os.chdir( old_cwd )

    def install_binary(self, installed, dep, prefix):
        old_cwd = os.getcwd()
        tmpd = os.path.join(get_temp_dir(), dep.name)
        atexit.register( lambda : cleanup_tmp(tmpd) )
        filenames = list()
        old_filenames = list()
        full_dir = os.path.join(tmpd, os.listdir(tmpd)[0])

        for path, subdirs, files in os.walk( tmpd ):
            for fn in files:
                full_path = os.path.join(path, fn)
                rel_path  = os.path.relpath(full_path, full_dir)
                fixed_path = os.path.join(prefix, rel_path)
                filenames.append(fixed_path)
                old_filenames.append(full_path)
        
        ### for now ignore errors when making the prefix folder
        try:
            os.makedirs( prefix )
        except:
            pass

        ### check whether currently installed
        if self.check_install(prefix, filenames):
            err.log("Dependency "+dep.name+" is already installed at location "+prefix)

        for i in range(0, len(filenames)):
            try:
                os.makedirs( os.path.dirname( filenames[i] ) )
            except:
                pass
            ### install to the prefix
            shutil.move( old_filenames[i], filenames[i] )
        ### chown back to user if prefix is home rooted
        if os.path.commonprefix([get_home_dir(), filenames[i]]):
            os.chown( prefix, get_original_uid()[0], get_original_uid()[1] )
            for root, dirs, files in os.walk(prefix):
                for d in dirs:
                    os.chown(os.path.join(root, d), get_original_uid()[0], get_original_uid()[1])
                for f in files:
                    os.chown(os.path.join(root, f), get_original_uid()[0], get_original_uid()[1])
        return installed_dependency( dep, True, prefix, filenames )    

    def install(self, installed, dep, prefix):
        old_cwd = os.getcwd()
        tmpd = os.path.join(get_temp_dir(), dep.name)
        atexit.register( lambda : cleanup_tmp(tmpd) )
        full_dir = os.path.join(tmpd, os.listdir(tmpd)[0],"build")
        os.chdir( full_dir )
        builder = str_to_class(dep.build_sys)()
        if not builder.install(installed, dep):
            err.log("Install stage for "+dep.name+" failed!")
        os.chdir( old_cwd )
        filenames = list()
        old_filenames = list()
        for path, subdirs, files in os.walk(tmpd+".tmp"):
            for fn in files:
                full_path = os.path.join(path, fn)
                rel_path  = os.path.relpath(full_path, tmpd+".tmp")
                fixed_path = os.path.join(prefix, rel_path)
                filenames.append(fixed_path)
                old_filenames.append(full_path)
        
        ### for now ignore errors when making the prefix folder
        try:
            os.makedirs( prefix )
        except:
            pass
        ### check whether currently installed
        if self.check_install(prefix, filenames):
            err.log("Dependency "+dep.name+" is already installed at location "+prefix)

        for i in range(0, len(filenames)):
            try:
                os.makedirs( os.path.dirname( filenames[i] ) )
            except:
                pass
            ### install to the prefix
            shutil.move( old_filenames[i], filenames[i] )
        ### chown back to user if prefix is home rooted
        if os.path.commonprefix([get_home_dir(), filenames[i]]):
            os.chown( prefix, get_original_uid()[0], get_original_uid()[1] )
            for root, dirs, files in os.walk(prefix):
                for d in dirs:
                    os.chown(os.path.join(root, d), get_original_uid()[0], get_original_uid()[1])
                for f in files:
                    os.chown(os.path.join(root, f), get_original_uid()[0], get_original_uid()[1])
        return installed_dependency( dep, True, prefix, filenames ) 
