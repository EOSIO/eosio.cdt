	OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' | cut -d'.' -f1 )

	MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 )
	CPU_SPEED=$( lscpu | grep "MHz" | tr -s ' ' | cut -d\  -f3 | cut -d'.' -f1 )
	CPU_CORE=$( lscpu | grep "^CPU(s)" | tr -s ' ' | cut -d\  -f2 )
	MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
	JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))

	DISK_TOTAL=$( df -h . | grep /dev | tr -s ' ' | cut -d\  -f2 | sed 's/[^0-9]//' )
	DISK_AVAIL=$( df -h . | grep /dev | tr -s ' ' | cut -d\  -f4 | sed 's/[^0-9]//' )

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

	
	DEP_ARRAY=( git gcc72.x86_64 gcc72-c++.x86_64 autoconf automake libtool make bzip2 \
	bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 libstdc++72.x86_64 \
	python27.x86_64 python36-devel.x86_64 libedit-devel.x86_64 doxygen.x86_64 graphviz.x86_64)
	COUNT=1
	DISPLAY=""
	DEP=""

	printf "\\n\\tChecking YUM for installed dependencies.\\n\\n"

	for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
	do
		pkg=$( "$YUM" info "${DEP_ARRAY[$i]}" 2>/dev/null | grep Repo | tr -s ' ' | cut -d: -f2 | sed 's/ //g' )

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
		printf '\texport PATH=${HOME}/opt/mongodb/bin:$PATH \n'
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
