#include "src.hpp"

int main() {
    int seed;
    if (!(std::cin >> seed)) return 0;
    sjtu::Tetris server(seed);
    server.run();
    return 0;
}

