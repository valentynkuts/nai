 


#include <functional>
#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <time.h>
#include <cmath>
#include "opt.cpp"

using namespace std;

typedef vector<char> chromosome_t;

int main()
{

    srand(time(0));
    
    std::random_device rd;
    std::mt19937 gen(rd());

    chromosome_t chromosome;
    chromosome_t X;
    chromosome_t Y;

    auto rand_gene = [](chromosome_t &chromosome, int size) {
        int gene;
        for (int i = 0; i < size; i++)
        {
            gene = rand() % 2;
            chromosome.push_back(gene);
        }
    };

    auto slice = [](chromosome_t &arr, int S, int E) {
        auto start = arr.begin() + S;
        auto end = arr.begin() + E + 1;

        chromosome_t sliced_vec(E - S + 1);

        copy(start, end, sliced_vec.begin());

        return sliced_vec;
    };

    auto print = [](chromosome_t &c) {
        for (auto e : c)
        {
            cout << (int)e;
        }
        cout << endl;
    };

    auto print_pop = [](auto population, auto print, auto decode, auto slice, auto bin_to_double) {
        int p_size = population.size();
        for (int i = 0; i < p_size; i++)
        {
            print(population[i]);

            vector<double> xy(2);
            xy = decode(population[i], slice, bin_to_double);
            double x = xy[0];
            double y = xy[1];

            //         cout << "x = " << x << endl;
            //         cout << "y = " << y << endl;

            cout << "x = " << x << "    "
                 << "y = " << y << endl;
        }
    };

    auto bin_to_double = [](chromosome_t arr, int size = 64) {
        reverse(arr.begin(), arr.end());
        double sum = 0;
        int c = 0;

        for (int i = -62; i < size - 62; i++)
        {

            if (c < size)
            {
                sum += arr[c] * pow(2, i);
                c++;
            }
        }
        return sum;
    };

    auto decode = [](chromosome_t &chromosome, auto slice, auto bin_to_double) {
        vector<double> arr_xy(2);
        chromosome_t X = slice(chromosome, 0, 63);
        chromosome_t Y = slice(chromosome, 64, 127);

        arr_xy[0] = bin_to_double(X); //x
        arr_xy[1] = bin_to_double(Y); //y

        return arr_xy;
    };

    auto himmelblau = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2);
    };

    auto fitness = [](auto goal, vector<double> xy) {
        //double x = xy.at(0);
        //double y = xy.at(1);
        double res = 1 / (1 + abs(goal(xy)));

        return res;
    };

    auto double_to_bin = [](double n) {
        double param, fractpart, intpart;

        //param = 3.14159265;
        param = n;
        fractpart = modf(param, &intpart);
        //printf ("%f = %f + %f \n", param, intpart, fractpart);
        cout << "param = " << intpart << " + " << fractpart << endl;
    };

    //     Селекция – выбор особей с наилучшей приспособленностью для воспроизводства (сортировка по значению целевой функции). Чем лучше приспособленность особи, тем выше ее шансы на скрещивание и наследование ее генов следующим поколением.

    auto selection = [&gen](auto population, auto decode, auto slice, auto bin_to_double, auto fitness, auto goal) {
        
        std::uniform_int_distribution<> distrib(0, population.size() - 1);

        int size = population.size();
        //cout << "pop size: " << population.size() << endl;
        vector<chromosome_t> selected_chrom;

        vector<double> xy(2);

        chromosome_t chrom1;
        chromosome_t chrom2;
        chromosome_t best_chrom;
        //vector<chromosome_t> tournament(2);
        vector<chromosome_t> tournament;
        double ocena_best;

        for (int i = 0; i < size; i++)
        {

            chrom1 = population.at(distrib(gen));
            //             cout << "chrom1: " << endl;
            //             print(chrom1);
            chrom2 = population.at(distrib(gen));
            //             cout << "chrom2: " << endl;
            //             print(chrom2);

            xy = decode(chrom1, slice, bin_to_double);
            double ocena1 = fitness(goal, xy);
            //cout << "ocena1: " << ocena1 << endl;

            xy = decode(chrom2, slice, bin_to_double);
            double ocena2 = fitness(goal, xy);
            //cout << "ocena2: " << ocena2 << endl;

            best_chrom = ocena1 >= ocena2 ? chrom1 : chrom2;
            ocena_best = ocena1 >= ocena2 ? ocena1 : ocena2;
            //             cout << "--best chromosome--" << ocena_best << endl;
            //             print(best_chrom);

            selected_chrom.push_back(best_chrom);
            //cout << "===================" << endl;
        }

        return selected_chrom;
    };

    auto crossover_two = [&gen](chromosome_t parent_chrom1, chromosome_t parent_chrom2) {
        
        std::uniform_int_distribution<> distrib(0, 127);

        int point = distrib(gen); 

        for (int i = 0; i < point; i++)
        {
            swap(parent_chrom1.at(i), parent_chrom2.at(i));
        }

        vector<chromosome_t> descendants;

        descendants.push_back(parent_chrom1);
        descendants.push_back(parent_chrom2);

        return descendants;
    };

    auto crossover = [](auto population, auto crossover_two) {
        int size = population.size();
        vector<chromosome_t> crossover_pop;
        //decltype(population) crossover_pop;

        for (int i = 0; i < size - 1; i += 2)
        {
            auto parent1 = population.at(i);
            auto parent2 = population.at(i + 1);

            vector<chromosome_t> descendants(2);

            descendants = crossover_two(parent1, parent2);

            crossover_pop.push_back(descendants.at(0));
            crossover_pop.push_back(descendants.at(1));
        }
        return crossover_pop;
    };

    auto mutation_one = [&gen](chromosome_t chromosome) {
        
        std::uniform_int_distribution<> distrib(0, 127);

        int gen_num = distrib(gen);

        chromosome.at(gen_num) = 1 - chromosome.at(gen_num);


        return chromosome;
    };

    auto mutation = [](auto population, auto mutation_one) {
        int size = population.size();
        vector<chromosome_t> mut_population;
        //decltype(population) mut_population;
        chromosome_t chrom_after_mut;
        for (int i = 0; i < size; i++)
        {
            chrom_after_mut = mutation_one(population.at(i));
            mut_population.push_back(chrom_after_mut);
        }

        return mut_population;
    };

    auto population_init = [](int pop_size, int chrom_size, auto rand_gene) {
        vector<chromosome_t> population(pop_size);
        cout << "init pop :" << population.size() << endl;
        for (int i = 0; i < pop_size; i++)
        {
            //population.push_back(chromosome_t(chrom_size)); //TODO delete
            rand_gene(population[i], chrom_size);
        }
        return population;
    };

    
    auto term_condition = [](int &iterations_max) {
        std::cout << "[" << iterations_max << "] ";
        bool flag;
        iterations_max--;
        flag = (iterations_max > 0) ? false : true;
        //         if (iterations_max > 0)
        //             return false;
        //         else
        //             return true;
        return flag;
    };

    auto ocena = [](auto population, auto decode, auto slice, auto bin_to_double, auto fitness, auto goal) {
        int size = population.size();

        vector<double> xy(2);
        double ocena;
        vector<double> oceny;

        for (int i = 0; i < size; i++)
        {

            xy = decode(population.at(i), slice, bin_to_double);
            ocena = fitness(goal, xy);
            //cout << ocena << endl;
            oceny.push_back(ocena);
        }

        return oceny;
    };

    auto print_oceny = [](auto oceny) {
        int size = oceny.size();
        for (int i = 0; i < size; i++)
        {
            cout <<  "  " << oceny.at(i);
        }
        cout << endl;
    };

    //cout << "----------selection-------------" << endl;
    int p_size = 4;
    vector<chromosome_t> population1(p_size);
    population1 = population_init(p_size, 128, rand_gene);

    int pop_size = population1.size();

    //cout << "pop_size: " << pop_size << endl;

    cout << "----------origin-------------" << endl;
    print_pop(population1, print, decode, slice, bin_to_double);
    auto oceny_start = ocena(population1, decode, slice, bin_to_double, fitness, himmelblau);
    cout << "oceny_start:  ";
    print_oceny(oceny_start);


    cout << "---------GA--------------" << endl;

    int iterations_max = 10;

    int size_pop = 4;
    vector<chromosome_t> population(size_pop);
    population = population_init(size_pop, 128, rand_gene);

    
    auto genetic_algorithm = []( auto population,
                                 auto selection,
                                 auto crossover,
                                 auto crossover_two,
                                 auto mutation,
                                 auto mutation_one,
                                 auto term_condition,
                                 auto iterations_max,
                                 auto decode, 
                                 auto slice, 
                                 auto bin_to_double, 
                                 auto fitness, 
                                 auto himmelblau,
                                 auto ocena){
        
            int size_pop = population.size();
            vector<chromosome_t> selected_pop(size_pop);
            vector<chromosome_t> crossover_pop(size_pop);
            vector<chromosome_t> mut_population(size_pop);
            vector<chromosome_t> new_pop(size_pop);

            while (!term_condition(iterations_max))
            {
                selected_pop = selection(population, decode, slice, bin_to_double, fitness, himmelblau);

                crossover_pop = crossover(selected_pop, crossover_two);

                mut_population = mutation(crossover_pop, mutation_one);

                new_pop = mut_population;
            }
            
            auto best = ocena(new_pop, decode, slice, bin_to_double, fitness, himmelblau);
            
            sort(best.begin(), best.end(), [](auto a, auto b) { return a > b; });
                        
    
            return best[0];
    };

    cout << "----------new population--------------" << endl;
    
    double best = genetic_algorithm(population,
                                selection,
                                crossover,
                                crossover_two,
                                mutation,
                                mutation_one,
                                term_condition,
                                iterations_max,
                                decode, 
                                slice, 
                                bin_to_double, 
                                fitness, 
                                himmelblau,
                                ocena);
    

    cout << best << endl;
}
