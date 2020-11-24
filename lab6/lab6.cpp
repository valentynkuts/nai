
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

class Func
{
   public:
       virtual double calculate(vector<double> x) = 0;
};



class ThreeHumpCamel : public Func
{
    
    public:
        
        ThreeHumpCamel(){}
     
        double calculate(vector<double> d)
        {
           double x = d.at(0);
           double y = d.at(1);
           return 2*x*x - 1.05*pow(x, 4.0)+pow(x, 6.0)/6 + x*y + y*y;
        }
         
}; 

class Easom : public Func
{
  
    
    public:
        
        Easom(){}

        
        double calculate(vector<double> d)
        {
           double x = d.at(0);
           double y = d.at(1);
           return -cos(x)*cos(y)*exp(-(pow(x-M_PI,2.0)+pow(y-M_PI, 2.0)));
        }
}; 

class Sphere : public Func
{

     
    public:
        
        Sphere(){}
        
        double calculate(vector<double> x)
        {
            double sum = 0; 
            for (auto e : x)
            {
                sum += e * e;
            }
            return sum;
        }

};
//==============================================
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
        
        void show(auto func, auto solution){
            
            cout << "result: [ ";
            for (auto e : solution)
                cout << e << " ";
            cout << "] -> " << func(solution) << endl;
            
        }

};


int main()
{

auto sphere_f = [](vector<double> x) {
        double sum = 0;
        for (auto e : x)
        {
            sum += e * e;
        }
        return sum;
    };
    
auto himmelblau = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2);
};

auto thc =  [](vector<double> d) {
           double x = d.at(0);
           double y = d.at(1);
           return 2*x*x - 1.05*pow(x, 4.0)+pow(x, 6.0)/6 + x*y + y*y;
};

auto easom =  [](vector<double> d) {
           double x = d.at(0);
           double y = d.at(1);
           return -cos(x)*cos(y)*exp(-((pow(x-M_PI,2.0)+pow(y-M_PI, 2.0))));
};

auto booth =  [](vector<double> d) {
           double x = d.at(0);
           double y = d.at(1);
           return pow(x+2*y-7, 2.0) + pow(2*x+y-5, 2.0);
};


/*
Opt o0(-5,5,100000);   
auto solution = o0.hill_climbing_r_alg(himmelblau);
o0.show(himmelblau, solution);

Opt o1(-5,5,100000);   
auto solution1 = o1.hill_climbing_r_alg(thc);
o1.show(thc, solution1);

Opt o2(-100,100,100000);   
auto solution2 = o2.hill_climbing_r_alg(easom);  //TODO
o2.show(easom, solution2);

Opt o3(-5,5,100000);   
auto solution3 = o3.hill_climbing_r_alg(sphere_f);
o3.show(sphere_f, solution3);

Opt o4(-10,10,100000); 
auto solution4 = o4.hill_climbing_r_alg(booth);
o4.show(booth, solution4);
*/
//cout<< M_PI<< endl;

//cout<< ''<< endl;
Opt o(-5,5,100000);   
// auto result = o.hill_climbing_r_alg(himmelblau);
// o.show(himmelblau, result);

//cout<< ''<< endl;
auto result1 = o.hill_climbing_r_alg(thc);
o.show(thc, result1);

// o.set_limits(-100,100);   
// auto result2 = o.hill_climbing_r_alg(easom);  //TODO
// o.show(easom, result2);
// 
// o.set_limits(-5,5);   
// auto result3 = o.hill_climbing_r_alg(sphere_f);
// o.show(sphere_f, result3);
// 
// o.set_limits(-10,10);
// auto result4 = o.hill_climbing_r_alg(booth);
// o.show(booth, result4);
    
 return 0;
}
 
 
 
 
 
