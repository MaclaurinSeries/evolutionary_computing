//
// Created by ChristianBudhiS on 20/09/2022.
//

#include "genetic_algorithm.h"
#include <functional>
#include <algorithm>
#include <set>


int PTS_COUNT;
std::vector<Chromosome> G;

std::function<int()> rand_PTS_COUNT;
std::function<int()> rand_ELITE_COUNT;

std::vector<Generation> history;

int delay_checking = 0;
int step_checking = 5;

double best_of_the_best = 0.0;

void generate_ancestor(){
    G.clear();
    std::vector<int> tmp(PTS_COUNT);

    std::function<bool(Chromosome&)> exist = [&] (Chromosome& current) {
        for (Chromosome& c : G) {
            bool sm = true;
            for (int i = 0; i < c.gene.size() && sm; i++)
                sm &= c.gene[i] == current.gene[i];
            if (sm) return true;
        }

        return false;
    };

    while (G.size() < ELITE_COUNT) {
        std::iota(tmp.begin(), tmp.end(), 0);
        for (int j = 0; j < PTS_COUNT; j++){

            int c = rand_PTS_COUNT();
            std::swap(tmp[c], tmp[j]);
        }

        Chromosome nc((char) PTS_COUNT);
        nc.gene = std::vector<DATATYPE>(tmp.begin(), tmp.end());
        nc.fitness();

        if (!exist(nc)) G.push_back(nc);
    }
}

void crossover_mutation(){
    std::vector<Chromosome> F;

    std::function<bool(Chromosome&)> exist = [&] (Chromosome& current) {
        for (Chromosome& c : F) {
            bool sm = true;
            for (int i = 0; i < c.gene.size() && sm; i++)
                sm &= c.gene[i] == current.gene[i];
            if (sm) return true;
        }

        return false;
    };

    while (F.size() < POPULATION_COUNT * CROSSOVER_FRACTION){
        char l, r;

        do {
            l = rand_ELITE_COUNT();
            r = rand_ELITE_COUNT();
        } while(l == r);

        std::pair<Chromosome, Chromosome> mp = G[l].pmx_crossover(G[r]);

        if (!exist(mp.first)) F.push_back(mp.first);
        if (!exist(mp.second)) F.push_back(mp.second);
    }
    while (F.size() < POPULATION_COUNT) {
        char l;
        l = rand_ELITE_COUNT();

        Chromosome m = G[l].mutation();

        if (!exist(m)) F.push_back(m);
    }

    G = F;
    for (int i = 0; i < G.size(); i++) G[i].fitness();
}

void natural_selection(){
    std::vector<Chromosome> F;

    double allsum = 0;
    for (Chromosome& c : G)
        allsum += c.fitness_score;

    std::function<int()> rand_ALLSUM = random(0, RAND_MAX - 5);

    for (int i = 0; i < ELITE_COUNT; i++){
        double accumulate = 0;
        double selected_score = (double)rand_ALLSUM() / (RAND_MAX - 4) * allsum;


        for (Chromosome& c : G) {
            if (accumulate + c.fitness_score > selected_score){
                F.push_back(c);
                break;
            }
            else accumulate += c.fitness_score;
        }
    }

    G = F;
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
            unique.insert(adjust(history[i].member[j].gene));
        }

        last_best_average += history[i].best_fitness;
        last_avg_average += history[i].average_fitness;

//        for (Chromosome c : history[i].member) {
//            unique.insert(adjust(c.gene));
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