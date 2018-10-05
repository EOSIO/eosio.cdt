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
	if ! YUM=$( command -v yum 2>/dev/null )
	then
		printf "\\n\\tYum must be installed to compile EOS.IO.\\n"
		printf "\\tExiting now.\\n\\n"
		exit 1;
	fi
	
	printf "\\tYum installation found at %s.\\n" "${YUM}"
	printf "\\n\\tChecking installation of Centos Software Collections Repository.\\n"
	
	SCL=$( command -v scl 2>/dev/null )
	if [ -z "${SCL}" ]; then
		printf "\\n\\tThe Centos Software Collections Repository, devtoolset-7 and Python3 are required to install EOSIO.\\n"
		printf "\\tDo you wish to install and enable this repository, devtoolset-7 and Python3 packages?\\n"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					printf "\\n\\n\\tInstalling SCL.\\n\\n"
					if ! sudo "${YUM}" -y --enablerepo=extras install centos-release-scl 2>/dev/null
					then
						printf "\\n\\tCentos Software Collections Repository installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tCentos Software Collections Repository installed successfully.\\n"
					fi
					printf "\\n\\n\\tInstalling devtoolset-7.\\n\\n"
					if ! sudo "${YUM}" install -y devtoolset-7 2>/dev/null
					then
						printf "\\n\\tCentos devtoolset-7 installation failed.\\n"
						printf "\\n\\tExiting now.\\n\\n"
						exit 1;
					else
						printf "\\n\\tCentos devtoolset installed successfully.\\n"
					fi
					printf "\\n\\n\\tInstalling Python3.\\n\\n"
					if ! sudo "${YUM}" install -y python33.x86_64 2>/dev/null
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

	printf "\\n\\tEnabling Centos devtoolset-7.\\n"
# shellcheck disable=SC1091
	if ! source "/opt/rh/devtoolset-7/enable" 2>/dev/null
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
	
	printf "\\n\\tUpdating YUM repository.\\n\\n"

	if ! sudo "${YUM}" -y update 2>/dev/null
	then
		printf "\\n\\tYUM update failed.\\n"
		printf "\\n\\tExiting now.\\n\\n"
		exit 1;
	fi

	printf "\\n\\tYUM repository successfully updated.\\n\\n"

	DEP_ARRAY=( git autoconf automake bzip2 libtool ocaml.x86_64 doxygen graphviz-devel.x86_64 \
	libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 \
	python-devel.x86_64 gettext-devel.x86_64 gcc-c++.x86_64)
	COUNT=1
	DISPLAY=""
	DEP=""

	printf "\\n\\tChecking YUM for installed dependencies.\\n\\n"

	for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
	do
		pkg=$( sudo "${YUM}" info "${DEP_ARRAY[$i]}" 2>/dev/null | grep Repo | tr -s ' ' | cut -d: -f2 | sed 's/ //g' )
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

	if [ "${ENABLE_COVERAGE_TESTING}" = true ]; then
		printf "\\n\\tChecking perl installation.\\n"
		perl_bin=$( command -v perl 2>/dev/null )
		if [ -z "${perl_bin}" ]; then
			printf "\\n\\tInstalling perl.\\n"
			if ! sudo "${YUM}" -y install perl
			then
				printf "\\n\\tUnable to install perl at this time.\\n"
				printf "\\n\\tExiting now.\\n\\n"
				exit 1;
			fi
		else
			printf "\\tPerl installation found at %s.\\n" "${perl_bin}"
		fi
		printf "\\n\\tChecking LCOV installation.\\n"
		lcov=$( command -v lcov 2>/dev/null )
		if [ -z  "${lcov}" ]; then
			printf "\\n\\tLCOV installation not found.\\n"
			printf "\\tInstalling LCOV.\\n"
			if ! cd "${TEMP_DIR}"
			then
				printf "\\tUnable to enter directory %s.\\n" "${TEMP_DIR}"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! git clone https://github.com/linux-test-project/lcov.git
			then
				printf "\\tUnable to clone LCOV at this time.\\n"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! cd "${TEMP_DIR}/lcov"
			then
				printf "\\tUnable to enter directory %s/lcov.\\n" "${TEMP_DIR}"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! sudo make install
			then
				printf "\\tUnable to install LCOV at this time.\\n"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! cd "${CWD}"
			then
				printf "\\tUnable to enter directory %s.\\n" "${CWD}"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! rm -rf "${TEMP_DIR}/lcov"
			then
				printf "\\tUnable to remove directory %s/lcov.\\n" "${TEMP_DIR}"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			printf "\\n\\tSuccessfully installed LCOV.\\n"
		else
			printf "\\tLCOV installation found @ %s.\\n" "${lcov}"
		fi
	fi

	printf "\\n\\tChecking CMAKE installation.\\n"
    if [ ! -e "${CMAKE}" ]; then
		printf "\\tInstalling CMAKE\\n"
		if [ ! -d "${HOME}/opt" ]; then
			if ! mkdir "${HOME}/opt"
			then
				printf "\\tUnable to create directory %s/opt.\\n" "${HOME}"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
		fi
		if ! cd "${HOME}/opt"
		then
			printf "\\tUnable to enter directory %s/opt.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		STATUS=$(curl -LO -w '%{http_code}' --connect-timeout 30 https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz)
		if [ "${STATUS}" -ne 200 ]; then
			printf "\\tUnable to download CMAKE at this time.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! tar xf "${HOME}/opt/cmake-3.10.2.tar.gz"
		then
			printf "\\tUnable to unarchive %s/opt/cmake-3.10.2.tar.gz.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! rm -f "${HOME}/opt/cmake-3.10.2.tar.gz"
		then
			printf "\\tUnable to remove %s/opt/cmake-3.10.2.tar.gz.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! ln -s "${HOME}/opt/cmake-3.10.2/" "${HOME}/opt/cmake"
		then
			printf "\\tUnable to symlink %s/opt/cmake-3.10.2 to %s/opt/cmake-3.10.2/cmake.\\n" "${HOME}" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! cd "${HOME}/opt/cmake"
		then
			printf "\\tUnable to enter directory %s/opt/cmake.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! ./bootstrap
		then
			printf "\\tError running bootstrap for CMAKE.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! make -j"${JOBS}"
		then
			printf "\\tCompiling CMAKE has exited with the above error.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		printf "\\tCMAKE successfully installed @ %s.\\n\\n" "${CMAKE}"
	else
		printf "\\tCMAKE found @ %s.\\n" "${CMAKE}"
	fi

	function print_instructions()
	{
		printf "\\n\\t%s -f %s &\\n" "$( command -v mongod )" "${MONGOD_CONF}"
		printf "\\tsource /opt/rh/python33/enable\\n"
		printf '\texport PATH=${HOME}/opt/mongodb/bin:$PATH\n'
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
