OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' )
OS_MAJ=$(echo "${OS_VER}" | cut -d'.' -f1)
OS_MIN=$(echo "${OS_VER}" | cut -d'.' -f2)

MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 || cut -d' ' -f2 )
CPU_SPEED=$( lscpu | grep -m1 "MHz" | tr -s ' ' | cut -d\  -f3 || cut -d' ' -f3 | cut -d'.' -f1 )
CPU_CORE=$( nproc )
MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
export JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1)
DISK_TOTAL_KB=$(df . | tail -1 | awk '{print $2}')
DISK_AVAIL_KB=$(df . | tail -1 | awk '{print $4}')
DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

printf "\\nOS name: ${OS_NAME}\\n"
printf "OS Version: ${OS_VER}\\n"
printf "CPU speed: ${CPU_SPEED}Mhz\\n"
printf "CPU cores: %s\\n" "${CPU_CORE}"
printf "Physical Memory: ${MEM_MEG} Mgb\\n"
printf "Disk install: ${DISK_INSTALL}\\n"
printf "Disk space total: ${DISK_TOTAL%.*}G\\n"
printf "Disk space available: ${DISK_AVAIL%.*}G\\n"

if [ "${MEM_MEG}" -lt 7000 ]; then
	printf "Your system must have 7 or more Gigabytes of physical memory installed.\\n"
	printf "Exiting now.\\n"
	exit 1
fi

case "${OS_NAME}" in
	"Linux Mint")
		if [ "${OS_MAJ}" -lt 18 ]; then
			printf "You must be running Linux Mint 18.x or higher to install EOSIO.\\n"
			printf "Exiting now.\\n"
			exit 1
		fi
	;;
	"Ubuntu")
		if [ "${OS_MAJ}" -lt 16 ]; then
			printf "You must be running Ubuntu 16.04.x or higher to install EOSIO.\\n"
			printf "Exiting now.\\n"
			exit 1
		fi
		# UBUNTU 18 doesn't have MONGODB 3.6.3
		if [ $OS_MAJ -gt 16 ]; then
			export MONGODB_VERSION=4.1.1
		fi
		# We have to re-set this with the new version
		export MONGODB_ROOT=${OPT_LOCATION}/mongodb-${MONGODB_VERSION}
	;;
	"Debian")
		if [ $OS_MAJ -lt 10 ]; then
			printf "You must be running Debian 10 to install EOSIO, and resolve missing dependencies from unstable (sid).\n"
			printf "Exiting now.\n"
			exit 1
	fi
	;;
esac

if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
	printf "You must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
	printf "Exiting now.\\n"
	exit 1
fi

# llvm-4.0 is installed into /usr/lib/llvm-4.0
DEP_ARRAY=(
	git llvm-4.0 clang-4.0 libclang-4.0-dev make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7 python2.7-dev python3 python3-dev \
	autoconf libtool curl zlib1g-dev sudo ruby
)
COUNT=1
DISPLAY=""
DEP=""

if [[ "${ENABLE_CODE_COVERAGE}" == true ]]; then
	DEP_ARRAY+=(lcov)
fi

printf "Do you wish to update repositories with apt-get update?\\n\\n"
select yn in "Yes" "No"; do
	case $yn in
		[Yy]* ) 
			printf "\\n\\nUpdating...\\n\\n"
			if ! apt-get update; then
				printf "\\nAPT update failed.\\n"
				printf "\\nExiting now.\\n\\n"
				exit 1;
			else
				printf "\\nAPT update complete.\\n"
			fi
		break;;
		[Nn]* ) echo "Proceeding without update!";;
		* ) echo "Please type 1 for yes or 2 for no.";;
	esac
done

printf "\\nChecking for installed dependencies.\\n\\n"

for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
do
	pkg=$( dpkg -s "${DEP_ARRAY[$i]}" 2>/dev/null | grep Status | tr -s ' ' | cut -d\  -f4 )
	if [ -z "$pkg" ]; then
		DEP=$DEP" ${DEP_ARRAY[$i]} "
		DISPLAY="${DISPLAY}${COUNT}. ${DEP_ARRAY[$i]}\\n"
		printf "Package %s ${bldred} NOT ${txtrst} found.\\n" "${DEP_ARRAY[$i]}"
		(( COUNT++ ))
	else
		printf "Package %s found.\\n" "${DEP_ARRAY[$i]}"
		continue
	fi
done		

if [ "${COUNT}" -gt 1 ]; then
	printf "\\nThe following dependencies are required to install EOSIO.\\n"
	printf "\\n${DISPLAY}\\n\\n" 
	printf "Do you wish to install these packages?\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* ) 
				printf "\\n\\nInstalling dependencies\\n\\n"
				if ! apt-get -y install ${DEP}
				then
					printf "\\nDPKG dependency failed.\\n"
					printf "\\nExiting now.\\n"
					exit 1
				else
					printf "\\nDPKG dependencies installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done
else 
	printf "\\nNo required dpkg dependencies to install."
fi


printf "\\n"

CMAKE=$(command -v cmake 2>/dev/null)
printf "Checking CMAKE installation...\\n"
if [ ! -d $SRC_LOCATION/cmake-$CMAKE_VERSION ]; then
	printf "Installing CMAKE...\\n"
	curl -LO https://cmake.org/files/v$CMAKE_VERSION_MAJOR.$CMAKE_VERSION_MINOR/cmake-$CMAKE_VERSION.tar.gz \
	&& tar xf cmake-$CMAKE_VERSION.tar.gz \
	&& cd cmake-$CMAKE_VERSION \
	&& ./bootstrap --prefix=$HOME \
	&& make -j"${JOBS}" \
	&& make install \
	&& cd .. \
	&& rm -f cmake-$CMAKE_VERSION.tar.gz \
	|| exit 1
	printf " - CMAKE successfully installed @ ${CMAKE}.\\n"
else
	printf " - CMAKE found @ ${CMAKE}.\\n"
fi