g++ -Wall -Wextra -Wno-unused-result -g3 -std=c++14 -O2 -static -fsanitize=address,undefined,leak -o $1 $1.cpp
./$1
