
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

    auto arr_to_binary = [](chromosome_t arr, int size = 64) {
        unsigned long long int num = 0;

        reverse(arr.begin(), arr.end());

        for (int i = 0; i < size; i++)
        {
            if (arr[i] == 1)
            {
                num = num | (1 << i);
            }
        }

        bitset<64> b(num);
        cout << b << endl;

        return num;
    };

    auto print = [](chromosome_t &c) {
        for (auto e : c)
        {
            cout << (int)e;
        }
        cout << endl;
    };

    auto bin_to_dec = [](chromosome_t arr, int size = 64) {
        reverse(arr.begin(), arr.end());
        unsigned long long int sum = 0;
        for (int i = 0; i < size; i++)
        {
            if (arr[i] == 1)
            {
                //cout << pow(2,i) <<" " << i << endl;
                sum += pow(2, i);
            }
            //sum += arr[i]*pow(2,i);
        }
        return sum;
    };

    auto bin_to_double = [](chromosome_t arr, int size = 64) {
        reverse(arr.begin(), arr.end());
        double sum = 0;
        int c = 0;

        for (int i = -60; i < size - 60; i++)
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

    auto booth = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return pow(x + 2 * y - 7, 2.0) + pow(2 * x + y - 5, 2.0);
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

    auto selection = [](auto population, auto decode, auto slice, auto bin_to_double, auto fitness, auto goal, auto print) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, population.size() - 1);

        int size = population.size();
        cout << "pop size: " << population.size() / 2 << endl;
        vector<chromosome_t> selected_chrom(size);

        vector<double> xy(2);

        chromosome_t chrom1;
        chromosome_t chrom2;
        chromosome_t best_chrom;
        vector<chromosome_t> tournament(2);

        for (int i = 0; i < size; i++)
        {

            for (int t = 0; t < 2; t++)
            {
                tournament.push_back(population.at(distrib(gen)));
                print(tournament[i]);
            }

            chrom1 = tournament.at(0);
            chrom2 = tournament.at(1);
            //chrom1 = population.at(distrib(gen));
            print(chrom1);
            // chrom2 = population.at(distrib(gen));
            print(chrom2);

            xy = decode(chrom1, slice, bin_to_double);
            double ocena1 = fitness(goal, xy);
            cout << ocena1 << endl;

            xy = decode(chrom2, slice, bin_to_double);
            double ocena2 = fitness(goal, xy);
            cout << ocena2 << endl;

            best_chrom = ocena1 >= ocena2 ? chrom1 : chrom2;
            print(best_chrom);

            selected_chrom.push_back(best_chrom);
        }

        return selected_chrom;
    };

    // Кроссовер – скрещивание. Случайным образом выбирается точка разрыва – участок между соседними битами в строке. Обе родительские структуры разрываются на два сегмента по этой точке. Затем, соответствующие сегменты различных родителей склеиваются и получаются два генотипа потомков.

    auto crossover = [](chromosome_t parent_chrom1, chromosome_t parent_chrom2) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 127);

        int point = distrib(gen); //TODO

        for (int i = 0; i < point; i++)
        {
            swap(parent_chrom1.at(i), parent_chrom2.at(i));
        }

        vector<chromosome_t> descendants;

        descendants.push_back(parent_chrom1);
        descendants.push_back(parent_chrom2);

        return descendants;
    };
    /*
    Мутация – случайное изменение генов. Случайным образом выбранный ген с некоторой вероятностью меняется на другой.*/

    auto mutation = [](chromosome_t chromosome) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 127);

        int gen_num = distrib(gen); //TODO

        chromosome.at(gen_num) = 1 - chromosome.at(gen_num);

        chromosome.at(gen_num) = chromosome.at(gen_num) + 7; //TODO delete

        //         if(chromosome.at(gen_num) == 1){
        //             chromosome.at(gen_num) = 0;
        //         }  else{
        //             chromosome.at(gen_num) = 1;
        //         }
        return chromosome;
    };

    auto population_init = [](int pop_size, int chrom_size, auto rand_gene) {
        vector<chromosome_t> population(pop_size);
        cout << "init pop :" << population.size() << endl;
        for (int i = 0; i < pop_size; i++)
        {
            population.push_back(chromosome_t(chrom_size));
            rand_gene(population[i], chrom_size);
        }
        return population;
    };

    //----------------------------------------
    int chrom_size = 128;

    rand_gene(chromosome, chrom_size);
    X = slice(chromosome, 0, 63);
    Y = slice(chromosome, 64, 127);

    print(chromosome);
    print(X);
    print(Y);

    //      cout<< endl;
    //     double a = bin_to_double(X);
    //     double b = bin_to_double(Y);
    //
    //     cout << a <<endl;
    //     cout << b << endl;

    cout << "------------------------" << endl;

    vector<double> xy(2);
    xy = decode(chromosome, slice, bin_to_double);
    double x = xy[0];
    double y = xy[1];

    cout << "x = " << x << endl;
    cout << "y = " << y << endl;

    // cout <<"----------" << endl;

    double res_himmelblau = himmelblau(xy);
    cout << "res_himmelblau = " << res_himmelblau << endl;

    // cout <<"------------------------" << endl;

    //     double res_fitness = fitness(himmelblau,xy);
    //     cout << "res_fitness = "<< res_fitness << endl;

    cout << "------------------------" << endl;

    //double_to_bin(x);

    int lowlimit = -5;
    int uplimit = 5;
    Opt o(lowlimit, uplimit, 1000);

    o.set_limits(lowlimit, uplimit);
    auto result = o.hill_climbing_r_alg_fitness(fitness, himmelblau, xy);
    o.show(himmelblau, result, "himmelblau");

    //--------------------
    cout << "------------lab9------------" << endl;
    int size = 128;
    chromosome_t chrom1;
    chromosome_t chrom2;

    rand_gene(chrom1, size);
    rand_gene(chrom2, size);

    print(chrom1);
    //print(chrom2);

    cout << "----------mutation--------------" << endl;

    chromosome_t chrom_mut1 = mutation(chrom1);
    print(chrom_mut1);

    cout << "----------crossover-------------" << endl;
    vector<chromosome_t> descendants(2);

    descendants = crossover(chrom1, chrom2);
    cout << "parents:" << endl;
    print(chrom1);
    print(chrom2);
    cout << "descendants:" << endl;
    print(descendants.at(0));
    print(descendants.at(1));

    cout << "----------selection-------------" << endl;
    int p_size = 4;
    vector<chromosome_t> population(p_size);
    population = population_init(p_size, 128, rand_gene);
    cout << "pop size----: " << population.size() << endl;
    for (int i = 0; i < p_size; i++)
    {
        print(population[i]);

        vector<double> xy(2);
        xy = decode(population[i], slice, bin_to_double);
        double x = xy[0];
        double y = xy[1];

        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
    }
    //     cout << "selected:" << endl;
    //     vector<chromosome_t> selected(p_size);
    //     selected = selection(population, decode, slice, bin_to_double, fitness, himmelblau,print);
    //
    //      for (int i = 0; i < p_size; i++)
    //     {
    //         print(selected[i]);
    //
    //         vector<double> xy(2);
    //         xy = decode(selected[i], slice, bin_to_double);
    //         double x = xy[0];
    //         double y = xy[1];
    //
    //         cout << "x = " << x << endl;
    //         cout << "y = " << y << endl;
    //     }
}
