terraform {
  backend "s3" {
    bucket = "eos-buildkite-pipelines-tf-state"
    key    = "eos-buildkite-pipelines.tfstate"
    region = "us-west-2"
  }
}

#variables
variable "billing-use" {
  default = "dev-eosio"
}

provider "buildkite" {
  api_token    = "${data.aws_ssm_parameter.buildkite_api_key.value}"
  organization = "EOSIO"
  version = "0.0.3"
}