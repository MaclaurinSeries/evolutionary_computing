//
// Created by ChristianBudhiS on 26/08/2022.
//

#ifndef EVOLUTIONARY_COMPUTING_GENETIC_ALGORITHM_H
#define EVOLUTIONARY_COMPUTING_GENETIC_ALGORITHM_H


#include <stdio.h>
#include <utility>
#include <vector>
#include <random>
#include <numeric>
#include <ctime>
#include <functional>
#include <algorithm>
#include "constant.h"

#define ERROR_PRINT(fmt, args ...) fprintf(stderr, \
                                    "\033[91mERROR\033[0m: " \
                                    "\033[93m%s\033[0m:" \
                                    "\033[34m%d\033[0m:" \
                                    "\033[33m%s()\033[0m: \033[91m" fmt "\033[0m\n", \
                                    __FILE__, __LINE__, __func__, ##args)

#define ASSERT(exp, fmt, args ...) if (!exp) { ERROR_PRINT(fmt, ##args); exit(0); }

#if defined(DEBUG) && DEBUG > 0
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, \
                                    "\033[33mDEBUG\033[0m: " \
                                    "\033[93m%s\033[0m:" \
                                    "\033[34m%d\033[0m:" \
                                    "\033[33m%s()\033[0m: " fmt "\n", \
                                    __FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* nothing */
#endif


void read_input();
void write_output();

double fitness(int len, std::vector<DATATYPE> gene);

struct Chromosome{
    std::vector<DATATYPE> gene;
    char len;
    bool evaluated;
    double fitness_score;

    Chromosome();
    Chromosome(char len);

    DATATYPE& operator[](int i);
    bool operator <(const Chromosome &rhs) const;
    bool operator<=(const Chromosome &rhs) const;
    bool operator >(const Chromosome &rhs) const;
    bool operator>=(const Chromosome &rhs) const;
    bool operator==(const Chromosome &rhs) const;
    bool operator!=(const Chromosome &rhs) const;


    void unfit();
    void fitness();

    std::pair<Chromosome, Chromosome> crossover(Chromosome &another, char split = 1);
    std::pair<Chromosome, Chromosome> pmx_crossover(Chromosome &another);
    Chromosome mutation();

    void print();
    std::string to_string();
};

struct Generation{
    std::vector<Chromosome> member;
    double best_fitness;
    double average_fitness;

    Generation(std::vector<Chromosome> m) : member(m) {
        average_fitness = 0;
        best_fitness = -1;
        for (Chromosome& c : m) {
            best_fitness = std::max(best_fitness, c.fitness_score);
            average_fitness += c.fitness_score;
        }

        std::sort(member.begin(), member.end(), std::greater<Chromosome>());

        average_fitness /= m.size();
    }
};

template<typename T>
std::function<int()> random(T range_from, T range_to) {
    std::mt19937                        generator(time(nullptr));
    std::uniform_int_distribution<T>    distr(range_from, range_to - 1);
    return [=]() mutable {
        return distr(generator);
    };
}

extern int PTS_COUNT;
extern std::vector<Chromosome> G;
extern std::function<int()> rand_PTS_COUNT;
extern std::function<int()> rand_ELITE_COUNT;
extern std::vector<Generation> history;

void generate_ancestor();
void crossover_mutation();
void natural_selection();
bool should_stop();

#endif //EVOLUTIONARY_COMPUTING_GENETIC_ALGORITHM_H