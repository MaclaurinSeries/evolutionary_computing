//
// Created by ChristianBudhiS on 20/09/2022.
//

#include <functional>
#include <algorithm>
#include <set>
#include "pso_algorithm.h"


int PTS_COUNT;
std::vector<Particle> G;

std::function<double()> randomizer;

std::vector<Generation> history;

int delay_checking = 0;
int step_checking = 5;

double best_of_the_best = 0.0;

void generate_ancestor(){
    G.clear();

    std::function<bool(Particle&)> exist = [&] (Particle& current) {
        for (Particle& c : G) {
            bool sm = true;
            for (int i = 0; i < c.solution.size() && sm; i++)
                sm &= c.solution[i] == current.solution[i];
            if (sm) return true;
        }

        return false;
    };

    while (G.size() < POPULATION_COUNT) {
        Particle nc((char) PTS_COUNT);
        nc.fitness();

        if (!exist(nc)) G.push_back(nc);
    }
}

void move_together() {
    std::vector<double> mask_global_best(PTS_COUNT, 0);
    double glb_fitness_score = INT_MIN;

    for (Particle p : G) {
        if (glb_fitness_score < p.cur_fitness_score) {
            mask_global_best = p.mask;
            glb_fitness_score = p.cur_fitness_score;
        }
    }

    for (Particle p : G) {
        p.move(mask_global_best, glb_fitness_score);
    }
}

bool should_stop() {
    delay_checking = (delay_checking + 1) % step_checking;
    if (delay_checking > 0) return false;

    int limit = GENERATION_LIMIT * HISTORY_LOOKUP;

    if (history.size() < limit)
        return false;

    std::function<std::vector<int>(std::vector<int>)> adjust =
            [](std::vector<int> v) {
        int len = v.size();
        std::vector<int> res;

        int start = 0;

        for (; start < len && v[start] > 0 ;start++);
        if (start == len) exit(0);

        res.push_back(v[start]);
        for (start = (start + 1) % len; v[start] > 0;start = (start + 1) % len) {
            res.push_back(v[start]);
        }

        if (len > 1 && res[1] > res[len - 1]) {
             for (int l = 1, r = len - 1; l < r; l++, r--)
                 std::swap(res[l], res[r]);
        }

        return res;
    };

    int pick = 8;
    std::set<std::vector<int>> unique;

    double last_best_average = 0.0;
    double last_best_standard_deviation = 0.0;

    double last_avg_average = 0.0;
    double last_avg_standard_deviation = 0.0;

    for (int i = history.size() - limit; i < history.size(); i++) {
        for (int j = 0; j < pick; j++) {
            unique.insert(adjust(history[i].member[j].solution));
        }

        last_best_average += history[i].best_fitness;
        last_avg_average += history[i].average_fitness;

//        for (Particle c : history[i].member) {
//            unique.insert(adjust(c.solution));
//        }

    }

    last_best_average /= limit;
    last_avg_average /= limit;

    for (int i = history.size() - limit; i < history.size(); i++) {
        last_best_standard_deviation +=
                (history[i].best_fitness - last_best_average) * (history[i].best_fitness - last_best_average);
        last_avg_standard_deviation +=
                (history[i].average_fitness - last_avg_average) * (history[i].average_fitness - last_avg_average);
    }

    last_best_standard_deviation = sqrt(last_best_standard_deviation / limit);
    last_avg_standard_deviation = sqrt(last_avg_standard_deviation / limit);

    int unique_cnt = unique.size();
    double gene_similarity = pick * 1.0 / unique_cnt;

    double score = gene_similarity * gene_similarity * 100.0;

    //history[history.size() - 1].member[0].print();
    //return score < gene_similarity;
    if (last_best_standard_deviation < best_of_the_best) return false;

    best_of_the_best = std::max(best_of_the_best, last_best_standard_deviation);

    return last_best_standard_deviation < last_avg_standard_deviation;
}

std::function<double()> random_ext() {
    std::mt19937                        generator(time(nullptr));
    std::uniform_int_distribution<int>    distribution(0, RAND_MAX);
    return [=]() mutable {
        return distribution(generator) * 0.1 / RAND_MAX;
    };
}