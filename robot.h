#ifndef _ROBOT_H
#define _ROBOT_H

#include <iostream>
using namespace std;
#include "Bench.h"
#include "Task.h"

//struct RPID {
//	float SetSpeed;//定义设定值
//	float ActualSpeed;//定义实际值
//	float err;//定义偏差值
//	float err_last;//定义上一个偏差值
//	float Kp, Ki, Kd;//定义比例、积分、微分系数
//	float voltage;//定义电压值(控制执行器的变量)
//	float integral;//定义积分值
//};
//void PID_init(RPID& pid);
//double PID_realize(RPID& pid, double speed);

class Robot {
private:
	int _id; // 机器人所处工作台id：-1表示无
	RPoint _pos; // 机器人当前位置
	double _rad; // 朝向rad	0：向右；Π/2：向上
	int _goodsId; // 0：表示没有1~7表示物品1~7
	double _timeCoff; // 时间系数,不挟带物体为0
	double _collisionCoff; // 碰撞系数,不挟带物体为0
	double _angleVel; // 角速度
	double _lineVelx; // 线速度
	double _lineVely; // 线速度
public:
	Robot();
	Robot(int id, double x, double y, double rad, int goodsId, double timeCoff, double collisionCoff, double angleVel, double lineVelx, double lineVely);

	int _crashState = 0;
	int _2bench = -1; // （到指定工作台去-1：没有任务）
	int _sell2WhichBench = -1; // 生产货物是卖给那个工作台的
	int _priorityTask = -1;
	TaskStack _tstack = TaskStack();
	vector<Task> _vTask = vector<Task>();
	int _radCnt = 0;
	double _lastRad = 0.;
	int _robotNum;

	void setId(int id);
	int getId();

	void setPos(RPoint p);
	RPoint getPos();

	void setGoodsId(int goodsId);
	int getGoodsId();

	double getRad();
	void setRad(double rad);

	double getTimeCoff();
	void setTimeCoff(double timeCoff);

	double getCollisionCoff();
	void setCollisionCoff(double collisionCoff);

	double getAngleVel();
	void setAngleVel(double angleVel);

	double getLineVelx();
	void setLineVelx(double lineVelx);

	double getLineVely();
	void setLineVely(double lineVely);


	~Robot();
};

#endif // !_ROBOT_H

