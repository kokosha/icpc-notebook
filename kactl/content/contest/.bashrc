comp() {
    g++ -std=c++17 -Ofast -Wall -Wshadow -fsanitize=address -D_GLIBCXX_DEBUG -W -Wextra -o $1 $1.cpp
}
run() {
    comp $1 && ./$1
}