#ifndef _BENCH_H
#define _BENCH_H

#include <iostream>
using namespace std;
#include <string>

struct RPoint {
	double x;
	double y;
};

// 工作台到原材料

class Bench {
private:
	int _benchNum;
	RPoint _pos; // 工作台坐标
	int _benchType; // 工作台类型0~8
	int _productTime; // 剩余生产时间-1表示没有生产
	int _rawState; // 原材料格状态110000拥有4、5物品
	int _produceState; // 生产格状态
public:
	Bench();
	Bench(int benchNum, int type, double x, double y);
	Bench(int benchNum, int type, double x, double y, int productTime, int rawState, int produceState);

	//int _rawState; // 原材料格状态110000拥有4、5物品
	vector<pair<int, int>> _robot_buyOrSell; // first：机器人编号（-1：无；编号：0123），second：机器人卖的货物，买为-1
	vector<int> _sell2WhichBench; // 针对345工作台应该买个6号工作台

	// 工作台虚拟状态置位
	int _rawStateGoing = 0; // 当前工作台原材料的处理状态（有机器人正在处理的状态） 
	int _productStateGoing = 0; // 当前工作台生产格的处理状态（有机器人正在处理的状态） 
	//int _rawStateGoingLast = 0;
	int _buyGoing = 0; // 当前工作台生产的物品已经被卖了（有机器人正在处理）
	vector<pair<int, int>> _vRawGoingLast; // pair first 机器人id second 原材料类型

	int getBenchNum();
	void setBenchNum(int benchNum);
	RPoint getPos();
	void setPos(RPoint p);
	int getBenchType();
	void setBenchType(int benchType);
	int getProductTime();
	void setProductTime(int productTime);
	int getRawState();
	void setRawState(int rawState);
	int getProduceState();
	void setProduceState(int produceState);

	~Bench();
};

#endif
