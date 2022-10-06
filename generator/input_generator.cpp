//
// Created by ChristianBudhiS on 21/09/2022.
//

/**
18
7 3
4 5
1 8
2 10
9 2
6 7
8 3
6 3
7 7
10 1
7 9
3 6
5 4
4 2
1 4
2 3
4 8
6 6
 */

#include <iostream>
#include <fstream>
#include <set>
#include <time.h>
#include <vector>

std::string output = "input.txt";

int main(int argc, char** argv) {
    srand(time(NULL));

    std::vector<std::string> args;
    if (argc > 1) args.assign(argv + 1, argv + argc);

    for (int i = 1; i < argc; i++){
        if (args[i - 1] == "--output" || args[i - 1] == "-o") {
            if (i + 1 >= argc) exit(0);

            output = args[i];
        }
    }


    int city = (rand() % 30) + 10;

    int limit = 5 * city;

    std::ofstream out(output);

    if (out.fail()) {
        perror("Failed");
        exit(0);
    }

    out << city << "\n";

    std::set<std::pair<int,int>> pts;
    while(pts.size() < city) {
        int x = rand() % limit;
        int y = rand() % limit;

        pts.insert({x, y});
    }

    for (const std::pair<int,int>& a : pts) {
        out << a.first << " " << a.second << "\n";
    }

    out.close();
}