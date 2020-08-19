#include "twiddle.h"

#include <vector>
#include <string>
#include <numeric>
#include <limits>
#include <iostream>

Twiddle::Twiddle(){
    this->tolerance = 0.1;
    this->coef = {0.0,0.0,0.0};
    this->index = 0;
    this->d_coef = {0.1,0.1,0.1};
    this->state_coef = INIT;
}
Twiddle::~Twiddle() {}

Twiddle::Twiddle(const float &tolerance, const vector<double> &INIT_COEF,const vector<double> &DEL_INIT_COEF){
    this->tolerance = tolerance;
    this->coef = INIT_COEF;
    this->index = 0;
    this->d_coef = DEL_INIT_COEF;
    this->state_coef = INIT;
    
}

vector<double> Twiddle::getCoefficients(){

    return(coef);
}

bool Twiddle::Optimize(double &error, const vector<double> &params){

    //double error = std::numeric_limits<unsigned>::max()-err;

    //error = std::abs(error);

    switch(state_coef)
    {
        case INIT:

            coef[index] += d_coef[index];
            state_coef = INCREASE;
            return false;
        
        case INCREASE:

            if(error<best_error)
            {
                best_error = error;
                d_coef[index] *= 1+(percent_change/100);

                index = (index + 1) % coef.size();
                state_coef = INIT;  

                return false;
            }else
            {
                coef[index] -= 2*d_coef[index];
                state_coef = DECREASE;
                return false;
            }
        
        case DECREASE:

            if(error<best_error)
            {
                best_error = error;
                d_coef[index] *= 1+(percent_change/100);


                index = (index + 1) % coef.size();
                state_coef = INIT;  

                return false;

            }else
            {
                coef[index] += d_coef[index];
                d_coef[index] *= 1-(percent_change/100);
                
                index = (index+1) % coef.size();
                //state_coef = INIT;
                
                coef[index] += d_coef[index];
                state_coef = INCREASE;
                                
                return false;
            }
        
    }
    return false;

}
