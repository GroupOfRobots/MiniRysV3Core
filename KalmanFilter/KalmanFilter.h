#ifndef KALMANFILTER_H
#define KALFMANFILTER_H


struct noise{
    float Q_angle;
    float Q_biasRate;
    float R_angle;
};

struct kalmanGain{
    float K[2];    
};

struct covariance{
    float P[2][2];    
};

class KalmanFilter{
public:
    KalmanFilter();
    void EstimateAngle(float angle, float angularRate, float deltaT);
    float getEstimatedAngle();
    float getEstimatedBiasRate();

private:
    // noise variable
    noise C;
    kalmanGain gain;
    covariance cov;
    
    float angle_e,biasRate_e;
    
    // state estimation variable
    float inv,S;
};

#endif
