#include "Qvector.h"
#include "triggers.h"
#include <math.h>
#include <vector>

Subevent3::Subevent3() {
	clear();
}

void Subevent3::clear() {
	Qx[0] = Qx[1] = Qx[2] = Qy[0] = Qy[1] = Qy[2] = 0;
}

Float_t Subevent3::Get_Psy(int a) {
	if (a > -1 && a < 3)
		return atan2(Qy[a], Qx[a]);
	return 0;
}

Float_t Subevent3::Get_Res(int a) {
	switch (a) {
	case 0: 
		return sqrt((cos(1 * (Get_Psy(0) - Get_Psy(1))) * cos(1 * (Get_Psy(0) - Get_Psy(2)))) / cos(1 * (Get_Psy(1) - Get_Psy(2))));
	case 1:
		return sqrt((cos(1 * (Get_Psy(1) - Get_Psy(2))) * cos(1 * (Get_Psy(1) - Get_Psy(0)))) / cos(1 * (Get_Psy(2) - Get_Psy(0))));
	case 2:
		return sqrt((cos(1 * (Get_Psy(2) - Get_Psy(0))) * cos(1 * (Get_Psy(2) - Get_Psy(1)))) / cos(1 * (Get_Psy(0) - Get_Psy(1))));
	default:
		return 0;
	}
}

void Subevent3::Resent(Subevent3 _a) {
	for (int i = 0; i < 3; i++) {
		this->Qx[i] = this->Qx[i] - _a.Qx[i];
		this->Qy[i] = this->Qy[i] - _a.Qy[i];
	}
}



Qvector::Qvector() {
	clear();
}

void Qvector::clear() {
	Qx = Qy = 0;
	for (int i = 0; i < 2; i++) {
		Qxsubev[i] = 0;
		Qysubev[i] = 0;
	}
	sub3.clear();
}

//i==1 Qx, i==2, Qy.
Float_t Qvector::GetComponent(int i) {
	if (i == 1) return Qx;
	if (i == 2) return Qy;
	else return 0;
}

void Qvector::FindQ(DataTreeEvent* _ev) {
	Int_t N_psd_modules = _ev->GetNPSDModules();
	if (N_psd_modules < 5) return;
	DataTreePSDModule* m_psd;
	Float_t m_phi, m_charge;
	Float_t Q = 0;
	for (int i = 0; i < N_psd_modules; i++) {
		m_psd = _ev->GetPSDModule(i);
		if ( m_psd->GetId()<0 )
			continue;
		if (!(Correct_FWhit(m_psd)))					//Oleg
			continue;	
		m_phi = m_psd->GetPhi();
		m_charge = m_psd->GetEnergy();
		Qx += m_charge*cos(m_phi);
		Qy += m_charge*sin(m_phi);
		Q += m_charge;
	}
	if (Q > 0) {
		Qx = Qx / Q;
		Qy = Qy / Q;
	}
	if (Q == 0 || (Q != Q)) {
		Qx = -999;
		Qy = -999;
	}
}

void Qvector::Recenter(Float_t _corx, Float_t _cory) {
	Qx -= _corx;
	Qy -= _cory;
}

Float_t Qvector::GetEventPlaneAngle() {
	return atan2(Qy, Qx);
}

//true if correct, false if error occurred
bool Qvector::Resolution(DataTreeEvent* _ev) {
	Int_t N_psd_modules = _ev->GetNPSDModules();
	if (N_psd_modules < 5) return false;
	DataTreePSDModule* m_psd;
	std::vector<DataTreePSDModule*> my_psd_mod;
	std::vector<DataTreePSDModule*> random_subevent1;
	std::vector<DataTreePSDModule*> random_subevent2;
	Float_t m_phi, m_charge;
	Float_t Q[2] = { 0,0 };
	for (int i = 0; i < N_psd_modules; i++) {
		m_psd = _ev->GetPSDModule(i);
		if (m_psd->GetId() < 0)
			continue;
		if (!(Correct_FWhit(m_psd)))					//Oleg
			continue;
		my_psd_mod.push_back(m_psd);
	}
	//2 rand subevents;
	N_psd_modules = my_psd_mod.size();
	if (N_psd_modules < 4) return false;
	for (int j = 0; j < N_psd_modules; j++) {
		if (rand() % 2) random_subevent1.push_back(my_psd_mod[j]);
		else random_subevent2.push_back(my_psd_mod[j]);
	}
	while (random_subevent1.size() != my_psd_mod.size() / 2 && random_subevent2.size() != my_psd_mod.size() / 2) {
		Int_t a;
		if (random_subevent1.size() > random_subevent2.size()) {
			a = rand() % random_subevent1.size();
			random_subevent2.push_back(random_subevent1[a]);
			random_subevent1.erase(random_subevent1.begin() + a);
		}
		else {
			a = rand() % random_subevent2.size();
			random_subevent1.push_back(random_subevent2[a]);
			random_subevent2.erase(random_subevent2.begin() + a);
		}
	}
	//we have 2 random subevents.
	for (std::vector<DataTreePSDModule*>::iterator it = random_subevent1.begin(); it != random_subevent1.end(); it++) {
		m_psd	 = *it;
		m_charge = m_psd->GetEnergy();
		m_phi	 = m_psd->GetPhi();
		Qxsubev[0] += m_charge * cos(m_phi);
		Qysubev[0] += m_charge * sin(m_phi);
		Q[0]	   += m_charge;
	}
	for (std::vector<DataTreePSDModule*>::iterator it = random_subevent2.begin(); it != random_subevent2.end(); it++) {
		m_psd	 = *it;
		m_charge = m_psd->GetEnergy();
		m_phi	 = m_psd->GetPhi();
		Qxsubev[1] += m_charge * cos(m_phi);
		Qysubev[1] += m_charge * sin(m_phi);
		Q[1]	   += m_charge;
	}
	Qxsubev[0] = Qxsubev[0] / Q[0];
	Qysubev[0] = Qysubev[0] / Q[0];
	Qxsubev[1] = Qxsubev[1] / Q[1];
	Qysubev[1] = Qysubev[1] / Q[1];
	return true;
}

void Qvector::RecenterRes(Float_t _corx1, Float_t _cory1, Float_t _corx2, Float_t _cory2){
	Qxsubev[0] -= _corx1;
	Qxsubev[1] -= _corx2;
	Qysubev[0] -= _cory1;
	Qysubev[1] -= _cory2;
}

Float_t Qvector::GetResolution(){
	return cos(1 * (this->GetSubEPAngle(1) - this->GetSubEPAngle(2)));
}

//a==1 a-b (auto); else b-a; function not used
Float_t Qvector::Getsin(Int_t a) {
	if (a==1)
		return sin(2 * (this->GetSubEPAngle(1) - this->GetSubEPAngle(2)) );
	else 
		return sin(2 * (this->GetSubEPAngle(2) - this->GetSubEPAngle(1)) );
}

//a==1 first sub event, a==2 second...
Float_t Qvector::GetSubEPAngle(Int_t a) {
	if (a == 1 || a == 2)
		return atan2(Qysubev[a - 1], Qxsubev[a - 1]);
	else return 0;
}


//true if correct, false if error occured
bool Qvector::Fillsub3(DataTreeEvent* _ev) {
	Int_t N_psd_modules = _ev->GetNPSDModules();
	if (N_psd_modules < 8) return false;
	DataTreePSDModule* m_psd;
	Int_t x, y;
	Int_t idx;
	Subevent3 cur;
	Float_t m_phi, m_charge;
	Float_t Q[3] = { 0,0,0 };
	Int_t count[3] = { 0,0,0 };
	for (int i = 0; i < N_psd_modules; i++) {
		m_psd = _ev->GetPSDModule(i);
		if (m_psd->GetId()<0)
			continue;
		if (!(Correct_FWhit(m_psd)))					//Oleg
			continue;
		m_phi = m_psd->GetPhi();
		m_charge = m_psd->GetEnergy();
		x = m_psd->GetPositionComponent(0);
		y = m_psd->GetPositionComponent(1);
		if (abs(x) < 250 && abs(y) < 250) {			//first 5 rings (0-4) <240
			idx = 0;
		}
		else if (abs(x) < 410 && abs(y) < 410) {	//next 2 rings (5-6) <400
			idx = 1;
		}
		else {										//next 3 rings (7-9)
			idx = 2;
		}
		cur.Qx[idx] += m_charge*cos(m_phi);
		cur.Qy[idx] += m_charge*sin(m_phi);
		Q[idx] += m_charge;
		count[idx]++;
	}
	for (int j = 0; j < 3; j++) {
		if (count[j] < 1) {
			return false;
		}
		cur.Qx[j] = cur.Qx[j] / Q[j];
		cur.Qy[j] = cur.Qy[j] / Q[j];
							
	}
	this->sub3 = cur;
	return true;
}

Float_t Qvector::Get_cos3(int a) {
	switch (a)
	{
	case 0: return cos(1 * (Get_Psy3(0) - Get_Psy3(1)));
		break;
	case 1: return cos(1 * (Get_Psy3(0) - Get_Psy3(2)));
		break;
	case 2: return cos(1 * (Get_Psy3(1) - Get_Psy3(2)));
		break;
	default:
		break;
	}
	return 0;
}

Subevent3 Qvector::Getsub3() {
	return this->sub3;
}

Float_t Qvector::Get_Psy3(int a) {
	return this->sub3.Get_Psy(a);
}

Float_t Qvector::Get_Res3(int a) {
	return this->sub3.Get_Res(a);
}

void Qvector::Resentsub3(Subevent3 a) {
	this->sub3.Resent(a);
}



//Solo

Float_t Get_res2sub(Float_t a) {
	return sqrt(2*a);
}

Float_t Get_error_res2sub(Float_t a, Float_t a_er) {
	return sqrt(1.0 / (4.0 * a)) * a_er;
}

//input <cos(a)>, <cos(b)>, <cos(c)>, and subevent number;  
Float_t Get_resolution(Float_t a, Float_t b, Float_t c, Int_t subnumber) {
	switch (subnumber)
	{
	case 0: return sqrt((a * b) / c);
	case 1: return sqrt((c * a) / b);
	case 2: return sqrt((b * c) / a);
	default:
		return 0;
	}
}

//input <cos(a)>, <cos(b)>, <cos(c)>, errors a,b,c, and subevent number;  
Float_t Get_error_resolution(Float_t a, Float_t b, Float_t c, Float_t a_er, Float_t b_er, Float_t c_er, Int_t subnumber) {
	switch (subnumber)
	{
	case 0: return 1.0 / 2.0 * sqrt((a_er*a_er * b / (a*c)) + (b_er*b_er * a / (b*c)) + (c_er*c_er * a*b / pow(c, 3)));
	case 1: return 1.0 / 2.0 * sqrt((a_er*a_er * c / (a*b)) + (c_er*c_er * a / (b*c)) + (b_er*b_er * a*c / pow(b, 3)));
	case 2: return 1.0 / 2.0 * sqrt((c_er*c_er * b / (a*c)) + (b_er*b_er * c / (a*b)) + (a_er*a_er * b*c / pow(a, 3)));
	default:
		return 0;
	}
}

//input <cos(km(psia-psib)>
Float_t Get_bessel_resolution(Float_t a) {
	//if (a<0.4) return Get_res2sub(a);
	Float_t hi = 0;
	Float_t resolution;
	Float_t err = a / 100;
	for (; hi < 3; hi += 0.0001) {
		resolution = 0.626657*hi - 0.09694*pow(hi, 3) + 0.02754*pow(hi, 4) - 0.002283*pow(hi, 5);		//k==1, v1
		//resolution = 0.25*pow(hi, 2) - 0.011414*pow(hi, 3) - 0.034726*pow(hi, 4) + 0.006815*pow(hi, 5);	//k==2, v2
		if (abs(resolution - a) < err)
			break;
	}
	//std::cerr << hi << std::endl;
	if (hi >= 3) return Get_res2sub(a);
	hi = hi*sqrt(2);
	resolution = 0.626657*hi - 0.09694*pow(hi, 3) + 0.02754*pow(hi, 4) - 0.002283*pow(hi, 5);		//k==1, v1
	//resolution = 0.25*pow(hi, 2) - 0.011414*pow(hi, 3) - 0.034726*pow(hi, 4) + 0.006815*pow(hi, 5);	//k==2, v2
	resolution = sqrt(resolution);
	return resolution;
}	