#include "header.h"

Bench::Bench() {
}

Bench::Bench(int benchNum, int type, double x, double y, int productTime, int rawState, int produceState) {
	this->setPos(RPoint({ x, y }));
	this->setBenchType(type);
	this->setProductTime(productTime);
	this->setRawState(rawState);
	this->setProduceState(produceState);
	this->setBenchNum(benchNum);
}

Bench::Bench(int benchNum, int type, double x, double y) {
	this->setPos(RPoint({ x, y }));
	this->setBenchType(type);
	this->setBenchNum(benchNum);
}

int Bench::getBenchNum() {
	return this->_benchNum;
}

void Bench::setBenchNum(int benchNum) {
	this->_benchNum = benchNum;
}

void Bench::setPos(RPoint p) {
	this->_pos = p;
}

RPoint Bench::getPos() {
	return this->_pos;
}

Bench::~Bench() {
}

int Bench::getBenchType() {
	return this->_benchType;
}

void Bench::setBenchType(int benchType) {
	this->_benchType = benchType;
}

int Bench::getProductTime() {
	return this->_productTime;
}

void Bench::setProductTime(int productTime) {
	this->_productTime = productTime;
}

int Bench::getRawState() {
	return this->_rawState;
}

void Bench::setRawState(int rawState) {
	this->_rawState = rawState;
}

int Bench::getProduceState() {
	return this->_produceState;
}

void Bench::setProduceState(int produceState) {
	this->_produceState = produceState;

}