#include "Controller.h"

Controller::Controller() {
	timePoint = std::chrono::high_resolution_clock::now();
	timePointPrevious = std::chrono::high_resolution_clock::now();
	angle = 0;
	anglePrevious = 0;
	anglePIDKp = 1;
	anglePIDKi = 0;
	anglePIDKd = 0;
	anglePIDIntegral = 0;
	anglePIDError = 0;
	speed = 0;
	speedPrevious = 0;
	speedFiltered = 0;
	speedPIDKp = 1;
	speedPIDKi = 0;
	speedPIDKd = 0;
	speedPIDIntegral = 0;
	speedPIDError = 0;
}

Controller::~Controller() {}

void Controller::restartTimer() {
	timePointPrevious = std::chrono::high_resolution_clock::now();
}

float Controller::timerValue() {
	std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(timePoint - timePointPrevious);
	return timeSpan.count();
}

float Controller::stabilityControl(float value, float setPoint, float dt) {
	// dt *= 1000;

	float error = setPoint - value;
	this->anglePIDIntegral += error * dt;

	float derivative = (error - this->anglePIDError) / dt;
	this->anglePIDError = error;

	return anglePIDKp * error + anglePIDKi * this->anglePIDIntegral + anglePIDKd * derivative;

	/*// Kd is implemented in two parts
	// The biggest one using only the input (sensor) part not the SetPoint input-input(t-2)
	// And the second using the setpoint to make it a bit more agressive setPoint-setPoint(t-1)
	float derivative = ((setPoint - setPointOld) - (value - PID_errorOld2)) / dt;
	float output = Kp * error + Kd * derivative;
	PID_errorOld2 = PID_errorOld;
	// error for Kd is only the input component
	PID_errorOld = value;
	setPointOld = setPoint;
	return output;*/
}

float Controller::speedControl(float value, float setPoint, float dt) {
	float error = setPoint - value;
	if (error > SPEED_MAX_ERROR) {
		error = SPEED_MAX_ERROR;
	} else if (error < -SPEED_MAX_ERROR) {
		error = -SPEED_MAX_ERROR;
	}

	this->speedPIDIntegral += error * dt;
	float maxIntegral = SPEED_MAX_INTEGRAL * dt;
	if (this->speedPIDIntegral > maxIntegral) {
		this->speedPIDIntegral = maxIntegral;
	} else if (this->speedPIDIntegral < -maxIntegral) {
		this->speedPIDIntegral = -maxIntegral;
	}

	float derivative = (error - this->speedPIDError) / dt;
	this->speedPIDError = error;

	return speedPIDKp * error + speedPIDKi * this->speedPIDIntegral + speedPIDKd * derivative;

	/*float error = setPoint - input;
	if (error > ITERM_MAX_ERROR) {
		error = ITERM_MAX_ERROR;
	} else if (error < -ITERM_MAX_ERROR) {
		error = -ITERM_MAX_ERROR;
	}

	PID_errorSum += error;
	if (PID_errorSum > ITERM_MAX) {
		PID_errorSum = ITERM_MAX;
	} else if (PID_errorSum < -ITERM_MAX) {
		PID_errorSum = -ITERM_MAX;
	}

	// DT is in miliseconds...
	return Kp * error + Ki * PID_errorSum * DT * 0.001;*/
}

void Controller::calculateSpeed(float angle, float speedLeft, float speedRight, int steering, int throttle, float &speedLeftNew, float &speedRightNew) {
	this->timePoint = std::chrono::high_resolution_clock::now();
	// float dt = timerValue()*1000;
	float loopTime = timerValue();
	this->timePointPrevious = this->timePoint;

	this->anglePrevious = this->angle;
	this->angle = angle;

	this->speedPrevious = this->speed;
	// Positive: forward
	this->speed = (speedLeft + speedRight) / 2;

	///TODO: decipher this, improve?
	// 90 is an empirical extracted factor to adjust for real units
	float angularVelocity = (this->angle - this->anglePrevious) * 90.0;
	// We use robot_speed(t-1) to compensate the delay
	float estimatedSpeed = -this->speedPrevious - angularVelocity;
	// low pass filter on estimated speed
	this->speedFiltered = speedFiltered * 0.95 + estimatedSpeed * 0.05;
	///TODO END

	// Speed control
	// input: user throttle
	// setPoint: estimated and filtered robot speed
	// output: target robot angle to get the desired speed
	float targetAngle = speedControl(speedFiltered, throttle, loopTime);
	if (targetAngle > MAX_ANGLE) {
		targetAngle = MAX_ANGLE;
	} else if (targetAngle < -MAX_ANGLE) {
		targetAngle = -MAX_ANGLE;
	}

	//??? We integrate the output (sumatory), so the output is really the motor acceleration, not motor speed.

	// Stability control
	// input: robot target angle(from SPEED CONTROL)
	// variable: robot angle
	// output: Motor speed
	float output = stabilityControl(angle, targetAngle, loopTime);
	if (output > MAX_OUTPUT) {
		output = MAX_OUTPUT;
	} else if (output < -MAX_OUTPUT) {
		output = -MAX_OUTPUT;
	}

	// The steering part from the user is injected directly into the output
	speedLeftNew = output + steering;
	speedRightNew = output - steering;
}

void Controller::setStabilityPID(float kp, float ki, float kd) {
	this->anglePIDKp = kp;
	this->anglePIDKi = kd;
	this->anglePIDKd = ki;
}

void Controller::setSpeedPID(float kp, float ki, float kd) {
	this->speedPIDKp = kp;
	this->speedPIDKi = kd;
	this->speedPIDKd = ki;
}
