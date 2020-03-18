echo "OS name: ${NAME}"
echo "OS Version: ${VERSION_ID}"
echo "CPU cores: ${CPU_CORES}"
echo "Physical Memory: ${MEM_GIG}G"
echo "Disk space total: ${DISK_TOTAL}G"
echo "Disk space available: ${DISK_AVAIL}G"

( [[ $NAME == "Ubuntu" ]] && ( [[ "$(echo ${VERSION_ID})" == "16.04" ]] || [[ "$(echo ${VERSION_ID})" == "18.04" ]] )  ) || ( echo " - You must be running 16.04.x or 18.04.x to install EOSIO." && exit 1 )

[[ $MEM_GIG -lt 7 ]] && echo "Your system must have 7 or more Gigabytes of physical memory installed." && exit 1
[[ "${DISK_AVAIL}" -lt "${DISK_MIN}" ]] && echo " - You must have at least ${DISK_MIN}GB of available storage to install EOSIO." && exit 1

ensure-build-essential
ensure-apt-packages "${REPO_ROOT}/scripts/eoscc_build_ubuntu_deps" $(echo ${EXTRA_DEPS[@]})
echo ""
# CMAKE Installation
#ensure-cmake
# CLANG Installation
build-clang
# LLVM Installation
ensure-llvm
