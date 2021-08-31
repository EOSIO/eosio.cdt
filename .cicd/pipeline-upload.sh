#!/bin/bash
set -eou pipefail

# attach pipeline documentation
echo '+++ :md: Attaching Documentation'
export DOCS_URL="https://github.com/EOSIO/eosio.cdt/blob/${BUILDKITE_COMMIT:-master}/.cicd/README.md"
export RETRY="$(buildkite-agent meta-data get pipeline-upload-retries --default '0')"
buildkite-agent meta-data set pipeline-upload-retries "$(( $RETRY + 1 ))"
if [[ "$BUILDKITE" == 'true' && "$RETRY" == '0' ]]; then
    echo "This documentation is also available on [GitHub]($DOCS_URL)." | buildkite-agent annotate --append --style 'info' --context 'documentation'
    cat ".cicd/README.md" | sed 's_<x>_<details>\n<summary>See More</summary>_' | sed 's_</x>_</details>_' | buildkite-agent annotate --append --style 'info' --context 'documentation'
elif [[ "$BUILDKITE" == 'true' ]]; then
    printf "Skipping \033]1339;url=$DOCS_URL;content=documentation\a upload for job retry number $RETRY.\n"
fi
export MACOS_10_14_TAG="eosio-cdt-macos-10.14-$(sha1sum .cicd/platforms/macos-10.14.sh | awk '{print $1}')"
export MACOS_10_15_TAG="eosio-cdt-macos-10.15-$(sha1sum .cicd/platforms/macos-10.15.sh | awk '{print $1}')"
# pipeline upload
echo '+++ :pipeline_upload: Deploying Pipeline Steps'
if [[ "$BUILDKITE" == 'true' ]]; then
    [[ "${DEBUG:-''}" == 'true' ]] && buildkite-agent artifact upload .cicd/pipeline.yml
    buildkite-agent pipeline upload .cicd/pipeline.yml
fi
echo '+++ :white_check_mark: Done! Good luck :)'
