//file that contains some useful functions
#include "ChainBuilder.h"
#include "Cumulant.h"

namespace utils
{
   //___________________________________________________
   //
   // Setup branche names
   //___________________________________________________
   std::vector<TString> 
   SetupBranchName(int harm0, int harm1, int nsub, std::string suffix = "")
   {
      std::vector<TString> brnames;
      brnames.push_back(Form("%sC%d%d%d_17",  suffix.c_str(), harm0, harm0, 2));
      brnames.push_back(Form("%sC%d%d%d_51",  suffix.c_str(), harm0, harm1, 4));

      if( harm0 == harm1 )
      {
         brnames.push_back(Form("%sC%d%d%d_119", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d",     suffix.c_str(), harm0, harm1, 8));
      }

      if( ( nsub <= 2 && harm0 != harm1 ) || ( nsub > 2 ) )
      {
         brnames.push_back(Form("%sC%d%d%d_33", suffix.c_str(), harm0, harm1, 2));
         brnames.push_back(Form("%sC%d%d%d_18", suffix.c_str(), harm1, harm0, 2));
         brnames.push_back(Form("%sC%d%d%d_34", suffix.c_str(), harm1, harm1, 2));
      }
      if( nsub > 2 && harm0 == harm1 )
      {
         brnames.push_back(Form("%sC%d%d%d_20", suffix.c_str(), harm0, harm1, 2));
         brnames.push_back(Form("%sC%d%d%d_36", suffix.c_str(), harm0, harm1, 2));
         brnames.push_back(Form("%sC%d%d%d_53",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_58",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_60",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_83",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_85",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_90",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_99",  suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_101", suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_108", suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_163", suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_165", suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_170", suffix.c_str(), harm0, harm1, 4));
         brnames.push_back(Form("%sC%d%d%d_123", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d_125", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d_183", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d_187", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d_231", suffix.c_str(), harm0, harm1, 6));
         brnames.push_back(Form("%sC%d%d%d_237", suffix.c_str(), harm0, harm1, 6));
      }

      return brnames;
   }

   //___________________________________________________
   //
   // Setup TChain branches
   //___________________________________________________
   void
   SetupBranch(TChain* ch, 
               int& noff, int& mult,
               std::vector<double>& CNM, std::vector<double>& wCNM,
               int harm0, int harm1, int nsub)
   {
      LOG_S(INFO) << "Trying to get branch 'Noff'";

      ch->SetBranchStatus("*", 0);
      ch->SetBranchStatus("Noff", 1);
      if(!ch->GetBranch("Noff"))
      {
         LOG_S(ERROR) << "Branch 'Noff' does not exist!!! Code stopped";
         throw std::string("Branch error type broke the code");
         return;
      }
      else
      {
         ch->SetBranchAddress("Noff", &noff);
      }
      LOG_S(INFO) << "Trying to get branch 'Mult'";
      ch->SetBranchStatus("Mult", 1);
      if(!ch->GetBranch("Mult"))
      {
         LOG_S(ERROR) << "Branch 'Mult' does not exist!!! Code stopped";
         throw std::string("Branch error type broke the code");
         return;
      }
      else
      {
         ch->SetBranchAddress("Mult", &mult);
      }

      std::vector<TString> brnames  = SetupBranchName(harm0, harm1, nsub);
      std::vector<TString> wbrnames = SetupBranchName(harm0, harm1, nsub, "w");


      for(int ibr = 0; ibr < static_cast<int>(brnames.size()); ibr++)
      {
         LOG_S(INFO) << "Trying to get branch '" << brnames[ibr].Data() << "'"; 
         ch->SetBranchStatus(brnames[ibr].Data(), 1);
         if(!ch->GetBranch(brnames[ibr]))
         {
            LOG_S(ERROR) << "Branch '" << brnames[ibr] << "' does not exist!!! Code stopped";
            throw std::string("Branch error type broke the code");
            return;
         }
         else
         {
            ch->SetBranchAddress(brnames[ibr], &CNM[ibr]);
         }
         LOG_S(INFO) << "Trying to get branch '" << wbrnames[ibr].Data() << "'";
         ch->SetBranchStatus(wbrnames[ibr].Data(), 1);
         if(!ch->GetBranch(wbrnames[ibr]))
         {
            LOG_S(ERROR) << "Branch '" << wbrnames[ibr].Data() << "' does not exist!!! Code stopped";
            throw std::string("Branch error type broke the code");
            return;
         }
         else
         {
            ch->SetBranchAddress(wbrnames[ibr], &wCNM[ibr]);
         }
      }
   }

   //___________________________________________________
   //
   // loop on TChain: correlator calculation
   //___________________________________________________
   void
   loopOnChain(TChain* ch,  int harm0,   int harm1,
               int noffmin, int noffmax,
               TH1I* &hNoff, TH1I* &hMult,  
               std::vector< std::vector< std::vector<double> > > &qNM,
               std::vector< std::vector< std::vector<double> > > &wqNM,
               int analyzedEvts, int nsub)
   {
      int noff = 0, mult = 0;
      std::vector<double>  CNM( static_cast<int>( qNM.size()), 0.);
      std::vector<double> wCNM( static_cast<int>(wqNM.size()), 0.);

      // Init branches
      SetupBranch(ch, noff, mult, CNM, wCNM, harm0, harm1, nsub);

      // Get N entries
      int ntrees = ch->GetNtrees();
      LOG_S(INFO) << "Number of Tree in the TChain: " << ntrees;

      if( analyzedEvts == 0 || analyzedEvts < -1 )
      {
         LOG_S(ERROR) << "No entries in this TTree! Are you kidding ME?";
         LOG_S(ERROR) << "Please check you are using the right FOLDER that contains the TTree you want to look at!!!";
         return;
      }
      else if( analyzedEvts == -1 )
      {
         LOG_S(INFO) << "Analyze all events!!!";
      }
      else
      {
         LOG_S(INFO) << "Number of events that will be analyzed: " << analyzedEvts;
      }

      // Loop over events
      int ievt = 0;
      //int nevt = ch->GetEntriesFast();
      while ( (ch->GetEntry(ievt) && ievt <= analyzedEvts) ||
              (ch->GetEntry(ievt) && analyzedEvts == -1)      ) 
      {
         if(!(ievt%10000000))
         {
            //std::cout << 
            //"\rievt = " << ievt 
            //<<
            //", tree number = " << ch->GetTreeNumber()
            //<<
            //" ~~~> " << std::setprecision(3) << (static_cast<double>(ch->GetTreeNumber())/static_cast<double>(ntrees))*100.  << " %" 
            //<< std::flush;
            //std::cout << 
            //"\rievt = " << ievt 
            //<<
            //", nevt = " << nevt
            //<<
            //" ~~~> " << std::setprecision(3) << (static_cast<double>(ievt)/static_cast<double>(nevt))*100.  << " %" 
            //<< std::flush;
         }
        
         //Skip event if out of range
         if(noff < noffmin || noff >= noffmax)
         {
            ++ievt;
            continue;
         }

         hNoff->Fill(noff);
         hMult->Fill(mult);

         // Compute cumulants
         for(int ibr = 0; ibr < static_cast<int>(CNM.size()); ibr++)
         {
           if( noff < qNM[ibr].size() )
           {
              if( mult < qNM[ibr][noff].size() )
              {
                  qNM[ibr][noff][mult] +=  CNM[ibr];
                 wqNM[ibr][noff][mult] += wCNM[ibr];
              }
           }
         }        
         // Next event
         ++ievt;
      }

      // Free memory from huge vectors
      CNM.clear();
      wCNM.clear();
      std::vector<double>().swap(CNM);       
      std::vector<double>().swap(wCNM); 
      std::cout << std::endl;
   }

   //___________________________________________________
   //
   // rebin results
   //___________________________________________________
   void
   rebinning(const std::vector< std::vector< std::vector<double> > > &qNM,
             const std::vector< std::vector< std::vector<double> > > &wqNM,
             std::vector< std::vector<double> > &cNM,
             std::vector< std::vector<double> > &wcNM,
             std::vector< std::vector<double> > &cNMreb,
             std::vector< std::vector<double> > &wcNMreb,
             int nbins, int binarray[], int nsub, int harm0, int harm1, bool quiet = false)
   {
      if(!quiet) LOG_S(INFO) << "Combining multiplicity bin for each N_{trk}^{offline} bin";
      for(unsigned int iord = 0; iord < static_cast<unsigned int>(cNM.size()); ++iord)
      {
         for(unsigned int inoff = 0; inoff < static_cast<int>(qNM[iord].size()); ++inoff)
         {
            for(unsigned int iref = 0; iref < static_cast<int>(qNM[iord][inoff].size()); ++iref)
            {
               if(iref < 2 && 2*iord+2 >= 2)
               {
                  continue;
               }
               else if(iref < 4 && 2*iord+2 >= 4)
               {
                  continue;
               }
               else if(iref < 6 && 2*iord+2 >= 6)
               {
                  continue;
               }
               else if(iref < 8 && 2*iord+2 >= 8)
               {
                  continue;
               }
               else cumulant(qNM, wqNM, cNM, wcNM, iord, harm0, harm1, inoff, iref, nsub);
            }
         }
      }

      if(!quiet) LOG_S(INFO) << "Rebinning in larger N_{trk}_{offline} bins now";
      LOG_S(INFO) << "SIZE cNM = " << cNM.size() << ", SIZE cNMreb = " << cNMreb.size();
      for(unsigned int iord = 0; iord < static_cast<unsigned int>(cNMreb.size()); ++iord)
      {
         int inoff = 0;
         for(unsigned int ibin = 0; ibin < nbins; ++ibin)
         {
            while( inoff >= binarray[ibin] && inoff < binarray[ibin+1] )
            {
                  cNMreb[iord][ibin]  += cNM[iord][inoff];
                  wcNMreb[iord][ibin] += wcNM[iord][inoff];
                  ++inoff;
            }

            if(wcNMreb[iord][ibin] != 0.) cNMreb[iord][ibin] /= wcNMreb[iord][ibin];
            else                          cNMreb[iord][ibin] = 0.;
         }

         for(unsigned int inoff = 0; inoff < static_cast<int>(qNM[0].size()); ++inoff)
         {
            if(wcNM[iord][inoff] != 0.) cNM[iord][inoff] /= wcNM[iord][inoff];
            else                        cNM[iord][inoff] = 0.;
         }
      }
   }

   //___________________________________________________
   //
   // Setup histo
   //___________________________________________________
   void
   setupHisto(std::vector<TH1D*> &hist, int marker, int noffmin, int noffmax, std::string title)
   {
      for(int iord = 0; iord < hist.size(); ++iord)
      {
         hist[iord]    = new TH1D(Form("%s%d",title.c_str(),2*iord+2), "", 
                                  noffmax, 0, noffmax);
         hist[iord]->SetMarkerStyle(marker);
         hist[iord]->SetMarkerColor(iord+1);
         hist[iord]->SetLineColor(iord+1);
      }
   }

   void
   setupHisto(std::vector<TH1D*> &hist, int marker, int nbins, double table[], std::string title)
   {
      for(int iord = 0; iord < hist.size(); ++iord)
      {
         hist[iord]    = new TH1D(Form("%s%d",title.c_str(),2*iord+2), "", 
                                 nbins, table);
         hist[iord]->SetMarkerStyle(marker);
         hist[iord]->SetMarkerColor(iord+1);
         hist[iord]->SetLineColor(iord+1);
      }
   }

   //___________________________________________________
   //
   // fill histograms
   //___________________________________________________
   void
   fillHistograms(const std::vector< std::vector<double> >& cNM, 
                  const std::vector< std::vector<double> >& wcNM, 
                  const std::vector< std::vector<double> >& cNMreb, 
                  const std::vector< std::vector<double> >& wcNMreb, 
                  std::vector<TH1D*> hcN_num, 
                  std::vector<TH1D*> hcN_den,
                  std::vector<TH1D*> hcNreb_num, 
                  std::vector<TH1D*> hcNreb_den,
                  std::vector<TH1D*> hcN, 
                  std::vector<TH1D*> hcNreb,
                  std::vector<TH1D*> hvN, 
                  std::vector<TH1D*> hvNreb, int nsub)
   {
      for(int iord = 0; iord < cNM.size(); ++iord)
      {
         for(int ibin = 0; ibin < hcN[iord]->GetNbinsX(); ++ibin)
         {
            if(ibin < 2 && iord >= 2) 
            {
               hcN_num[iord]->SetBinContent(ibin+1, 0.);
               hcN_den[iord]->SetBinContent(ibin+1, 0.);
               hcN[iord]->SetBinContent(ibin+1, 0.);
               hvN[iord]->SetBinContent(ibin+1, 0.);
            }
            else if(ibin < 4 && iord >= 4) 
            {
               hcN_num[iord]->SetBinContent(ibin+1, 0.);
               hcN_den[iord]->SetBinContent(ibin+1, 0.);
               hcN[iord]->SetBinContent(ibin+1, 0.);
               hvN[iord]->SetBinContent(ibin+1, 0.);
            }
            else if(ibin < 6 && iord >= 6) 
            {
               hcN_num[iord]->SetBinContent(ibin+1, 0.);
               hcN_den[iord]->SetBinContent(ibin+1, 0.);
               hcN[iord]->SetBinContent(ibin+1, 0.);
               hvN[iord]->SetBinContent(ibin+1, 0.);
            }
            else if(ibin < 8 && iord >= 8) 
            {
               hcN_num[iord]->SetBinContent(ibin+1, 0.);
               hcN_den[iord]->SetBinContent(ibin+1, 0.);
               hcN[iord]->SetBinContent(ibin+1, 0.);
               hvN[iord]->SetBinContent(ibin+1, 0.);
            }
            else{ 
               hcN_num[iord]->SetBinContent(ibin+1, cNM[iord][ibin] * wcNM[iord][ibin]);
               hcN_den[iord]->SetBinContent(ibin+1, wcNM[iord][ibin]);

               if( cNM[iord][ibin] != 0. ) 
                  hcN[iord]->SetBinContent(ibin+1, cNM[iord][ibin]);
               double vn = computeVn(iord, hcN[iord]->GetBinContent(ibin+1), nsub);
               if( vn != 0. ) hvN[iord]->SetBinContent(ibin+1, vn);
            }
         }

         for(int ibin = 0; ibin < hcNreb[iord]->GetNbinsX(); ++ibin)
         {
            hcNreb_num[iord]->SetBinContent(ibin+1, cNMreb[iord][ibin] * wcNMreb[iord][ibin]);
            hcNreb_den[iord]->SetBinContent(ibin+1, wcNMreb[iord][ibin]);

            if( cNMreb[iord][ibin] != 0. ) 
               hcNreb[iord]->SetBinContent(ibin+1, cNMreb[iord][ibin]);
            double vn =  computeVn(iord, hcNreb[iord]->GetBinContent(ibin+1), nsub);
            if( vn != 0. ) hvNreb[iord]->SetBinContent(ibin+1, vn);
         }
      }
   }

   //___________________________________________________
   //
   // process TChain
   //___________________________________________________
   void 
   process(TChain* ch, TFile* fout, 
           TString folder,
           int noffmin,      int noffmax, int multmax, 
           int cumumaxorder, int harm0,   int harm1, 
           int nbins,        int binarray[], 
           int analyzedEvts, int nsub) 
   {
      LOG_S(INFO) << "Number of trees in the TChain: " << ch->GetNtrees();
      //if ( ch->GetNtrees() == 1 ) 
      //   LOG_S(INFO) << "Number of events: " << ch->GetEntriesFast();
      //LOG_S(INFO) << "Number of events: " << ch->GetEntries();
      LOG_S(INFO) << "Maximum cumulant order to be computed: " << cumumaxorder;

      //int nbranches = ch->GetNbranches() - 4;
      int nbranches = SetupBranchName(harm0, harm1, nsub).size();
      //int nbranches = 10;
      LOG_S(INFO) << "Number of branches in TTrees: "<< nbranches;

      //init vectors
      //  -- 2D vector correlator (numerator and denominator)
      std::vector < std::vector< std::vector<double> > >  
      qNM( nbranches,
           std::vector< std::vector<double> > ( binarray[nbins], 
                                                std::vector<double>(multmax, 0.) ) );
      std::vector < std::vector< std::vector<double> > >  
      wqNM( nbranches,
            std::vector< std::vector<double> > ( binarray[nbins], 
                                                 std::vector<double>(multmax, 0.) ) );

      // -- cumulant after multipilicity bin re-combination
      std::vector < std::vector<double> >  
      cNM( cumumaxorder / 2,
           std::vector<double>( binarray[nbins], 0.) );
      std::vector < std::vector<double> >  
      wcNM( cumumaxorder / 2,
            std::vector<double>( binarray[nbins], 0.) );
      // -- cumulant after rebinning in noff
      std::vector < std::vector<double> >  
      cNMreb( cumumaxorder / 2,
              std::vector<double>( nbins, 0.) );
      std::vector < std::vector<double> >  
      wcNMreb( cumumaxorder / 2,
               std::vector<double>( nbins, 0.) );

      // -- Control histograms
      TH1I* hNoff = new TH1I("hNoff", "N_{off}", noffmax, 0, noffmax);
      TH1I* hMult = new TH1I("hMult", "multiplicity", 800, 0, 800);

      //Loop on chain an fill stuff!!!
      try
      {
         LOG_S(INFO) << "Looping on TChain...";
         loopOnChain(ch,           harm0,   harm1, 
                     noffmin,      noffmax,
                     hNoff,        hMult, 
                     qNM,          wqNM, 
                     analyzedEvts, nsub);
      }
      catch(std::string const& e)
      {
         LOG_S(ERROR) << "Be careful, something might break";
         return;
      }

      //Rebinning
      LOG_S(INFO) << "Rebinning cumulants...";
      rebinning(qNM, wqNM, cNM, wcNM, cNMreb, wcNMreb, nbins, binarray, nsub, harm0, harm1);

      //Fill histograms
      LOG_S(INFO) << "Fill histograms";
      // -- init histograms
      // ---- c_{n} num/dem
      std::vector<TH1D*> hcN_num;
      hcN_num.resize(cNM.size());
      std::vector<TH1D*> hcN_den;
      hcN_den.resize(cNM.size());
      std::vector<TH1D*> hcNreb_num;
      hcNreb_num.resize(cNMreb.size());
      std::vector<TH1D*> hcNreb_den;
      hcNreb_den.resize(cNMreb.size());
      // ---- c_{n}
      std::vector<TH1D*> hcN;
      hcN.resize(cNM.size());
      std::vector<TH1D*> hcNreb;
      hcNreb.resize(cNMreb.size());
      // ---- v_{n}
      std::vector<TH1D*> hvN;
      hvN.resize(cNM.size());
      std::vector<TH1D*> hvNreb;
      hvNreb.resize(cNMreb.size());

      // ---- Create a temporaty table for binning
      double *tmp = new double[nbins+1];
      for(int i = 0; i < nbins+1; ++i)
      {
         tmp[i] = (double) binarray[i];
      }

      // ---- Setup histo
      setupHisto(hcN_num,    25, 0,     noffmax, Form("hC%d%d_num",    harm0, harm1));
      setupHisto(hcN_den,    26, 0,     noffmax, Form("hC%d%d_den",    harm0, harm1));
      setupHisto(hcNreb_num, 25, nbins, tmp,     Form("hC%d%dreb_num", harm0, harm1));
      setupHisto(hcNreb_den, 26, nbins, tmp,     Form("hC%d%dreb_den", harm0, harm1));

      setupHisto(hcN,    20, 0,     noffmax, Form("hC%d%d",    harm0, harm1));
      setupHisto(hcNreb, 24, nbins, tmp,     Form("hCreb%d%d", harm0, harm1));
      setupHisto(hvN,    20, 0,     noffmax, Form("hV%d%d",    harm0, harm1));
      setupHisto(hvNreb, 24, nbins, tmp,     Form("hVreb%d%d", harm0, harm1));

      // ---- fill them 
      fillHistograms(cNM,       wcNM,       cNMreb, wcNMreb, 
                     hcN_num,    hcN_den, 
                     hcNreb_num, hcNreb_den, 
                     hcN,        hcNreb, 
                     hvN,        hvNreb, 
                     nsub);
      delete[] tmp;

      // ---- Write histo
      fout->cd();
      hNoff->Write();
      hMult->Write();
      for(int iord = 0; iord < cNM.size(); ++iord)
      {
         hcN_num[iord]->Write();
         hcN_den[iord]->Write();
         hcNreb_num[iord]->Write();
         hcNreb_den[iord]->Write();
         hcN[iord]   ->Write();
         hcNreb[iord]->Write();
         hvN[iord]   ->Write();
         hvNreb[iord]->Write();
      }

      // Free memory from huge vectors
      for(int i = 0; i < qNM.size(); ++i)
      {
         for(int j = 0; j < qNM.size(); ++j)
         {
            qNM[i][j].clear();
            wqNM[i][j].clear();
         }
         qNM[i].clear();
         wqNM[i].clear();
      }

      qNM.clear();
      wqNM.clear();
      std::vector< std::vector< std::vector<double> > >().swap(qNM);       
      std::vector< std::vector< std::vector<double> > >().swap(wqNM);
      for(int i = 0; i < cNM.size(); ++i)
      {
         cNM[i].clear();
         wcNM[i].clear();
      }
      cNM.clear();
      wcNM.clear();
      std::vector< std::vector<double> >().swap(cNM);       
      std::vector< std::vector<double> >().swap(wcNM); 
      for(int i = 0; i < cNMreb.size(); ++i)
      {
         cNMreb[i].clear();
         wcNMreb[i].clear();
      }
      cNMreb.clear();
      wcNMreb.clear();
      std::vector< std::vector<double> >().swap(cNMreb);       
      std::vector< std::vector<double> >().swap(wcNMreb); 
   }
}
