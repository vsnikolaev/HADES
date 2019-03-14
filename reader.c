//#include "HADES_constants.h"
#include "Qvector.h"
#include "triggers.h"
#include <iostream>


{
	auto f = new TFile("out.root");
	auto t = (TTree*)f->Get("DataTree");
	auto ev = new DataTreeEvent;
	auto DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);

//Q vectors
	TProfile* meanQx = new TProfile("MeanQx vs Centrality"    ,"centrality mean Qx",10,0,50);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality"    ,"centrality mean Qy",10,0,50);
	TProfile* resolu = new TProfile("Resolution vs Centrality","centrality R_{1}"  ,10,0,50);
	TProfile* v2obse = new TProfile("v2 observable vs Centrality","centrality mean v2 obs",10,0,50);
	TProfile* v2real = new TProfile("real v2 vs Centrality"   ,"centrality mean v2 real",10,0,50);
	TProfile* realre = new TProfile("real resolution", "centrality mean resolution",10,0,50);

	Long64_t N_events = (Long64_t)t->GetEntries();
	Int_t n_tracks = 0;
	Int_t n_hits = 0;
	Int_t n_tracks_selected = 0;
	Float_t PsiEP;
	Float_t rapidity = 0;
	Float_t phi = 0;
	Float_t pt = 0;

	Float_t centrality;
	Float_t vn = 0;
	Float_t Qxcor;
	Float_t Qycor;
	Float_t res;
	nTriggers particle;
	particle.SetParticle(-2);	//-2 off

	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	//Find mean
	for (int i = 0; i<N_events && i<500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(); 
		curQ.findQ(ev);
		if( curQ.GetComponent(1)!=curQ.GetComponent(1) || curQ.GetComponent(2)!=curQ.GetComponent(2) )
			continue;
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
	}

	//recentre + v2 observable.
	for (int i = 0; i < N_events && i < 500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality();
		curQ.findQ(ev);
		if( curQ.GetComponent(1)!=curQ.GetComponent(1) || curQ.GetComponent(2)!=curQ.GetComponent(2) )
			continue;
		Qxcor = meanQx->GetBinContent(centrality/5 + 1);
		Qycor = meanQy->GetBinContent(centrality/5 + 1);
		curQ.Recenter(Qxcor,Qycor);
		curQ.Resolution(ev);
		///curQ.RecenterRes(Qxcor,Qycor);
		res = curQ.GetResolution();
		resolu->Fill(centrality, res);
		PsiEP = curQ.GetEventPlaneAngle();
		n_tracks_selected = ev->GetNVertexTracks();
		vn=0;
		for (int j = 0; j < n_tracks_selected; j++){
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)){
				continue;
			}
			phi = tr->GetPhi();
			vn = cos(2 * (phi - PsiEP) );
			v2obse->Fill(centrality, vn);
		}
  }

  //use resolution to find real v2;
	for (int i=0; i<10; i++){
		res = resolu->GetBinContent(i + 1);
		realre->Fill(i*5+3, sqrt(2*res));
	}
	for (int i=0; i<10; i++){
		vn = v2obse->GetBinContent(i + 1);
		res= realre->GetBinContent(i + 1);
		v2real->Fill( i*5 + 3, vn/res);
	}

	//save
	TFile* w = new TFile("my1.root", "recreate");
	w->cd();
	meanQx->Write();
	meanQy->Write();
	resolu->Write();
	realre->Write();
	v2obse->Write();
	v2real->Write();
}