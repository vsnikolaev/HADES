//#include "HADES_constants.h"
#include "Qvector.h"
#include "triggers.h"
#include <iostream>

//
{
	auto f = new TFile("AuAu.root");
	auto t = (TTree*)f->Get("DataTree");
	auto ev = new DataTreeEvent;
	auto DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);
	//from centrality
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 10, 0, 50);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 10, 0, 50);
	TProfile* resolu = new TProfile("Resolution vs Centrality", "centrality R_{2}", 10, 0, 50);
	TProfile* v2obse = new TProfile("v2 observable vs Centrality", "centrality mean v2 obs", 10, 0, 50);
	TProfile* v2real = new TProfile("real v2 vs Centrality", "centrality mean v2 real", 10, 0, 50);
	TProfile* realre = new TProfile("real resolution vs centrality", "centrality mean resolution", 10, 0, 50);

	TProfile* meansina = new TProfile("<sin(#Psi_{a}-#Psi_{b})> vs Centrality", "centrality <sin(#Psi_{a}-#Psi_{b})>", 10, 0, 50);
	TProfile* meansinb = new TProfile("<sin(#Psi_{b}-#Psi_{a})> vs Centrality", "centrality <sin(#Psi_{b}-#Psi_{a})>", 10, 0, 50);
	
	//from pt
	TProfile* pv2obse = new TProfile("v2 observable vs pt", "pt mean v2 obs", 10, 0, 2);
	TProfile* pv2real = new TProfile("real v2 vs pt", "pt mean v2 real", 10, 0, 2);
	TProfile* presolu = new TProfile("resolution vs pt", "pt mean resolution", 10, 0, 2);
	//from rapidity
	/*
	TProfile* rv2obse = new TProfile("v2 observable vs Centrality", "rapidity mean v2 obs", 10, -1, 2);
	TProfile* rv2real = new TProfile("real v2 vs rapidity", "rapidity mean v2 real", 10, -1, 2);
	TProfile* rresolu = new TProfile("real resolution vs rapidity", "rapidity mean resolution", 10, -1, 2);
	//maybe later make profile2d;
	*/

	Long64_t N_events = (Long64_t)t->GetEntries();
	Int_t n_tracks_selected = 0;
	Float_t PsiEP;
	Float_t phi = 0;
	Float_t pt;
	Float_t rap;

	Float_t centrality;
	Float_t vn = 0;
	Float_t Qxcor;
	Float_t Qycor;
	Float_t res;
	TLorentzVector fMomentum;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off

	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	//Find mean
	for (int i = 0; i < N_events && i < 500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality();
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) != curQ.GetComponent(1) || curQ.GetComponent(2) != curQ.GetComponent(2))
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
		curQ.FindQ(ev);
		if( curQ.GetComponent(1)!=curQ.GetComponent(1) || curQ.GetComponent(2)!=curQ.GetComponent(2) )
			continue;
		Qxcor = meanQx->GetBinContent(centrality / 5 + 1);
		Qycor = meanQy->GetBinContent(centrality / 5 + 1);
		curQ.Recenter(Qxcor,Qycor);
		curQ.Resolution(ev);
		res = curQ.GetResolution();
		//
		meansina->Fill(centrality, curQ.Getsin());
		meansinb->Fill(centrality, curQ.Getsin(2));
		//
		resolu->Fill(centrality, res);
		PsiEP = curQ.GetEventPlaneAngle();
		n_tracks_selected = ev->GetNVertexTracks();
		vn = 0;
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}
			pt = tr->GetPt();
			phi = tr->GetPhi();
			fMomentum = tr->GetMomentum();
			vn = cos(2 * (phi - PsiEP));
			v2obse->Fill(centrality, vn);
			pv2obse->Fill(pt, vn);
			presolu->Fill(pt, res);
			//rv2obse->Fill(fMomentum.PseudoRapidity(), vn);
			//rresolu->Fill(fMomentum.PseudoRapidity(), res);
		}
  }

  //use resolution to find real v2;
	for (int i = 0; i < 10; i++) {
		res = resolu->GetBinContent(i + 1);
		realre->Fill(i * 5 + 3, sqrt(2 * res));
	}
	for (int i = 0; i < 10; i++) {
		vn = v2obse->GetBinContent(i + 1);
		res = realre->GetBinContent(i + 1);
		v2real->Fill(i * 5 + 3, vn / res);
	}
	//pt
	for (int i = 0; i < 10; i++) {
		vn = pv2obse->GetBinContent(i + 1);
		res = presolu->GetBinContent(i + 1);
		if (res < 0 || res != res) continue;
		pv2real->Fill(i * 0.2 + 0.1, vn / sqrt(2 * res) );
	}
	for (int i = 0; i < 10; i++) {
		vn = rv2obse->GetBinContent(i + 1);
		res = rresolu->GetBinContent(i + 1);
		if (res < 0 || res != res) continue;
		rv2real->Fill(i * 0.2 + 0.1, vn / sqrt(2 * res));
	}


	//save
	TFile* w = new TFile("mycent1.root", "recreate");
	w->cd();
	meanQx->Write();
	meanQy->Write();
	resolu->Write();
	realre->Write();
	v2obse->Write();
	v2real->Write();
	
	presolu->Write();
	pv2obse->Write();
	pv2real->Write();
	rresolu->Write();
	rv2obse->Write();
	rv2real->Write();
	
	meansina->Write();
	meansinb->Write();

	w->Close();
}