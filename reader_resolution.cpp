#include "Qvector.h"
#include "triggers.h"

TString reader_resolution(TString init_string) {

	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 14, 0, 70);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 14, 0, 70);
	TProfile* resolu = new TProfile("mean cos", "cos from centrality", 14, 0, 70);

	TProfile*  Qx3sub1 = new TProfile("Qx 3sub vs Centrality 1", "Qx sub1", 14, 0, 70);
	TProfile*  Qx3sub2 = new TProfile("Qx 3sub vs Centrality 2", "Qx sub2", 14, 0, 70);
	TProfile*  Qx3sub3 = new TProfile("Qx 3sub vs Centrality 3", "Qx sub3", 14, 0, 70);
	TProfile*  Qy3sub1 = new TProfile("Qy 3sub vs Centrality 1", "Qy sub1", 14, 0, 70);
	TProfile*  Qy3sub2 = new TProfile("Qy 3sub vs Centrality 2", "Qy sub2", 14, 0, 70);
	TProfile*  Qy3sub3 = new TProfile("Qy 3sub vs Centrality 3", "Qy sub3", 14, 0, 70);

	TProfile*  cos10 = new TProfile("cos10 vs Centrality 3 sub", "cos21", 14, 0, 70);
	TProfile*  cos20 = new TProfile("cos20 vs Centrality 3 sub", "cos31", 14, 0, 70);
	TProfile*  cos12 = new TProfile("cos12 vs Centrality 3 sub", "cos32", 14, 0, 70);

	TTree* t;
	TBranch* DTEvent;
	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events;
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off (all particles)
	TFile* f = new TFile(init_string);
	t = (TTree*)f->Get("DataTree");
	DataTreeEvent* ev = new DataTreeEvent;
	DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);
	N_events = t->GetEntries();
	//find mean 2 rand subevent + 3 sub
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut); //5
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) == -999) {	//error check
			continue;
		}
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
		Subevent3 cursub3;		//3sub
		if (!curQ.Fillsub3(ev)) continue;
		cursub3 = curQ.Getsub3();
		Qx3sub1->Fill(centrality, cursub3.Qx[0]);
		Qy3sub1->Fill(centrality, cursub3.Qy[0]);
		Qx3sub2->Fill(centrality, cursub3.Qx[1]);
		Qy3sub2->Fill(centrality, cursub3.Qy[1]);
		Qx3sub3->Fill(centrality, cursub3.Qx[2]);
		Qy3sub3->Fill(centrality, cursub3.Qy[2]);
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
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut);
		while (!sub2good) {		//2rand
			if (!curQ.Resolution(ev)) break;
			Float_t Qxcor, Qycor;
			Qxcor = meanQx->GetBinContent((int)centrality / 5 + 1);
			Qycor = meanQy->GetBinContent((int)centrality / 5 + 1);
			curQ.RecenterRes(Qxcor, Qycor, Qxcor, Qycor);
			res = curQ.GetResolution();
			resolu->Fill(centrality, res);
			sub2good = true;
		}
		if (!sub2good) continue;	//2rand error->3 sub error 100%
		Subevent3 curmeansub3;		//3sub
		curmeansub3.Qx[0] = Qx3sub1->GetBinContent((int)centrality / 5 + 1);
		curmeansub3.Qy[0] = Qy3sub1->GetBinContent((int)centrality / 5 + 1);
		curmeansub3.Qx[1] = Qx3sub2->GetBinContent((int)centrality / 5 + 1);
		curmeansub3.Qy[1] = Qy3sub2->GetBinContent((int)centrality / 5 + 1);
		curmeansub3.Qx[2] = Qx3sub3->GetBinContent((int)centrality / 5 + 1);
		curmeansub3.Qy[2] = Qy3sub3->GetBinContent((int)centrality / 5 + 1);
		Float_t cos3[3];
		if (!curQ.Fillsub3(ev)) continue;
		curQ.Resentsub3(curmeansub3);
		for (int i = 0; i < 3; i++) {
			cos3[i] = curQ.Get_cos3(i);
		}
		cos10->Fill(centrality, cos3[0]);
		cos20->Fill(centrality, cos3[1]);
		cos12->Fill(centrality, cos3[2]);
	}
	f->Close();
	delete f;
	delete ev;
	delete[] st;
	TSting outhist;
	outstring.SetSize(init_string.GetShortSize());
	outhist[0] = 'r';
	outhist[1] = '/';
	outhist[2] = 'o';
	for (int i = 0; i < init_string.GetShortSize(); i++)
		outhish[i + 3] = inithist[i];
	TFile* w = new TFile(outhist, "recreate");
	resolu->Write();	//2sub
	cos10->Write();		//3sub
	cos20->Write();
	cos12->Write();
	w->Close();
	delete w;
	return outhist;
}
