resource "buildkite_pipeline" "taurus_cdt" {
  name                 = "taurus-cdt"
  repository           = "https://github.com/b1-as/taurus-cdt.git"
  slug                 = "eosio-dot-cdt"
  branch_configuration = "master develop develop-boxed release/* v*.*.*"
  default_branch       = "master"
  description          = "Build, test, and binary packaging of the eosio.cdt repo"
  env                  =  {
    "BUILDKITE_CLEAN_CHECKOUT" = "true"
#    "EOSIO_CDT_REGISTRY" = "436617320021.dkr.ecr.us-west-2.amazonaws.com/eosio/docker-hub-mirror/eosio.cdt"
    "EOSIO_CDT_REGISTRY" = "taurus_cdt"
    "TIMEOUT" = "60"
  }
  step = [
    {
      type    = "script"
      name    = ":pipeline: Pipeline Upload"
      command = "if [[ -f ./.cicd/pipeline-upload.sh ]]; then ./.cicd/pipeline-upload.sh; else buildkite-agent pipeline upload ./.cicd/pipeline.yml; fi"
      agent_query_rules = [
        "queue=automation-taurus-basic-builder-fleet"
      ]
    }
  ]
}
output "taurus_cdt_webhook_url" {
  value = "${buildkite_pipeline.taurus_cdt.webhook_url}"
}