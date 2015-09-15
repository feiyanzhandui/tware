#!/bin/bash
clang++ -std=c++11 -O3 -ffast-math -mavx simd.cpp -o vsimd
clang++ -std=c++11 -O3 -ffast-math -mavx -fno-vectorize simd.cpp -o ssimd
clang++ -std=c++11 -O3 -ffast-math -mavx fission.cpp -o fission
clang++ -std=c++11 -O3 -ffast-math -mavx fused.cpp -o fused
clang++ -std=c++11 -O3 -ffast-math -mavx bandwidth.cpp -o vband
clang++ -std=c++11 -O3 -ffast-math -mavx -fno-vectorize bandwidth.cpp -o sband
clang++ -std=c++11 -O3 -ffast-math -mavx tiled.cpp -o tiled
clang++ -std=c++11 -O3 -ffast-math -mavx bulk.cpp -o bulk
./vsimd 'vsimd'
./ssimd 'ssimd'
./fission
./fused
./vband 'vband'
./sband 'sband'
./tiled
./bulk
