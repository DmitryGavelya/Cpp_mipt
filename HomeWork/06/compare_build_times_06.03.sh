#!/bin/bash

if ! command -v g++ &> /dev/null; then echo "g++ not found"; exit 1; fi
if ! command -v bc &> /dev/null; then echo "bc not found"; exit 1; fi

if [ ! -d "06.01" ]; then echo "Folder 06.01 not found"; exit 1; fi
cd 06.01
rm -f *.o app_classic

TIMEFORMAT='%3R'

# Замер времени
t_rat_c=$( { time g++ -std=c++20 -c rational.cpp -o rational.o; } 2>&1 )
t_main_c=$( { time g++ -std=c++20 -c main.cpp -o main.o; } 2>&1 )
t_link_c=$( { time g++ -std=c++20 rational.o main.o -o app_classic; } 2>&1 )

# Замер размеров
s_rat_c=$(stat -c%s rational.o 2>/dev/null || stat -f%z rational.o)
s_main_c=$(stat -c%s main.o 2>/dev/null || stat -f%z main.o)
s_exe_c=$(stat -c%s app_classic 2>/dev/null || stat -f%z app_classic)

cd ..

if [ ! -d "06.02" ]; then echo "Folder 06.02 not found"; exit 1; fi
cd 06.02
rm -f *.o app_modules gcm.cache -r

g++ -std=c++20 -fmodules-ts -x c++-system-header iostream vector compare numeric cmath cassert sstream 2>/dev/null

# Замер времени
t_rat_ixx=$( { time g++ -std=c++20 -fmodules-ts -x c++ -c rational.ixx -o rational_mod.o; } 2>&1 )
t_rat_cpp=$( { time g++ -std=c++20 -fmodules-ts -c rational.cpp -o rational_impl.o; } 2>&1 )
t_main_m=$( { time g++ -std=c++20 -fmodules-ts -c main.cpp -o main.o; } 2>&1 )
t_link_m=$( { time g++ -std=c++20 -fmodules-ts rational_mod.o rational_impl.o main.o -o app_modules; } 2>&1 )

# Замер размеров
if [ -f rational_mod.o ]; then
    s_mod=$(stat -c%s rational_mod.o 2>/dev/null || stat -f%z rational_mod.o)
    s_impl=$(stat -c%s rational_impl.o 2>/dev/null || stat -f%z rational_impl.o)
    s_rat_m=$(( s_mod + s_impl ))
else
    s_rat_m=0
fi

s_main_m=$(stat -c%s main.o 2>/dev/null || stat -f%z main.o)
s_exe_m=$(stat -c%s app_modules 2>/dev/null || stat -f%z app_modules)

cd ..

t_rat_c=$(echo "$t_rat_c" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
t_main_c=$(echo "$t_main_c" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
t_link_c=$(echo "$t_link_c" | grep -oE '[0-9]+\.[0-9]+' | head -n1)

t_rat_ixx=$(echo "$t_rat_ixx" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
t_rat_cpp=$(echo "$t_rat_cpp" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
t_main_m=$(echo "$t_main_m" | grep -oE '[0-9]+\.[0-9]+' | head -n1)
t_link_m=$(echo "$t_link_m" | grep -oE '[0-9]+\.[0-9]+' | head -n1)

if [[ -n "$t_rat_ixx" && -n "$t_rat_cpp" ]]; then
    t_rat_m_total=$(echo "$t_rat_ixx + $t_rat_cpp" | bc)
else
    t_rat_m_total="ERR"
fi

echo "METRIC                  | CLASSIC         | MODULES"
echo "------------------------|-----------------|-----------------"
printf "Compile Rational        | %-15s | %-15s\n" "${t_rat_c}s" "${t_rat_m_total}s"
printf "Compile Main            | %-15s | %-15s\n" "${t_main_c}s" "${t_main_m}s"
printf "Linking                 | %-15s | %-15s\n" "${t_link_c}s" "${t_link_m}s"
echo "------------------------|-----------------|-----------------"
printf "Size: Rational obj      | %-15s | %-15s\n" "$s_rat_c B" "$s_rat_m B"
printf "Size: Main obj          | %-15s | %-15s\n" "$s_main_c B" "$s_main_m B"
printf "Size: EXE               | %-15s | %-15s\n" "$s_exe_c B" "$s_exe_m B"
