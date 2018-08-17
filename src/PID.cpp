#include "PID.h"
#include <iostream>
#include <vector>
#include "math.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    // Initialize PID coefficients
    Kp_ = Kp;
    Ki_ = Ki;
    Kd_ = Kd;
    p_error_ = 0.0;
    i_error_ = 0.0;
    d_error_ = 0.0;
    last_cte_ = 0.0;

}

void PID::TwiddleInit(vector<double> coefficients, vector<double> deltas, 
                      double error_threshold, int train_steps, int max_steps)
{
    if (coefficients.size() != 3 || deltas.size() != 3){
        cout << "Error: Improper vector size. "  
             << "Coefficients and deltas must both contain exactly 3 values!"
             << endl;
    }

    // Set values for Twiddle
    coeffs_ = coefficients;
    deltas_ = deltas;
    err_thresh_ = error_threshold;
    train_steps_ = train_steps;
    max_steps_ = max_steps;
    curr_step_ = 0;
    p_idx_ = 0;
    added_ = false;
    subtracted_ = false;
    Kp_ = coefficients[0];
    Ki_ = coefficients[1];
    Kd_ = coefficients[2]; 
    best_error_ = 100000;
    sum_d_ = deltas_[0] + deltas_[1] + deltas_[2];
    
}

void PID::UpdateError(double cte) {
    
    //Update the error
    p_error_ = cte;
    i_error_ += cte;
    d_error_ = cte - last_cte_;

    last_cte_ = cte;
}

bool PID::TwiddleUpdate(double cte){

    // Check to see if we're done training
    if ( curr_step_ == max_steps_ || sum_d_ < 0.00001 || best_error_ < err_thresh_){
        std::cout << "TRAINING FINISHED" << std::endl;
        return false;
    }

    // Perform PID update step
    UpdateError(cte);

    // Update Error each step
    training_error_ += sqrt(cte*cte);
    

    // If we're done with this set of training steps...
    if(curr_step_ % train_steps_ == 0){
        std::cout << "Training for param " << p_idx_ << " --------------------" << std::endl;
        
        // If we improved...
        if(training_error_ < best_error_){
            std::cout << "Error Decreased by: " << best_error_ - training_error_ << std::endl;
            
            // Update Error
            best_error_ = training_error_;
            
            // Increase Delta
            deltas_[p_idx_] *= 1.1;
            
            // Reset Controls
            added_ = false;
            subtracted_ = false;
            p_idx_ += 1;
            p_idx_ %= 3;
            std::cout << "Moving on to Parameter " << p_idx_ << std::endl; 
        }
        else{ // If we did not improve...
            std::cout << "Error Increased by: " << training_error_ - best_error_ << std::endl;
            // Decrease Delta
            deltas_[p_idx_] *= 0.9;
        }
            
        // If we didn't add yet...
        if(!added_){
            coeffs_[p_idx_] += deltas_[p_idx_];
            added_ = true;
        }
        // If we didn't subtract yet...
        else if(added_ && !subtracted_){
            coeffs_[p_idx_] -= 2.0 * deltas_[p_idx_];
            subtracted_ = true;
        }
        // We added and subtracted...
        else{
            coeffs_[p_idx_] += deltas_[p_idx_];
            deltas_[p_idx_] *= 0.9;
            p_idx_ += 1;
            p_idx_ %= 3;
            std::cout << "Moving on to Parameter " << p_idx_ << std::endl;
            added_ = false;
            subtracted_ = false;
        }

        // Reset Training Error
        training_error_ = 0.0;
        sum_d_ = deltas_[0] + deltas_[1] + deltas_[2];
        Kp_ = coeffs_[0];
        Ki_ = coeffs_[1];
        Kd_ = coeffs_[2]; 
        std::cout << "Current Values:" << std::endl;
        std::cout << "[" << Kp_ << ", " << Ki_ << ", " << Kd_ << "]" << std::endl;
    }
    
    // Update step count
    curr_step_++;


    return true;
}

double PID::TotalError() {
    // Return the output of the PID step
    return Kp_ * p_error_ + Ki_ * i_error_ + Kd_ * d_error_;
}

