#pragma once

#ifndef MYQVEC
#define MYQVEC

#include <fstream>
#include <iostream>
#include <vector>
#include "HADES_constants.h"

class Subevent3 {
public:
	Float_t Qx[3];
	Float_t Qy[3];
	Subevent3();
	~Subevent3() {};
	void clear();
	Float_t Get_Psy(int a);
	Float_t Get_Res(int a);
	void Resent(Subevent3 a);
};

class Qvector {
private:
	Float_t Qx;
	Float_t Qy;
	Float_t Qxsubev[2];
	Float_t Qysubev[2];
	Subevent3 sub3;
public:
	Qvector();
	~Qvector() {};
	void clear();
	Float_t GetComponent(int i);
	void FindQ(DataTreeEvent* _ev);
	void Recenter(Float_t _corx, Float_t _cory);
	Float_t GetEventPlaneAngle();
	//2random sub
	bool Resolution(DataTreeEvent* _ev);
	void RecenterRes(Float_t _corx1, Float_t _cory1, Float_t _corx2, Float_t _cory2);
	Float_t GetResolution();
	Float_t Getsin(Int_t a = 1);
	Float_t GetSubEPAngle(Int_t a);
	//3sub
	bool Fillsub3(DataTreeEvent* _ev);
	Subevent3 Getsub3();
	Float_t Get_Psy3(int a);
	Float_t Get_Res3(int a);
	Float_t Get_cos3(int a);
	void Resentsub3(Subevent3 a);
};

Float_t Get_res2sub(Float_t a);
Float_t Get_error_res2sub(Float_t a, Float_t a_er);
Float_t Get_resolution(Float_t a, Float_t b, Float_t c, Int_t subnumber);
Float_t Get_error_resolution(Float_t a, Float_t b, Float_t c, Float_t a_er, Float_t b_er, Float_t c_er, Int_t subnumber);
Float_t Get_bessel_resolution(Float_t a);

#endif // MYQVEC
