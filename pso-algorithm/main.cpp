//
// Created by ChristianBudhiS on 26/08/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "lib/pso_algorithm.h"
#include "main.h"

std::string input = "input.txt";
std::string output = "-1";


std::vector<std::pair<int,int>> pts;


double RHO = 0.9;
double C1 = 2;
double C2 = 2;
int POPULATION_COUNT = 100;
double HISTORY_LOOKUP = 0.01;
int GENERATION_LIMIT = 10000;

void printf_generation(int g, double best, double avg);

int main(int argc, char** argv){
    std::vector<std::string> args;
    if (argc > 1) args.assign(argv + 1, argv + argc);

    for (int i = 1; i < argc; i++){
        if (args[i - 1] == "--help" || args[i - 1] == "-h") help();
        else if (args[i - 1] == "--input" || args[i - 1] == "-i") {
            if (i + 1 >= argc) wrong_param();

            input = args[i];
        }else if (args[i - 1] == "--output" || args[i - 1] == "-o") {
            if (i + 1 >= argc) wrong_param();

            output = args[i];
        }else if (args[i - 1] == "--constant-1" || args[i - 1] == "-c1") {
            if (i + 1 >= argc) wrong_param();

            std::stringstream ss(args[i]);
            ss >> C1;
        }else if (args[i - 1] == "--constant-2" || args[i - 1] == "-c2") {
            if (i + 1 >= argc) wrong_param();

            std::stringstream ss(args[i]);
            ss >> C2;
        }else if (args[i - 1] == "--rho" || args[i - 1] == "-r") {
            if (i + 1 >= argc) wrong_param();

            std::stringstream ss(args[i]);
            ss >> RHO;
        }else if (args[i - 1] == "--population-size" || args[i - 1] == "-ps") {
            if (i + 1 >= argc) wrong_param();

            std::stringstream ss(args[i]);
            ss >> POPULATION_COUNT;
        }else if (args[i - 1] == "--max-iteration" || args[i - 1] == "-mi") {
            if (i + 1 >= argc) wrong_param();

            std::stringstream ss(args[i]);
            ss >> GENERATION_LIMIT;
        }
    }

    read_input();


    generate_ancestor();

    for (int generation = 0; generation < GENERATION_LIMIT && !should_stop(); generation++){
        move_together();

        Generation now(G);
        history.push_back(now);

        printf_generation(generation, now.best_fitness, now.average_fitness);
    }

    write_output();
}

double dist(std::pair<int,int> a, std::pair<int,int> b) {
    int x = a.first - b.first;
    int y = a.second - b.second;
    return sqrt(x * x + y * y);
}

double fitness(int len, std::vector<DATATYPE> gene){
    int sum = 0;

    for (int i = 0; i < len; i++){
        sum += dist(pts[gene[i]], pts[gene[(i + 1) % len]]);
    }

    return -sum;
}

void read_input(){
    std::ifstream input_file(input);

    if (input_file.fail()) {
        perror("Input file");
        exit(0);
    }

    pts.clear();

    int n;
    input_file >> n;

    while (n--) {
        int a, b;
        input_file >> a >> b;
        pts.emplace_back(a, b);
    }

    input_file.close();

    PTS_COUNT = pts.size();

    randomizer = random_ext();
}

void write_output() {
    printf("out...\n");
    if (output == "-1") {
        std::time_t cur_time = std::time(NULL);

        std::string prefix_name = "output/output_";
        std::string ext = ".csv";

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&cur_time), "%d-%m-%Y %H-%M-%S");
        std::string str_time = oss.str();

        output = prefix_name + str_time + ext;
    }

    std::ofstream output_file(output);

    if (output_file.fail()) {
        perror("Output file");
        exit(0);
    }

    output_file << "generation;solution;fitness\n";
    for (int i = 0; i < history.size(); i++){
        for (Particle c : history[i].member) {
            output_file << i + 1 << ";" << c.to_string() << ";" << c.cur_fitness_score << "\n";
        }
    }

    output_file.close();
}

void printf_generation(int g, double best, double avg) {
    printf("generation: %d   \tbest: %.3f\taverage: %.3f\n",
           g, best, avg);
}

void help(){
    printf("\033[1mUsage:\033[0m\n"
           "    solution-alg \033[1;33m[options] \033[0;36minput-filename\n"
           "\033[1mOptions:\033[0m\n"
           " \033[1;33m-e\033[0m, \033[1;33m--enc\033[0m\tEncode a file to a text file.\n"
           " \033[1;33m-d\033[0m, \033[1;33m--dec\033[0m\tDecode a text file to a file.\n"
           " \033[1;33m-o\033[0m, \033[1;33m--output\033[0m\tOutput file name.\n");
    exit(0);
}

void wrong_param(){
    printf("\033[1mWrong usage\033[0m\n"
           "stop it, use \033[1m--help\033[0m to get some help\n");
    exit(0);
}