#!/bin/bash

            ##### sanitize branch names for use in URIs (docker containers) #####
# tr '/' '_'                       # convert forward-slashes '/' to underscores '_'
# sed -E 's/[^-_.a-zA-Z0-9]+/-/g'  # convert invalid docker chars to '-'
# sed -E 's/-+/-/g'                # replace multiple dashes in a series "----" with a single dash '-'
# sed -E 's/-*_+-*/_/g'            # replace dashes '-' and underscores '_' in-series with a single underscore '_'
# sed -E 's/_+/_/g'                # replace multiple underscores in a row "___" with a single underscore '_'
# sed -E 's/(^[-_.]+|[-_.]+$)//g'  # ensure tags do not begin or end with separator characters [-_.]
function sanitize()
{
    echo "$1" | tr '/' '_' | sed -E 's/[^-_.a-zA-Z0-9]+/-/g' | sed -E 's/-+/-/g' | sed -E 's/-*_+-*/_/g' | sed -E 's/_+/_/g' | sed -E 's/(^[-_.]+|[-_.]+$)//g'
}
