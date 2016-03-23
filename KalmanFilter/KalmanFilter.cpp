#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(){
    
    C.Q_angle = 0.001;
    C.Q_biasRate = 0.003;
    C.R_angle = 0.03;
    
    cov.P[0][0] = 0; // because we can assume the initial condition of the estimation state
    cov.P[0][1] = 0; // not always zero
    cov.P[1][0] = 0; // not always zero
    cov.P[1][1] = 0; // not always zero
}

void KalmanFilter::EstimateAngle(float angle, float angularRate, float deltaT){
    
    // Angle prior estimate    
    angle_e = angle_e + deltaT*(angularRate - biasRate_e);//biasRate_e); // different approach
    //*rate = angularRate - biasRate_e;
            
    // prior error covariance
    cov.P[0][0] = cov.P[0][0] + deltaT*(deltaT*(cov.P[1][1]) - cov.P[0][1] - cov.P[1][0] + C.Q_angle);
    cov.P[0][1] = cov.P[0][1] - deltaT*(cov.P[1][1]);
    cov.P[1][0] = cov.P[1][0] - deltaT*(cov.P[1][1]);
    cov.P[1][1] = cov.P[1][1] + C.Q_biasRate*deltaT;
    
    // innovation  
    inv = angle - angle_e;
    
    // innovation covariance
    S = cov.P[0][0] + C.R_angle;
    
    // Kalman Gain
    gain.K[0] = cov.P[0][0]/S;
    gain.K[1] = cov.P[1][0]/S;
    
    // update the estimation at time k
    angle_e = angle_e + (gain.K[0])*inv;
    biasRate_e = biasRate_e + (gain.K[1])*inv;
    
    // update the error covariance
    float P00_temp = cov.P[0][0];
    float P01_temp = cov.P[0][1];
    cov.P[0][0] = cov.P[0][0] - gain.K[0]*P00_temp;
    cov.P[0][1] = cov.P[0][1] - gain.K[0]*P01_temp;
    cov.P[1][0] = cov.P[1][0] - gain.K[1]*P00_temp;
    cov.P[1][1] = cov.P[1][1] - gain.K[1]*P01_temp;     
}

float KalmanFilter::getEstimatedAngle(){
    return angle_e;
}

float KalmanFilter::getEstimatedBiasRate(){
    return biasRate_e;
}
