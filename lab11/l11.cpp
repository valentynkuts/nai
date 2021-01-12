#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <cmath>  

using namespace std;

class Neuron{
public:
    double val;
    vector<double> inputs;
    vector<double> weights;
    function<double(double)>  activationFunc;

    Neuron(double num){
          val = num;
    }

    Neuron(double num, vector<double> w,function<double(double)> func){
          val = num;
          weights = w;
          activationFunc = func;
    }

     Neuron(vector<double> i, vector<double> w,function<double(double)> func){
          inputs = i;
          weights = w;
          activationFunc = func;
    }
    
    double getVal(){
        return val;
    }

    void setVal(double v){
        val = v;
    }

    vector<double> getWeight(){
        return weights;
    }

    void setWeight(vector<double> w){
        weights = w;
    }

    void setActivationFunc(function<double(double)> f){
        activationFunc = f;
    }

    double sum(vector<double> inputs, vector<double> weights){
        double sum = 0;
        int n = inputs.size();
        for (int i = 0; i < n; i++) {
              sum += inputs.at(i)*weights.at(i);
         }
         return sum;
    }
    
    double resActivFunc(){
        double s = sum(inputs,weights);
        return activationFunc(s);
        
    }


};

int main(int argc, char **argv)
{

    auto active = [](double x) {
        double res = 1/(1+exp(x));
       return res;
    };

    vector<double> w = {1.2, 3.4};
    vector<double> i = {2.3, 4.1};

    Neuron* n = new Neuron(i, w, active);
    
    delete n;
    
    return 0;
}
