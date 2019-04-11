//#include "HADES_constants.h"
#include "Qvector.h"
#include "triggers.h"
#include <iostream>

//
void reader() {
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
	//TProfile* meansina = new TProfile("<sin(#Psi_{a}-#Psi_{b})> vs Centrality", "centrality <sin(#Psi_{a}-#Psi_{b})>", 10, 0, 50);
	//TProfile* meansinb = new TProfile("<sin(#Psi_{b}-#Psi_{a})> vs Centrality", "centrality <sin(#Psi_{b}-#Psi_{a})>", 10, 0, 50);
	
	//from pt
	TProfile* pv2obse = new TProfile("v2 observable vs pt", "pt mean v2 obs", 10, 0, 2);
	TProfile* pv2real = new TProfile("real v2 vs pt", "pt mean v2 real", 10, 0, 2);
	TProfile* presolu = new TProfile("resolution vs pt", "pt mean resolution", 10, 0, 2);
	
	TProfile2D* resolu2d = new TProfile2D("resolution vs Centrality and pt", "resolution ", 10, 0, 50, 10, 0, 2, 0, 20);
	TProfile2D* v2obse2d = new TProfile2D("v2 observable vs Centrality and pt", "v2 obs", 10, 0, 50, 10, 0, 2, 0, 20);
	TProfile2D* v2real2d = new TProfile2D("real v2 vs Centrality and pt", "v2 real", 10, 0, 50, 10, 0, 2, 0, 20);

	TProfile*  cos10 = new TProfile("cos10 vs Centrality 3 sub", "centrality cos10 3 subevent", 10, 0, 50);
	TProfile*  cos20 = new TProfile("cos20 vs Centrality 3 sub", "centrality cos20 3 subevent", 10, 0, 50);
	TProfile*  cos12 = new TProfile("cos12 vs Centrality 3 sub", "centrality cos12 3 subevent", 10, 0, 50);

	TProfile*     resol3  = new TProfile("Resolurion vs Centrality 3 sub", "centrality R_{2} 3 subevent", 10, 0, 50);
	TProfile*     v2real3 = new TProfile("real v2 vs Centrality 3 sub", "centrality mean v2 real 3 subevent", 10, 0, 50);
	TProfile2D* Qx3sub = new TProfile2D("Qx 3sub vs Centrality class", "Qx centrality class number of sub eve", 10, 1, 10, 3, 0, 3, 0, 20);
	TProfile2D* Qy3sub = new TProfile2D("Qy 3sub vs Centrality class", "Qy centrality class number of sub eve", 10, 1, 10, 3, 0, 3, 0, 20);
	TProfile2D* resolution3sub = new TProfile2D("resolution 3sub vs Centrality class", "resolution centrality class number of sub eve", 10, 1, 10, 3, 0, 3, 0, 20);
	TProfile2D* v23sub = new TProfile2D("v2 real 3sub vs Centrality class", "real v2 centrality class number of sub eve", 10, 1, 10, 3, 0, 3, 0, 20);

	TH1F* qx = new TH1F("qx", "Qx", 100, -1, 1);
	TH1F* histQx  = new TH1F("Qx", "Qx centrality 20-30", 100, -1, 1);
	TH1F* histQxr = new TH1F("Qxr", "Qx centrality 20-30 recentered", 100, -1, 1);
	TH1F* histQy = new TH1F("Qy", "Qy centrality 20-30", 100, -1, 1);
	TH1F* histQyr = new TH1F("Qyr", "Qy centrality 20-30 recentered", 100, -1, 1);
	
	//TH2F* xymodules = new TH2F("xy", "x vs y", 400, -400, 400, 400, -400, 400);

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
	Float_t res, pres;
	Float_t res3[4];
	Float_t err1, err2;
	TLorentzVector fMomentum;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off

	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	//Find mean 2 rand subeve
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
		if (centrality >= 20 && centrality <= 30) {
			histQx->Fill(curQ.GetComponent(1));
			histQy->Fill(curQ.GetComponent(2));
		}/*
		//test delete later
		Int_t N_psd_modules = ev->GetNPSDModules();
		for (int i = 0; i < N_psd_modules; i++) {
			m_psd = ev->GetPSDModule(i);
			Int_t x, y;
			x = m_psd->GetPositionComponent(0);
			y = m_psd->GetPositionComponent(1);
			xymodules->Fill(x, y);
		}*/
	}
	//Find mean 3 subeve
	for (int i = 0; i < N_events && i < 500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		Subevent3 cursub3;
		centrality = ev->GetCentrality(0);
		curQ.Fillsub3(ev);
		cursub3=curQ.Getsub3();
		for (int j = 0; j < 3; j++) {
			if (cursub3.Qx[j] == 0 || cursub3.Qy[j] == 0) continue;
			Qx3sub->Fill(centrality / 5 + 1, j + 0.5, cursub3.Qx[j], 1);
			Qy3sub->Fill(centrality / 5 + 1, j + 0.5, cursub3.Qy[j], 1);
		}
	}
	//resolution + sub3 resent.
	for (int i = 0; i < N_events && i < 500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		Subevent3 curmeansub3;
		centrality = ev->GetCentrality(0);
		for (int j = 0; j < 3; j++) {
			curmeansub3.Qx[j] = Qx3sub->GetBinContent(centrality / 5 + 1, j + 1);
			curmeansub3.Qy[j] = Qy3sub->GetBinContent(centrality / 5 + 1, j + 1);
		}
		curQ.Resolution(ev);//now Resolution is bool,  so I must take it state. if() continue;...
		curQ.Fillsub3(ev);
		curQ.Resentsub3(curmeansub3);
		for (int i = 0; i < 3; i++) {
			res3[i] = curQ.Get_cos3(i);//here it is cos
		}
		cos10->Fill(centrality, res3[0]);
		cos20->Fill(centrality, res3[1]);
		cos12->Fill(centrality, res3[2]);
		qx->Fill(curQ.Get_subx(1));

		res = curQ.GetResolution();
		if (res != res) continue;
		resolu->Fill(centrality, res);
		n_tracks_selected = ev->GetNVertexTracks();
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}
			pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());
			phi = tr->GetPhi();
			fMomentum = tr->GetMomentum();
			presolu->Fill(pt, res);
		}
	}
	//3sub res
	for (int i = 0; i < 10; i++) {
		res3[0] = cos10->GetBinContent(i + 1);
		res3[1] = cos20->GetBinContent(i + 1);
		res3[2] = cos12->GetBinContent(i + 1);
		res3[3] = Get_resolution(res3[0], res3[1], res3[2], 0);
		if (res3[3] != res3[3]) continue;
		if (res3[3] == 0) continue;
		resol3->Fill(i * 5 + 3, res3[3]);
		for (int j = 0; j < 3; j++) {
			res3[3] = Get_resolution(res3[0], res3[1], res3[2], j);
			if (res3[3] != res3[3]) continue;
			if (res3[3] == 0) continue;
			resolution3sub->Fill(i + 1, j + 0.5, res3[3], 1);
		}
	}
	//v2
	for (int i = 0; i < N_events && i < 500000; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(0);
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) != curQ.GetComponent(1) || curQ.GetComponent(2) != curQ.GetComponent(2))
			continue;
		Qxcor = meanQx->GetBinContent(centrality / 5 + 1);
		Qycor = meanQy->GetBinContent(centrality / 5 + 1);
		curQ.Recenter(Qxcor, Qycor);
		if (centrality >= 20 && centrality <= 30) {
			histQxr->Fill(curQ.GetComponent(1));
			histQyr->Fill(curQ.GetComponent(2));
		}
		PsiEP = curQ.GetEventPlaneAngle();
		n_tracks_selected = ev->GetNVertexTracks();
		vn = 0;
		res = resolu->GetBinContent(centrality / 5 + 1);
		res = sqrt(2 * res);
		realre->Fill(centrality, res);
		res3[3] = resol3->GetBinContent(centrality / 5 + 1);//res3 - 3 sub event 0 and 3 must be equal;
		res3[0] = resolution3sub->GetBinContent(centrality / 5 + 1, 1);
		res3[1] = resolution3sub->GetBinContent(centrality / 5 + 1, 2);
		res3[2] = resolution3sub->GetBinContent(centrality / 5 + 1, 3);
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}
			//2 sub
			pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());//chek it
			phi = tr->GetPhi();
			vn = cos(2 * (phi - PsiEP));
			v2obse->Fill(centrality, vn);
			pv2obse->Fill(pt, vn);
			v2obse2d->Fill(centrality, pt, vn, 1);
			v2real->Fill(centrality, vn / res);
			pres = presolu->GetBinContent((int)(pt * 5) + 1);
			pv2real->Fill(pt, vn / sqrt(2 * pres));
			v2real2d->Fill(centrality, pt, vn / sqrt(2 * pres), 1);
			//3 sub
			if (res3[3] != res3[3]) continue;
			vn = cos(2 * (phi - PsiEP));
			v2real3->Fill(centrality, vn / res3[3]);
			for (int k = 0; k < 3; k++) {
				if (res3[j] != res3[j]) continue;
				v23sub->Fill(centrality / 5 + 1, j + 0.5, vn / res3[j], 1);
			}
		}
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

	cos10->Write();
	cos20->Write();
	cos12->Write();
	resol3->Write();
	v2real3->Write();
	v2obse2d->Write();
	v2real2d->Write();
	//xymodules->Write();
	qx->Write();
	Qx3sub->Write();
	Qy3sub->Write();
	resolution3sub->Write();
	v23sub->Write();

	histQx->Write();
	histQxr->Write();
	histQy->Write();
	histQyr->Write();
	//meansina->Write();
	//meansinb->Write();
	resolu2d->Write();
	v2obse2d->Write();
	v2real2d->Write();

	w->Close();
}