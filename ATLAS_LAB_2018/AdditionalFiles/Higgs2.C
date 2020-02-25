// James Cummins & Dominic Williamson 16/11/18 (James Edit)

///////////////////////////////////////////////////////////////////////////////////////////////
// Throughout this program, you can use the variable "choice" as the name of the decay chain //
// you're interested in, e.g. running the python script "MultiRun.py" with "Zee" as an input //
// changes any mention of choice to the word "Zee" in this file.                             //
///////////////////////////////////////////////////////////////////////////////////////////////

// Header guard to ensure file is imported properly.
#ifndef MyAnalysis
#define MyAnalysis

// Include the file that lets the program know about the data.
#include "Headers/mini.h"
#include "TLorentzVector.h"
#include "math.h"

// This function is where you "book" your histograms.
// N.B. Be sure to declare any histograms in MyAnalysis.h, or it won't run correctly!  
void mini::Book(std::string choice){
		// 1-dimensional histograms (TH1F) are defined like: h1 = new TH1F("My_Histogram_Name", "Title", number of bins, x_min, x_max);
		
		// Example: booking of a histogram (to plot number of leptons per event)
  	h_lep_n = new TH1F("Lepton_no", "Number of Leptons", 10, -0.5, 9.5);
  	
  	// Example: using "choice" to name the histogram and its title.
  	// TString::Format is a function that takes a string of your choosing and substitutes it for %s in another string.
  	// For example, in this code, if choice == "Zee", then the name of the histogram would be "Zee_pt" and the title "Zee: p_t distribution". 
  	// N.B. You should try and use this format when you get on to analysing multiple datasets!
   	h_lep_higgs2 = new TH1F("Higgs2", TString::Format("%s: Transverse Momentum",choice.c_str()) , 100, 0, 10000);
   	h_lep_higgs4 = new TH1F("HIggs4", TString::Format("%s: Invariant Mass",choice.c_str()) , 100, 0, 180000);
	} // End of function Book
   
// This function is where you select events and fill your histograms. It happens once per event.
void mini::FillHist(std::string choice){

	 // Weight calculation --- PLEASE DO NOT CHANGE OR REMOVE!
	 ////////////////////////////////////////////////////////////////////////////////////
         std::unordered_map<std::string, double> ScaleFactor_Lumi = {
		{"Zee", 4.01173e-08},
		{"Zmumu", 4.41736e-08},
		{"Ztautau", 4.273775e-08},
		{"DYeeM08to15", 0.02162},
		{"DYeeM15to40", 0.0212436},
		{"DYmumuM08to15", 0.019266},
		{"DYmumuM15to40", 0.024265},
		{"WenuB",0.27983},
		{"WenuNoB", 0.907435},
		{"WenuNoBJets", 0.350585},
		{"WmunuB", 0.289016},
		{"WmunuNoB", 1.04169},
		{"WmunuNoBJets", 0.328063},
		{"WtaunuB", 0.18807},
		{"WtaunuNoB",0.920297 },
		{"WtaunuNoBJets", 0.343289},
		{"WW", 0.0216398},
		{"WZ", 0.00723487},
		{"ZZ", 0.00663601},
		{"ttLep", 0.0382083},
		{"ttHad", 0.0118436},
		{"ZPrime500", 0.03945},
		{"ggH4lep", 3.1687e-05},
		{"VBFH4lep", 2.84628e-06}
	 };
	 double Scale_factor, Event_weight, weight;

	 if(choice=="Muons" || choice=="Electrons")
		 {
			 weight = 1;
		 }
	 else
		 {
		 	 // Calculate weights:
		   Scale_factor = ScaleFactor_Lumi[choice] * scaleFactor_ELE * scaleFactor_MUON * scaleFactor_TRIGGER;
		 	 Event_weight = scaleFactor_PILEUP * scaleFactor_ZVERTEX * mcWeight;
		 	 weight = Scale_factor * Event_weight;
		 }
     ////////////////////////////////////////////////////////////////////////////////////
     // End of weight calculation
	
	TLorentzVector mu1(0., 0., 0., 0.);
     	TLorentzVector mu2(0., 0., 0., 0.);
	TLorentzVector mu3(0., 0., 0., 0.);
	TLorentzVector mu4(0., 0., 0., 0.);

	 float mll, mll_L, mll_2;
	 int fillTest = 0;
	 int jet_check = 0;
	 int charge_check = 0;
	 int lep_charge1 = 0;
	 int lep_charge2 = 0;
	 int lep_charge3 = 0;
	 int lep_charge4 = 0;
	 //unsigned int lepton_type = 11;
     
	 // 4 lepton events
	 for (size_t lep_i = 0; lep_i < lep_n; lep_i++)
	 {

		 if (lep_n == 4)
		 {			 
			 if (lep_i == 0)
			 {
				 mu1.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge1 = lep_charge[lep_i];
			 }
		
			 if (lep_i == 1)
			 {
				 mu2.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge2 = lep_charge[lep_i];
			 }

			 if (lep_i == 2)
			 {
				 mu3.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge3 = lep_charge[lep_i];
			 }
			 
			 if (lep_i == 3)
			 {
				 mu3.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge4 = lep_charge[lep_i];
			 }

		 }

	 }
	 
	 charge_check = lep_charge1 + lep_charge2 + lep_charge3 + lep_charge4;
	 
	 if (lep_n == 4 && charge_check == 0)
	 {
	 	mll = (mu1 + mu2 + mu3 + mu4).M();
	 	h_lep_higgs4->Fill(mll, weight);
	 }
	 
	 
	 // 2 lepton events
	 for (size_t lep_i = 0; lep_i < lep_n; lep_i++)
	 {

		 if (lep_n == 2)
		 {	 
		 	 if (lep_i == 0)
			 {
				 mu1.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge1 = lep_charge[lep_i];
			 }
		
			 if (lep_i == 1)
			 {
				 mu2.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
				 lep_charge2 = lep_charge[lep_i];
			 }
			 
			 
			 
			 mll = (mu1 + mu2).M();
			 mll_Phi = (mu1 + mu2).DeltaPhi(ETMiss)
			 
			 
			 
			 
		 	 if (lep_type[0] == lep_type[1] && mll > 12000 && abs(mll - 91000) > 15000 && met_et > 40000)
		 	 {
		 	 	fillTest = 1;
		 	 }
		 	 
		 	 if (lep_type[0] != lep_type[1] && mll > 10000 && met_et > 20000)
		 	 {
		 	 	fillTest = 1;
		 	 }		 
			
		 
		 }

	 }
	 
	 charge_check = lep_charge1 + lep_charge2;
	 mll_L    = (mu1 + mu2).Pt();
	 
	 for (size_t jet_i = 0; jet_i < jet_n; jet_i++)
	 {
		 if (jet_pt[jet_i] > 25000) jet_check = jet_check + 1;		 
	 }
	 
	 if (fillTest == 1)
	 {
	 	if (lep_n == 2 && lep_pt[0] > 25000 && lep_pt[1] > 25000 && charge_check == 0 && jet_check == 0 && mll_L > 30000 && mll < 55000)
	 	{
	 	h_lep_higgs4->Fill(mll, weight);
	 	}
	 }

}  // End of function FillHist

// This function is where you can control the style elements of your histograms and then write them to a file.
void mini::Style(std::string choice){

	h_lep_higgs2->SetLineColor(kBlue);
	h_lep_higgs2->GetXaxis()->SetTitle("Transverse Momentum (MeV)");
	h_lep_higgs2->GetYaxis()->SetTitle("Number of Entries");
	h_lep_higgs2->SetAxisColor(2,"x");
	h_lep_higgs2->Write();
	
	h_lep_higgs4->SetLineColor(kBlue);
	h_lep_higgs4->GetXaxis()->SetTitle("Invariant Mass (MeV)");
	h_lep_higgs4->GetYaxis()->SetTitle("Number of Entries");
	h_lep_higgs4->SetAxisColor(2,"x");
	h_lep_higgs4->Write();

} // End of function Style

#endif // End of header guard
