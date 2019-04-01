#!/bin/bash
set -e # exit on failure of any "simple" command (excludes &&, ||, or | chains)
echo "Extracting build directory..."
tar -zxf build.tar.gz
cd build
CPU_CORES=$(getconf _NPROCESSORS_ONLN)
echo "$CPU_CORES cpu cores detected."
TEST_COUNT=$(ctest -N | grep -i 'Total Tests: ' | cut -d ':' -f 2 | awk '{print $1}')
[[ $TEST_COUNT > 0 ]] && echo "$TEST_COUNT tests found." || (echo "ERROR: No tests registered with ctest! Exiting..." && exit 1)
echo "$ ctest -j $CPU_CORES -V -T Test"
ctest -j $CPU_CORES -V -T Test
echo "Uploading xUnit test results XML file..."
XML_FILENAME="test-results.xml"
mv $(pwd)/Testing/$(ls $(pwd)/Testing/ | grep '20' | tail -n 1)/Test.xml $XML_FILENAME
buildkite-agent artifact upload $XML_FILENAME
echo "Done uploading $XML_FILENAME."