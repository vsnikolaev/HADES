void reader()
{
    auto fStyle = new TFile("style.root");
    auto style = (TStyle*) fStyle->Get("style");
    gROOT->ForceStyle();
    style->cd();
    fStyle->Close();
    auto f = new TFile("out.root");
    auto t = (TTree*) f->Get("DataTree");
    auto ev = new DataTreeEvent;
    auto DTEvent = (TBranch*) t->GetBranch("DTEvent");
    DTEvent->SetAddress(&ev);



    TH2F*  betta_vs_pt = new TH2F("1/β vs p","betta vs momentum",1000,0,5,1000,0,5);
    betta_vs_pt->GetXaxis()->SetTitle("momentum, p GeV/c");
    betta_vs_pt->GetYaxis()->SetTitle("1/β");
    
    TH1F*  m_ent = new TH1F("m entries","mass",1000,-0.4,1.4);
    m_ent->GetXaxis()->SetTitle("massa, m, GeV/c^2");

    TH2F*  m_ent_vs_pt = new TH2F("m vs p","mass vs momentum",1000,0.2,2.5,1000,-0.4,1.4);
    m_ent_vs_pt->GetXaxis()->SetTitle("momentum, p GeV/c");
    m_ent_vs_pt->GetYaxis()->SetTitle("m, GeV/c^2");

    TH2F*  m_vs_pq = new TH2F("mass^2 vs p*q","massa vs momentum",1000,-5,5,1000,-2,2);
    m_vs_pq->GetXaxis()->SetTitle("momentum*q, p*q GeV/c");
    m_vs_pq->GetYaxis()->SetTitle("m^2, (GeV/c^2)^2");
    
    TH1F*  id_hist = new TH1F("id entries","mass",100,0,2500);
    id_hist->GetXaxis()->SetTitle("particle mc Id, Id");

    TH1F*  length_hist = new TH1F("L","lenth of path",1000,0,0);
    length_hist->GetXaxis()->SetTitle("Length");

    TH1F*  time_hist = new TH1F("t","time of flight",1000,0,0);
    time_hist->GetXaxis()->SetTitle("time");

    TH1F*  betta_hist = new TH1F("β","betta=L/t",1000,0,0);
    betta_hist->GetXaxis()->SetTitle("betta");

    TH1F*  betta2_hist = new TH1F("β2","betta=L/(300*t)",1000,0,0);
    betta2_hist->GetXaxis()->SetTitle("betta");
   

    TH1F*  cent_hist = new TH1F("cent","cent",1000,0,0);
    cent_hist->GetXaxis()->SetTitle("cent");

//pi
    TH1F*  m1_ent = new TH1F("m pi+ entries","mass pi+",1000,-0.4,0);
    m1_ent->GetXaxis()->SetTitle("massa, m, GeV/c^2");

    TH1F*  m4_ent = new TH1F("m pi- entries","mass pi-",1000,0,0.4);
    m1_ent->GetXaxis()->SetTitle("massa, m, GeV/c^2");
//k
    TH1F*  m2_ent = new TH1F("m k entries","mass k",1000,0.3,0.7);
    m2_ent->GetXaxis()->SetTitle("massa, m, GeV/c^2");
//p
    TH1F*  m3_ent = new TH1F("m p entries","mass p",1000,0.6,1.4);
    m3_ent->GetXaxis()->SetTitle("massa, m, GeV/c^2");

    Long64_t N_events = (Long64_t) t->GetEntries();
    Float_t betta, length, time, charge, Z, p, m;
    Float_t sigma0=0.19, sigmap=0.01, sigma, centrality;
    DataTreeTrack* tr;
    DataTreeTOFHit* tof_hit;
    float counter=0;
    int counter2=0, counter3=0;
    std::cout<<"/";
    for(int i=0;i<N_events;i++)
    {
        counter =(float) i / (float)N_events;
        if (100*counter>=counter3+10*counter2) {
            std::cout<<".";
            counter2++;
            if (counter2==10){
                counter2=0;
                counter3++;
                std::cout<<"/"<<counter3<<"0%;"<<std::endl;
                std::cout<<"/";
            }
        }
        DTEvent->GetEntry(i);
        centrality=ev->GetCentrality();
        cent_hist->Fill(centrality);
        if (centrality<1 || centrality>44) continue;
        Int_t n_tracks = ev->GetNVertexTracks();
        for(int j=0; j<n_tracks; j++)
        {
            tof_hit = ev->GetTOFHit(j);
            tr = ev->GetVertexTrack(j);
            p = tr->GetP();
          //  if (p<0.2) continue;
            Int_t status=tof_hit->GetStatus();
            //if (status)   status=1 TOF status=0 RPC
            charge = tof_hit->GetCharge();
            Z = charge/fabs(charge);
            length = tof_hit->GetPathLength();
            if (length>2450) continue;
            time = tof_hit->GetTime();
            sigma=sqrt(time*time*sigmap*sigmap+sigma0*sigma0);
            betta=length/time;
            betta_hist->Fill(betta);
            betta=betta/300;
            betta2_hist->Fill(betta);
            betta=1.0/betta;

            length_hist->Fill(length);
            time_hist->Fill(time);
            betta_vs_pt->Fill(p, betta);

            if (betta<1) continue;
            m=p*p*(betta*betta-1);
            m_vs_pq->Fill(Z*p, m);
            m=p*sqrt(betta*betta - 1);
            m = m*Z;
            const Float_t mpi=0.14,mp=0.94,mk=0.49;
            m_ent->Fill(m);
            m_ent_vs_pt->Fill(p, m);
            if (m<0)
                m1_ent->Fill(m);
            else if (m>0 && m<0.4)
                m4_ent->Fill(m);
            if (m>0.3 && m<0.7)
                m2_ent->Fill(m);
            if (m>0.6 && m<1.4)
                m3_ent->Fill(m);
/*                //identification
                Float_t sigmabetta = sigma*length / (300*time*time);
                Float_t sigmam = p/300*sigmabetta*2*betta/sqrt(betta*betta-1);
                Int_t Id=0;
                if (m-sigmam<mpi && m+sigmam>mpi) Id=211;
                else if(m-sigmam<mk && m+sigmam>mk) Id=2212;
                else if(m-sigmam<mp && m+sigmam>mp) Id=321;
                if (Id) id_hist->Fill(Id);
*/
        }
    }

//    histo_PsiRP->Draw();

//h->Fit("gaus","V","E1",-1,1.5);
    TFile* w = new TFile("histo.root","recreate");
    w->cd();
    betta_vs_pt->Write();
    m_ent->Write();
    m_ent_vs_pt->Write();
    m_vs_pq->Write();
    id_hist->Write();
    length_hist->Write();
    time_hist->Write();
    betta_hist->Write();
    betta2_hist->Write();
    cent_hist->Write();
    m1_ent->Write();
    m4_ent->Write();
    m2_ent->Write();
    m3_ent->Write();
    style->Write();
    w->Close();
}
