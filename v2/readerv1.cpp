#include "Qvector.h"
#include "triggers.h"
//pt сделать из 10 бинов 40
void readerv1() {
	auto f = new TFile("out.root");
	auto t = (TTree*)f->Get("DataTree");
	auto ev = new DataTreeEvent;
	auto DTEvent = (TBranch*)t->GetBranch("DTEvent");
	DTEvent->SetAddress(&ev);

	//including histograms.
	TProfile* meanQx = new TProfile("MeanQx vs Centrality", "centrality mean Qx", 14, 0, 70);
	TProfile* meanQy = new TProfile("MeanQy vs Centrality", "centrality mean Qy", 14, 0, 70);
	TProfile* resolu = new TProfile("Resolution vs Centrality", "centrality cos", 14, 0, 70);	//mean cos()
	TProfile* sqrtre = new TProfile("mean sqrt(cos) vs centrality", "centrality mean sqrt(cos)", 14, 0, 70);
	TProfile* realre = new TProfile("real resolution vs centrality", "centrality mean resolution", 14, 0, 70);
	TProfile* v2obse = new TProfile("v2 observable vs Centrality", "centrality mean v2 obs", 14, 0, 70);	//del later?
	TProfile* v2real = new TProfile("real v2 vs Centrality", "centrality mean v2 real", 14, 0, 70);			//del later?

	//3sub
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

	TProfile* v23sub1 = new TProfile("v2 real 3sub vs Centrality class 1", "real v2 centrality  sub eve 1", 14, 0, 70);
	TProfile* v23sub2 = new TProfile("v2 real 3sub vs Centrality class 2", "real v2 centrality  sub eve 2", 14, 0, 70);
	TProfile* v23sub3 = new TProfile("v2 real 3sub vs Centrality class 3", "real v2 centrality  sub eve 3", 14, 0, 70);

	TProfile* v2real21  = new TProfile("mean v2 cent10 pt, 2 rand", "2 rand, 00-10", 10, 0, 2);
	TProfile* v2real22  = new TProfile("mean v2 cent15 pt, 2 rand", "2 rand, 10-15", 10, 0, 2);
	TProfile* v2real23  = new TProfile("mean v2 cent20 pt, 2 rand", "2 rand, 15-20", 10, 0, 2);
	TProfile* v2real24  = new TProfile("mean v2 cent25 pt, 2 rand", "2 rand, 20-25", 10, 0, 2);
	TProfile* v2real25  = new TProfile("mean v2 cent30 pt, 2 rand", "2 rand, 25-30", 10, 0, 2);
	TProfile* v2real26  = new TProfile("mean v2 cent35 pt, 2 rand", "2 rand, 30-35", 10, 0, 2);
	TProfile* v2real27  = new TProfile("mean v2 cent40 pt, 2 rand", "2 rand, 35-40", 10, 0, 2);
	TProfile* v2real28  = new TProfile("mean v2 cent45 pt, 2 rand", "2 rand, 40-45", 10, 0, 2);
	TProfile* v2real29  = new TProfile("mean v2 cent50 pt, 2 rand", "2 rand, 45-50", 10, 0, 2);
	TProfile* v2real210 = new TProfile("mean v2 cent55 pt, 2 rand", "2 rand, 50-55", 10, 0, 2);
	TProfile* v2real211 = new TProfile("mean v2 cent60 pt, 2 rand", "2 rand, 55-60", 10, 0, 2);
	TProfile* v2real212 = new TProfile("mean v2 cent65 pt, 2 rand", "2 rand, 60-65", 10, 0, 2);
	TProfile* v2real213 = new TProfile("mean v2 cent70 pt, 2 rand", "2 rand, 65-70", 10, 0, 2);

	TProfile* v2real311  = new TProfile("mean v2 cent10 pt, 1 sub 3r", "1sub, 00-10", 10, 0, 2);
	TProfile* v2real321  = new TProfile("mean v2 cent10 pt, 2 sub 3r", "2sub, 00-10", 10, 0, 2);
	TProfile* v2real331  = new TProfile("mean v2 cent10 pt, 3 sub 3r", "3sub, 00-10", 10, 0, 2);
	TProfile* v2real312  = new TProfile("mean v2 cent15 pt, 1 sub 3r", "1sub, 10-15", 10, 0, 2);
	TProfile* v2real322  = new TProfile("mean v2 cent15 pt, 2 sub 3r", "2sub, 10-15", 10, 0, 2);
	TProfile* v2real332  = new TProfile("mean v2 cent15 pt, 3 sub 3r", "3sub, 10-15", 10, 0, 2);
	TProfile* v2real313  = new TProfile("mean v2 cent20 pt, 1 sub 3r", "1sub, 15-20", 10, 0, 2);
	TProfile* v2real323  = new TProfile("mean v2 cent20 pt, 2 sub 3r", "2sub, 15-20", 10, 0, 2);
	TProfile* v2real333  = new TProfile("mean v2 cent20 pt, 3 sub 3r", "3sub, 15-20", 10, 0, 2);
	TProfile* v2real314  = new TProfile("mean v2 cent25 pt, 1 sub 3r", "1sub, 20-25", 10, 0, 2);
	TProfile* v2real324  = new TProfile("mean v2 cent25 pt, 2 sub 3r", "2sub, 20-25", 10, 0, 2);
	TProfile* v2real334  = new TProfile("mean v2 cent25 pt, 3 sub 3r", "3sub, 20-25", 10, 0, 2);
	TProfile* v2real315  = new TProfile("mean v2 cent30 pt, 1 sub 3r", "1sub, 25-30", 10, 0, 2);
	TProfile* v2real325  = new TProfile("mean v2 cent30 pt, 2 sub 3r", "2sub, 25-30", 10, 0, 2);
	TProfile* v2real335  = new TProfile("mean v2 cent30 pt, 3 sub 3r", "3sub, 25-30", 10, 0, 2);
	TProfile* v2real316  = new TProfile("mean v2 cent35 pt, 1 sub 3r", "1sub, 30-35", 10, 0, 2);
	TProfile* v2real326  = new TProfile("mean v2 cent35 pt, 2 sub 3r", "2sub, 30-35", 10, 0, 2);
	TProfile* v2real336  = new TProfile("mean v2 cent35 pt, 3 sub 3r", "3sub, 30-35", 10, 0, 2);
	TProfile* v2real317  = new TProfile("mean v2 cent40 pt, 1 sub 3r", "1sub, 35-40", 10, 0, 2);
	TProfile* v2real327  = new TProfile("mean v2 cent40 pt, 2 sub 3r", "2sub, 35-40", 10, 0, 2);
	TProfile* v2real337  = new TProfile("mean v2 cent40 pt, 3 sub 3r", "3sub, 35-40", 10, 0, 2);
	TProfile* v2real318  = new TProfile("mean v2 cent45 pt, 1 sub 3r", "1sub, 40-45", 10, 0, 2);
	TProfile* v2real328  = new TProfile("mean v2 cent45 pt, 2 sub 3r", "2sub, 40-45", 10, 0, 2);
	TProfile* v2real338  = new TProfile("mean v2 cent45 pt, 3 sub 3r", "3sub, 40-45", 10, 0, 2);
	TProfile* v2real319  = new TProfile("mean v2 cent50 pt, 1 sub 3r", "1sub, 45-50", 10, 0, 2);
	TProfile* v2real329  = new TProfile("mean v2 cent50 pt, 2 sub 3r", "2sub, 45-50", 10, 0, 2);
	TProfile* v2real339  = new TProfile("mean v2 cent50 pt, 3 sub 3r", "3sub, 45-50", 10, 0, 2);
	TProfile* v2real3110 = new TProfile("mean v2 cent55 pt, 1 sub 3r", "1sub, 50-55", 10, 0, 2);
	TProfile* v2real3210 = new TProfile("mean v2 cent55 pt, 2 sub 3r", "2sub, 50-55", 10, 0, 2);
	TProfile* v2real3310 = new TProfile("mean v2 cent55 pt, 3 sub 3r", "3sub, 50-55", 10, 0, 2);
	TProfile* v2real3111 = new TProfile("mean v2 cent60 pt, 1 sub 3r", "1sub, 55-60", 10, 0, 2);
	TProfile* v2real3211 = new TProfile("mean v2 cent60 pt, 2 sub 3r", "2sub, 55-60", 10, 0, 2);
	TProfile* v2real3311 = new TProfile("mean v2 cent60 pt, 3 sub 3r", "3sub, 55-60", 10, 0, 2);
	TProfile* v2real3112 = new TProfile("mean v2 cent65 pt, 1 sub 3r", "1sub, 60-65", 10, 0, 2);
	TProfile* v2real3212 = new TProfile("mean v2 cent65 pt, 2 sub 3r", "2sub, 60-65", 10, 0, 2);
	TProfile* v2real3312 = new TProfile("mean v2 cent65 pt, 3 sub 3r", "3sub, 60-65", 10, 0, 2);
	TProfile* v2real3113 = new TProfile("mean v2 cent70 pt, 1 sub 3r", "1sub, 65-70", 10, 0, 2);
	TProfile* v2real3213 = new TProfile("mean v2 cent70 pt, 2 sub 3r", "2sub, 65-70", 10, 0, 2);
	TProfile* v2real3313 = new TProfile("mean v2 cent70 pt, 3 sub 3r", "3sub, 65-70", 10, 0, 2);

	TProfile* v2realrapid = new TProfile("mean v2 cent30-40 rap, 2 rand", "2 rand rap, 30-40", 10, -1, 1);

	//temp hists
	//just some test hists
	TH1F* histQx = new TH1F("Qx", "Qx centrality 20-30", 1000, -1, 1);
	TH1F* histQxr = new TH1F("Qxr", "Qx centrality 20-30 recentered", 1000, -1, 1);
	TH1F* histQy = new TH1F("Qy", "Qy centrality 20-30", 1000, -1, 1);
	TH1F* histQyr = new TH1F("Qyr", "Qy centrality 20-30 recentered", 1000, -1, 1);
	TH1F* histQx1 = new TH1F("Qx1", "Qx centrality 10-15", 1000, -1, 1);
	TH1F* histQxr1 = new TH1F("Qxr1", "Qx centrality 10-15 recentered", 1000, -1, 1);
	TH1F* histQy1 = new TH1F("Qy1", "Qy centrality 10-15", 1000, -1, 1);
	TH1F* histQyr1 = new TH1F("Qyr1", "Qy centrality 10-15 recentered", 1000, -1, 1);

	TH2F* xymodules = new TH2F("xy", "x vs y", 100, -1000, 1000, 100, -1000, 1000);
	
	TH1F* histhit = new TH1F("hits", "hits", 500, 1, 1);
	TH1F* histmom = new TH1F("momentum", "momentum", 500, 1, 1);
	TH1F* histerr = new TH1F("errr", "errr", 2, 0, 2);
	TH1F* histrap = new TH1F("rapid", "rapid", 500, -1, 1);

	TH1F* histbeta = new TH1F("beta", "beta", 200, 0, 1);
	//end temp
	DataTreeTrack* tr;
	DataTreeTOFHit* tof_hit;
	DataTreePSDModule* m_psd;
	Int_t N_events = t->GetEntries();
	Float_t centrality;
	nTriggers particle;
	particle.SetParticle(14);	//-2 off (all particles)

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
			histerr->Fill(1);
			continue;
		}
		meanQx->Fill(centrality, curQ.GetComponent(1));
		meanQy->Fill(centrality, curQ.GetComponent(2));
		//del later.
		histQx1->Fill(curQ.GetComponent(1));
		histQy1->Fill(curQ.GetComponent(2));
		if (centrality >= 20 && centrality <= 30) { 
			histQx->Fill(curQ.GetComponent(1));
			histQy->Fill(curQ.GetComponent(2));
		}
		Int_t N_psd_modules = ev->GetNPSDModules();
		for (int i = 0; i < N_psd_modules; i++) {
			m_psd = ev->GetPSDModule(i);
			Int_t x, y;
			x = m_psd->GetPositionComponent(0);
			y = m_psd->GetPositionComponent(1);
			xymodules->Fill(x, y);
			histbeta->Fill(m_psd->GetBeta());
		}
		//end del later
		Subevent3 cursub3;		//3sub
		if(!curQ.Fillsub3(ev)) continue;
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
			err = Get_error_resolution(cos3[0], cos3[1], cos3[2], cos3err[0], cos3err[1], cos3err[2], j);
			if (res != res) continue;	//del?
			if (res == 0) continue;		//del?
			switch (j)
			{
			case 0:
				res3s1->Fill(i * 5 + 2, res + sqrt(2)*err);
				res3s1->Fill(i * 5 + 2, res - sqrt(2)*err);
				break;
			case 1:
				res3s2->Fill(i * 5 + 2, res + sqrt(2)*err);
				res3s2->Fill(i * 5 + 2, res - sqrt(2)*err);
				break;
			case 2:
				res3s3->Fill(i * 5 + 2, res + sqrt(2)*err);
				res3s3->Fill(i * 5 + 2, res - sqrt(2)*err);
			default:
				break;
			}
		}		
		res = resolu->GetBinContent(i+1);		//2sub res
		sqrtre->Fill(i * 5 + 2, sqrt(res) + sqrt(2)*resolu->GetBinError(i + 1));
		sqrtre->Fill(i * 5 + 2, sqrt(res) - sqrt(2)*resolu->GetBinError(i + 1));
		err = Get_error_res2sub(res, resolu->GetBinError(i + 1));
		res = Get_bessel_resolution(res);
		realre->Fill(i * 5 + 2, res + sqrt(2)*err);
		realre->Fill(i * 5 + 2, res - sqrt(2)*err);
	}
	std::cerr << "resolution found" << std::endl;

	//v1
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
		//del later
		histQxr1->Fill(curQ.GetComponent(1));
		histQyr1->Fill(curQ.GetComponent(2));
		if (centrality >= 20 && centrality <= 30) {
			histQxr->Fill(curQ.GetComponent(1));
			histQyr->Fill(curQ.GetComponent(2));
		}
		//end del later
		Float_t PsiEP = curQ.GetEventPlaneAngle();
		Float_t vn = 0;
		Float_t res = realre->GetBinContent((int)centrality / 5 + 1);
		Float_t res3[3];
		TLorentzVector momentum;
		double BETA = sqrt(1.0 - 0.938*0.938 / 1.23 / 1.23);
		TVector3 b; b.SetXYZ(0, 0, -BETA);
		res3[0] = res3s1->GetBinContent((int)centrality / 5 + 1);
		res3[1] = res3s2->GetBinContent((int)centrality / 5 + 1);
		res3[2] = res3s3->GetBinContent((int)centrality / 5 + 1);
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
			histmom->Fill(momentum.Rapidity());
			//temp rap
			Float_t pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());
			Float_t phi = tr->GetPhi();
			vn = cos(1 * (phi - PsiEP));
			if (res > 0) 
				if (centrality>20 && centrality<30)
					v2realrapid->Fill(momentum.Rapidity(), vn / res);
			//end temp
			if (momentum.Rapidity()<-0.25 || momentum.Rapidity()>-0.15) continue;
			histrap->Fill(momentum.Rapidity());
			//Float_t pt = sqrt(tr->GetPx() * tr->GetPx() + tr->GetPy() * tr->GetPy());
			//Float_t phi = tr->GetPhi();
			//vn = cos(1 * (phi - PsiEP));
			vn = vn*(-1);
			v2obse->Fill(centrality, vn);
			if (res > 0) {
				v2real->Fill(centrality, vn / res);
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
					//break;		//temp coment
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
				case 10:
					v2real210->Fill(pt, vn / res);
					break;
				case 11:
					v2real211->Fill(pt, vn / res);
					break;
				case 12:
					v2real212->Fill(pt, vn / res);
					break;
				case 13:
					v2real213->Fill(pt, vn / res);
					break;
				default:
					break;
				}
			}
			//3 sub
			vn = cos(1 * (phi - PsiEP));
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
				//break;				//temp coment
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
				v2real318->Fill(pt, vn / res3[0]);
				v2real328->Fill(pt, vn / res3[1]);
				v2real338->Fill(pt, vn / res3[2]);
				break;
			case 9:
				v2real319->Fill(pt, vn / res3[0]);
				v2real329->Fill(pt, vn / res3[1]);
				v2real339->Fill(pt, vn / res3[2]);
				break;
			case 10:
				v2real3110->Fill(pt, vn / res3[0]);
				v2real3210->Fill(pt, vn / res3[1]);
				v2real3310->Fill(pt, vn / res3[2]);
				break;
			case 11:
				v2real3111->Fill(pt, vn / res3[0]);
				v2real3211->Fill(pt, vn / res3[1]);
				v2real3311->Fill(pt, vn / res3[2]);
				break;
			case 12:
				v2real3112->Fill(pt, vn / res3[0]);
				v2real3212->Fill(pt, vn / res3[1]);
				v2real3312->Fill(pt, vn / res3[2]);
				break;
			case 13:
				v2real3113->Fill(pt, vn / res3[0]);
				v2real3213->Fill(pt, vn / res3[1]);
				v2real3313->Fill(pt, vn / res3[2]);
				break;
			default:
				break;
			}
		}
	}
	std::cerr << "v1 found" << std::endl;
	
	//save
	TFile* w = new TFile("v1b.root", "recreate");
	w->cd();
	meanQx->Write();
	meanQy->Write();
	resolu->Write();
	sqrtre->Write();
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
	v2real210->Write();
	v2real211->Write();
	v2real212->Write();
	v2real213->Write();

	Qx3sub1->Write();
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

	v23sub1->Write();
	v23sub2->Write();
	v23sub3->Write();

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
	v2real319->Write();
	v2real329->Write();
	v2real339->Write();
	v2real3110->Write();
	v2real3210->Write();
	v2real3310->Write();
	v2real3111->Write();
	v2real3211->Write();
	v2real3311->Write();
	v2real3112->Write();
	v2real3212->Write();
	v2real3312->Write();
	v2real3113->Write();
	v2real3213->Write();
	v2real3313->Write();
	v2realrapid->Write();

	histQx->Write();
	histQxr->Write();
	histQy->Write();
	histQyr->Write();
	histQx1->Write();
	histQxr1->Write();
	histQy1->Write();
	histQyr1->Write();
	histmom->Write();
	xymodules->Write();
	histerr->Write();
	histrap->Write();
	histbeta->Write();

	w->Close();
	
	std::cerr << "save done" << std::endl;
	std::cerr << "program complete" << std::endl;
}