#!/bin/bash

##
## Copyright (c) 2016-18, Lawrence Livermore National Security, LLC.
##
## Produced at the Lawrence Livermore National Laboratory.
##
## LLNL-CODE-689114
##
## All rights reserved.
##
## This file is part of RAJA.
##
## For details about use and distribution, please read RAJA/LICENSE.
##

rm -rf build_lc_blueos_nvcc9.1_clang-coral-2017.12.06 >/dev/null
mkdir build_lc_blueos_nvcc9.1_clang-coral-2017.12.06 && cd build_lc_blueos_nvcc9.1_clang-coral-2017.12.06

module load cmake/3.9.2

RAJA_DIR=$(git rev-parse --show-toplevel)

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -C ${RAJA_DIR}/host-configs/lc-builds/blueos/nvcc_clang_coral_2017_12_06.cmake \
  -DENABLE_OPENMP=On \
  -DENABLE_CUDA=On \
  -DCUDA_TOOLKIT_ROOT_DIR=/usr/tce/packages/cuda/cuda-9.1.85 \
  -DCMAKE_INSTALL_PREFIX=${RAJA_DIR}/install_lc_blueos_nvcc9.1_clang-coral-2017.12.06 \
  "$@" \
  ${RAJA_DIR}
