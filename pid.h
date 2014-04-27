/*
 *  Author - David Capper
 */
 
#ifndef PID_H
#define PID_H

typedef struct pid_bundle{
    double p_coef;
    double i_coef;
    double d_coef;
    double i_decay;
} pid_bundle;


class PIDController{
    private:
        double p, i, d;
        pid_bundle coeffs;
    
    public:
        void init(pid_bundle coeffs);
        
        void update(double delta, long millis);
        double output();
};

#endif
