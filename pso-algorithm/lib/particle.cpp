//
// Created by ChristianBudhiS on 26/08/2022.
//

#include <stdlib.h>
#include <unordered_map>
#include "pso_algorithm.h"


Particle::Particle() {}

Particle::Particle(char len) : len(len), cur_fitness_score(INT_MIN), peb_fitness_score(INT_MIN) {
    solution.resize(len);
    mask.resize(len);
    personal_best.resize(len);
    velocity.resize(len);

    for (int i = 0; i < len; i++) {
        mask[i] = randomizer();
        velocity[i] = 0;
    }

    normalize();
}

void Particle::unfit() {
    printf("unimplemented");
}

void Particle::fitness() {
    reindex_solution();
    cur_fitness_score = ::fitness(len, solution);

    if (cur_fitness_score > peb_fitness_score) {
        peb_fitness_score = cur_fitness_score;
        for (int i = 0; i < len; i++)
            personal_best[i] = mask[i];
    }
}

void Particle::normalize() {
    double MIN = mask[0];
    double MAX = mask[0];

    for (int i = 1; i < len; i++) {
        MIN = std::min(MIN, mask[i]);
        MAX = std::max(MAX, mask[i]);
    }

    for (int i = 0; i < len; i++) {
        mask[i] = (mask[i] - MIN) / (MAX - MIN);
    }
}

void Particle::reindex_solution() {
    std::vector<std::pair<double, int>> tmp_(len);

    for (int i = 0; i < len; i++) {
        tmp_[i] = {mask[i], i};
    }

    std::sort(tmp_.begin(), tmp_.end());

    for (int i = 0; i < len; i++) {
        solution[i] = tmp_[i].second;
    }
}

void Particle::move(std::vector<double> global_best, double glb_fitness_score) {
    /**
     * vi = rho * v_(i - 1) + r1 * c1 * (pbest_i - x_i) + r2 * c2 * (gbest - x_i)
     */

    for (int i = 0; i < len; i++) {
        double r1 = randomizer();
        double r2 = randomizer();
        velocity[i] = RHO * velocity[i] + r1 * C1 * (personal_best[i] - mask[i]) + r2 * C2 * (global_best[i] -
                mask[i]);

        mask[i] += velocity[i];

        //printf("%f ", velocity[i]);
    }
    //printf("\n");

    normalize();

    fitness();
}

void Particle::print() {
    printf("len : %d\n", len);
    printf("fsc : %f\n", cur_fitness_score);
    printf("solution : \n");
    for(int i = 0; i < len; i++){
        printf("%c ", solution[i] + 'A');
    }
    printf("\n");
}

std::string Particle::to_string() {
    std::string s = "";
    for (int i = 0; i < len; i++) {
        if (i > 0) s += ",";
        s += std::to_string(solution[i]);
    }
    return s;
}

bool Particle::operator < (const Particle &rhs) const {
    return cur_fitness_score < rhs.cur_fitness_score;
}
bool Particle::operator <=(const Particle &rhs) const {
    return cur_fitness_score <= rhs.cur_fitness_score;
}
bool Particle::operator > (const Particle &rhs) const {
    return cur_fitness_score > rhs.cur_fitness_score;
}
bool Particle::operator >=(const Particle &rhs) const {
    return cur_fitness_score >= rhs.cur_fitness_score;
}
bool Particle::operator ==(const Particle &rhs) const {
    return cur_fitness_score == rhs.cur_fitness_score;
}
bool Particle::operator !=(const Particle &rhs) const {
    return cur_fitness_score != rhs.cur_fitness_score;
}