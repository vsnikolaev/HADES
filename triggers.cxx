#include "triggers.h"

//-1 not identified| 2 e+ | 3 e- | 8 pi+ | 9 pi- | 11 k+ | 14 p | 45 d | 46 t | 47 alfa | 49 He3
void nTriggers::SetParticle(Int_t p) {
	particle = p;
}

Int_t nTriggers::GetParticle() {
	return particle;
}

//particle==-2 off
bool nTriggers::DeterminedParticle(DataTreeTrack* tr) {
	if (particle == -2) return true;
	if (tr->GetPdgId() == particle) {
		return true;
	}
	return false;
}

//to off triggers input false
bool Correct_event(DataTreeEvent* _ev, bool work) {
	if (!work) return true;
	DataTreeEvent* ev = _ev;
	if (ev->GetVertexPositionComponent(2) > 0 || ev->GetVertexPositionComponent(2) < -60)
	{
		return false;
	}
	Float_t Rx = ev->GetVertexPositionComponent(0), Ry = ev->GetVertexPositionComponent(1);
	if (sqrt(Rx*Rx + Ry*Ry) > 3)
		return false;
	if (ev->GetVertexQuality() < 0.5 || ev->GetVertexQuality() > 40)
		return false;

	if (!ev->GetTrigger(HADES_constants::kGoodVertexClust)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodVertexCand)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodSTART)->GetIsFired())
		return false;

	if (!ev->GetTrigger(HADES_constants::kNoPileUpSTART)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodSTARTVETO)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodSTARTMETA)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kNoVETO)->GetIsFired())
		return false;

	return true;
}

//to off triggers input false
bool Correct_track(DataTreeEvent* _ev, Int_t idx, bool work) {
	if (!work) return true;
	DataTreeEvent* ev = _ev;
	DataTreeTrack* tr = ev->GetVertexTrack(idx);
	DataTreeTOFHit* tof = ev->GetTOFHit(idx);
	Float_t time = tof->GetTime();
	Float_t leng = tof->GetPathLength();
	if (tr->GetDCAComponent(1) > 15)
	{
		return false;
	}
	if (leng / (time*_c) > 1)
		return 0;
	if (tof->GetPositionComponent(0) < -5 || tof->GetPositionComponent(0) > 5)
	{
		return false;
	}
	if (tof->GetPositionComponent(1) < -5 || tof->GetPositionComponent(1) > 5)
	{
		return false;
	}
	if (tr->GetChi2() > 100)
	{
		return false;
	}
	return true;
}

//to off input Correct_FWhit(m_psd,false);
bool Correct_FWhit(DataTreePSDModule* m_psd, bool work) {
	if (!work) return true;
	if (m_psd->GetEnergy() < 80)				//Oleg
		return false;
	if (m_psd->GetEnergy() > 600)				
		return false;
	if (m_psd->GetBeta() > 1.0)
		return false;
	Int_t x, y;
	x = m_psd->GetPositionComponent(0);
	y = m_psd->GetPositionComponent(1);
	if (abs(x) < 250 && abs(y) < 250) {			//first 5 rings (0-4) <240
		if (m_psd->GetBeta() < 0.84)
			return false;
	}
	else if (abs(x) < 410 && abs(y) < 410) {	//next 2 rings (5-6) <400
		if (m_psd->GetBeta() < 0.85)
			return false;
	}
	else {										//next 3 rings (7-9)
		if (m_psd->GetBeta() < 0.8)
			return false;
	}
//	Float_t t;
//	t = m_psd->GetTime();
//	if (t < 10 || t>120) 
//		return false;
	return true;
}

//base all that off
bool only_off_trigger(DataTreeEvent* _ev, Int_t a) {
	if (a == 100) return !Correct_event(_ev);
	DataTreeEvent* ev = _ev;
	Float_t Rx = ev->GetVertexPositionComponent(0), Ry = ev->GetVertexPositionComponent(1);
	switch (a) {
	case 0:
		if (!ev->GetTrigger(HADES_constants::kGoodVertexClust)->GetIsFired())
			return true;
		else return false;
	case 1:
		if (!ev->GetTrigger(HADES_constants::kGoodVertexCand)->GetIsFired())
			return true;
		else return false;
	case 2:
		if (!ev->GetTrigger(HADES_constants::kGoodSTART)->GetIsFired())
			return true;
		else return false;
	case 3:
		if (!ev->GetTrigger(HADES_constants::kNoPileUpSTART)->GetIsFired())
			return true;
		else return false;
	case 12:
		if (!ev->GetTrigger(HADES_constants::kNoVETO)->GetIsFired())
			return true;
		else return false;
	case 13:
		if (!ev->GetTrigger(HADES_constants::kGoodSTARTVETO)->GetIsFired())
			return true;
		else return false;
	case 14:
		if (!ev->GetTrigger(HADES_constants::kGoodSTARTMETA)->GetIsFired())
			return true;
		else return false;
	case 30:		//included in 1
		if (ev->GetVertexPositionComponent(2) > 0 || ev->GetVertexPositionComponent(2) < -60)
			return true;
		else return false;
	case 31:
		if (sqrt(Rx*Rx + Ry*Ry) > 3)
			return true;
		else return false;
	case 32:
		if (ev->GetVertexQuality() < 0.5 || ev->GetVertexQuality() > 40)
			return true;
		else return false;
	default:
		return false;
	}
	return false;
}

//base all that off
bool only_off_track(DataTreeEvent* _ev, Int_t idx, Int_t a) {
	if (a == 100) return !Correct_track(_ev, idx);
	DataTreeEvent* ev = _ev;
	DataTreeTrack* tr = ev->GetVertexTrack(idx);
	DataTreeTOFHit* tof = ev->GetTOFHit(idx);
	Float_t time = tof->GetTime();
	Float_t leng = tof->GetPathLength();
	switch (a) {
	case 1:
		if (tr->GetDCAComponent(1) > 15)
			return true;
		else return false;
	case 2:
		if (leng / (time*_c) > 1)
			return true;
		else return false;
	case 3:
		if (tof->GetPositionComponent(0) < -5 || tof->GetPositionComponent(0) > 5)
			return true;
		else if (tof->GetPositionComponent(1) < -5 || tof->GetPositionComponent(1) > 5)
			return true;
		else return false;
	case 4:
		if (tr->GetChi2() > 100)
			return true;
		else return false;
	default:
		return false;
	}
	return false;
}

//to off triggers input false
bool Centrality_good_event(DataTreeEvent* _ev, bool work) {
	if (!work) return true;
	DataTreeEvent* ev = _ev;
	if (!ev->GetTrigger(HADES_constants::kGoodVertexClust)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodVertexCand)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kGoodSTART)->GetIsFired())
		return false;
	if (!ev->GetTrigger(HADES_constants::kNoPileUpSTART)->GetIsFired())
		return false;
	return true;
}

//I do not remember why it is here
void read_header_file(std::vector<std::string> &_p) {
	std::ifstream in("header_file.txt", ios::in);
	if (!in) {
		_p.clear();
		return;
	}
	std::string s;
	while (!in.eof()) {
		std::getline(in, s);
		_p.push_back(s);
	}
	in.close();
}