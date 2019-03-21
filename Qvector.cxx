#include "Qvector.h"
#include <math.h>
#include <vector>


Qvector::Qvector() {
	clear();
}

//i==1 Qx, i==2, Qy.
Float_t Qvector::GetComponent(int i) {
	if (i == 1) return Qx;
	if (i == 2) return Qy;
	else return 0;
}


void Qvector::clear() {
	Qx = Qy = 0;
	for (int i = 0; i < 2; i++) {
		Qxsubev[i] = 0;
		Qysubev[i] = 0;
	}
}
void Qvector::Recenter(Float_t _corx, Float_t _cory) {
	Qx -= _corx;
	Qy -= _cory;
}

void Qvector::FindQ(DataTreeEvent* _ev) {
	Int_t N_psd_modules = _ev->GetNPSDModules();
	DataTreePSDModule* m_psd;
	Float_t m_phi, m_charge;
	Float_t Q = 0;
	for (int i = 0; i < N_psd_modules; i++) {
		m_psd = _ev->GetPSDModule(i);
		if ( m_psd->GetId()<0 )
			continue;
		m_phi = m_psd->GetPhi();
		m_charge = m_psd->GetEnergy();
		Qx += m_charge*cos(m_phi);
		Qy += m_charge*sin(m_phi);
		Q += m_charge;
	}
	Qx = Qx / Q;
	Qy = Qy / Q;
}

Float_t Qvector::GetEventPlaneAngle() {
	return atan2(Qy, Qx);
}


void Qvector::Resolution(DataTreeEvent* _ev) {	
	Int_t N_psd_modules = _ev->GetNPSDModules();
	DataTreePSDModule* m_psd;
	std::vector<DataTreePSDModule*> my_psd_mod;
	std::vector<DataTreePSDModule*> random_subevent1;
	std::vector<DataTreePSDModule*> random_subevent2;
	Float_t m_phi, m_charge;
	Float_t Q[2] = { 0,0 };
	Float_t PsiEP[2];
	for (int i = 0; i < N_psd_modules; i++) {
		m_psd = _ev->GetPSDModule(i);
		if (m_psd->GetId()<0)
			continue;
		my_psd_mod.push_back(m_psd);
	}
	//2 rand subevents;
	N_psd_modules = my_psd_mod.size();
	for (int j = 0; j<N_psd_modules; j++) {
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
	N_psd_modules = random_subevent1.size();
	for (int i=0; i < N_psd_modules; i++){
		m_charge = random_subevent1[i]->GetEnergy();
		m_phi = random_subevent1[i]->GetPhi();
		Qxsubev[0] += m_charge * cos(m_phi);
		Qysubev[0] += m_charge * sin(m_phi);
		Q[0] += m_charge;
	}	
	N_psd_modules = random_subevent2.size();
	for (int i=0; i < N_psd_modules; i++){
		m_charge = random_subevent2[i]->GetEnergy();
		m_phi = random_subevent2[i]->GetPhi();
		Qxsubev[1] += m_charge * cos(m_phi);
		Qysubev[1] += m_charge * sin(m_phi);
		Q[1] += m_charge;
	}
	if (Q[0]!=0) { 
		Qxsubev[0] = Qxsubev[0] / Q[0];
		Qysubev[0] = Qysubev[0] / Q[0];
	}
	if (Q[1]!=0){
		Qxsubev[1] = Qxsubev[1] / Q[1];
		Qysubev[1] = Qysubev[1] / Q[1];
	}
}


void Qvector::RecenterRes(Float_t _corx, Float_t _cory){
	Qxsubev[0] -= _corx;
	Qxsubev[1] -= _corx;
	Qysubev[0] -= _cory;
	Qysubev[1] -= _cory;
}

Float_t Qvector::GetResolution(){
	return cos(2 * (atan2(Qysubev[0], Qxsubev[0]) - atan2(Qysubev[1], Qxsubev[1])));
}

//a==1 a-b (auto); else b-a;
Float_t Qvector::Getsin(Int_t a) {
	if (a==1)
		return sin(2 * ( atan2(Qysubev[0], Qxsubev[0]) - atan2(Qysubev[1], Qxsubev[1]) ) );
	else 
		return sin(2 * (atan2(Qysubev[1], Qxsubev[1]) - atan2(Qysubev[0], Qxsubev[0]) ) );
}

//a==1 first sub event, a==2 second...
Float_t Qvector::GetSubEPAngle(Int_t a) {
	if (a == 1 || a == 2)
		return atan2(Qysubev[a - 1], Qxsubev[a - 1]);
	else return -INT_MAX;
}


