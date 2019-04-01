OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' )

MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 )
CPU_SPEED=$( lscpu | grep "MHz" | tr -s ' ' | cut -d\  -f3 | cut -d'.' -f1 )
CPU_CORE=$( nproc )
MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
export JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

DISK_INSTALL=$( df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 )
DISK_TOTAL_KB=$( df . | tail -1 | awk '{print $2}' )
DISK_AVAIL_KB=$( df . | tail -1 | awk '{print $4}' )
DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

DEP_ARRAY=( 
	git sudo procps-ng which gcc gcc-c++ autoconf automake libtool make \
	bzip2-devel wget bzip2 compat-openssl10 \
	openssl-devel gmp-devel libstdc++-devel python2 python2-devel python3 python3-devel \
	libedit ncurses-devel swig
)
COUNT=1
DISPLAY=""
DEP=""

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
	exit 1;
fi

if [ "${OS_VER}" -lt 25 ]; then
	printf "You must be running Fedora 25 or higher to install EOSIO.\\n"
	printf "Exiting now.\\n"
	exit 1;
fi

if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
	printf "You must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
	printf "Exiting now.\\n"
	exit 1;
fi

printf "\\nChecking Yum installation...\\n"
if ! YUM=$( command -v yum 2>/dev/null ); then
		printf "!! Yum must be installed to compile EOS.IO !!\\n"
		printf "Exiting now.\\n"
		exit 1;
fi
printf " - Yum installation found at %s.\\n" "${YUM}"

PACKAGE_MANAGER_UPDATE_COMMAND="$YUM -y update"
while true; do
	if [[ -z $1 ]] || [[ $1 == 0 ]]; then read -p "Do you wish to ${PACKAGE_MANAGER_UPDATE_COMMAND}? (y/n) " answer; else answer=$1; fi
	case "${answer}" in
		"" ) echo " - Please type something.";;
		1 | [Yy]* )
			printf "Updating...\\n\\n"
			if ! sudo ${PACKAGE_MANAGER_UPDATE_COMMAND}; then
				printf " - Update failed.\\n"
				exit 1;
			else
				printf " - ${PACKAGE_MANAGER_UPDATE_COMMAND} complete.\\n"
			fi
		break;;
		[Nn]* ) echo " - Skipping."; break;;
		* ) echo " - Please type 'y' for yes or 'n' for no.";;
	esac
done

printf "\\n"

printf "Checking RPM for installed dependencies...\\n"
for (( i=0; i<${#DEP_ARRAY[@]}; i++ )); do
	pkg=$( rpm -qi "${DEP_ARRAY[$i]}" 2>/dev/null | grep Name )
	if [[ -z $pkg ]]; then
		DEP=$DEP" ${DEP_ARRAY[$i]} "
		DISPLAY="${DISPLAY}${COUNT}. ${DEP_ARRAY[$i]}\\n"
		printf "!! Package %s ${bldred} NOT ${txtrst} found !!\\n" "${DEP_ARRAY[$i]}"
		(( COUNT++ ))
	else
		printf " - Package %s found.\\n" "${DEP_ARRAY[$i]}"
		continue
	fi
done
printf "\\n"
PACKAGE_MANAGER_INSTALL_COMMAND="apt-get -y install"
if [ "${COUNT}" -gt 1 ]; then
	printf "The following dependencies are required to install EOSIO:\\n"
	printf "${DISPLAY}\\n\\n"
	while true; do
		if [[ -z $1 ]] || [[ $1 == 0 ]]; then read -p "Do you wish to install required dependencies? (y/n) " answer; else answer=$1; fi
		case "${answer}" in
			"" ) echo " - Please type something.";;
			1 | [Yy]* )
				printf "Installing dependencies...\\n"
				if ! sudo ${PACKAGE_MANAGER_INSTALL_COMMAND} ${DEP}; then
					printf " - ${DEP} dependency failed installation!\\n"
					exit 1
				else
					printf " - Dependencies installed successfully.\\n"
				fi
			break;;
			[Nn]* ) echo " - User aborting installation of required dependencies."; exit;;
			* ) echo " - Please type 'y' for yes or 'n' for no.";;
		esac
	done
else
	printf " - No required dependencies to install.\\n"
fi


printf "\\n"


printf "Checking CMAKE installation...\\n"
if [ -z $CMAKE ]; then
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
