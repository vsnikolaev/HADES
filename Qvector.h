#pragma once

#ifndef MYQVEC
#define MYQVEC

#include <fstream>
#include <iostream>
#include <vector>
#include "HADES_constants.h"

class Qvector {
	friend std::ostream& operator<<(std::ostream &is, const Qvector &s);
	friend std::istream& operator>>(std::istream &is, Qvector &s);
private:
	Float_t Qx;
	Float_t Qy;
	Float_t Qxrecent[2];
	Float_t Qyrecent[2];
public:
	Qvector();
	~Qvector() {};
	Float_t GetComponent(int i);
	void clear();
	void Recenter(Float_t _corx, Float_t _cory);
	void findQ(DataTreeEvent* _ev);
	Float_t GetEventPlaneAngle();
	void Resolution(DataTreeEvent* _ev);
	void RecenterRes(Float_t _corx, Float_t _cory);
	Float_t GetResolution();
};

#endif // MYQVEC
