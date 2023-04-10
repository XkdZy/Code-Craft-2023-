#include "header.h"

Robot::Robot() {
}

Robot::Robot(int id, double x, double y, double rad, int goodsId, double timeCoff, double collisionCoff, double angleVel, double lineVelx, double lineVely) {
	this->setPos(RPoint({ x, y }));
	this->setId(id);
	this->setRad(rad);
	this->setGoodsId(goodsId);
	this->setTimeCoff(timeCoff);
	this->setCollisionCoff(collisionCoff);
	this->setAngleVel(angleVel);
	this->setLineVelx(lineVelx);
	this->setLineVely(lineVely);
}

void Robot::setPos(RPoint p) {
	this->_pos = p;
}

RPoint Robot::getPos() {
	return this->_pos;
}

void Robot::setId(int id) {
	this->_id = id;
}

int Robot::getId() {
	return this->_id;
}

double Robot::getRad() {
	return this->_rad;
}
void Robot::setRad(double rad) {
	this->_rad = rad;
}

void Robot::setGoodsId(int goodsId) {
	this->_goodsId = goodsId;
}

int Robot::getGoodsId() {
	return _goodsId;
}

double Robot::getTimeCoff() {
	return this->_timeCoff;
}

void Robot::setTimeCoff(double timeCoff) {
	this->_timeCoff = timeCoff;
}

double Robot::getCollisionCoff() {
	return this->_collisionCoff;
}

void Robot::setCollisionCoff(double collisionCoff) {
	this->_collisionCoff = collisionCoff;
}

double Robot::getAngleVel() {
	return this->_angleVel;
}

void Robot::setAngleVel(double angleVel) {
	this->_angleVel = angleVel;
}

double Robot::getLineVelx() {
	return this->_lineVelx;
}

void Robot::setLineVelx(double lineVelx) {
	this->_lineVelx = lineVelx;
}

double Robot::getLineVely() {
	return this->_lineVely;
}

void Robot::setLineVely(double lineVely) {
	this->_lineVely = lineVely;
}

Robot::~Robot() {
}

//void PID_init(RPID& pid) {
//	pid.SetSpeed = 0.0;
//	pid.ActualSpeed = 0.0;
//	pid.err = 0.0;
//	pid.err_last = 0.0;
//	pid.voltage = 0.0;
//	pid.integral = 0.0;
//	pid.Kp = 0.2;
//	//pid.Ki = 0.015;
//	pid.Ki = 0.0;
//	pid.Kd = 0.2;
//}
//
//double PID_realize(RPID& pid, double speed) {
//	pid.SetSpeed = speed;
//	pid.err = pid.SetSpeed - pid.ActualSpeed;
//	pid.integral += pid.err;
//	pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
//	pid.err_last = pid.err;
//	pid.ActualSpeed = pid.voltage * 1.0;
//	return pid.ActualSpeed;
//}