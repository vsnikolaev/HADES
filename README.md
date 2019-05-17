# HADES
Base distribution
readerv1.c 	find resolution and flows, but works only with one file...
reader_resolution.cpp 	find resolutions SE method. It requires the full path to the trees at file header_file.txt. Before launching, create the directory 'r' in the program folder
reader_flows.cpp		find flows. It requires the full path to the trees at file header_file.txt. Before running this file run reader_resolution.cpp.
triggers.cxx 	select particle, track and event selecting
Qvector.cxx 	work with Q-vectors.
rootlogon.c		File where you must write your path to DataTree. 

How to use:
1)Write rite your path to DataTree at rootlogon.c
2)Write full path to the trees at file header_file.txt
3)(Optional) Сhange the tree name in the 5th line of the file readerv1.c
4)Run:
root rootlogon.c readerv1.c 
or 
mkdir r
root rootlogon.c reader_resolution.cpp
root rootlogon.c reader_flows.cpp