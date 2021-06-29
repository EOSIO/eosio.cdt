#!/bin/bash
set -eou pipefail

export MACOS_10_14_FILE_HASH="eosio-cdt-macos-10.14-$(sha1sum ./.cicd/docker/macos-10.14.sh | awk '{print $1}')"
export MACOS_10_15_FILE_HASH="eosio-cdt-macos-10.15-$(sha1sum ./.cicd/docker/macos-10.15.sh | awk '{print $1}')"
export VARS='$MACOS_10_14_FILE_HASH:$MACOS_10_15_FILE_HASH'
envsubst "$VARS" < "./.cicd/pipeline.yml" > "./.cicd/pipeline.yml.out"
buildkite-agent artifact upload pipeline.yml.out
buildkite-agent pipeline upload pipeline.yml.out
