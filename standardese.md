# Building the documentation

```
rm -rf standardese_generated
docker build -t eosio-cdt-standardese -f ./standardese.dockerfile .
docker create --name eosio-cdt-standardese-temp eosio-cdt-standardese
docker cp eosio-cdt-standardese-temp:/root/eosio-cdt/docs/standardese_generated .
docker rm eosio-cdt-standardese-temp
ls -al ./standardese_generated
```

The built documentation is at `/root/eosio-cdt/docs/standardese_generated` inside the image. This copies it to the local `standardese_generated` directory.
