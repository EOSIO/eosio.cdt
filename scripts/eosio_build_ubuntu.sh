	OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' )
	OS_MAJ=$(echo "${OS_VER}" | cut -d'.' -f1)
	OS_MIN=$(echo "${OS_VER}" | cut -d'.' -f2)

	MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 || cut -d' ' -f2 )
	CPU_SPEED=$( lscpu | grep -m1 "MHz" | tr -s ' ' | cut -d\  -f3 || cut -d' ' -f3 | cut -d'.' -f1 )
	CPU_CORE=$( lscpu | grep "^CPU(s)" | tr -s ' ' | cut -d\  -f2 || cut -d' ' -f2 )

	MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
	JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

	DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1)
	DISK_TOTAL_KB=$(df . | tail -1 | awk '{print $2}')
	DISK_AVAIL_KB=$(df . | tail -1 | awk '{print $4}')
	DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
	DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

	CMAKE_VERSION_MAJOR=3
	CMAKE_VERSION_MINOR=10
	CMAKE_VERSION_PATCH=2
	CMAKE_VERSION=${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}
	MONGODB_VERSION=3.6.3
	MONGO_C_DRIVER_VERSION=1.9.3
	MONGO_CXX_DRIVER_VERSION=3.2
	SRC_LOCATION=/usr/local/src
	BOOST_VERSION_MAJOR=1
	BOOST_VERSION_MINOR=66
	BOOST_VERSION_PATCH=0
	BOOST_VERSION=${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_PATCH}
	LLVM_CLANG_VERSION=release_40
	TINI_VERSION=0.18.0

	printf "\\n\\tOS name: %s\\n" "${OS_NAME}"
	printf "\\tOS Version: %s\\n" "${OS_VER}"
	printf "\\tCPU speed: %sMhz\\n" "${CPU_SPEED}"
	printf "\\tCPU cores: %s\\n" "${CPU_CORE}"
	printf "\\tPhysical Memory: %s Mgb\\n" "${MEM_MEG}"
	printf "\\tDisk install: %s\\n" "${DISK_INSTALL}"
	printf "\\tDisk space total: %sG\\n" "${DISK_TOTAL%.*}"
	printf "\\tDisk space available: %sG\\n" "${DISK_AVAIL%.*}"

	if [ "${MEM_MEG}" -lt 7000 ]; then
		printf "\\tYour system must have 7 or more Gigabytes of physical memory installed.\\n"
		printf "\\tExiting now.\\n"
		exit 1
	fi

	case "${OS_NAME}" in
		"Linux Mint")
		   if [ "${OS_MAJ}" -lt 18 ]; then
			   printf "\\tYou must be running Linux Mint 18.x or higher to install EOSIO.\\n"
			   printf "\\tExiting now.\\n"
			   exit 1
		   fi
		;;
		"Ubuntu")
			if [ "${OS_MAJ}" -lt 16 ]; then
				printf "\\tYou must be running Ubuntu 16.04.x or higher to install EOSIO.\\n"
				printf "\\tExiting now.\\n"
				exit 1
			fi
		;;
	esac

	if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
		printf "\\tYou must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
		printf "\\tExiting now.\\n"
		exit 1
	fi

	DEP_ARRAY=( git python3 python3-dev clang-4.0 lldb-4.0 libclang-4.0-dev cmake make libbz2-dev \
				libssl-dev libgmp3-dev autotools-dev build-essential libbz2-dev libicu-dev \
				python-dev autoconf libtool curl mongodb doxygen graphviz libsoci-dev )
	COUNT=1
	DISPLAY=""
	DEP=""
	printf "\\n\\tChecking for installed dependencies.\\n\\n"

	for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
	do
		pkg=$( dpkg -s "${DEP_ARRAY[$i]}" 2>/dev/null | grep Status | tr -s ' ' | cut -d\  -f4 )
		if [ -z "$pkg" ]; then
			DEP=$DEP" ${DEP_ARRAY[$i]} "
			DISPLAY="${DISPLAY}${COUNT}. ${DEP_ARRAY[$i]}\\n\\t"
			printf "\\tPackage %s ${bldred} NOT ${txtrst} found.\\n" "${DEP_ARRAY[$i]}"
			(( COUNT++ ))
		else
			printf "\\tPackage %s found.\\n" "${DEP_ARRAY[$i]}"
			continue
		fi
	done		

	if [ "${COUNT}" -gt 1 ]; then
		printf "\\n\\tThe following dependencies are required to install EOSIO.\\n"
		printf "\\n\\t${DISPLAY}\\n\\n"
		printf "\\tDo you wish to install these packages?\\n"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					printf "\\n\\n\\tInstalling dependencies\\n\\n"
					sudo apt-get update
					if ! sudo apt-get -y install ${DEP}
					then
						printf "\\n\\tDPKG dependency failed.\\n"
						printf "\\n\\tExiting now.\\n"
						exit 1
					else
						printf "\\n\\tDPKG dependencies installed successfully.\\n"
					fi
				break;;
				[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
				* ) echo "Please type 1 for yes or 2 for no.";;
			esac
		done
	else 
		printf "\\n\\tNo required dpkg dependencies to install.\\n"
	fi

	function print_instructions()
	{
		printf '\n\texport PATH=${HOME}/opt/mongodb/bin:$PATH\n'
		printf "\\t%s -f %s &\\n" "$( command -v mongod )" "${MONGOD_CONF}"
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
