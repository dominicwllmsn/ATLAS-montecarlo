// James Cummins & Dominic Williamson 06/11/18 (Task 6.3 onwards)

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
  	h_lep_type = new TH1F("type", TString::Format("%s: Lepton type",choice.c_str()) , 10, 10.5, 20.5);
   	h_lep_pt = new TH1F("pt", TString::Format("%s: Invariant Mass",choice.c_str()) , 100, 80000, 100000);

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

     // Whenever you fill a histogram, you should write histogram->Fill(quantity, weight), as shown in examples below:

	 // Example: Fill histogram with the number of leptons in each event
	 h_lep_n->Fill(lep_n,weight);
	
         // Loop over each lepton 
         TLorentzVector mu1(0.,0.,0.,0.);
         TLorentzVector mu2(0.,0.,0.,0.);
         
         float mll;
         int charge_check = 0;
         int mu_check = 0;
         int lepton_type = 13;
         
         for (size_t lep_i=0; lep_i<lep_n; lep_i++)  
           	{	
           		h_lep_type->Fill(lep_type[lep_i],weight);
           		
           		if (lep_type[lep_i]==lepton_type && lep_n==2)  
           		{
           			// define momenta and energy of each lepton
           			charge_check = charge_check + lep_charge[lep_i];
           			mu_check = mu_check + 1;
     				if (lep_i==0) mu1.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
     				if (lep_i==1) mu2.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
           		}
           	}
         if (charge_check == 0 && lep_type[0]==lepton_type && lep_type[1]==lepton_type) 
		 {                         
	   	 mll = (mu1 + mu2).M();
		 h_lep_pt->Fill(mll,weight);
		 }
}  // End of function FillHist





// This function is where you can control the style elements of your histograms and then write them to a file.
void mini::Style(std::string choice){

	// Example: Change some properties!
	h_lep_n->SetLineColor(6); // Awesome pink line
	h_lep_pt->GetXaxis()->SetTitle("p_t (MeV)"); // X-axis title
	h_lep_pt->GetYaxis()->SetTitle("Number of entries"); // Y-axis title
	h_lep_pt->SetAxisColor(2,"x"); // Set colour of X-axis
	// etc... (See online ROOT documentation!)
	
	// Write the histograms to the file:
	h_lep_n->Write();
	h_lep_type->Write();
	h_lep_pt->Write();

} // End of function Style

#endif // End of header guard
