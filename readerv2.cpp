#include "Qvector.h"
#include "triggers.h"

void readerv2() {
	auto f = new TFile("AuAu.root");
	auto t = (TTree*)f->Get("DataTree");
	auto ev = new DataTreeEvent;
	auto DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);

	//including histograms.
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 10, 0, 10);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 10, 0, 10);
	TProfile* resolu = new TProfile("Resolution vs Centrality", "centrality R_{2}", 10, 0, 10);
	TProfile* v2obse = new TProfile("v2 observable vs Centrality", "centrality mean v2 obs", 10, 0, 10);
	TProfile* v2real = new TProfile("real v2 vs Centrality", "centrality mean v2 real", 10, 0, 10);
	TProfile* realre = new TProfile("real resolution vs centrality", "centrality mean resolution", 10, 0, 10);

	TProfile2D* resolu2d = new TProfile2D("resolution vs Centrality and pt", "resolution ", 10, 0, 10, 10, 0, 2, 0, 20);
	TProfile2D* v2obse2d = new TProfile2D("v2 observable vs Centrality and pt", "v2 obs", 10, 0, 10, 10, 0, 2, 0, 20);
	TProfile2D* v2real2d = new TProfile2D("real v2 vs Centrality and pt", "v2 real", 10, 0, 10, 10, 0, 2, 0, 20);
	//3sub
	TProfile2D* Qx3sub = new TProfile2D("Qx 3sub vs Centrality class", "Qx centrality class number of sub eve", 10, 0, 10, 3, 0, 3, 0, 20);
	TProfile2D* Qy3sub = new TProfile2D("Qy 3sub vs Centrality class", "Qy centrality class number of sub eve", 10, 0, 10, 3, 0, 3, 0, 20);

	TProfile*  cos10 = new TProfile("cos10 vs Centrality 3 sub", "centrality cos10 3 subevent", 10, 0, 10);
	TProfile*  cos20 = new TProfile("cos20 vs Centrality 3 sub", "centrality cos20 3 subevent", 10, 0, 10);
	TProfile*  cos12 = new TProfile("cos12 vs Centrality 3 sub", "centrality cos12 3 subevent", 10, 0, 10);

	TProfile2D* resolution3sub = new TProfile2D("resolution 3sub vs Centrality class", "resolution centrality class number of sub eve", 10, 0, 10, 3, 0, 3, 0, 20);
	TProfile2D* v23sub  = new TProfile2D("v2 real 3sub vs Centrality class", "real v2 centrality class number of sub eve", 10, 0, 10, 3, 0, 3, 0, 20);
	TProfile2D* v2real3 = new TProfile2D("mean v2 vs Centrality and pt, 3 sub", "mean v2 from centrality and pt, 3sub", 10, 0, 10, 10, 0, 2, 0, 20);
	//just some test hists
	TH1F* histQx  = new TH1F("Qx", "Qx centrality 20-30", 100, -1, 1);
	TH1F* histQxr = new TH1F("Qxr", "Qx centrality 20-30 recentered", 100, -1, 1);
	TH1F* histQy  = new TH1F("Qy", "Qy centrality 20-30", 100, -1, 1);
	TH1F* histQyr = new TH1F("Qyr", "Qy centrality 20-30 recentered", 100, -1, 1);
	TH2F* xymodules = new TH2F("xy", "x vs y", 400, -800, 800, 400, -800, 800);

	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events = t->GetEntries();
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off
	const Int_t centmove = 5;
	//find mean 2 rand subevent + 3 sub
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = (int)(ev->GetCentrality(0) / centmove) + 0.5;
		curQ.FindQ(ev);
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
		if (centrality >= 4 && centrality <= 6) { //del later.
			histQx->Fill(curQ.GetComponent(1));
			histQy->Fill(curQ.GetComponent(2));
		}
		//del later.
		Int_t N_psd_modules = ev->GetNPSDModules();
		for (int i = 0; i < N_psd_modules; i++) {
			m_psd = ev->GetPSDModule(i);
			Int_t x, y;
			x = m_psd->GetPositionComponent(0);
			y = m_psd->GetPositionComponent(1);
			xymodules->Fill(x, y);
		}
		Subevent3 cursub3;		//3sub
		curQ.Fillsub3(ev);
		cursub3 = curQ.Getsub3();
		if (!cursub3.GoodSubEvents()) continue;
		for (int j = 0; j < 3; j++) {
			Qx3sub->Fill(centrality, j + 0.5, cursub3.Qx[j], 1);
			Qy3sub->Fill(centrality, j + 0.5, cursub3.Qy[j], 1);
		}
	}
	
	//recent + resolution
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		Float_t res;
		bool sub2good = false;
		centrality = (int)(ev->GetCentrality(0) / centmove) + 0.5;
		while (!sub2good) {		//2rand
			if (!curQ.Resolution(ev)) break;
			Float_t Qxcor, Qycor;
			Qxcor = meanQx->GetBinContent((int)centrality + 1);
			Qycor = meanQy->GetBinContent((int)centrality + 1);
			curQ.RecenterRes(Qxcor, Qycor, Qxcor, Qycor);
			res = curQ.GetResolution();
			resolu->Fill(centrality, res);
			sub2good = true;
		}
		Subevent3 curmeansub3;		//3sub
		for (int j = 0; j < 3; j++) {
			curmeansub3.Qx[j] = Qx3sub->GetBinContent((int)centrality + 1, j + 1);
			curmeansub3.Qy[j] = Qy3sub->GetBinContent((int)centrality + 1, j + 1);
		}
		Float_t cos3[3];
		curQ.Fillsub3(ev);
		curQ.Resentsub3(curmeansub3);
		for (int i = 0; i < 3; i++) {
			cos3[i] = curQ.Get_cos3(i);
		}
		cos10->Fill(centrality, cos3[0]);
		cos20->Fill(centrality, cos3[1]);
		cos12->Fill(centrality, cos3[2]);
		//maybe add res from pt? but it need tracks, so...
	}
	for (int i = 0; i < 10; i++) {	//3sub resolutions
		Float_t cos3[3], res;
		cos3[0] = cos10->GetBinContent(i + 1);
		cos3[1] = cos20->GetBinContent(i + 1);
		cos3[2] = cos12->GetBinContent(i + 1);
		for (int j = 0; j < 3; j++) {	//take errors and enter them.
			res = Get_resolution(cos3[0], cos3[1], cos3[2], j);
			if (res != res) continue;
			if (res == 0) continue;
			resolution3sub->Fill(i + 1, j + 0.5, res, 1);
		}
	}

	//v2
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = (int)(ev->GetCentrality(0) / centmove) + 0.5;
		curQ.FindQ(ev);
		Float_t Qxcor = meanQx->GetBinContent((int)centrality + 1);
		Float_t Qycor = meanQy->GetBinContent((int)centrality + 1);
		curQ.Recenter(Qxcor, Qycor);
		if (centrality >= 4 && centrality <= 6) {
			histQxr->Fill(curQ.GetComponent(1));
			histQyr->Fill(curQ.GetComponent(2));
		}
		Float_t PsiEP = curQ.GetEventPlaneAngle();
		Int_t n_tracks_selected = ev->GetNVertexTracks();
		Float_t vn = 0;
		Float_t res = resolu->GetBinContent((int)centrality + 1);
		res = sqrt(2 * res);
		realre->Fill(centrality, res);
		Float_t res3[3];
		res3[0] = resolution3sub->GetBinContent((int)centrality + 1, 1);
		res3[1] = resolution3sub->GetBinContent((int)centrality + 1, 2);
		res3[2] = resolution3sub->GetBinContent((int)centrality + 1, 3);
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}//2sub
			Float_t pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());//chek it
			Float_t phi = tr->GetPhi();
			vn = cos(2 * (phi - PsiEP));
			v2obse->Fill(centrality, vn);
			v2obse2d->Fill(centrality, pt, vn, 1);
			v2real->Fill(centrality, vn / res);
			v2real2d->Fill(centrality, pt, vn / res, 1);
			//3 sub
			vn = cos(2 * (phi - PsiEP));
			v2real3->Fill(centrality, pt, vn / res3[0], 1);
			for (int k = 0; k < 3; k++) {
				if (res3[k] != res3[k]) continue;
				v23sub->Fill((int)centrality + 1, k + 0.5, vn / res3[k], 1);
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

	Qx3sub->Write();
	Qy3sub->Write();
	cos10->Write();
	cos20->Write();
	cos12->Write();

	resolu2d->Write();
	v2obse2d->Write();
	v2real2d->Write();
	
	resolution3sub->Write();
	v23sub->Write();
	v2real3->Write();

	histQx->Write();
	histQxr->Write();
	histQy->Write();
	histQyr->Write();
	xymodules->Write();

	w->Close();
}	