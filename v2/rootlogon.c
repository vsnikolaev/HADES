{
    gSystem->Load("/u/vnikolae/DataTree/build/libDataTree.so");
	gROOT->ProcessLine(".L triggers.h");
	gROOT->ProcessLine(".L triggers.cxx");
	gROOT->ProcessLine(".L Qvector.h");
	gROOT->ProcessLine(".L Qvector.cxx");
}
