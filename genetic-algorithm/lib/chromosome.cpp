//
// Created by ChristianBudhiS on 26/08/2022.
//

#include <stdlib.h>
#include <unordered_map>
#include "genetic_algorithm.h"


Chromosome::Chromosome() {}

Chromosome::Chromosome(char len) : len(len), evaluated(false), fitness_score(-1) {
    gene.resize(len);
}


void Chromosome::unfit() {
    fitness_score = -1;
    evaluated = false;
}

void Chromosome::fitness() {
    if (evaluated) return;

    fitness_score = ::fitness(len, gene);
    evaluated = true;
}

std::pair<Chromosome, Chromosome> Chromosome::pmx_crossover(Chromosome &another) {
    ASSERT(len == another.len, "Crossover Invalid [different chromosome length] %d, %d", len, another.len);

    Chromosome child_1(*this);
    Chromosome child_2(another);

    child_1.unfit();
    child_2.unfit();

    char l, r;

    l = rand_PTS_COUNT();
    r = rand_PTS_COUNT();

    if (r < l) std::swap(l, r);
    r += 1;

    std::unordered_map<DATATYPE,DATATYPE> mapper_1;
    std::unordered_map<DATATYPE,DATATYPE> mapper_2;

    for (int i = l; i < r; i++){
        mapper_1[gene[i]] = another.gene[i];
        mapper_2[another.gene[i]] = gene[i];

        child_1[i] = another.gene[i];
        child_2[i] = gene[i];
    }

    auto deep_find = [&] (int i) {
        while (mapper_2.find(child_1[i]) != mapper_2.end()) {
            child_1[i] = mapper_2[child_1[i]];
        }
        while (mapper_1.find(child_2[i]) != mapper_1.end()) {
            child_2[i] = mapper_1[child_2[i]];
        }
    };

    for (int i = 0; i < l; i++) deep_find(i);
    for (int i = r; i < len; i++) deep_find(i);

    return {child_1, child_2};
}

Chromosome Chromosome::mutation() {
    Chromosome child(*this);

    char l, r;

    l = rand_PTS_COUNT();
    r = rand_PTS_COUNT();

    if (r < l) std::swap(l, r);
    r += 1;

    for (int i = l, j = r - 1; i < j; i++, j--) {
        std::swap(child.gene[i], child.gene[j]);
    }

    return child;
}

void Chromosome::print() {
    printf("len : %d\n", len);
    printf("eva : %d\n", evaluated);
    printf("fsc : %f\n", fitness_score);
    printf("gene : \n");
    for(int i = 0; i < len; i++){
        printf("%c ", gene[i] + 'A');
    }
    printf("\n");
}

std::string Chromosome::to_string() {
    std::string s = "";
    for (int i = 0; i < len; i++) {
        if (i > 0) s += ",";
        s += std::to_string(gene[i]);
    }
    return s;
}

DATATYPE& Chromosome::operator[](int i) {
    //ASSERT(i < len && i >= 0, "Invalid reference %d %d", i, len);
    return gene[i];
}
bool Chromosome::operator < (const Chromosome &rhs) const {
    return fitness_score < rhs.fitness_score;
}
bool Chromosome::operator <=(const Chromosome &rhs) const {
    return fitness_score <= rhs.fitness_score;
}
bool Chromosome::operator > (const Chromosome &rhs) const {
    return fitness_score > rhs.fitness_score;
}
bool Chromosome::operator >=(const Chromosome &rhs) const {
    return fitness_score >= rhs.fitness_score;
}
bool Chromosome::operator ==(const Chromosome &rhs) const {
    return fitness_score == rhs.fitness_score;
}
bool Chromosome::operator !=(const Chromosome &rhs) const {
    return fitness_score != rhs.fitness_score;
}