//Before running make list of files. at terminal write.
// find $PWD -type f | cat > list_of_files.txt
// if need also dirs(just helpful comand): find $PWD -type f -o -type d


void read_header_file(std::vector<std::string> &_p);
Float_t Get_resolution(Float_t a, Float_t b, Float_t c, Int_t subnumber);
Float_t Get_error_resolution(Float_t a, Float_t b, Float_t c, Float_t a_er, Float_t b_er, Float_t c_er, Int_t subnumber);
Float_t Get_bessel_resolution(Float_t a);

Float_t Get_res2sub(Float_t a) {
	return sqrt(2 * a);
}

Float_t Get_error_res2sub(Float_t a, Float_t a_er) {
	return sqrt(1.0 / (4.0 * a)) * a_er;
}



void sumres() {
	//to make list of files write: find $PWD -type f | cat > list_of_files.txt
	int current_file_number = 0;
	std::vector<std::string> all_files;
	read_header_file(all_files);
	if (all_files.size() == 0) return;
	std::string current_file;
	current_file = all_files.at(current_file_number);

	TProfile* resolusum = new TProfile("cos2", "cos 2 rand", 14, 0, 70);		//temp hists.
	TProfile* cos10s = new TProfile("s cos10 vs Centrality 3 sub", "cos21", 14, 0, 70);
	TProfile* cos20s = new TProfile("s cos20 vs Centrality 3 sub", "cos31", 14, 0, 70);
	TProfile* cos12s = new TProfile("s cos12 vs Centrality 3 sub", "cos32", 14, 0, 70);

	//open file + clear histograms.
	while (true) {
		TString stv2 = current_file;
		TFile* f = new TFile(stv2);
		TProfile* resolu = (TProfile*)f->Get("mean cos;1");
		TProfile* cos10 = (TProfile*)f->Get("cos10 vs Centrality 3 sub;1");
		TProfile* cos20 = (TProfile*)f->Get("cos20 vs Centrality 3 sub;1");
		TProfile* cos12 = (TProfile*)f->Get("cos12 vs Centrality 3 sub;1");
		resolusum->Add(resolu);
		cos10s->Add(cos10);
		cos20s->Add(cos20);
		cos12s->Add(cos12);

		f->Close();
		delete f;

		current_file_number++;
		if (current_file_number == all_files.size()) break;	//went through the entire list
		current_file = all_files.at(current_file_number);			//else...
		if (current_file.size() < 5) break;					//usual stop. Last line is empty one. or line st.
	}
	TProfile* resum = new TProfile("res2ssum", "2 rand", 14, 0, 70);			//final hists
	TProfile* r3s1sum = new TProfile("res3s1sum", "sub 1", 14, 0, 70);
	TProfile* r3s2sum = new TProfile("res3s2sum", "sub 2", 14, 0, 70);
	TProfile* r3s3sum = new TProfile("res3s3sum", "sub 3", 14, 0, 70);

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


void read_header_file(std::vector<std::string> &_p) {
	std::ifstream in("list_of_files.txt", ios::in);
	if (!in) {
		_p.clear();
		return;
	}
	std::string s;
	while (!in.eof()) {
		std::getline(in, s);
		_p.push_back(s);
	}
	in.close();
}




//input <cos(a)>, <cos(b)>, <cos(c)>, and subevent number;  
Float_t Get_resolution(Float_t a, Float_t b, Float_t c, Int_t subnumber) {
	switch (subnumber)
	{
	case 0: return sqrt((a * b) / c);
	case 1: return sqrt((c * a) / b);
	case 2: return sqrt((b * c) / a);
	default:
		return 0;
	}
}

//input <cos(a)>, <cos(b)>, <cos(c)>, errors a,b,c, and subevent number;  
Float_t Get_error_resolution(Float_t a, Float_t b, Float_t c, Float_t a_er, Float_t b_er, Float_t c_er, Int_t subnumber) {
	switch (subnumber)
	{
	case 0: return 1.0 / 2.0 * sqrt((a_er*a_er * b / (a*c)) + (b_er*b_er * a / (b*c)) + (c_er*c_er * a*b / pow(c, 3)));
	case 1: return 1.0 / 2.0 * sqrt((a_er*a_er * c / (a*b)) + (c_er*c_er * a / (b*c)) + (b_er*b_er * a*c / pow(b, 3)));
	case 2: return 1.0 / 2.0 * sqrt((c_er*c_er * b / (a*c)) + (b_er*b_er * c / (a*b)) + (a_er*a_er * b*c / pow(a, 3)));
	default:
		return 0;
	}
}

//input <cos(km(psia-psib)>
Float_t Get_bessel_resolution(Float_t a) {
	//if (a<0.4) return Get_res2sub(a);
	Float_t hi = 0;
	Float_t resolution;
	Float_t err = a / 100;
	for (; hi < 3; hi += 0.0001) {
		resolution = 0.626657*hi - 0.09694*pow(hi, 3) + 0.02754*pow(hi, 4) - 0.002283*pow(hi, 5);		//k==1, v1
		//resolution = 0.25*pow(hi, 2) - 0.011414*pow(hi, 3) - 0.034726*pow(hi, 4) + 0.006815*pow(hi, 5);	//k==2, v2
		if (abs(resolution - a) < err)
			break;
	}
	//std::cerr << hi << std::endl;
	if (hi >= 3) return Get_res2sub(a);
	hi = hi*sqrt(2);
	resolution = 0.626657*hi - 0.09694*pow(hi, 3) + 0.02754*pow(hi, 4) - 0.002283*pow(hi, 5);		//k==1, v1
	//resolution = 0.25*pow(hi, 2) - 0.011414*pow(hi, 3) - 0.034726*pow(hi, 4) + 0.006815*pow(hi, 5);	//k==2, v2
	resolution = sqrt(resolution);
	return resolution;
}