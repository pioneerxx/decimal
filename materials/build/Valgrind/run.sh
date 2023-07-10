#!/bin/bash

cd ../../../
docker rmi -f ereva/valgrind/decimal
docker build . -t ereva/valgrind/decimal -f materials/build/Valgrind/Dockerfile
docker run --rm ereva/valgrind/decimal
