# taurus-cdt-tf
resource "buildkite_pipeline" "taurus_cdt_tf" {
  name           = "taurus-cdt-tf"
  repository     = "https://github.com/b1-as/taurus-cdt.git"
  default_branch = "BLU-27816"
  description    = "CI/CD for the cloud infrastructure and CI code in the taurus-cdt repo, the Bullish EOSIO.cdt fork"

  branch_configuration = "BLU-27816"
  cancel_intermediate_builds = false
  skip_intermediate_builds = false

  provider_settings {
    build_branches = true
    build_tags = true
    build_pull_requests = true
    build_pull_request_forks = false
    build_pull_request_ready_for_review = false
    cancel_deleted_branch_builds = true
    filter_enabled = false
    pull_request_branch_filter_enabled = false
    publish_blocked_as_pending = true
    publish_commit_status = true
    publish_commit_status_per_step = false
    separate_pull_request_statuses = false
    skip_pull_request_builds_for_existing_commits = true
    trigger_mode = "code"
  }

  steps = <<-YAML
    steps:
      - label: ":pipeline: Pipeline Upload"
        command: "buildkite-agent pipeline upload .cicd/taurus-cdt.yml"
        agents:
          queue: "automation-gke-ha-dev-basic-modern-builder-fleet"
        timeout: 10
    YAML

  team = [
    {
      access_level = "MANAGE_BUILD_AND_READ"
      slug         = "automation"
    },
    {
      access_level = "BUILD_AND_READ"
      slug         = "blockchain-team"
    },
    {
      access_level = "BUILD_AND_READ"
      slug         = "technical-services"
    },
  ]
}

output "taurus_cdt_tf_pipeline_graphql_id" {
  value = buildkite_pipeline.taurus_cdt_tf.id
}
output "taurus_cdt_tf_pipeline_url" {
  value = "https://buildkite.com/${var.buildkite_org}/${buildkite_pipeline.taurus_cdt_tf.slug}"
}
output "taurus_cdt_tf_pipeline_webhook_url" {
  value = buildkite_pipeline.taurus_cdt_tf.webhook_url
}

# taurus-cdt
resource "buildkite_pipeline" "taurus_cdt" {
  name           = "taurus-cdt"
  repository     = "https://github.com/b1-as/taurus-cdt.git"
  default_branch = "develop-boxed"
  description    = "CI/CD for the Bullish EOSIO fork using a pinned compiler"

  branch_configuration = "develop-boxed BLU-27816" # "develop develop-boxed main master release/*  v*.*.* zach-ci"
  cancel_intermediate_builds = false
  skip_intermediate_builds = false

  provider_settings {
    build_branches = true
    build_tags = true
    build_pull_requests = true
    build_pull_request_forks = false
    build_pull_request_ready_for_review = false
    cancel_deleted_branch_builds = true
    filter_enabled = false
    pull_request_branch_filter_enabled = false
    publish_blocked_as_pending = true
    publish_commit_status = true
    publish_commit_status_per_step = false
    separate_pull_request_statuses = false
    skip_pull_request_builds_for_existing_commits = true
    trigger_mode = "code"
  }

  steps = <<-YAML
    env:
      TIMEOUT: "10"
    steps:
      - label: ":pipeline: Pipeline Upload"
        command: |
          if [[ -f ./.cicd/pipeline-upload.sh ]]; then
            ./.cicd/pipeline-upload.sh;
          else
            buildkite-agent pipeline upload ./.cicd/pipeline.yml;
          fi
        agents:
          queue: "automation-gke-ha-dev-taurus-basic-builder-fleet"
        timeout: 15
    YAML

  team = [
    {
      access_level = "MANAGE_BUILD_AND_READ"
      slug         = "automation"
    },
    {
      access_level = "BUILD_AND_READ"
      slug         = "blockchain-team"
    },
    {
      access_level = "BUILD_AND_READ"
      slug         = "technical-services"
    },
  ]
}

output "taurus_cdt_pipeline_graphql_id" {
  value = buildkite_pipeline.taurus_cdt.id
}
output "taurus_cdt_pipeline_url" {
  value = "https://buildkite.com/${var.buildkite_org}/${buildkite_pipeline.taurus_cdt.slug}"
}
output "taurus_cdt_pipeline_webhook_url" {
  value = buildkite_pipeline.taurus_cdt.webhook_url
}