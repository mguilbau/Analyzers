#ifndef SUBSAMP_H
#define SUBSAMP_H

//file that contains some useful functions
#include "ChainBuilder.h"
#include "Cumulant.h"

namespace subsample
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
      brnames.push_back(Form("%sC%d%d%d_119", suffix.c_str(), harm0, harm1, 6));
      brnames.push_back(Form("%sC%d%d%d",     suffix.c_str(), harm0, harm1, 8));

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
   // Get histograms from output file
   //___________________________________________________
   void GetHistogramValues(TFile* fout,
                           int harm0, int harm1,
                           std::vector<TH1D*> &hcN_num,
                           std::vector<TH1D*> &hcN_den,
                           std::vector<TH1D*> &hcN,
                           std::vector<TH1D*> &hvN,
                           std::vector<TH1D*> &hcNreb_num,
                           std::vector<TH1D*> &hcNreb_den,
                           std::vector<TH1D*> &hcNreb,
                           std::vector<TH1D*> &hvNreb)
   {
      // -- setup and get histos
      fout->cd();

      for(int iord = 0; iord < hcN_num.size(); ++iord)
      {
         hcN_num[iord]    = (TH1D*) fout->Get(Form("hC%d%d_num%d", harm0, harm1, 2*iord+2));
         hcN_den[iord]    = (TH1D*) fout->Get(Form("hC%d%d_den%d", harm0, harm1, 2*iord+2));
         hcNreb_num[iord] = (TH1D*) fout->Get(Form("hC%d%dreb_num%d", harm0, harm1, 2*iord+2));
         hcNreb_den[iord] = (TH1D*) fout->Get(Form("hC%d%dreb_den%d", harm0, harm1, 2*iord+2));
         hcN[iord]        = (TH1D*) fout->Get(Form("hC%d%d%d", harm0, harm1, 2*iord+2));
         hcNreb[iord]     = (TH1D*) fout->Get(Form("hCreb%d%d%d", harm0, harm1, 2*iord+2));
         hvN[iord]        = (TH1D*) fout->Get(Form("hV%d%d%d", harm0, harm1, 2*iord+2));
         hvNreb[iord]     = (TH1D*) fout->Get(Form("hVreb%d%d%d", harm0, harm1, 2*iord+2));
      }
   }

   //___________________________________________________
   //
   // Get bin from noff value
   //___________________________________________________
   int
   getNoffBin( int noff,
               int nbins, int binarray[] )
   {
      int bin = -1;
      for(int i = 0; i < nbins; ++i)
      {
         if( noff >= binarray[i] && noff < binarray[i+1] ) 
         {
           bin = i;
           continue;
         }
      }
      return bin;
   }

   //___________________________________________________
   //
   // LoopOnChain
   //___________________________________________________

   void
   loopOnChain(TChain* ch,  int harm0,   int harm1,
               int noffmin, int noffmax,
               int nSubSamp, 
               std::vector< std::vector< std::vector< std::vector<double> > > > &qNM,
               std::vector< std::vector< std::vector< std::vector<double> > > > &wqNM,
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
      while ( (ch->GetEntry(ievt) && ievt <= analyzedEvts) ||
              (ch->GetEntry(ievt) && analyzedEvts == -1)      ) 
      {
         if(!(ievt%1000))
         {
            std::cout << 
            "\rievt = " << ievt 
            <<
            ", tree number = " << ch->GetTreeNumber()
            <<
            " ~~~> " << std::setprecision(3) << (static_cast<double>(ch->GetTreeNumber())/static_cast<double>(ntrees))*100.  << " %" 
            << std::flush;
         }
        
         //Skip event if out of range
         if(noff < noffmin || noff >= noffmax)
         {
            ++ievt;
            continue;
         }

         // Variance
         int ntest = rand() % nSubSamp;
         for(int itest = 0; itest < nSubSamp; ++itest)
         {
            if(itest != ntest)
            {
               for(int ibr = 0; ibr < static_cast<int>(CNM.size()); ibr++)
               {
                 if( noff < qNM[ibr].size() )
                 {
                    if( mult < qNM[ibr][noff].size() )
                    {
                        qNM[itest][ibr][noff][mult] +=  CNM[ibr];
                       wqNM[itest][ibr][noff][mult] += wCNM[ibr];
                    }
                 }
               }
            }
         }
         // Next event
         ++ievt;
      }
      std::cout << std::endl;

      // Free memory from huge vectors
      LOG_S(INFO) << "Let's free some space and memory now";
      CNM.clear();
      wCNM.clear();
      std::vector<double>().swap(CNM);       
      std::vector<double>().swap(wCNM);
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
               cumulant(qNM, wqNM, cNM, wcNM, iord, harm0, harm1, inoff, iref, nsub);
            }
         }
      }

      if(!quiet) LOG_S(INFO) << "Rebinning in larger N_{trk}_{offline} bins now";
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
   // compute variance
   //___________________________________________________
   void
   getVariance(const std::vector< std::vector < std::vector<double> > > &cNM, 
               const std::vector< std::vector < std::vector<double> > > &cNMreb, 
               const std::vector<TH1D*> &hcN,
               const std::vector<TH1D*> &hcNreb,
               std::vector< std::vector<double> > &cNMvar, 
               std::vector< std::vector<double> > &cNMvarreb)
   {
      for(int itest = 0; itest < cNM.size(); ++itest)
      {
         for(int iord = 0; iord < cNM[itest].size(); ++iord)
         {
            for(int inoff = 0; inoff < cNM[itest][iord].size(); ++inoff)
            {
                 cNMvar[iord][inoff] += TMath::Power(cNM[itest][iord][inoff] - hcN[iord]->GetBinContent(inoff+1), 2);
            }
            for(int ibin = 0; ibin < cNMreb[itest][iord].size(); ++ibin)
            {
                 cNMvarreb[iord][ibin] += TMath::Power(cNMreb[itest][iord][ibin] - hcNreb[iord]->GetBinContent(ibin+1), 2);
            }
         }
      }
   } 

   //___________________________________________________
   //
   // process subsample
   //___________________________________________________
   void 
   process(TChain* ch, TFile* fout, 
           TString folder,
           int noffmin,      int noffmax, int multmax, 
           int cumumaxorder, int harm0,   int harm1, 
           int nbins,        int binarray[], 
           int analyzedEvts, int nsub,
           int nSubSamp) 
   {
      LOG_S(INFO) << "~~~~ Processing to Jacknife resampling now ~~~~";
      LOG_S(INFO) << "Number of trees in the TChain: " << ch->GetNtrees();
      LOG_S(INFO) << "Maximum cumulant order to be computed: " << cumumaxorder;

      int nbranches = ch->GetNbranches();
      LOG_S(INFO) << "Number of branches in TTrees: "<< nbranches;

      //init vectors
      //  -- 2D vector correlator (numerator and denominator)
      std::vector < std::vector < std::vector< std::vector<double> > > > 
      qNM( nSubSamp,
           std::vector< std::vector < std::vector<double> > > ( nbranches / 2,
                                                               std::vector< std::vector<double> > ( binarray[nbins], 
                                                                                                    std::vector<double>(multmax, 0.) ) ) );
      std::vector < std::vector < std::vector< std::vector<double> > > > 
      wqNM( nSubSamp,
            std::vector< std::vector < std::vector<double> > > ( nbranches / 2,
                                                                 std::vector< std::vector<double> > ( binarray[nbins], 
                                                                                                      std::vector<double>(multmax, 0.) ) ) );
      // -- cumulant after multipilicity bin re-combination
      std::vector < std::vector < std::vector<double> > >
      cNM( nSubSamp,
           std::vector< std::vector<double> > ( cumumaxorder / 2,
                                                std::vector<double>( binarray[nbins], 0.) ) );
      std::vector < std::vector < std::vector<double> > >
      wcNM( nSubSamp,
            std::vector< std::vector<double> > ( cumumaxorder / 2,
                                                 std::vector<double>( binarray[nbins], 0.) ) );
      // -- cumulant after rebinning in noff
      std::vector < std::vector < std::vector<double> > >
      cNMreb( nSubSamp,
              std::vector< std::vector<double> > ( cumumaxorder / 2,
                                                   std::vector<double>( nbins, 0.) ) );
      std::vector < std::vector < std::vector<double> > >
      wcNMreb( nSubSamp,
               std::vector< std::vector<double> > ( cumumaxorder / 2,
                                                    std::vector<double>( nbins, 0.) ) );
      // -- to store variance
      std::vector< std::vector<double> > 
      cNMvar( cumumaxorder / 2, 
              std::vector<double> ( binarray[nbins], 0.) );
      std::vector< std::vector<double> > 
      cNMvarreb( cumumaxorder / 2, 
                 std::vector<double> ( nbins, 0.) );


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
      // ---- c_{n} / v_{n}
      std::vector<TH1D*> hcN;
      hcN.resize(cNM.size());
      std::vector<TH1D*> hvN;
      hvN.resize(cNM.size());

      std::vector<TH1D*> hcNreb;
      hcNreb.resize(cNMreb.size());
      std::vector<TH1D*> hvNreb;
      hvNreb.resize(cNMreb.size());

      // -- grab values from histo
      GetHistogramValues(fout,       harm0,      harm1, 
                         hcN_num,    hcN_den,
                         hcN,        hvN,
                         hcNreb_num, hcNreb_den,
                         hcNreb,     hvNreb);


      // -- printout values
      /*for(int ibin = 0; ibin < nEvt.size(); ++ibin)
      {
        LOG_S(INFO) << "N_{evt}[" << ibin << "] = " << nEvt[ibin];
        for(int iord = 0; iord < cNM.size(); ++iord)
        LOG_S(INFO) << "	~~> cnm[" << iord << "] = " << cNM[iord][ibin]; 
      }*/

      // -- loop on chain
      LOG_S(INFO) << "Starting error estimation with Jacknife";
      try
      {
         loopOnChain(ch,      
                     harm0,   harm1, 
                     noffmin, noffmax,
                     nSubSamp,
                     qNM,     wqNM,     
                     analyzedEvts, nsub);
      }
      catch(std::string const& e)
      {
         LOG_S(ERROR) << "Be careful, something might break";
         return;
      }
      LOG_S(INFO) << "End of error estimation with Jacknife";

      //Rebinning
      LOG_S(INFO) << "Rebinning cumulants...";
      for(int itest = 0; itest < nSubSamp; ++itest)
      {
         rebinning(qNM[itest],    wqNM[itest], 
                   cNM[itest],    wcNM[itest], 
                   cNMreb[itest], wcNMreb[itest], 
                   nbins,         binarray, 
                   nsub, harm0, harm1);
      }

      //Compute variance
      getVariance(cNM,    cNMreb,  
                  hcN,    hcNreb,
                  cNMvar, cNMvarreb);

      for(int iord = 0; iord < hcN.size(); ++iord)
      {
         for(int ibin = 0; ibin < hcN[iord]->GetNbinsX(); ++ibin)
         {
            if(cNMvar[iord][ibin] > 0.)
            { 
               hcN[iord]->SetBinError(ibin+1, TMath::Sqrt(cNMvar[iord][ibin])/nSubSamp);
               hvN[iord]->SetBinError(ibin+1, errorVn(iord,
                                                     hcN[iord]->GetBinContent(ibin+1),
                                                     TMath::Sqrt(cNMvar[iord][ibin])/nSubSamp,
                                                     hvN[iord]->GetBinContent(ibin+1), nsub));
            }
            else
            {
               hcN[iord]->SetBinError(ibin+1, 0.*hcN[iord]->GetBinContent(ibin+1));
               hvN[iord]->SetBinError(ibin+1, 0.*hvN[iord]->GetBinContent(ibin+1));
            }
         }
         for(int ibin = 0; ibin < hcNreb[iord]->GetNbinsX(); ++ibin)
         {
            if(cNMvarreb[iord][ibin] > 0.)
            { 
               hcNreb[iord]->SetBinError(ibin+1, TMath::Sqrt(cNMvarreb[iord][ibin])/nSubSamp);
               hvNreb[iord]->SetBinError(ibin+1, errorVn(iord,
                                                     hcNreb[iord]->GetBinContent(ibin+1),
                                                     TMath::Sqrt(cNMvarreb[iord][ibin])/nSubSamp,
                                                     hvNreb[iord]->GetBinContent(ibin+1), nsub));
            }
            else
            {
               hcNreb[iord]->SetBinError(ibin+1, 0.*hcNreb[iord]->GetBinContent(ibin+1));
               hvNreb[iord]->SetBinError(ibin+1, 0.*hvNreb[iord]->GetBinContent(ibin+1));
            }
         }
       }

       // -- Write histo
       fout->cd();
       for(int iord = 0; iord < cNM.size(); ++iord)
       {
          hcN[iord]   ->Write("",TObject::kOverwrite);
          hvN[iord]   ->Write("",TObject::kOverwrite);
          hcNreb[iord]->Write("",TObject::kOverwrite);
          hvNreb[iord]->Write("",TObject::kOverwrite);
       }

       // -- Free memory from huge vectors
       for(int i = 0; i < qNM.size(); ++i)
       {
          for(int j = 0; j < qNM[i].size(); ++j)
          {
             for(int k = 0; k < qNM[i][j].size(); ++j)
             {
                qNM[i][j][k].clear();
                wqNM[i][j][k].clear();
             }
             qNM[i][j].clear();
             wqNM[i][j].clear();
          }
          qNM[i].clear();
          wqNM[i].clear();
       }
       qNM.clear();
       wqNM.clear();
       std::vector< std::vector< std::vector< std::vector<double> > > >().swap(qNM);       
       std::vector< std::vector< std::vector< std::vector<double> > > >().swap(wqNM);

       for(int i = 0; i < cNM.size(); ++i)
       {
         for(int j = 0; j < cNM[i].size(); ++j)
         {
           cNM[i][j].clear();
           wcNM[i][j].clear();
         }
         cNM[i].clear();
         wcNM[i].clear();
       }
       cNM.clear();
       wcNM.clear();
       std::vector< std::vector< std::vector<double> > >().swap(cNM);       
       std::vector< std::vector< std::vector<double> > >().swap(wcNM); 

       for(int i = 0; i < cNMreb.size(); ++i)
       {
         for(int j = 0; j < cNMreb[i].size(); ++j)
         {
           cNMreb[i][j].clear();
           wcNMreb[i][j].clear();
         }
         cNMreb[i].clear();
         wcNMreb[i].clear();
       }
       cNMreb.clear();
       wcNMreb.clear();
       std::vector< std::vector< std::vector<double> > >().swap(cNMreb);       
       std::vector< std::vector< std::vector<double> > >().swap(wcNMreb); 
   }
}

#endif
