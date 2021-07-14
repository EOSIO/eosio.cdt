#!/bin/bash
set -eou pipefail

export MACOS_10_14_TAG="eosio-cdt-macos-10.14-$(sha1sum ./.cicd/platforms/macos-10.14.sh | awk '{print $1}')"
export MACOS_10_15_TAG="eosio-cdt-macos-10.15-$(sha1sum ./.cicd/platforms/macos-10.15.sh | awk '{print $1}')"
export VARS='$MACOS_10_14_TAG:$MACOS_10_15_TAG'
envsubst "$VARS" < "./.cicd/pipeline.yml" > "./.cicd/pipeline.yml.out"
buildkite-agent artifact upload ./.cicd/pipeline.yml.out
buildkite-agent pipeline upload ./.cicd/pipeline.yml.out
