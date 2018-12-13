	OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' \
	| cut -d'.' -f1 )

	MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 )
	CPU_SPEED=$( lscpu | grep "MHz" | tr -s ' ' | cut -d\  -f3 | cut -d'.' -f1 )
	CPU_CORE=$( lscpu | grep "^CPU(s)" | tr -s ' ' | cut -d\  -f2 )
	MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
	JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

	DISK_INSTALL=$( df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 )
	DISK_TOTAL_KB=$( df . | tail -1 | awk '{print $2}' )
	DISK_AVAIL_KB=$( df . | tail -1 | awk '{print $4}' )
	DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
	DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

	printf "\\n\\tOS name: %s\\n" "${OS_NAME}"
	printf "\\tOS Version: %s\\n" "${OS_VER}"
	printf "\\tCPU speed: %sMhz\\n" "${CPU_SPEED}"
	printf "\\tCPU cores: %s\\n" "${CPU_CORE}"
	printf "\\tPhysical Memory: %s Mgb\\n" "${MEM_MEG}"
	printf "\\tDisk install: %s\\n" "${DISK_INSTALL}"
	printf "\\tDisk space total: %sG\\n" "${DISK_TOTAL%.*}"
	printf "\\tDisk space available: %sG\\n" "${DISK_AVAIL%.*}"

	if [ "${MEM_MEG}" -lt 7000 ]; then
		printf "\\n\\tYour system must have 7 or more Gigabytes of physical memory installed.\\n"
		printf "\\tExiting now.\\n\\n"
		exit 1;
	fi

	if [ "${OS_VER}" -lt 7 ]; then
		printf "\\n\\tYou must be running Centos 7 or higher to install EOSIO.\\n"
		printf "\\tExiting now.\\n\\n"
		exit 1;
	fi

	if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
		printf "\\n\\tYou must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
		printf "\\tExiting now.\\n\\n"
		exit 1;
	fi

	printf "\\n\\tChecking Yum installation\\n"
	if ! YUM=$( command -v yum 2>/dev/null ); then
		printf "\\n\\tYum must be installed to compile EOS.IO.\\n"
		printf "\\tExiting now.\\n\\n"
		exit 1;
	fi 
	printf "\\tYum installation found at %s.\\n" "${YUM}"


	printf "\\n\\tChecking installation of Centos Software Collections Repository...\\n"
	if [ -z "$(command -v scl 2>/dev/null)" ]; then
		printf "\\n\\tThe Centos Software Collections Repository, devtoolset-7 and Python3 are required to install EOSIO.\\n"
		printf "\\tDo you wish to install and enable this repository, devtoolset-7 and Python3 packages?\\n"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					printf "\\n\\n\\tInstalling SCL.\\n\\n"
					if ! sudo "${YUM}" -y --enablerepo=extras install centos-release-scl
					then
						printf "\\n\\tCentos Software Collections Repository installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tCentos Software Collections Repository installed successfully.\\n"
					fi
					printf "\\n\\n\\tInstalling devtoolset-7.\\n\\n"
					if ! sudo "${YUM}" install -y devtoolset-7
					then
						printf "\\n\\tCentos devtoolset-7 installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tCentos devtoolset installed successfully.\\n"
					fi
					printf "\\n\\n\\tInstalling Python3.\\n\\n"
					if ! sudo "${YUM}" install -y python33.x86_64
					then
						printf "\\n\\tCentos Python3 installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tCentos Python3 installed successfully.\\n"
					fi
				break;;
				[Nn]* ) echo "User aborting installation of required Centos Software Collections Repository, Exiting now."; exit;;
				* ) echo "Please type 1 for yes or 2 for no.";;
			esac
		done
	else 
		printf "\\tCentos Software Collections Repository found.\\n\\n"
	fi
	printf "\\n\\tEnabling Centos devtoolset-7...\\n"
	# shellcheck disable=SC1091
	if ! source "/opt/rh/devtoolset-7/enable"
	then
		printf "\\n\\tUnable to enable Centos devtoolset-7 at this time.\\n"
		printf "\\n\\tExiting now.\\n\\n"
		exit 1;
	fi 
	printf "\\tCentos devtoolset-7 successfully enabled.\\n\\n"


	# 	printf "\\n\\tEnabling Centos python3 installation.\\n"
	# shellcheck disable=SC1091
	# 	if ! source /opt/rh/python33/enable
	# 	then
	# 		printf "\\n\\tUnable to enable Centos python3 at this time.\\n"
	# 		printf "\\n\\tExiting now.\\n\\n"
	# 		exit 1;
	# 	fi
	# 	printf "\\tCentos python3 successfully enabled.\\n"
	

	printf "\\n\\tDo you wish to update YUM repositories?\\n\\n"
	select yn in "Yes" "No"; do
		case $yn in
			[Yy]* ) 
				printf "\\n\\n\\tUpdating...\\n\\n"
				if ! sudo "${YUM}" -y update; then
					printf "\\n\\tYUM update failed.\\n"
					printf "\\n\\tExiting now.\\n\\n"
					exit 1;
				else
					printf "\\n\\tYUM update complete.\\n"
				fi
			break;;
			[Nn]* ) echo "Proceeding without update!";;
			* ) echo "Please type 1 for yes or 2 for no.";;
		esac
	done

	printf "\\n\\tChecking YUM for installed dependencies...\\n\\n"
	DEP_ARRAY=( git autoconf automake libtool make bzip2 \
                bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 \
                ocaml.x86_64 doxygen libicu-devel.x86_64 python-devel.x86_64 \
                gettext-devel.x86_64 file gcc-c++)
	COUNT=1
	DISPLAY=""
	DEP=""
	for (( i=0; i<${#DEP_ARRAY[@]}; i++ )); do
		pkg=$( "${YUM}" info "${DEP_ARRAY[$i]}" 2>/dev/null | grep Repo | tr -s ' ' | cut -d: -f2 | sed 's/ //g' )
		if [ "$pkg" != "installed" ]; then
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
		printf "\\tDo you wish to install these dependencies?\\n"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					printf "\\n\\n\\tInstalling dependencies\\n\\n"
					if ! sudo "${YUM}" -y install ${DEP}
					then
						printf "\\n\\tYUM dependency installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tYUM dependencies installed successfully.\\n"
					fi
				break;;
				[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
				* ) echo "Please type 1 for yes or 2 for no.";;
			esac
		done
	else 
		printf "\\n\\tNo required YUM dependencies to install.\\n"
	fi


	printf "\\n\\tChecking CMAKE installation...\\n"
    if [ -z "$(command -v cmake 2>/dev/null)" ]; then
		printf "\\tInstalling CMAKE...\\n"
		curl -LO https://cmake.org/files/v${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}/cmake-${CMAKE_VERSION}.tar.gz \
    	&& tar xf cmake-${CMAKE_VERSION}.tar.gz \
    	&& cd cmake-${CMAKE_VERSION} \
    	&& ./bootstrap \
    	&& make -j$( nproc ) \
    	&& make install \
    	&& cd .. \
    	&& rm -f cmake-${CMAKE_VERSION}.tar.gz
		printf "\\tCMAKE successfully installed @ %s.\\n\\n" "${CMAKE}"
	else
		printf "\\tCMAKE found @ $(command -v cmake 2>/dev/null).\\n" "${CMAKE}"
	fi

	function print_instructions()
	{
		printf "\\n\\t%s -f %s &\\n" "$( command -v mongod )" "${MONGOD_CONF}"
		printf "\\tsource /opt/rh/python33/enable\\n"
		printf '\texport PATH=${HOME}/opt/mongodb/bin:$PATH\n'
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
