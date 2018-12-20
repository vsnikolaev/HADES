void proj() {
	TFile *f2 = new TFile("nvs2.root","READ");
	TH2F *h = (TH2F*)f2 -> Get("mass^2 vs p*q;1");
	TFile *f1 = new TFile("m2qp_py.root","recreate");
	TF1 *m1 = new TF1("m1","gaus",-0.2,0.15);
	TF1 *m3 = new TF1("m3","gaus",0.7,1.2);
	TF1 *total = new TF1("mstotal","gaus(0)+gaus(3)+gaus(6)",-0.2,1.2);
	Double_t par[9];

	for (int i = 535; i < 610; i += 3 ) { 
		//cout << i << endl;
		TH1D *projY = h -> ProjectionY("_py", i, i + 5);

		projY->Fit(m1,"R");
		projY->Fit(m3,"R+");
		m1->GetParameters(&par[0]);
		m3->GetParameters(&par[3]);
		total->SetParameters(par);
		projY->Fit(total,"R+");
		projY -> Write();
	}
	f1 -> Close();
}


