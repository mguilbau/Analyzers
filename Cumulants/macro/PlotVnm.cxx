//C++ headers
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

//ROOT headers
#include <TStyle.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TFile.h>
#include <TProfile.h>
#include <TGraphErrors.h>

//USER headers
// logging library
#define LOGURU_IMPLEMENTATION 1
#include "../interface/MultiCumulants/vendor/loguru/loguru.hpp"
#include "../interface/MultiCumulants/vendor/cmdline.h"
//utlis include
#include "utils.h"

using namespace std;

int 
main(int argc, char** argv) {
//--------------------------------------------------------
//================ Init debug library ====================
//--------------------------------------------------------
	loguru::set_thread_name("MAIN");
	// logs everything to the debug.log file every run
	loguru::add_file("bin/debug.log", loguru::Truncate, loguru::Verbosity_MAX);

	// sometimes the "stream" form of the logger is more convenient, use LOG_S( LEVEL ) << "MESSAGE";
	// No need for an endl at the end of a line

//--------------------------------------------------------
//=========== Command parser configuration ===============
//--------------------------------------------------------
	cmdline::parser parser;
	parser.add<std::string>("output", '\0', "output file name and path", false, "../output/cnm_vnm.root");
	parser.add<std::string>("input",  '\0', "input file name and path", false, "../test/cumulants.root");
        parser.add<std::string>("folder", '\0', "folder of tree", false, "anaV2");
	parser.add<int>("noffmin"       , '\0', "minimum N_{trk}^offline", false, 0);
	parser.add<int>("noffmax"       , '\0', "maximum N_{trk}^offline", false, 800);
	parser.add<int>("cumumaxorder"  , '\0', "maximum cumulant order", false, 8);
	parser.add<int>("harmonicorder0", '\0', "harmonic order", false, 2);
        parser.add<int>("harmonicorder1", '\0', "harmonic order", false, 2);
	parser.add<int>("nevents"       , '\0', "Number of events to be analyzed", false, -1);
	parser.add<int>("subevt"        , '\0', "Number of sub-events", false, 1);
	parser.add("process"            , '\0', "process TTree");
	parser.parse_check( argc, argv );

	std::cout << parser.usage() << endl;

//--------------------------------------------------------
//================== Start Analysis ======================
//--------------------------------------------------------

        gStyle->SetOptStat(110);
        std::string inputFileNames = parser.get<std::string>( "input" );
        std::string outputFileName = parser.get<std::string>( "output" );
        std::string folderName = parser.get<std::string>( "folder" );

        const int noffmin = parser.get<int>( "noffmin" );
        const int noffmax = parser.get<int>( "noffmax" );
        const int multmax = 3*noffmax;
        const int cumumaxorder  = parser.get<int>( "cumumaxorder" );
        const int harmonicorder0 = parser.get<int>( "harmonicorder0" );
        const int harmonicorder1 = parser.get<int>( "harmonicorder1" );
        int nevents = parser.get<int>( "nevents" );
        int subevts = parser.get<int>( "subevt" );

//pPb 8TeV: 0, 10, 30, 50, 70, 90, 120, 150, 185, 220, 250, 300, 320, 350,
        const int nbins = 15;
        int binarray[nbins+1] = {0,   10,  30,  50,  70,  90,
                                 120, 150, 185, 190, 220, 250, 
                                 300, 320, 350, 400};

      //---------------------------------------------------------
      //================== Chainer builder ======================
      //---------------------------------------------------------

      ChainBuilder* b = new ChainBuilder();
      b->AddDir(inputFileNames);
      //b->AddDir("/eos/cms/store/group/phys_heavyions/flowcorr/SubCumu/PAHighMultiplicity7/*_std_*/*/*/*.root");
      //b->AddDir("/eos/cms/store/group/phys_heavyions/flowcorr/SubCumu/PAHighMultiplicity*/*_std_*/*/*/*.root");
      //b->AddDir("/eos/cms/store/group/phys_heavyions/flowcorr/SubCumu/PAHighMultiplicity7/RecoSkim2016_pPb250_cumulants_std_v10/171110_162518/0000/cumulants_std_299.root");
      b->ReadDir();
      b->PrintList();
      LOG_S(INFO) << "Building chain ";
      b->BuildChain(folderName);
      //b->GetChain()->Print();

      //Check that TChain is valid
      LOG_S(INFO) << "Getting chain ";
      TChain* ch = b->GetChain();
      LOG_S(INFO) << "Trying run on: " << ch->GetName();

      if( !ch )
      {
         LOG_S(ERROR) << "Invalid TChain";
         return 0;
      }
      else
      {
         if( nevents == -1 ) 
         {
            LOG_S(INFO) << "We will run on the full statistics of the TChain";
         }
         else
         {
            LOG_S(INFO) << "We will run over N_{evt} = " << nevents;
         }
      }


        //output file
        TFile* fout = 0x0;
        //if process, recreate output file
        fout = TFile::Open(outputFileName.c_str(), "RECREATE");

        //process the tree
        utils::process(ch, fout, folderName, 
                       noffmin, noffmax, multmax, 
                       cumumaxorder, harmonicorder0, harmonicorder1, 
                       nbins, binarray,
                       nevents, subevts);
        
        fout->Close();
        delete fout;

        if(!b)  delete b;
        if(!ch) delete ch;
 
        return 0;
}