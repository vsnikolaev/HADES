#pragma once


#ifndef MYTRIG
#define MYTRIG
#include "HADES_constants.h"
#include <vector>
#include <string>

const float pi = 3.1415;
const float _c = 299.792458;

class nTriggers{
private:
    Int_t particle;
public:
	nTriggers() { particle = -2; };
	~nTriggers() {};
    void SetParticle(Int_t p);
    Int_t GetParticle();
    bool DeterminedParticle(DataTreeTrack* tr);
};

bool Correct_event(DataTreeEvent* _ev, bool work = true);
bool Correct_track(DataTreeEvent* _ev, Int_t idx, bool work = true);
bool Correct_FWhit(DataTreePSDModule* m_psd, bool work = true);
bool only_off_trigger(DataTreeEvent* _ev, Int_t a = 100);
bool only_off_track(DataTreeEvent* _ev, Int_t idx, Int_t a = 100);
bool Centrality_good_event(DataTreeEvent* _ev, bool work = true);

void read_header_file(std::vector<std::string> &_p);


#endif // MYTRIG