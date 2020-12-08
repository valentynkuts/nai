#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

class Opt
{
  
    public:
        int lowlimit;
        int uplimit;
        int max_iterations;
        
        Opt(){}
        Opt( int low, int up, int num_iter){
            this->lowlimit = low;
            this->uplimit = up;
            this->max_iterations = num_iter;
            //return this;
        }
        
        void set_limits(int lowLimit, int upLimit)
        {
            this->lowlimit = lowLimit;
            this->uplimit = upLimit;
        }
        
        void set_iterations(int num_iter)
        {
            this->max_iterations = num_iter;
        }
       
         vector<vector<double>> neighbours(vector<double> x, double dx = 0.001)
         {
                vector<vector<double>> ret;
                for (int i = 0; i < x.size(); i++)
                {
                    auto nx = x;
                    nx[i] += dx;
                    ret.push_back(nx);
                    nx[i] -= 2.0 * dx;
                    ret.push_back(nx);
                }
                return ret;
         }
       
        
        vector<double> random(){
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_real_distribution<double> uniform_dist(lowlimit, uplimit);
            vector<double> x(2);
            x[0] = uniform_dist(e1);
            x[1] = uniform_dist(e1);
            return x;
        }
        

        vector<double> hill_climbing_alg (auto func) {
            auto current_solution = random();     
            for (int iteration = 0; iteration < max_iterations; iteration++)
            {
                auto next_solutions = neighbours(current_solution);
                next_solutions.push_back(current_solution);
                current_solution = *(max_element(next_solutions.begin(), next_solutions.end(), [&func](auto a, auto b) {
                    return func(a) > func(b);
            }));
            
           }
           return current_solution;
        }
        
        void debug_print(vector<double> v, auto func) {
            cout << v.at(0) << " " << v.at(1) << " " << func(v) << endl;
       };
       
       void debug_print1(int i, vector<double> v, auto func) {         //TODO
            int stepprint = max_iterations / 100;
            //if ((i % stepprint) == 0)
                cout << i << " " << func(v) << endl;
       };
    
    
        vector<double>  hill_climbing_r_alg(auto func) {
            std::random_device r;
            std::default_random_engine e1(r());
        
            auto current_solution = random();
            for (int iteration = 0; iteration < max_iterations; iteration++)
            {
                auto next_solutions = neighbours(current_solution);
                uniform_int_distribution<int> uniform_dist(0, next_solutions.size()-1);
                auto next_sol = next_solutions.at(uniform_dist(e1));
                if (func(current_solution) > func(next_sol)) {
                    current_solution = next_sol;
                }
                
               // debug_print(current_solution, func);
                debug_print1(iteration, current_solution, func);
            }
            return current_solution;
        };
        
        
        vector<double>  hill_climbing_r_alg_fitness(auto fitness,auto goal, vector<double> xy) {
            std::random_device r;
            std::default_random_engine e1(r());
      
            auto current_solution = xy;

            for (int iteration = 0; iteration < max_iterations; iteration++)
            {
                auto next_solutions = neighbours(current_solution);
                uniform_int_distribution<int> uniform_dist(0, next_solutions.size()-1);
                auto next_sol = next_solutions.at(uniform_dist(e1));
                if (fitness(goal,current_solution) > fitness(goal,next_sol)) {
                    current_solution = next_sol;
                }
            
            }
            return current_solution;
        };
        
        void show(auto func, auto solution, string str){
            cout<< str<<endl;
            cout << "result: [ ";
            for (auto e : solution)
                cout << e << " ";
            cout << "] -> " << func(solution) << endl;
            
        }

};
 
