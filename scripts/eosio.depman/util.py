import sys, os, subprocess, distro, platform, tempfile, pwd, grp
from logger import err, warn, log, verbose_log

def execute_cmd( cmd ):
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ro, re = proc.communicate()
    return [ro.decode(), re.decode(), proc.returncode]

def execute_cmd_dump_output( cmd ):
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    with proc.stdout:
        for line in iter( proc.stdout.readline, b'' ):
            verbose_log.log(str(line))
    proc.wait()
    return proc.returncode == 0

def strip( s ):
    return s.lstrip().rstrip()

def str_to_class(cn):
    for mod in sys.modules:
        try:
            return getattr(sys.modules[mod], cn)
        except AttributeError:
            continue

def get_home_dir():
    return os.path.expanduser("~")

def is_owner_for_dir(path):
    head, tail = os.path.split(path)
    try:
        if os.stat(head).st_uid == os.geteuid():
            return True
    except FileNotFoundError:
        path = head
        pass
    
    while True:
        head, tail = os.path.split(path)
        if head == path:
            break
        path = head

        if os.path.isdir(head):
            try:
                if os.stat(head).st_uid == os.geteuid():
                    return True
            except FileNotFoundError:
                path = head
                err.log(path)
                continue

    return False

def get_os():
    if platform.system() == "Darwin":
        return ["Darwin", "OSX", platform.mac_ver()[0]]
    return [platform.system(), platform.dist()[0], platform.dist()[1]]

def is_apple():
    return get_os()[0] == "Darwin"

def is_linux():
    return get_os()[0] == "Linux"

def is_windows():
    return get_os()[0] == "Windows"

def get_temp_dir():
    return tempfile.gettempdir()

def get_install_dir():
    if is_apple() or is_linux():
        return "/usr/local"
    else:
        err.log("Windows is currently not supported")

def get_file_dir():
    if is_apple() or is_linux():
        return "/usr/local/etc"
    else:
        err.log("Windows is currently not supported")

### get the uid of the parent even in sudo
def get_original_uid():
    try:
        if 'SUDO_USER' in os.environ:
            return [pwd.getpwnam(os.environ['SUDO_USER']).pw_uid, grp.getgrnam(os.environ['SUDO_USER']).gr_gid]
        else:
            return [pwd.getpwnam(os.environ['USER']).pw_uid, grp.getgrnam(os.environ['USER']).gr_gid]
    except:
            return [pwd.getpwnam(os.environ['USER']).pw_uid, grp.getgrnam("staff").gr_gid]

def get_package_manager_name():
    if is_linux():
        eo, ee, ec = execute_cmd("apt-get --help")
        if ec == 0:
            return "apt_get"
        else:
            eo, ee, ec = execute_cmd("yum --help")
            if ec == 0:
                return "yum"
            else:
                err.log("Unsupported Linux distributiosn")
    elif is_apple():
        return "brew"
    else:
        err.log("Unsupported OS")
