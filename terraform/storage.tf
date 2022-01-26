resource "aws_s3_bucket" "taurus_cdt_tf_state_bucket" {
  bucket = "taurus-cdt-tf-state"
  versioning {
    enabled = true
  }
  lifecycle {
    prevent_destroy = true
  }
  server_side_encryption_configuration {
    rule {
      bucket_key_enabled = false
      apply_server_side_encryption_by_default {
        sse_algorithm = "AES256"
      }
    }
  }
  tags = {
    Description = "Terraform state files for the Bullish EOSIO implementation - taurus-cdt",
    Name = "taurus-cdt-tf-state",
  }
}