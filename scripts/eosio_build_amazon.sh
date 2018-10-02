	OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' | cut -d'.' -f1 )

	MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 )
	CPU_SPEED=$( lscpu | grep "MHz" | tr -s ' ' | cut -d\  -f3 | cut -d'.' -f1 )
	CPU_CORE=$( lscpu | grep "^CPU(s)" | tr -s ' ' | cut -d\  -f2 )
	MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
	JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

	DISK_TOTAL=$( df -h . | grep /dev | tr -s ' ' | cut -d\  -f2 | sed 's/[^0-9]//' )
	DISK_AVAIL=$( df -h . | grep /dev | tr -s ' ' | cut -d\  -f4 | sed 's/[^0-9]//' )

	printf "\\n\\tOS name: %s\\n" "${OS_NAME}"
	printf "\\tOS Version: %s\\n" "${OS_VER}"
	printf "\\tCPU speed: %sMhz\\n" "${CPU_SPEED}"
	printf "\\tCPU cores: %s\\n" "${CPU_CORE}"
	printf "\\tPhysical Memory: %sMgb\\n" "${MEM_MEG}"
	printf "\\tDisk space total: %sGb\\n" "${DISK_TOTAL}"
	printf "\\tDisk space available: %sG\\n" "${DISK_AVAIL}"

	if [ "${MEM_MEG}" -lt 7000 ]; then
		printf "\\tYour system must have 7 or more Gigabytes of physical memory installed.\\n"
		printf "\\texiting now.\\n"
		exit 1
	fi

	if [ "${OS_VER}" -lt 2017 ]; then
		printf "\\tYou must be running Amazon Linux 2017.09 or higher to install EOSIO.\\n"
		printf "\\texiting now.\\n"
		exit 1
	fi

	if [ "${DISK_AVAIL}" -lt "${DISK_MIN}" ]; then
		printf "\\tYou must have at least %sGB of available storage to install EOSIO.\\n" "${DISK_MIN}"
		printf "\\texiting now.\\n"
		exit 1
	fi

	printf "\\n\\tChecking Yum installation.\\n"
	if ! YUM=$( command -v yum 2>/dev/null )
	then
		printf "\\n\\tYum must be installed to compile EOS.IO.\\n"
		printf "\\n\\tExiting now.\\n"
		exit 1
	fi
	
	printf "\\tYum installation found at %s.\\n" "${YUM}"
	printf "\\tUpdating YUM.\\n"
	if ! UPDATE=$( sudo "$YUM" -y update )
	then
		printf "\\n\\tYUM update failed.\\n"
		printf "\\n\\tExiting now.\\n"
		exit 1
	fi
	printf "\\t%s\\n" "${UPDATE}"

	DEP_ARRAY=( git gcc72.x86_64 gcc72-c++.x86_64 autoconf automake libtool make bzip2 \
	bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 libstdc++72.x86_64 \
	python27.x86_64 python36-devel.x86_64 libedit-devel.x86_64 doxygen.x86_64 graphviz.x86_64)
	COUNT=1
	DISPLAY=""
	DEP=""

	printf "\\n\\tChecking YUM for installed dependencies.\\n\\n"

	for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
	do
		pkg=$( sudo "$YUM" info "${DEP_ARRAY[$i]}" 2>/dev/null | grep Repo | tr -s ' ' | cut -d: -f2 | sed 's/ //g' )

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
					printf "\\n\\n\\tInstalling dependencies.\\n\\n"
					if ! sudo "${YUM}" -y install ${DEP}
					then
						printf "\\n\\tYUM dependency installation failed.\\n"
						printf "\\n\\tExiting now.\\n"
						exit 1
					else
						printf "\\n\\tYUM dependencies installed successfully.\\n"
					fi
				break;;
				[Nn]* ) printf "\\nUser aborting installation of required dependencies,\\n Exiting now.\\n"; exit;;
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
		if [ ! -e "/usr/local/bin/lcov" ]; then
			printf "\\n\\tLCOV installation not found.\\n"
			printf "\\tInstalling LCOV.\\n"
			if ! cd "${TEMP_DIR}"
			then
				printf "\\n\\tUnable to enter %s. Exiting now.\\n" "${TEMP_DIR}"
				exit 1;
			fi
			if ! git clone "https://github.com/linux-test-project/lcov.git"
			then
				printf "\\n\\tUnable to clone LCOV at this time.\\n"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			if ! cd "${TEMP_DIR}/lcov"
			then
				printf "\\n\\tUnable to enter %s/lcov. Exiting now.\\n" "${TEMP_DIR}"
				exit 1;
			fi
			if ! sudo make install
			then
				printf "\\n\\tUnable to install LCOV at this time.\\n"
				printf "\\tExiting now.\\n\\n"
				exit 1;
			fi
			rm -rf "${TEMP_DIR}/lcov"
			printf "\\n\\tSuccessfully installed LCOV.\\n\\n"
		else
			printf "\\n\\tLCOV installation found @ /usr/local/bin.\\n"
		fi
	fi

	printf "\\n\\tChecking CMAKE installation.\\n"
    if [ ! -e "${CMAKE}" ]; then
		printf "\\tInstalling CMAKE.\\n"
		if ! mkdir -p "${HOME}/opt/" 2>/dev/null
		then
			printf "\\n\\tUnable to create directory %s/opt.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! cd "${HOME}/opt"
		then
			printf "\\n\\tUnable to enter directory %s/opt.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		STATUS=$( curl -LO -w '%{http_code}' --connect-timeout 30 "https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz" )
		if [ "${STATUS}" -ne 200 ]; then
			printf "\\tUnable to clone CMAKE repo.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! tar xf "${HOME}/opt/cmake-3.10.2.tar.gz"
		then
			printf "\\tUnable to unarchive file %s/opt/cmake-3.10.2.tar.gz at this time.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! rm -f "${HOME}/opt/cmake-3.10.2.tar.gz"
		then
			printf "\\tUnable to remove file %s/opt/cmake-3.10.2.tar.gz.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! ln -s "${HOME}/opt/cmake-3.10.2/" "${HOME}/opt/cmake"
		then
			printf "\\tUnable to symlink directory %s/opt/cmake-3.10.2/ to %s/opt/cmake at this time.\\n" "${HOME}" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! cd "${HOME}/opt/cmake/"
		then
			printf "\\n\\tUnable to change directory into %s/opt/cmake.\\n" "${HOME}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! ./bootstrap
		then
			printf "\\tRunning bootstrap for CMAKE exited with the above error.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! make -j"${JOBS}"
		then
			printf "\\tError compiling CMAKE.\\n"
			printf "\\tExiting now.\\n\\n"
			exit 1;
		fi
		printf "\\tCMAKE successfully installed @ %s.\\n" "${CMAKE}"
	else
		printf "\\tCMAKE found @ %s.\\n" "${CMAKE}"
	fi

	if [ -d "${HOME}/opt/boost_1_67_0" ]; then
		if ! mv "${HOME}/opt/boost_1_67_0" "$BOOST_ROOT"
		then
			printf "\\n\\tUnable to move directory %s/opt/boost_1_67_0 to %s.\\n" "${HOME}" "${BOOST_ROOT}"
			printf "\\n\\tExiting now.\\n"
			exit 1
		fi
		if [ -d "$BUILD_DIR" ]; then
			if ! rm -rf "$BUILD_DIR"
			then
			printf "\\tUnable to remove directory %s. Please remove this directory and run this script %s again. 0\\n" "$BUILD_DIR" "${BASH_SOURCE[0]}"
			printf "\\tExiting now.\\n\\n"
			exit 1;
			fi
		fi
	fi

	function print_instructions()
	{
		printf "\\n\\t%s -f %s &\\n" "$( command -v mongod )" "${MONGOD_CONF}"
		printf '\texport PATH=${HOME}/opt/mongodb/bin:$PATH \n'
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
