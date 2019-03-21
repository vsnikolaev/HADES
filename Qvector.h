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
	Float_t Qxsubev[2];
	Float_t Qysubev[2];
public:
	Qvector();
	~Qvector() {};
	Float_t GetComponent(int i);
	void clear();
	void Recenter(Float_t _corx, Float_t _cory);
	void FindQ(DataTreeEvent* _ev);
	Float_t GetEventPlaneAngle();
	void Resolution(DataTreeEvent* _ev);
	void RecenterRes(Float_t _corx, Float_t _cory);
	Float_t GetResolution();
	Float_t Getsin(Int_t a = 1);
	Float_t GetSubFlow(Int_t a = 1);
	Float_t GetSubEPAngle(Int_t a);
};

#endif // MYQVEC
