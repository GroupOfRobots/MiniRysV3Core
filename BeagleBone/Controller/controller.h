#ifndef _CONTROLLER_INCLUDED_
#define _CONTROLLER_INCLUDED_

#include <ctime>
#include <ratio>
#include <chrono>


#define MAX_THROTTLE 580
#define MAX_STEERING 150
#define MAX_TARGET_ANGLE 12

#define KP 0.19
#define KD 28
#define KP_THROTTLE 0.07
#define KI_THROTTLE 0.04

#define ITERM_MAX_ERROR 25
#define ITERM_MAX 8000

#define MAX_CONTROL_OUTPUT 500


class Controller{
private:
	std::chrono::high_resolution_clock::time_point timer_old = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point timer_value;
	float dt;

	float angle_adjusted;
	float angle_adjusted_Old;

	float Kp = KP;
	float Kd = KD;
	float Kp_thr = KP_THROTTLE;
	float Ki_thr = KI_THROTTLE;
	float PID_errorSum;
	float PID_errorOld = 0;
	float PID_errorOld2 = 0;
	float setPointOld = 0;
	float target_angle;
	float throttle;
	float steering;
	float max_throttle = MAX_THROTTLE;
	float max_steering = MAX_STEERING;
	float max_target_angle = MAX_TARGET_ANGLE;
	float control_output;

	float actual_robot_speed;
	float actual_robot_speed_Old;
	float estimated_speed_filtered;

	float stabilityPDControl(float DT, float input, float setPoint,  float Kp, float Kd);
	float speedPIControl(float DT, float input, float setPoint,  float Kp, float Ki);


public:
	void calculate_speed(float actualangle, int actualleftspeed, int actualrightspeed, int steering, int throttle, float &speedleft, float &speedright);
	float timerValue();
	void timerStart();
	void timerStop();
};

#endif
