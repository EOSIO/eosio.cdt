terraform {
  backend "s3" {
    region = "ap-southeast-1"
    bucket = "taurus-cdt-tf-state2"
    key    = "taurus-cdt.tfstate"
  }
}

# variables
variable "billing-use" {
  default = "dev-eosio"
}

variable "buildkite_org" {
  default = "b1x"
}

# providers
provider "aws" {
  max_retries = "50"
  region      = "ap-southeast-1"
  default_tags {
    tags = {
      billing-use = var.billing-use,
      pipeline = "taurus-cdt-tf",
      repo = "taurus-cdt",
      terraform = "true"
    }
  }
}

provider "buildkite" {
  # input "api_token" is read from "BUILDKITE_API_TOKEN" in the environment
  organization = var.buildkite_org
}