export ROOT_DIR=$( dirname "${BASH_SOURCE[0]}" )/../..
export BUILD_DIR=$ROOT_DIR/build
export CICD_DIR=$ROOT_DIR/.cicd
export HELPERS_DIR=$CICD_DIR/helpers
export JOBS=${JOBS:-"$(getconf _NPROCESSORS_ONLN)"}
export MOUNTED_DIR='/root/eosio/cdt'


# load buildkite intrinsic environment variables for use in docker run
function buildkite-intrinsics()
{
    BK_ENV=''
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        while read -r var; do
            BK_ENV="$BK_ENV --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi
    echo "$BK_ENV"
}