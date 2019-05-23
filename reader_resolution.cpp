#include "Qvector.h"
#include "triggers.h"

void reader_resolution() {

	int current_file_number = 0;
	std::vector<std::string> all_files;
	read_header_file(all_files);
	if (all_files.size() == 0) return;
	std::string current_file;
	current_file = all_files.at(current_file_number);

	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 14, 0, 70);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 14, 0, 70);
	TProfile* resolu = new TProfile("mean cos", "cos from centrality", 14, 0, 70);
	TProfile* realre = new TProfile("res2s", "2 rand", 14, 0, 70);

	TProfile*  Qx3sub1 = new TProfile("Qx 3sub vs Centrality 1", "Qx sub1", 14, 0, 70);
	TProfile*  Qx3sub2 = new TProfile("Qx 3sub vs Centrality 2", "Qx sub2", 14, 0, 70);
	TProfile*  Qx3sub3 = new TProfile("Qx 3sub vs Centrality 3", "Qx sub3", 14, 0, 70);
	TProfile*  Qy3sub1 = new TProfile("Qy 3sub vs Centrality 1", "Qy sub1", 14, 0, 70);
	TProfile*  Qy3sub2 = new TProfile("Qy 3sub vs Centrality 2", "Qy sub2", 14, 0, 70);
	TProfile*  Qy3sub3 = new TProfile("Qy 3sub vs Centrality 3", "Qy sub3", 14, 0, 70);

	TProfile*  cos10 = new TProfile("cos10 vs Centrality 3 sub", "cos21", 14, 0, 70);
	TProfile*  cos20 = new TProfile("cos20 vs Centrality 3 sub", "cos31", 14, 0, 70);
	TProfile*  cos12 = new TProfile("cos12 vs Centrality 3 sub", "cos32", 14, 0, 70);

	TProfile* res3s1 = new TProfile("res3s1", "sub 1", 14, 0, 70);
	TProfile* res3s2 = new TProfile("res3s2", "sub 2", 14, 0, 70);
	TProfile* res3s3 = new TProfile("res3s3", "sub 3", 14, 0, 70);

	TProfile* resolusum = new TProfile("cos2", "cos 2 rand", 14, 0, 70);
	TProfile* resum     = new TProfile("res2ssum", "2 rand", 14, 0, 70);
	TProfile* cos10s    = new TProfile("s cos10 vs Centrality 3 sub", "cos21", 14, 0, 70);
	TProfile* cos20s    = new TProfile("s cos20 vs Centrality 3 sub", "cos31", 14, 0, 70);
	TProfile* cos12s    = new TProfile("s cos12 vs Centrality 3 sub", "cos32", 14, 0, 70);
	TProfile* r3s1sum   = new TProfile("res3s1sum", "sub 1", 14, 0, 70);
	TProfile* r3s2sum   = new TProfile("res3s2sum", "sub 2", 14, 0, 70);
	TProfile* r3s3sum   = new TProfile("res3s3sum", "sub 3", 14, 0, 70);

	TTree* t;
	TBranch* DTEvent;
	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events;
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off (all particles)

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
	resolu->Reset();
	Qx3sub1->Reset();
	Qx3sub2->Reset();
	Qx3sub3->Reset();
	Qy3sub1->Reset();
	Qy3sub2->Reset();
	Qy3sub3->Reset();
	cos10->Reset();
	cos20->Reset();
	cos12->Reset();
	std::cerr << "Running file: " << current_file << std::endl;

	N_events = t->GetEntries();
	//if (N_events > 5000) N_events = 5000;		//for testing
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
	std::cerr << "mean found" << std::endl;

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
	for (int i = 0; i < 14; i++) {
		Float_t cos3[3], cos3err[3], res, err;	//3sub resolutions
		cos3[0] = cos10->GetBinContent(i + 1);
		cos3[1] = cos20->GetBinContent(i + 1);
		cos3[2] = cos12->GetBinContent(i + 1);
		cos3err[0] = cos10->GetBinError(i + 1);
		cos3err[1] = cos20->GetBinError(i + 1);
		cos3err[2] = cos12->GetBinError(i + 1);
		for (int j = 0; j < 3; j++) {	//take errors and enter them.
			res = Get_resolution(cos3[0], cos3[1], cos3[2], j);
			if (res != res) continue;
			err = Get_error_resolution(cos3[0], cos3[1], cos3[2], cos3err[0], cos3err[1], cos3err[2], j);
			err = sqrt(2)*err;
			switch (j)
			{
			case 0:
				res3s1->Fill(i * 5 + 2, res + err);
				res3s1->Fill(i * 5 + 2, res - err);
				break;
			case 1:
				res3s2->Fill(i * 5 + 2, res + err);
				res3s2->Fill(i * 5 + 2, res - err);
				break;
			case 2:
				res3s3->Fill(i * 5 + 2, res + err);
				res3s3->Fill(i * 5 + 2, res - err);
			default:
				break;
			}
		}
		res = resolu->GetBinContent(i + 1);		//2sub res
		err = Get_error_res2sub(res, resolu->GetBinError(i + 1));
		err = sqrt(2)*err;
		res = Get_bessel_resolution(res);
		if (res != res) continue;
		realre->Fill(i * 5 + 2, res + err);
		realre->Fill(i * 5 + 2, res - err);
	}
	resolusum->Add(resolu);
	cos10s->Add(cos10);
	cos20s->Add(cos20);
	cos12s->Add(cos12);
	std::cerr << "resolution found" << std::endl;
	f->Close();
	delete f;
	delete ev;
	delete[] st;

	TFile* w = new TFile(Form("r/res%d.root", current_file_number), "recreate");	//if error occurred to save data...
	meanQx->Write();	//2sub
	meanQy->Write();
	resolu->Write();
	realre->Write();
	Qx3sub1->Write();	//3sub
	Qx3sub2->Write();
	Qx3sub3->Write();
	Qy3sub1->Write();
	Qy3sub2->Write();
	Qy3sub3->Write();
	cos10->Write();
	cos20->Write();
	cos12->Write();
	res3s1->Write();
	res3s2->Write();
	res3s3->Write();
	w->Close();
	delete w;

	current_file_number++;
	if (current_file_number == all_files.size()) goto SaveFile;	//went through the entire list
	current_file = all_files.at(current_file_number);			//else...
	if (current_file.size() < 5) goto SaveFile;					//usual stop. Last line is empty one. or line st.
	goto NextFile;

SaveFile:
	for (int i = 0; i < 14; i++) {
		float res = resolusum->GetBinContent(i + 1);		//2sub res
		float err = resolusum->GetBinError(i + 1);
		err = Get_error_res2sub(res, err);
		res = Get_bessel_resolution(res);
		if (res != res) continue;
		resum->Fill(i * 5 + 2, res + sqrt(2)*err);
		resum->Fill(i * 5 + 2, res - sqrt(2)*err);	
		float cos3[3], cos3err[3];							//3sub res
		cos3[0] = cos10s->GetBinContent(i + 1);
		cos3[1] = cos20s->GetBinContent(i + 1);
		cos3[2] = cos12s->GetBinContent(i + 1);
		cos3err[0] = cos10s->GetBinError(i + 1);
		cos3err[1] = cos20s->GetBinError(i + 1);
		cos3err[2] = cos12s->GetBinError(i + 1);
		for (int j = 0; j < 3; j++) {
			res = Get_resolution(cos3[0], cos3[1], cos3[2], j);
			if (res != res) continue;
			err = Get_error_resolution(cos3[0], cos3[1], cos3[2], cos3err[0], cos3err[1], cos3err[2], j);
			err = sqrt(2)*err;
			switch (j)
			{
			case 0:
				r3s1sum->Fill(i * 5 + 2, res + err);
				r3s1sum->Fill(i * 5 + 2, res - err);
				break;
			case 1:
				r3s2sum->Fill(i * 5 + 2, res + err);
				r3s2sum->Fill(i * 5 + 2, res - err);
				break;
			case 2:
				r3s3sum->Fill(i * 5 + 2, res + err);
				r3s3sum->Fill(i * 5 + 2, res - err);
			default:
				break;
			}
		}
	}

	TFile* s = new TFile("resolutions.root", "recreate");
	s->cd();
	resolusum->Write();
	resum->Write();
	cos10s->Write();
	cos20s->Write();
	cos12s->Write();
	r3s1sum->Write();
	r3s2sum->Write();
	r3s3sum->Write();
	s->Close();
	delete s;
}
