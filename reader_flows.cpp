#include "Qvector.h"
#include "triggers.h"

void reader_flows(TString init_string, TString outFile) {

	//init hists
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 14, 0, 70);		//recreate hist
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 14, 0, 70);		//recreate hist
	TProfile* mean31Qx = new TProfile("MeanQx vs Centrality31", "centrality mean Qx31", 14, 0, 70);	//recreate hist
	TProfile* mean31Qy = new TProfile("MeanQy vs Centrality31", "centrality mean Qy31", 14, 0, 70);	//recreate hist
	TProfile* mean32Qx = new TProfile("MeanQx vs Centrality32", "centrality mean Qx32", 14, 0, 70);	//recreate hist
	TProfile* mean32Qy = new TProfile("MeanQy vs Centrality32", "centrality mean Qy32", 14, 0, 70);	//recreate hist
	TProfile* mean33Qx = new TProfile("MeanQx vs Centrality33", "centrality mean Qx33", 14, 0, 70);	//recreate hist
	TProfile* mean33Qy = new TProfile("MeanQy vs Centrality33", "centrality mean Qy33", 14, 0, 70);	//recreate hist
	TProfile* v1PtCent2r[20][14];
	TProfile* v1RapCent2r[20][14];
	TProfile* v1PtCent1s[20][14];
	TProfile* v1RapCent1s[20][14];
	TProfile* v1PtCent2s[20][14];
	TProfile* v1RapCent2s[20][14];
	TProfile* v1PtCent3s[20][14];
	TProfile* v1RapCent3s[20][14];
	//Pt 0.0-0.1; 0.1-0.2;...;1.9-2.0;	Rap -0.95--0.85; -0.85--0.75;...;-0.5-0.5;...;0.95-1.05;
	for (int i = 0; i < 20; i++) {		//Pt or rapidity
		for (int j = 0; j < 14; j++) {	//Centrality
			Float_t minp, maxp;
			Int_t minc, maxc;
			minp = i*0.1;	maxp = i*0.1 + 0.1;
			minc = j * 5;	maxc = j * 5 + 5;
			v1PtCent2r[i][j] = new TProfile(Form("2r Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), Form("2r Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), 20, -0.95, 1.05);
			v1PtCent1s[i][j] = new TProfile(Form("1s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), Form("1s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), 20, -0.95, 1.05);
			v1PtCent2s[i][j] = new TProfile(Form("2s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), Form("2s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), 20, -0.95, 1.05);
			v1PtCent3s[i][j] = new TProfile(Form("3s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), Form("3s Rap vs v1, Pt %1.1f - %1.1f , Cent %i - %i", minp, maxp, minc, maxc), 20, -0.95, 1.05);
			minp -= 0.95;		maxp -= 0.95;
			v1RapCent2r[i][j] = new TProfile(Form("2r Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), Form("2r Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), 20, 0, 2);
			v1RapCent1s[i][j] = new TProfile(Form("1s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), Form("1s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), 20, 0, 2);
			v1RapCent2s[i][j] = new TProfile(Form("2s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), Form("2s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), 20, 0, 2);
			v1RapCent3s[i][j] = new TProfile(Form("3s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), Form("3s Pt vs v1, Rap %1.2f - %1.2f , Cent %i - %i", minp, maxp, minc, maxc), 20, 0, 2);
		}
	}

	TTree* t;
	TBranch* DTEvent;
	DataTreeTrack* tr;
	Int_t N_events;
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off (all particles)

	Float_t res2[14], res3[3][14];

	//take resolutions,
	TFile *f2 = new TFile("resolutions.root", "READ");
	TProfile* histresolution2 = (TProfile*)f2->Get("res2ssum;1");
	TProfile* histresolution31 = (TProfile*)f2->Get("res3s1sum;1");
	TProfile* histresolution32 = (TProfile*)f2->Get("res3s2sum;1");
	TProfile* histresolution33 = (TProfile*)f2->Get("res3s3sum;1");
	for (int i = 0; i < 14; i++) {
		res2[i] = histresolution2->GetBinContent(i + 1);
		res3[0][i] = histresolution31->GetBinContent(i + 1);
		res3[1][i] = histresolution32->GetBinContent(i + 1);
		res3[2][i] = histresolution33->GetBinContent(i + 1);
	}
	f2->Close();
	TFile* f = new TFile(init_string);
	t = (TTree*)f->Get("DataTree");
	DataTreeEvent* ev = new DataTreeEvent;
	DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);
	//std::cerr << "Running file: " << current_file << std::endl;

	N_events = t->GetEntries();
	//filn mean Q
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut); //5
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) == -999) continue;
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
		Subevent3 cursub3;		//3sub
		if (!curQ.Fillsub3(ev)) continue;
		cursub3 = curQ.Getsub3();
		mean31Qx->Fill(centrality, cursub3.Qx[0]);
		mean31Qy->Fill(centrality, cursub3.Qy[0]);
		mean32Qx->Fill(centrality, cursub3.Qx[1]);
		mean32Qy->Fill(centrality, cursub3.Qy[1]);
		mean33Qx->Fill(centrality, cursub3.Qx[2]);
		mean33Qy->Fill(centrality, cursub3.Qy[2]);
	}

	//Find flows (v1)
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		bool good3subeve = true;
		Qvector curQ;
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut);
		int binCent;
		binCent = (int)centrality / 5;
		if (centrality > 70) continue;
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) == -999) continue;
		Float_t Qxcor = meanQx->GetBinContent(binCent + 1);
		Float_t Qycor = meanQy->GetBinContent(binCent + 1);
		curQ.Recenter(Qxcor, Qycor);
		if (!curQ.Fillsub3(ev)) good3subeve = false;		//3sub
		Subevent3 meansub3;
		meansub3.Qx[0] = mean31Qx->GetBinContent(binCent + 1);
		meansub3.Qy[0] = mean31Qy->GetBinContent(binCent + 1);
		meansub3.Qx[1] = mean32Qx->GetBinContent(binCent + 1);
		meansub3.Qy[1] = mean32Qy->GetBinContent(binCent + 1);
		meansub3.Qx[2] = mean33Qx->GetBinContent(binCent + 1);
		meansub3.Qy[2] = mean33Qy->GetBinContent(binCent + 1);
		curQ.Resentsub3(meansub3);
		Float_t PsiEP = curQ.GetEventPlaneAngle();
		Float_t PsiEP3[3];
		for (int j = 0; j < 3; j++)
			PsiEP3[j] = curQ.Get_Psy3(j);
		Float_t vn = 0;
		TLorentzVector momentum;
		double BETA = sqrt(1.0 - 0.938*0.938 / 1.23 / 1.23);
		TVector3 b; b.SetXYZ(0, 0, -BETA);
		Int_t n_tracks_selected = ev->GetNVertexTracks();
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}
			momentum = tr->GetMomentum();
			momentum.Boost(b);
			Float_t pt = tr->GetPt();
			if (pt > 2.0) continue;			//pt>2 -> do not fill hists. same next 2 lines.
			if (momentum.Rapidity() < -0.95) continue;
			if (momentum.Rapidity() > 1.05) continue;
			Float_t phi = tr->GetPhi();
			int binPt, binRap;				//start from 0. it is hist numbers.
			binPt = (int)(pt * 10);
			binRap = (int)((momentum.Rapidity() + 0.95) * 10);

			vn = cos(2 * (phi - PsiEP)) / res2[binCent];
			if (vn != vn) continue;
			v1PtCent2r[binPt][binCent]->Fill(momentum.Rapidity(), vn);
			if (momentum.Rapidity() < 0) vn = -vn;
			v1RapCent2r[binRap][binCent]->Fill(pt, vn);

			if (!good3subeve) continue;
			vn = cos(2 * (phi - PsiEP3[0])) / res3[0][binCent];
			if (vn != vn) continue;
			v1PtCent1s[binPt][binCent]->Fill(momentum.Rapidity(), vn);
			if (momentum.Rapidity() < 0) vn = -vn;
			v1RapCent1s[binRap][binCent]->Fill(pt, vn);

			vn = cos(2 * (phi - PsiEP3[1])) / res3[1][binCent];
			if (vn != vn) continue;
			v1PtCent2s[binPt][binCent]->Fill(momentum.Rapidity(), vn);
			if (momentum.Rapidity() < 0) vn = -vn;
			v1RapCent2s[binRap][binCent]->Fill(pt, vn);

			vn = cos(2 * (phi - PsiEP3[2])) / res3[2][binCent];
			if (vn != vn) continue;
			v1PtCent3s[binPt][binCent]->Fill(momentum.Rapidity(), vn);
			if (momentum.Rapidity() < 0) vn = -vn;
			v1RapCent3s[binRap][binCent]->Fill(pt, vn);
		}
	}
	//std::cerr << "Flow found" << std::endl;
	f->Close();
	delete f;
	delete ev;

	TFile* w = new TFile("f/"+outFile+".root", "recreate");
	w->cd();
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 14; j++) {
			v1PtCent2r[i][j]->Write();
			v1PtCent1s[i][j]->Write();
			v1PtCent2s[i][j]->Write();
			v1PtCent3s[i][j]->Write();
		}
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 14; j++) {
			v1RapCent2r[i][j]->Write();
			v1RapCent1s[i][j]->Write();
			v1RapCent2s[i][j]->Write();
			v1RapCent3s[i][j]->Write();
		}
	}
	w->Close();
	delete w;
}