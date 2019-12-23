#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

[[ $TRAVIS != true ]] && buildkite-agent artifact download build.tar.gz . --step "$PLATFORM_FULL_NAME - Build"

if [[ $(uname) == 'Darwin' ]]; then

    tar -xzf build.tar.gz

    bash -c "cd build/packages && chmod 755 ./*.sh && ./generate_package.sh brew"

    ARTIFACT='*.rb;*.tar.gz'
    cd build/packages
    [[ -d x86_64 ]] && cd 'x86_64' # backwards-compatibility with release/1.6.x
    buildkite-agent artifact upload "./$ARTIFACT" --agent-access-token $BUILDKITE_AGENT_ACCESS_TOKEN
    for A in $(echo $ARTIFACT | tr ';' ' '); do
        if [[ $(ls $A | grep -c '') == 0 ]]; then
            echo "+++ :no_entry: ERROR: Expected artifact \"$A\" not found!"
            pwd
            ls -la
            exit 1
        fi
    done

else # Linux

    ARGS=${ARGS:-"--rm --init $(buildkite-intrinsics) -v $(pwd):$(pwd)"}

    PRE_COMMANDS="cd $(pwd)/build/packages && chmod 755 $(pwd)/build/packages/*.sh"

    if [[ "$IMAGE_TAG" =~ "ubuntu" ]]; then
        ARTIFACT='*.deb'
        PACKAGE_TYPE='deb'
        PACKAGE_COMMANDS="./generate_package.sh $PACKAGE_TYPE"
    elif [[ "$IMAGE_TAG" =~ "centos" ]]; then
        ARTIFACT='*.rpm'
        PACKAGE_TYPE='rpm'
        PACKAGE_COMMANDS="mkdir -p ~/rpmbuild/BUILD && mkdir -p ~/rpmbuild/BUILDROOT && mkdir -p ~/rpmbuild/RPMS && mkdir -p ~/rpmbuild/SOURCES && mkdir -p ~/rpmbuild/SPECS && mkdir -p ~/rpmbuild/SRPMS && yum install -y rpm-build && ./generate_package.sh $PACKAGE_TYPE"
    fi

    PACKAGE_COMMANDS="cd $(pwd) && tar -xzf build.tar.gz && $PRE_COMMANDS && $PACKAGE_COMMANDS"
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    echo "$ docker run $ARGS $FULL_TAG bash -c \"$PACKAGE_COMMANDS\""
    eval docker run $ARGS $FULL_TAG bash -c \"$PACKAGE_COMMANDS\"

    cd build/packages
    [[ -d x86_64 ]] && cd 'x86_64' # backwards-compatibility with release/1.6.x
    buildkite-agent artifact upload "./$ARTIFACT" --agent-access-token $BUILDKITE_AGENT_ACCESS_TOKEN
    for A in $(echo $ARTIFACT | tr ';' ' '); do
        if [[ $(ls $A | grep -c '') == 0 ]]; then
            echo "+++ :no_entry: ERROR: Expected artifact \"$A\" not found!"
            pwd
            ls -la
            exit 1
        fi
    done

fi