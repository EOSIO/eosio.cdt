#!/bin/bash

rm -rf ./standardese_generated
docker build -t eosio-cdt-standardese -f ./standardese.dockerfile .
docker create --name eosio-cdt-standardese-temp eosio-cdt-standardese
docker cp eosio-cdt-standardese-temp:/root/eosio-cdt/docs/standardese_generated .
docker rm eosio-cdt-standardese-temp
ls -al ./standardese_generated
pwd

