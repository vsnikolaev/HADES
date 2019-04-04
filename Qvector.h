#pragma once

#ifndef MYQVEC
#define MYQVEC

#include <fstream>
#include <iostream>
#include <vector>
#include "HADES_constants.h"

class Subevent {
public:
	Float_t Qx[3];
	Float_t Qy[3];
	Subevent() { Qx[0] = Qx[1] = Qx[2] = Qy[0] = Qy[1] = Qy[2] = 0; };
	~Subevent() {};
	Float_t Get_Psy(int a);
	Float_t Get_res(int a);
	void Resent(Subevent a);
};

class Qvector {
	friend std::ostream& operator<<(std::ostream &is, const Qvector &s);
	friend std::istream& operator>>(std::istream &is, Qvector &s);
private:
	Float_t Qx;
	Float_t Qy;
	Float_t Qxsubev[2];
	Float_t Qysubev[2];
	Subevent sub3;
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
	void Fillsub3(DataTreeEvent* _ev);
	Subevent Getsub3();
	Float_t Get_Psy(int a);
	Float_t Get_res(int a);
	Float_t Get_cos(int a);
	Float_t Get_subx(int a);
	void Resentsub3(Subevent a);
};

Float_t Get_resolution(Float_t a, Float_t b, Float_t c, Int_t subnumber);

#endif // MYQVEC
