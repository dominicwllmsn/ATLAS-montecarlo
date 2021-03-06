// James Cummins & Dominic Williamson 16/11/18 (James - Higgs Edit)

#ifndef MyAnalysis
#define MyAnalysis

#include "Headers/mini.h"
#include "TLorentzVector.h"
#include "math.h"

void mini::Book(std::string choice){
	// 1-dimensional histograms (TH1F) are defined like: h1 = new TH1F("My_Histogram_Name", "Title", number of bins, x_min, x_max);
	
  	// Example: using "choice" to name the histogram and its title.
  	// TString::Format is a function that takes a string of your choosing and substitutes it for %s in another string.
  	// For example, in this code, if choice == "Zee", then the name of the histogram would be "Zee_pt" and the title "Zee: p_t distribution". 
  	// N.B. You should try and use this format when you get on to analysing multiple datasets!
   	h_lep_cone = new TH1F("ptcone", TString::Format("%s: Transverse Momentum Cone",choice.c_str()) , 200, -500, 10000);
	h_lep_pt = new TH1F("pt", TString::Format("%s: Invariant Mass",choice.c_str()), 200, 0, 200000);
	}
   
void mini::FillHist(std::string choice){

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
		   Scale_factor = ScaleFactor_Lumi[choice] * scaleFactor_ELE * scaleFactor_MUON * scaleFactor_TRIGGER;
		 	 Event_weight = scaleFactor_PILEUP * scaleFactor_ZVERTEX * mcWeight;
		 	 weight = Scale_factor * Event_weight;
		 }
	 
	 TLorentzVector mu1(0., 0., 0., 0.);
     	TLorentzVector mu2(0., 0., 0., 0.);
	 TLorentzVector ETMiss(0., 0., 0., 0.);

	 float mll;
	 float mll_L;
	 float mll_Phi1;
	 float mll_Phi2;
	 int charge_check = 0;
	 int jet_check = 0;
	 int type_check1 = 0;
	 int type_check2 = 0;
	 int lep_charge1 = 0;
	 int lep_charge2 = 0;
	 int lep_charge3 = 0;
	 float Z_mass = 91000;
	 float pt_limit = 25000;

	 //float WTransverseMass(lepton, lorentz) :
		 //return math.sqrt(2 * lep_pt[lepton]*met_et*(1 - math.cos(lorentz.DeltaPhi(ETMiss))));

	 for (size_t lep_i = 0; lep_i < lep_n; lep_i++)
	 {

		 if (lep_n == 2)
		 {
			 charge_check = charge_check + lep_charge[lep_i];
			 if (lep_i == 0)
			 {
				 type_check1 = abs(lep_type[lep_i]);
				 mu1.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
			 } 
			 if (lep_i == 1)
			 {
				 type_check2 = abs(lep_type[lep_i]);
				 mu2.SetPtEtaPhiE(lep_pt[lep_i], lep_eta[lep_i], lep_phi[lep_i], lep_E[lep_i]);
			 }
		 }

	 }

	 //ETMiss.SetPtEtaPhiE(0., 0., met_phi, met_et);

	 for (size_t jet_i = 0; jet_i < jet_n; jet_i++)
	 {
		 if (jet_pt[jet_i] > 25000) jet_check = jet_check + 1;		 
	 }

	 if (charge_check == 0 && lep_type[0] == lepton_type && lep_type[1] == lepton_type && lep_pt[0] > pt_limit && lep_pt[1] > pt_limit
		 && jet_check == 0 && type_check1 == type_check 2)
	 {
		 mll      = (mu1 + mu2).M();
		 mll_L    = (mu1 + mu2).Pt();
		 //mll_Phi1 = (mu1 + mu2).DeltaPhi(ETMiss);
		 //mll_Phi2 = mu1.DeltaPhi(mu2);
		 
		 if (mll > 0 && mll < 55000 && abs(mll - Z_mass) > 15000 && met_et > 40000 && mll_L > 30000) // && mll_Phi1 > math.pi/2.0
			 // && mll_Phi2 < 1.8 && WTransverseMass(0, mu1) > 30000 && WTransverseMass(1, mu2) > 30000)
		 {
			 h_lep_pt->Fill(mll, weight);
		 }

	 }

	 if (charge_check == 0 && lep_type[0] == lepton_type && lep_type[1] == lepton_type && lep_pt[0] > pt_limit && lep_pt[1] > pt_limit
		 && jet_check == 0 && type_check1 != type_check 2)
	 {
		 mll      = (mu1 + mu2).M();
		 mll_L    = (mu1 + mu2).Pt();
		 //mll_Phi1 = (mu1 + mu2).DeltaPhi(ETMiss.tlv());
		 //mll_Phi2 = mu1.DeltaPhi(mu2);
		 
		 if (mll > 0000 && mll < 5500000 && met_et > 20000 && mll_L > 30000) // && mll_Phi1 > math.pi/2.0 && mll_Phi2 < 1.8
			 // && WTransverseMass(0, mu1) > 30000 && WTransverseMass(1, mu2) > 30000)
		 {
			 h_lep_pt->Fill(mll, weight);
		 }

	 }
     
}

void mini::Style(std::string choice){

	// Example: Change some properties!
	h_lep_pt->SetLineColor(kBlue);
	h_lep_pt->GetXaxis()->SetTitle("Invariant Mass (MeV)"); // X-axis title
	h_lep_pt->GetYaxis()->SetTitle("Number of Entries"); // Y-axis title
	h_lep_pt->SetAxisColor(2,"x"); // Set colour of X-axis
	// etc... (See online ROOT documentation!)
	
	// Write the histograms to the file:
	h_lep_pt->Write();

}

#endif
