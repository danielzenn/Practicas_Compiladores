#!/bin/sh

set -e
CXX=g++
FLAGS="-std=c++17 -Wall -Wextra"

echo " Preparacion"
$CXX $FLAGS -o nfa_to_dfa src/main.cpp src/nfa.cpp src/dfa.cpp src/subset_construction.cpp
./nfa_to_dfa
echo

echo " Prueba para: epsilon_closure"
$CXX $FLAGS -o tests/test_epsilon_closure tests/test_epsilon_closure.cpp src/nfa.cpp src/subset_construction.cpp
./tests/test_epsilon_closure
echo

echo " Prueba para: move"
$CXX $FLAGS -o tests/test_move tests/test_move.cpp src/nfa.cpp src/subset_construction.cpp
./tests/test_move
echo

echo " Prueba para: subset_construction"
$CXX $FLAGS -o tests/test_subset_construction tests/test_subset_construction.cpp src/nfa.cpp src/dfa.cpp src/subset_construction.cpp
./tests/test_subset_construction
