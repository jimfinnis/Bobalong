#define RUDDER_P 5
#define RUDDER_I 0
#define RUDDER_D 0 

typedef enum {SAILING, TACKING, HOLDING} State;



typedef struct Boat{
    Coordinate location;
    
    double compass_heading;
    double gps_heading;

    double velocity;

    double wind_bearing;
    int wind_speed;
    
    int sail_angle = 0;
    int rudder_pos = 0;  
    State current_state;

} Boat;


int update(double delta, long millis){
    
    
    static const double p_coef = RUDDER_P;
    static const double i_coef = RUDDER_I;
    static const double d_coef = RUDDER_D;
    static const double i_decay = RUDDER_I_DECAY;

    static double t_0 = 1;
    static double p_0 = 0;
    
    double t = millis/1000;

    p = delta;
    i = (i+p) * t * rudder_pid.i_decay;
    d = (p - rudder_pid.p_0)/(t - rudder_pid.t_0);

    return p * rudder_pid.p_coef + i * rudder_pid.i_coef + d * rudder_pid.d_coef;
}


int update_sensors(Boat *boat);
int update_state(Boat *boat);
int act_on_state(Boat *boat);

int hold_heading(Boat *boat, int heading){
    double n_heading = heading -180;
    double n_compass_heading = boat->compas_heading - 180;

    double delta = n_heading - n_compass_heading;

        
        
}
 
int set_sail(Boat *boat){
    if (boat->wind_bearing < 0){
        boat->sail_angle =  90 - (x/2);
    } else
    if (boat->wind_bearing > 0){
        boat->sail_angle = (x/2) - 90;
    } else {
        if (boat->sail_angle < 0)
            { boat->sail_angle = -90; }
        else   
            { boat->sail_angle = 90; }
    }
    return 0;
} 
