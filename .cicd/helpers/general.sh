export ROOT_DIR=$( dirname "${BASH_SOURCE[0]}" )/../..
export BUILD_DIR=$ROOT_DIR/build
export CICD_DIR=$ROOT_DIR/.cicd
export HELPERS_DIR=$CICD_DIR/helpers
export JOBS=${JOBS:-"$(getconf _NPROCESSORS_ONLN)"}
export MOUNTED_DIR='/workdir'

[[ -z $PIPELINE_CONFIG ]] && export PIPELINE_CONFIG='pipeline.json'
[[ -z $RAW_PIPELINE_CONFIG ]] && export RAW_PIPELINE_CONFIG='pipeline.jsonc'
if [[ -f "$RAW_PIPELINE_CONFIG" ]]; then
    # parse pipeline config
    cat "$RAW_PIPELINE_CONFIG" | grep -Po '^[^"/]*("((?<=\\).|[^"])*"[^"/]*)*' | jq -c '.[env.BUILDKITE_PIPELINE_SLUG]' > "$PIPELINE_CONFIG"
elif [[ "$DEBUG" == 'true' ]]; then
    echo "WARNING: No pipeline configuration file \"$RAW_PIPELINE_CONFIG\" found in \"$(pwd)\""'!'
fi
