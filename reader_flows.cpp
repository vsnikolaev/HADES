#include "Qvector.h"
#include "triggers.h"

void reader_flows() {

	int current_file_number = 0;
	std::vector<std::string> all_files;
	read_header_file(all_files);
	if (all_files.size() == 0) return;
	std::string current_file;
	current_file = all_files.at(current_file_number);

	//Add
	//init hists
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 14, 0, 70);	//recreate hist
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 14, 0, 70);	//recreate hist
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
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events;
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off (all particles)

	Float_t res2[14], res3[3][14];

	//take resolutions,
	TFile *f2 = new TFile("resolutions.root", "READ");
	TProfile* histresolution2  = (TProfile*)f2->Get("res2ssum;1");
	TProfile* histresolution31 = (TProfile*)f2->Get("res3s1sum;1");
	TProfile* histresolution32 = (TProfile*)f2->Get("res3s2sum;1");
	TProfile* histresolution33 = (TProfile*)f2->Get("res3s3sum;1");
	for (int i = 0; i < 14; i++) {
		res2[i]    = histresolution2->GetBinContent(i + 1);
		res3[0][i] = histresolution31->GetBinContent(i + 1);
		res3[1][i] = histresolution32->GetBinContent(i + 1);
		res3[2][i] = histresolution33->GetBinContent(i + 1);
	}
	f2->Close();

	//open file + clear histograms.
NextFile:
	char *st = new char[current_file.size()];	//WHERE is support of new lines,
	for (int i = 0; i + 1 < current_file.size(); i++) {
		st[i] = current_file.at(i);
	}
	TFile* f = new TFile(st);
	t = (TTree*)f->Get("DataTree");
	DataTreeEvent* ev = new DataTreeEvent;
	DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);
	meanQx->Reset();
	meanQy->Reset();
	std::cerr << "Running file: " << current_file << std::endl;

	N_events = t->GetEntries();
	//if (N_events > 5000) N_events = 5000;	//for testing
	//filn mean Q
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut); //5
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) == -999) continue;	 //error check
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
	}

	//Find flows (v1)
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentrality(HADES_constants::kNhitsTOF_RPC_cut);
		curQ.FindQ(ev);
		if (curQ.GetComponent(1) == -999) continue;
		Float_t Qxcor = meanQx->GetBinContent((int)centrality / 5 + 1);
		Float_t Qycor = meanQy->GetBinContent((int)centrality / 5 + 1);
		curQ.Recenter(Qxcor, Qycor);
		Float_t PsiEP = curQ.GetEventPlaneAngle();
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
			//2sub
			momentum = tr->GetMomentum();
			momentum.Boost(b);
			Float_t pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());
			if (pt > 2.0) continue;	//pt>2 -> do not fill hists. same next 2 lines.
			if (momentum.Rapidity() < -0.95) continue;
			if (momentum.Rapidity() > 1.05) continue;
			Float_t phi = tr->GetPhi();
			vn = cos(1 * (phi - PsiEP));
			if (momentum.Rapidity() < 0) vn = -vn;
			int binCent, binPt, binRap;	//start from 0. it is hist number.
			binCent = (int)centrality / 5;
			binPt = (int)(pt * 10);
			binRap = (int)((momentum.Rapidity() + 0.95) * 10);
			v1PtCent2r[binPt][binCent]->Fill(momentum.Rapidity(), vn / res2[binCent]);
			v1PtCent1s[binPt][binCent]->Fill(momentum.Rapidity(), vn / res3[0][binCent]);
			v1PtCent2s[binPt][binCent]->Fill(momentum.Rapidity(), vn / res3[1][binCent]);
			v1PtCent3s[binPt][binCent]->Fill(momentum.Rapidity(), vn / res3[2][binCent]);
			v1RapCent2r[binRap][binCent]->Fill(pt, vn / res2[binCent]);
			v1RapCent1s[binRap][binCent]->Fill(pt, vn / res3[0][binCent]);
			v1RapCent2s[binRap][binCent]->Fill(pt, vn / res3[1][binCent]);
			v1RapCent3s[binRap][binCent]->Fill(pt, vn / res3[2][binCent]);
		}
	}
	std::cerr << "Flow found" << std::endl;
	f->Close();
	delete f;
	delete ev;
	delete[] st;

	TFile* w = new TFile(Form("f/Flov%d.root", current_file_number % 10 + 48), "recreate");	//if error occurred to save data...
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

	current_file_number++;
	if (current_file_number == all_files.size()) goto SaveFile;	//went through the entire list
	current_file = all_files.at(current_file_number);	//else...
	if (current_file.size() < 5) goto SaveFile;					//usual stop. Last line is empty one.
	goto NextFile;

SaveFile:
	TFile* s = new TFile("Flov_v1.root", "recreate");
	s->cd();
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
	s->Close();
	delete s;
	//std::cerr << "save done" << std::endl;
	//std::cerr << "program complete" << std::endl;
}