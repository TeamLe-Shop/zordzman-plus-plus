#!/bin/bash

$PYTHON -m zm.cgen Egress client/net/messages.hpp.in > client/net/egress.hpp
$PYTHON -m zm.cgen Ingress client/net/messages.hpp.in > client/net/ingress.hpp
$PYTHON -m zm.cgen Egress Ingress client/net/Client.hpp.in > client/net/Client.hpp
mkdir conda-build/
(cd conda-build/ && \
    $PREFIX/bin/cmake ../ \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH=$PREFIX \
    -DCMAKE_INSTALL_PREFIX=$PREFIX)
(cd conda-build/ && make zordzman -j $(getconf _NPROCESSORS_ONLN))
(cd conda-build/ && make zordzman install)
