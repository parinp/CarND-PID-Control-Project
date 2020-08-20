#ifndef TWIDDLE_H
#define TWIDDLE_H

#include <vector>
#include <string>
#include <numeric>
#include <limits>

using std::vector;

enum State{
    INIT,
    INCREASE,
    DECREASE   
};

class Twiddle{
    public:
        /**
         * Contructor
         */

        Twiddle();
        
        Twiddle(const float &tolerance, const vector<double> &INIT_COEF, const vector<double> &DEL_INIT_COEF);

        /**
         * Destructor
         */
         
        virtual ~Twiddle();

        bool Optimize(double &count, double &cte_error,const vector<double> &params);

        vector<double> getCoefficients();

    private:
        /**
         * Private variables
         */
         
        vector<double> coef;
        vector<double> d_coef;
        int index, state_coef;
        float tolerance;
        float percent_change = 25;
        double best_count = std::numeric_limits<double>::max();
        double best_cte_error = std::numeric_limits<double>::max();
         
};

#endif //TWIDDLE_H