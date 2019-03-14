#include "triggers.h"

//to off triggers input false
bool Correct_event(DataTreeEvent* _ev, bool work) {
	if (!work) return 1;
	DataTreeEvent* ev = _ev;
	if (ev->GetVertexPositionComponent(2) > 0 || ev->GetVertexPositionComponent(2) < -60)
	{
		return 0;
	}
	Float_t Rx = ev->GetVertexPositionComponent(0), Ry = ev->GetVertexPositionComponent(1);
	if (sqrt(Rx*Rx + Ry*Ry) > 3)
		return 0;
	if (ev->GetVertexQuality() < 0.5 || ev->GetVertexQuality() > 40)
		return 0;

	if (!ev->GetTrigger(HADES_constants::kGoodVertexClust)->GetIsFired())
		return 0;
	if (!ev->GetTrigger(HADES_constants::kGoodVertexCand)->GetIsFired())
		return 0;
	if (!ev->GetTrigger(HADES_constants::kGoodSTART)->GetIsFired())
		return 0;

	if (!ev->GetTrigger(HADES_constants::kNoPileUpSTART)->GetIsFired())
		return 0;
	if (!ev->GetTrigger(HADES_constants::kGoodSTARTVETO)->GetIsFired())
		return 0;
	if (!ev->GetTrigger(HADES_constants::kGoodSTARTMETA)->GetIsFired())
		return 0;
	if (!ev->GetTrigger(HADES_constants::kNoVETO)->GetIsFired())
		return 0;

	return 1;
}

//to off triggers input false
bool Correct_track(DataTreeEvent* _ev, Int_t idx, bool work) {
	if (!work) return 1;
	DataTreeEvent* ev = _ev;
	DataTreeTrack* tr = ev->GetVertexTrack(idx);
	DataTreeTOFHit* tof = ev->GetTOFHit(idx);
	Float_t time = tof->GetTime();
	Float_t leng = tof->GetPathLength();
	if (tr->GetDCAComponent(1) > 15)
	{
		return 0;
	}
	if (leng / (time*_c) > 1)
		return 0;
	if (tof->GetPositionComponent(0) < -5 || tof->GetPositionComponent(0) > 5)
	{
		return 0;
	}
	if (tof->GetPositionComponent(1) < -5 || tof->GetPositionComponent(1) > 5)
	{
		return 0;
	}
	if (tr->GetChi2() > 100)
	{
		return 0;
	}
	return 1;
}

//base all that off
bool only_off_trigger(DataTreeEvent* _ev, Int_t a){
	if (a==100) return !Correct_event(_ev);
	DataTreeEvent* ev = _ev;
	Float_t Rx = ev->GetVertexPositionComponent(0), Ry = ev->GetVertexPositionComponent(1);

	switch (a){
	case 1: 
		if (ev->GetVertexPositionComponent(2) > 0 || ev->GetVertexPositionComponent(2) < -60)
			return 1;
		else return 0;
	case 2:
		if (sqrt(Rx*Rx + Ry*Ry) > 3)
			return 1;
		else return 0;
	case 3: 
		if (ev->GetVertexQuality() < 0.5 || ev->GetVertexQuality() > 40)
			return 1;
		else return 0;
	case 4: 
		if (!ev->GetTrigger(HADES_constants::kGoodVertexClust)->GetIsFired())
			return 1;
		else return 0;
	case 5:
		if (!ev->GetTrigger(HADES_constants::kGoodVertexCand)->GetIsFired())
			return 1;
		else return 0; 
	case 6: 
		if (!ev->GetTrigger(HADES_constants::kGoodSTART)->GetIsFired())
			return 1;
		else return 0;
	case 7:
		if (!ev->GetTrigger(HADES_constants::kNoPileUpSTART)->GetIsFired())
			return 1;
		else return 0;
	case 8:
		if (!ev->GetTrigger(HADES_constants::kGoodSTARTVETO)->GetIsFired())
			return 1;
		else return 0;
	case 9: 
		if (!ev->GetTrigger(HADES_constants::kGoodSTARTMETA)->GetIsFired())
			return 1;
		else return 0;
	case 10:
		if (!ev->GetTrigger(HADES_constants::kNoVETO)->GetIsFired())
			return 1;
		else return 0;
	default:
		return 0;
	}
	return 0;
}

//base all that off
bool only_off_track(DataTreeEvent* _ev, Int_t idx, Int_t a){
	if (a==100) return !Correct_track(_ev, idx);
	DataTreeEvent* ev = _ev;
	DataTreeTrack* tr = ev->GetVertexTrack(idx);
	DataTreeTOFHit* tof = ev->GetTOFHit(idx);
	Float_t time = tof->GetTime();
	Float_t leng = tof->GetPathLength();
	switch (a){
	case 1:
		if (tr->GetDCAComponent(1) > 15)
			return 1;
		else return 0;
	case 2:
		if (leng / (time*_c) > 1)
			return 1;
		else return 0;
	case 3:
		if (tof->GetPositionComponent(0) < -5 || tof->GetPositionComponent(0) > 5)
			return 1;
		else if (tof->GetPositionComponent(1) < -5 || tof->GetPositionComponent(1) > 5)
			return 1;
		else return 0;
	case 4:
		if (tr->GetChi2() > 100)
			return 1;
		else return 0;
	default:
		return 0;
	}
	return 0;
}