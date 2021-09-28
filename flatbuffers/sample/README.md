# Sample

This is a simple example of how to use `afb-fbgen` with a custom template added to working folder. this sample just make the conversion from json to flatbuffers and pass the converted buffer as a subcall to an other api.

## generate sample

To generate sample just run

```sh
afb-fbgen sample.fbs
```

afb-fbgen will use the local fb-template-api.c instead of the one installed in your system.

## compile the generated API

You can now compile the generated API :

```sh
mkdir build && cd build
cmake ..
make
```

## Run the generated API

You can run th generated API with the binder :

```sh
afb-binder -b ./libsample-api.so --rpc-client tcp:<destinationIP>:<port>/<api> -vvv
```
