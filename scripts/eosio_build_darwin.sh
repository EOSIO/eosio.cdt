	OS_VER=$(sw_vers -productVersion)
	OS_MAJ=$(echo "${OS_VER}" | cut -d'.' -f1)
	OS_MIN=$(echo "${OS_VER}" | cut -d'.' -f2)
	OS_PATCH=$(echo "${OS_VER}" | cut -d'.' -f3)

	MEM_GIG=$(bc <<< "($(sysctl -in hw.memsize) / 1024000000)")

	CPU_SPEED=$(bc <<< "scale=2; ($(sysctl -in hw.cpufrequency) / 10^8) / 10")
	CPU_CORE=$( sysctl -in machdep.cpu.core_count )

	DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1)
	blksize=$(df . | head -1 | awk '{print $2}' | cut -d- -f1)
	gbfactor=$(( 1073741824 / blksize ))
	total_blks=$(df . | tail -1 | awk '{print $2}')
	avail_blks=$(df . | tail -1 | awk '{print $4}')
	DISK_TOTAL=$((total_blks / gbfactor ))
	DISK_AVAIL=$((avail_blks / gbfactor ))


	printf "\\nOS name: ${OS_NAME}\\n"
	printf "OS Version: ${OS_VER}\\n"
	printf "CPU speed: ${CPU_SPEED}Mhz\\n"
	printf "CPU cores: %s\\n" "${CPU_CORE}"
	printf "Physical Memory: ${MEM_GIG} Gbytes\\n"
	printf "Disk install: ${DISK_INSTALL}\\n"
	printf "Disk space total: ${DISK_TOTAL}G\\n"
	printf "Disk space available: ${DISK_AVAIL}G\\n"

	if [ "${MEM_GIG}" -lt 7 ]; then
		echo "Your system must have 7 or more Gigabytes of physical memory installed."
		echo "Exiting now."
		exit 1
	fi

	if [ "${OS_MIN}" -lt 12 ]; then
		echo "You must be running Mac OS 10.12.x or higher to install EOSIO."
		echo "Exiting now."
		exit 1
	fi

	if [ "${DISK_AVAIL}" -lt "$DISK_MIN" ]; then
		echo "You must have at least ${DISK_MIN}GB of available storage to install EOSIO."
		echo "Exiting now."
		exit 1
	fi

	printf "Checking xcode-select installation\\n"
	if ! XCODESELECT=$( command -v xcode-select)
	then
		printf "\\nXCode must be installed in order to proceed.\\n\\n"
		printf "Exiting now.\\n"
		exit 1
	fi

	printf "xcode-select installation found @ \\n"
	printf "%s \\n\\n" "${XCODESELECT}"

	printf "Checking Ruby installation.\\n"
	if ! RUBY=$( command -v ruby)
	then
		printf "\\nRuby must be installed in order to proceed.\\n\\n"
		printf "Exiting now.\\n"
		exit 1
	fi

	printf "Ruby installation found @ \\n"
	printf "%s \\n\\n" "${RUBY}"

	printf "Checking Home Brew installation\\n"
	if ! BREW=$( command -v brew )
	then
		printf "Homebrew must be installed to compile EOS.IO\\n\\n"
		printf "Do you wish to install Home Brew?\\n"
		select yn in "Yes" "No"; do
			case "${yn}" in
				[Yy]* ) 
				"${XCODESELECT}" --install 2>/dev/null;
				if ! "${RUBY}" -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
				then
					echo "Unable to install homebrew at this time. Exiting now."
					exit 1;
				else
					BREW=$( command -v brew )
				fi
				break;;
				[Nn]* ) echo "User aborted homebrew installation. Exiting now.";
						exit 1;;
				* ) echo "Please enter 1 for yes or 2 for no.";;
			esac
		done
	fi

	printf "Home Brew installation found @\\n"
	printf "%s\\n\\n" "${BREW}"
	
	COUNT=1
	PERMISSION_GETTEXT=0
	DISPLAY=""
	DEP=""

	printf "Checking dependencies.\\n"
	var_ifs="${IFS}"
	IFS=","
	while read -r name tester testee brewname uri
	do
		printf "Checking %s ... " "${name}"
		if [ "${tester}" "${testee}" ]; then
			printf " %s found\\n" "${name}"
			continue
		fi
		# resolve conflict with homebrew glibtool and apple/gnu installs of libtool
		if [ "${testee}" == "/usr/local/bin/glibtool" ]; then
			if [ "${tester}" "/usr/local/bin/libtool" ]; then
				printf " %s found\\n" "${name}"
				continue
			fi
		fi
		if [ "${brewname}" = "gettext" ]; then
			PERMISSION_GETTEXT=1
		fi
		DEP=$DEP"${brewname} "
		DISPLAY="${DISPLAY}${COUNT}. ${name}\\n"
		printf " %s ${bldred}NOT${txtrst} found.\\n" "${name}"
		(( COUNT++ ))
	done < scripts/eosio_build_dep
	IFS="${var_ifs}"
		
	printf "Checking Python3 ... "
	if [  -z "$( python3 -c 'import sys; print(sys.version_info.major)' 2>/dev/null )" ]; then
		DEP=$DEP"python@3 "
		DISPLAY="${DISPLAY}${COUNT}. Python 3\\n"
		printf " python3 ${bldred}NOT${txtrst} found.\\n"
		(( COUNT++ ))
	else
		printf " Python3 found\\n"
	fi

	if [ $COUNT -gt 1 ]; then
		printf "\\nThe following dependencies are required to install EOSIO.\\n"
		printf "\\n%s\\n\\n" "${DISPLAY}"
		echo "Do you wish to install these packages?"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					if [ $PERMISSION_GETTEXT -eq 1 ]; then
						sudo chown -R "$(whoami)" /usr/local/share
					fi
					"${XCODESELECT}" --install 2>/dev/null;
					printf "Updating Home Brew.\\n"
					if ! brew update
					then
						printf "Unable to update Home Brew at this time.\\n"
						printf "Exiting now.\\n\\n"
						exit 1;
					fi
					printf "Installing Dependencies.\\n"
					if ! "${BREW}" install --force ${DEP}
					then
						printf "Homebrew exited with the above errors.\\n"
						printf "Exiting now.\\n\\n"
						exit 1;
					fi
					if ! "${BREW}" unlink {DEP} && "${BREW}" link --force ${DEP}
					then
						printf "Homebrew exited with the above errors.\\n"
						printf "Exiting now.\\n\\n"
						exit 1;
					fi
				break;;
				[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
				* ) echo "Please type 1 for yes or 2 for no.";;
			esac
		done
	else 
		printf "\\nNo required Home Brew dependencies to install.\\n"
	fi
  
	function print_instructions()
	{
		printf "cd %s; make test\\n\\n" "${BUILD_DIR}"
      return 0
	}
