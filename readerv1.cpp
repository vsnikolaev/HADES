#include "Qvector.h"
#include "triggers.h"

void readerv1() {
	auto f = new TFile("out_1.root");
	auto t = (TTree*)f->Get("DataTree");
	auto ev = new DataTreeEvent;
	auto DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);

	//including histograms.
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 10, 0, 50);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 10, 0, 50);
	TProfile* resolu = new TProfile("Resolution vs Centrality", "centrality R_{2}", 10, 0, 50);
	TProfile* v2obse = new TProfile("v2 observable vs Centrality", "centrality mean v2 obs", 10, 0, 50);
	TProfile* v2real = new TProfile("real v2 vs Centrality", "centrality mean v2 real", 10, 0, 50);
	TProfile* realre = new TProfile("real resolution vs centrality", "centrality mean resolution", 10, 0, 50);

	TProfile2D* resolu2d = new TProfile2D("resolution vs Centrality and pt", "resolution ", 10, 0, 50, 10, 0, 2, 0, 20);
	TProfile2D* v2obse2d = new TProfile2D("v2 observable vs Centrality and pt", "v2 obs", 10, 0, 50, 10, 0, 2, 0, 20);
	TProfile2D* v2real2d = new TProfile2D("real v2 vs Centrality and pt", "v2 real", 10, 0, 50, 10, 0, 2, 0, 20);
	//3sub
	TProfile2D* Qx3sub = new TProfile2D("Qx 3sub vs Centrality class", "Qx centrality class number of sub eve", 10, 0, 50, 3, 0, 3, 0, 20);
	TProfile2D* Qy3sub = new TProfile2D("Qy 3sub vs Centrality class", "Qy centrality class number of sub eve", 10, 0, 50, 3, 0, 3, 0, 20);

	TProfile*  cos10 = new TProfile("cos10 vs Centrality 3 sub", "centrality cos10 3 subevent", 10, 0, 50);
	TProfile*  cos20 = new TProfile("cos20 vs Centrality 3 sub", "centrality cos20 3 subevent", 10, 0, 50);
	TProfile*  cos12 = new TProfile("cos12 vs Centrality 3 sub", "centrality cos12 3 subevent", 10, 0, 50);

	TProfile* res3s1 = new TProfile("res3s1", "centrality mean resolution 1", 10, 0, 50);
	TProfile* res3s2 = new TProfile("res3s2", "centrality mean resolution 2", 10, 0, 50);
	TProfile* res3s3 = new TProfile("res3s3", "centrality mean resolution 3", 10, 0, 50);



	TProfile* v23sub1 = new TProfile("v2 real 3sub vs Centrality class 1", "real v2 centrality  sub eve 1", 10, 0, 50);
	TProfile* v23sub2 = new TProfile("v2 real 3sub vs Centrality class 2", "real v2 centrality  sub eve 2", 10, 0, 50);
	TProfile* v23sub3 = new TProfile("v2 real 3sub vs Centrality class 3", "real v2 centrality  sub eve 3", 10, 0, 50);

	TProfile* v2real31 = new TProfile("mean v2 cent10 pt, 1 sub", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real32 = new TProfile("mean v2 cent25 pt, 1 sub", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real33 = new TProfile("mean v2 cent40 pt, 1 sub", "mean v2  pt, 3sub", 10, 0, 2);

	TProfile* v2real21 = new TProfile("mean v2 cent10 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real22 = new TProfile("mean v2 cent15 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real23 = new TProfile("mean v2 cent20 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real24 = new TProfile("mean v2 cent25 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real25 = new TProfile("mean v2 cent30 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real26 = new TProfile("mean v2 cent35 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real27 = new TProfile("mean v2 cent40 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real28 = new TProfile("mean v2 cent45 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);
	TProfile* v2real29 = new TProfile("mean v2 cent50 pt, 2 rand", "mean v2  pt, 2 rand", 10, 0, 2);


	TProfile* v2real311 = new TProfile("mean v2 cent10 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real321 = new TProfile("mean v2 cent10 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real331 = new TProfile("mean v2 cent10 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real312 = new TProfile("mean v2 cent15 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real322 = new TProfile("mean v2 cent15 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real332 = new TProfile("mean v2 cent15 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real313 = new TProfile("mean v2 cent20 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real323 = new TProfile("mean v2 cent20 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real333 = new TProfile("mean v2 cent20 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real314 = new TProfile("mean v2 cent25 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real324 = new TProfile("mean v2 cent25 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real334 = new TProfile("mean v2 cent25 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real315 = new TProfile("mean v2 cent30 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real325 = new TProfile("mean v2 cent30 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real335 = new TProfile("mean v2 cent30 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real316 = new TProfile("mean v2 cent35 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real326 = new TProfile("mean v2 cent35 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real336 = new TProfile("mean v2 cent35 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real317 = new TProfile("mean v2 cent40 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real327 = new TProfile("mean v2 cent40 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real337 = new TProfile("mean v2 cent40 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);
	TProfile* v2real318 = new TProfile("mean v2 cent50 pt, 1 sub 3r", "mean v2  pt, 1sub", 10, 0, 2);
	TProfile* v2real328 = new TProfile("mean v2 cent50 pt, 2 sub 3r", "mean v2  pt, 2sub", 10, 0, 2);
	TProfile* v2real338 = new TProfile("mean v2 cent50 pt, 3 sub 3r", "mean v2  pt, 3sub", 10, 0, 2);


	TProfile2D* resolution3sub = new TProfile2D("resolution 3sub vs Centrality class", "resolution centrality class number of sub eve", 10, 0, 50, 3, 0, 3, 0, 20);
	TProfile2D* v23sub = new TProfile2D("v2 real 3sub vs Centrality class", "real v2 centrality class number of sub eve", 10, 0, 50, 3, 0, 3, 0, 20);
	TProfile2D* v2real3 = new TProfile2D("mean v2 vs Centrality and pt, 3 sub", "mean v2 from centrality and pt, 3sub", 10, 0, 50, 10, 0, 2, 0, 20);
	//just some test hists
	TH1F* histQx = new TH1F("Qx", "Qx centrality 20-30", 100, -1, 1);
	TH1F* histQxr = new TH1F("Qxr", "Qx centrality 20-30 recentered", 100, -1, 1);
	TH1F* histQy = new TH1F("Qy", "Qy centrality 20-30", 100, -1, 1);
	TH1F* histQyr = new TH1F("Qyr", "Qy centrality 20-30 recentered", 100, -1, 1);
	TH1F* histQx1 = new TH1F("Qx1", "Qx centrality 10-15", 100, -1, 1);
	TH1F* histQxr1 = new TH1F("Qxr1", "Qx centrality 10-15 recentered", 100, -1, 1);
	TH1F* histQy1 = new TH1F("Qy1", "Qy centrality 10-15", 100, -1, 1);
	TH1F* histQyr1 = new TH1F("Qyr1", "Qy centrality 10-15 recentered", 100, -1, 1);

	TH2F* xymodules = new TH2F("xy", "x vs y", 100, -1000, 1000, 100, -1000, 1000);
	//temp hists
	TH1F* histhit = new TH1F("hits", "hits", 100, 1, 1);
	//end temp
	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events = t->GetEntries();
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off
	//find mean 2 rand subevent + 3 sub
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentralityEstimator(5);
		curQ.FindQ(ev);
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
		if (centrality >= 20 && centrality <= 30) { //del later.
			histQx->Fill(curQ.GetComponent(1));
			histQy->Fill(curQ.GetComponent(2));
		}
		if (centrality >= 10 && centrality <= 15) { //del later.
			histQx1->Fill(curQ.GetComponent(1));
			histQy1->Fill(curQ.GetComponent(2));
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
		if(!curQ.Fillsub3(ev)) continue;
		cursub3 = curQ.Getsub3();
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
		centrality = ev->GetCentralityEstimator(5);
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
		for (int j = 0; j < 3; j++) {
			curmeansub3.Qx[j] = Qx3sub->GetBinContent((int)centrality / 5 + 1, j + 1);
			curmeansub3.Qy[j] = Qy3sub->GetBinContent((int)centrality / 5 + 1, j + 1);
		}
		Float_t cos3[3];
		if (!curQ.Fillsub3(ev)) continue;
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
		Float_t cos3[3], cos3err[3], res, err;
		cos3[0] = cos10->GetBinContent(i + 1);
		cos3[1] = cos20->GetBinContent(i + 1);
		cos3[2] = cos12->GetBinContent(i + 1);
		cos3err[0] = cos10->GetBinError(i + 1);
		cos3err[1] = cos20->GetBinError(i + 1);
		cos3err[2] = cos12->GetBinError(i + 1);
		for (int j = 0; j < 3; j++) {	//take errors and enter them.
			res = Get_resolution(cos3[0], cos3[1], cos3[2], j);
			err = Get_error_resolution(cos3[0], cos3[1], cos3[2], cos3err[0], cos3err[1], cos3err[2], j);
			if (res != res) continue;
			if (res == 0) continue;
			resolution3sub->Fill(i * 5 + 2.5, j + 0.5, res, 1);
			resolution3sub->SetBinError(i + 1, j + 1, err, 0);
			if (j == 0) {
				res3s1->Fill(i * 5 + 2, res + err);
				res3s1->Fill(i * 5 + 2, res - err);
			}
			if (j == 1) {
				res3s2->Fill(i * 5 + 2, res + err);
				res3s2->Fill(i * 5 + 2, res - err);
			}
			if (j == 2) {
				res3s3->Fill(i * 5 + 2, res + err);
				res3s3->Fill(i * 5 + 2, res - err);
			}
		}
		//2subres
		res = resolu->GetBinContent(i+1);
		//err = Get_error_res2sub(res, resolu->GetBinError(i + 1));
		err = Get_error_res2sub(res, resolu->GetBinError(i + 1));
		//std::cerr << err << std::endl;
		res = Get_bessel_resolution(res);
		realre->Fill(i * 5 + 2, res + 2*err);
		realre->Fill(i * 5 + 2, res - 2*err);
		//realre->SetBinError(i+1, 0, err);
	}

	//v1
	for (Int_t i = 0; i < N_events; i++) {
		DTEvent->GetEntry(i);
		if (!(Correct_event(ev))) {
			continue;
		}
		Qvector curQ;
		centrality = ev->GetCentralityEstimator(5);
		curQ.FindQ(ev);
		Float_t Qxcor = meanQx->GetBinContent((int)centrality / 5 + 1);
		Float_t Qycor = meanQy->GetBinContent((int)centrality / 5 + 1);
		curQ.Recenter(Qxcor, Qycor);
		if (centrality >= 20 && centrality <= 30) {//del later
			histQxr->Fill(curQ.GetComponent(1));
			histQyr->Fill(curQ.GetComponent(2));
		}
		if (centrality >= 10 && centrality <= 15) {//del later
			histQxr1->Fill(curQ.GetComponent(1));
			histQyr1->Fill(curQ.GetComponent(2));
		}
		Float_t PsiEP = curQ.GetEventPlaneAngle();
		Float_t vn = 0;
		Float_t res = realre->GetBinContent((int)centrality / 5 + 1);
		Float_t res3[3];
		TLorentzVector momentum;
		res3[0] = resolution3sub->GetBinContent((int)centrality / 5 + 1, 1);
		res3[1] = resolution3sub->GetBinContent((int)centrality / 5 + 1, 2);
		res3[2] = resolution3sub->GetBinContent((int)centrality / 5 + 1, 3);
		Int_t n_tracks_selected = ev->GetNVertexTracks();
		for (int j = 0; j < n_tracks_selected; j++) {
			if (!(Correct_track(ev, j))) {
				continue;
			}
			tr = ev->GetVertexTrack(j);
			if (!particle.DeterminedParticle(tr)) {
				continue;
			}//2sub
			momentum = tr->GetMomentum();
			Float_t pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());
			Float_t phi = tr->GetPhi();
			vn = cos(1 * (phi - PsiEP));
			v2obse->Fill(centrality, vn);
			v2obse2d->Fill(centrality, pt, vn, 1);
			if (res > 0) {
				v2real->Fill(centrality, vn / res);
				v2real2d->Fill(centrality, pt, vn / res, 1);
				switch ((int)centrality / 5) {
				case 0:
				case 1:
					v2real21->Fill(pt, vn / res);
					break;
				case 2:
					v2real22->Fill(pt, vn / res);
					break;
				case 3:
					v2real23->Fill(pt, vn / res);
					break;
				case 4:
					v2real24->Fill(pt, vn / res);
					break;
				case 5:
					v2real25->Fill(pt, vn / res);
					break;
				case 6:
					v2real26->Fill(pt, vn / res);
					break;
				case 7:
					v2real27->Fill(pt, vn / res);
					break;
				case 8:
					v2real28->Fill(pt, vn / res);
					break;
				case 9:
					v2real29->Fill(pt, vn / res);
					break;
				default:
					break;
				}
			}
			//3 sub
			vn = cos(1 * (phi - PsiEP));
			v2real3->Fill(centrality, pt, vn / res3[0], 1);
			if (centrality <= 10) v2real31->Fill(pt, vn / res3[0]);
			else if (centrality<=25) v2real32->Fill(pt, vn / res3[0]);
			else v2real33->Fill(pt, vn / res3[0]);
			v23sub1->Fill(centrality, vn / res3[0]);
			v23sub2->Fill(centrality, vn / res3[1]);
			v23sub3->Fill(centrality, vn / res3[2]);
			switch ((int)centrality / 5) {
			case 0:
			case 1:
				v2real311->Fill(pt, vn / res3[0]);
				v2real321->Fill(pt, vn / res3[1]);
				v2real331->Fill(pt, vn / res3[2]);
				break;
			case 2:
				v2real312->Fill(pt, vn / res3[0]);
				v2real322->Fill(pt, vn / res3[1]);
				v2real332->Fill(pt, vn / res3[2]);
				break;
			case 3:
				v2real313->Fill(pt, vn / res3[0]);
				v2real323->Fill(pt, vn / res3[1]);
				v2real333->Fill(pt, vn / res3[2]);
				break;
			case 4:
				v2real314->Fill(pt, vn / res3[0]);
				v2real324->Fill(pt, vn / res3[1]);
				v2real334->Fill(pt, vn / res3[2]);
				break;
			case 5:
				v2real315->Fill(pt, vn / res3[0]);
				v2real325->Fill(pt, vn / res3[1]);
				v2real335->Fill(pt, vn / res3[2]);
				break;
			case 6:
				v2real316->Fill(pt, vn / res3[0]);
				v2real326->Fill(pt, vn / res3[1]);
				v2real336->Fill(pt, vn / res3[2]);
				break;
			case 7:
				v2real317->Fill(pt, vn / res3[0]);
				v2real327->Fill(pt, vn / res3[1]);
				v2real337->Fill(pt, vn / res3[2]);
				break;
			case 8:
			case 9:
				v2real318->Fill(pt, vn / res3[0]);
				v2real328->Fill(pt, vn / res3[1]);
				v2real338->Fill(pt, vn / res3[2]);
				break;
			default:
				break;
			}
			for (int k = 0; k < 3; k++) {
				if (res3[k] != res3[k]) continue;
				v23sub->Fill((int)centrality, k + 0.5, vn / res3[k], 1);
			}
		}
	}

	//save
	TFile* w = new TFile("v2.root", "recreate");
	w->cd();
	meanQx->Write();
	meanQy->Write();
	resolu->Write();
	realre->Write();
	v2obse->Write();
	v2real->Write();

	v2real21->Write();
	v2real22->Write();
	v2real23->Write();
	v2real24->Write();
	v2real25->Write();
	v2real26->Write();
	v2real27->Write();
	v2real28->Write();
	v2real29->Write();


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

	v23sub1->Write();
	v23sub2->Write();
	v23sub3->Write();
	v2real31->Write();
	v2real32->Write();
	v2real33->Write();

	v2real311->Write();
	v2real321->Write();
	v2real331->Write();
	v2real312->Write();
	v2real322->Write();
	v2real332->Write();
	v2real313->Write();
	v2real323->Write();
	v2real333->Write();
	v2real314->Write();
	v2real324->Write();
	v2real334->Write();
	v2real315->Write();
	v2real325->Write();
	v2real335->Write();
	v2real316->Write();
	v2real326->Write();
	v2real336->Write();
	v2real317->Write();
	v2real327->Write();
	v2real337->Write();
	v2real318->Write();
	v2real328->Write();
	v2real338->Write();

	histQx->Write();
	histQxr->Write();
	histQy->Write();
	histQyr->Write();
	histQx1->Write();
	histQxr1->Write();
	histQy1->Write();
	histQyr1->Write();
	xymodules->Write();

	res3s1->Write();
	res3s2->Write();
	res3s3->Write();
	w->Close();


	cos12->Draw();
	cos10->SetLineColor(2);
	cos10->Draw("same");
	cos20->SetLineColor(5);
	cos20->Draw("same");
	//Tlegend->Draw();
	

}