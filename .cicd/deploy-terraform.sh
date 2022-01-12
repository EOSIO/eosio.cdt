#!/bin/bash
set -eo pipefail
cd "$(dirname "$BASH_SOURCE[0]")/../terraform"
echo '+++ :terraform: Terraform Deployment'
rm -rf ./plan.out
TF_INIT='terraform init'
echo "$ $TF_INIT"
eval $TF_INIT
TF_PLAN='terraform plan -out=plan.out'
echo "$ $TF_PLAN"
eval $TF_PLAN
if [[ ! -f plan.out ]]; then
    echo 'Plan file not found. See plan output for details.' 1>&2
    exit 1
fi
if [[ "$(echo "$BUILDKITE_BRANCH" | grep -cP '^develop-boxed|BLU-27816$')" == '1' ]] ; then
    TF_APPLY='terraform apply plan.out'
    echo "$ $TF_APPLY"
    eval $TF_APPLY
else
    echo "${BUILDKITE_BRANCH:-No} branch specified. Skipping terraform apply."
fi
echo 'Done.'