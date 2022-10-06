//
// Created by ChristianBudhiS on 26/08/2022.
//

#ifndef EVOLUTIONARY_COMPUTING_PSO_ALGORITHM_H
#define EVOLUTIONARY_COMPUTING_PSO_ALGORITHM_H


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


/**
 * nanti struct ini bakalan immutable
 */
struct Particle{
    std::vector<double> mask;
    std::vector<int> solution;
    std::vector<double> personal_best;
    std::vector<double> velocity;
    char len;
    double cur_fitness_score;
    double peb_fitness_score;

    Particle();
    Particle(char len);

    bool operator <(const Particle &rhs) const;
    bool operator<=(const Particle &rhs) const;
    bool operator >(const Particle &rhs) const;
    bool operator>=(const Particle &rhs) const;
    bool operator==(const Particle &rhs) const;
    bool operator!=(const Particle &rhs) const;


    void unfit();
    void fitness();
    void normalize();
    void reindex_solution();

    void move(std::vector<double> global_best, double glb_fitness_score);

    void print();
    std::string to_string();
};

struct Generation{
    std::vector<Particle> member;
    double best_fitness;
    double average_fitness;

    Generation(std::vector<Particle> m) : member(m) {
        average_fitness = 0;
        best_fitness = INT_MIN;
        for (Particle& c : m) {
            best_fitness = std::max(best_fitness, c.cur_fitness_score);
            average_fitness += c.cur_fitness_score;
        }

        std::sort(member.begin(), member.end(), std::greater<Particle>());

        average_fitness /= m.size();
    }
};

std::function<double()> random_ext();

extern int PTS_COUNT;
extern std::vector<Particle> G;
extern std::function<double()> randomizer;
extern std::vector<Generation> history;

void generate_ancestor();
void move_together();
bool should_stop();

#endif //EVOLUTIONARY_COMPUTING_PSO_ALGORITHM_H