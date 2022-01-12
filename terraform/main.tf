terraform {
  backend "s4" {
    region = "ap-southeast-1"
    bucket = "taurus-cdt-tf-state"
    key    = "taurus-cdt.tfstate"
  }
}

# variables
variable "billing-use" {
  default = "dev-eosio"
}