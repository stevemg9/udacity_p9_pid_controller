#ifndef PID_H
#define PID_H

#include <vector>

using std::vector;

class PID {

public:

    // Errors
    double p_error_;
    double i_error_;
    double d_error_;


    // Coefficients
    double Kp_;
    double Ki_;
    double Kd_;

    // CTE tracking
    double last_cte_;
    double sum_cte_;

    // Twiddle Params
    vector<double> coeffs_;
    vector<double> deltas_;
    double sum_d_;
    double err_thresh_;
    double training_error_;
    double best_error_;
    int train_steps_;
    int max_steps_;
    int curr_step_;
    bool added_;
    bool subtracted_;
    int p_idx_;

    // Constructor
    PID();

    // Destructor.
    virtual ~PID();
    
    // Initialize PID.
    void Init(double Kp, double Ki, double Kd);

    // Twiddle
    void TwiddleInit(vector<double> coefficients, vector<double> deltas, 
                      double error_threshold, int train_steps, int max_steps);

    // Update the PID error variables given cross track error
    void UpdateError(double cte);

    // Update for twiddle
    bool TwiddleUpdate(double cte);
    
    // Calculate the total PID error.
    double TotalError();
};

#endif /* PID_H */
