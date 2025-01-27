#include "RunAnalysis.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <stdlib.h>

#include "BranchInit.h"
#include "include/RoccoR.h"
#include "include/XYMETCorrection.h"

#include "Util.h"

#include "TFile.h"
#include "TRandom.h"

int main(int argc, char **argv)
{
    //TH1::AddDirectory(kFALSE);
    CmdOptions options = ParseOptions( argc, argv );

    // Parse the text file and form the configuration object
    AnaConfig ana_config = ParseConfig( options.config_file, options );
    std::cout << "Configured " << ana_config.size() 
	      << " analysis modules " << std::endl;

    RunModule runmod;
    ana_config.Run(runmod, options);

    std::cout << "^_^ Finished ^_^" << std::endl;


}

void RunModule::initialize( TChain * chain, TTree * outtree, TFile *outfile,
                            const CmdOptions & options,
			    std::vector<ModuleConfig> &configs ) {

    _input_tree = chain;
    _outfile = outfile;
    // *************************
    // initialize trees
    // *************************
    InitINTree(chain);
    InitOUTTree( outtree );

    h_EventWeight = new TH1D("weighthist","weighthist",2,-1.1,1.1);
    OUT::mu_pt20_n                              = 0;
    OUT::mu_pt30_n                              = 0;
    OUT::mu_pt_rc                               = 0;
    OUT::mu_e_rc                                = 0;
    OUT::mu_pt_rc_up                            = 0;
    OUT::mu_e_rc_up                             = 0;
    OUT::mu_pt_rc_down                          = 0;
    OUT::mu_e_rc_down                           = 0;
    OUT::mu_pt_Sigma_up                            = 0;
    OUT::mu_e_Sigma_up                             = 0;
    OUT::mu_pt_Sigma_down                          = 0;
    OUT::mu_e_Sigma_down                           = 0;
    OUT::mu_passTight                           = 0;
    OUT::mu_passMedium                          = 0;
    OUT::mu_passLoose                           = 0;
    OUT::mu_hasTruthMatchMu                     = 0;
    OUT::mu_truthMatchMu_dr                     = 0;
    OUT::mu_truthMatchMu_pt                     = 0;
    OUT::mu_hasTrigMatch                        = 0;
    OUT::mu_trigMatch_dr                        = 0;

    OUT::el_pt30_n                              = 0;
    OUT::el_pt35_n                              = 0;
    OUT::el_pt40_n                              = 0;
    OUT::el_passVeryLoose                       = 0;
    OUT::el_passLoose                           = 0;
    OUT::el_passMedium                          = 0;
    OUT::el_passTight                           = 0;
    OUT::el_hasTrigMatch                        = 0;
    OUT::el_trigMatch_dr                        = 0;
    OUT::el_hasTruthMatchEl                     = 0;
    OUT::el_truthMatchEl_dr                     = 0;
    OUT::el_truthMatchEl_pt                     = 0;
    OUT::el_hasTruthMatchPh                     = 0;
    OUT::el_truthMatchPh_dr                     = 0;
    OUT::el_truthMatchPh_pt                     = 0;

    //OUT::ph_chIsoCorr                           = 0;
    //OUT::ph_neuIsoCorr                          = 0;
    //OUT::ph_phoIsoCorr                          = 0;
    OUT::ph_min_el_dr                          = 0;
    OUT::ph_IsEB                                = 0;
    OUT::ph_IsEE                                = 0;
    OUT::ph_passTight                           = 0;
    OUT::ph_passMedium                          = 0;
    OUT::ph_passLoose                           = 0;
    OUT::ph_passLooseNoSIEIE                    = 0;
    OUT::ph_passHOverELoose                     = 0;
    OUT::ph_passHOverEMedium                    = 0;
    OUT::ph_passHOverETight                     = 0;
    OUT::ph_passSIEIELoose                      = 0;
    OUT::ph_passSIEIEMedium                     = 0;
    OUT::ph_passSIEIETight                      = 0;
    OUT::ph_passChIsoCorrLoose                  = 0;
    OUT::ph_passChIsoCorrMedium                 = 0;
    OUT::ph_passChIsoCorrTight                  = 0;
    OUT::ph_passNeuIsoCorrLoose                 = 0;
    OUT::ph_passNeuIsoCorrMedium                = 0;
    OUT::ph_passNeuIsoCorrTight                 = 0;
    OUT::ph_passPhoIsoCorrLoose                 = 0;
    OUT::ph_passPhoIsoCorrMedium                = 0;
    OUT::ph_passPhoIsoCorrTight                 = 0;
    OUT::ph_hasTruthMatchPh                     = 0;
    OUT::ph_truthMatchPh_dr                     = 0;
    OUT::ph_truthMatchPh_pt                     = 0;
    OUT::ph_hasTruthMatchEl                     = 0;
    OUT::ph_truthMatchEl_dr                     = 0;
    OUT::ph_truthMatchEl_pt                     = 0;
    OUT::ph_hasTruthMatchMu                     = 0;
    OUT::ph_truthMatchMu_dr                     = 0;
    OUT::ph_truthMatchMu_pt                     = 0;

    OUT::jet_CSVLoose_n                         = 0;
    OUT::jet_CSVMedium_n                        = 0;
    OUT::jet_CSVTight_n                         = 0;
    OUT::jet_DeepJetLoose_n                         = 0;
    OUT::jet_DeepJetMedium_n                        = 0;
    OUT::jet_DeepJetTight_n                         = 0;
    OUT::jet_IdLoose                            = 0;
    OUT::jet_IdTight                            = 0;
    OUT::jet_IdTightLep                         = 0;
    OUT::jet_btagged                         = 0;
    
    OUT::m_lep_ph                               = 0;
    OUT::m_lep_ph_comb_leadLep                  = 0;
    OUT::m_lep_ph_comb_sublLep                  = 0;
    OUT::m_lep_met_ph                           = 0;
    OUT::m_mt_lep_met_ph = 0;
    OUT::m_mt_lep_met_ph_forcewmass = 0;
    OUT::mt_w = 0;
    OUT::mt_res = 0;
    OUT::mt_lep_ph  = 0;
    OUT::dphi_lep_ph                            = 0;
    OUT::dr_lep_ph                              = 0;
    OUT::dr_lep2_ph                             = 0;
    OUT::mt_lep_met                             = 0;
    OUT::m_lep_met                              = 0;
    OUT::pt_lep_met                             = 0;
    OUT::dphi_lep_met                           = 0;
    OUT::mt_lep_met_ph                          = 0;
    OUT::mt_lep_met_ph_inv                      = 0;
    OUT::dphi_ph_w                              = 0;
    OUT::pt_lep_met_ph                          = 0;
    OUT::RecoWMass                              = 0;
    OUT::recoM_lep_nu_ph                        = 0;
    OUT::recoMet_eta                            = 0;
    OUT::recoW_pt                               = 0;
    OUT::recoW_eta                              = 0;
    OUT::recoW_phi                              = 0;
    OUT::m_ll                                   = 0;
    OUT::m_llph                                 = 0;
    OUT::met_PhotonEnUpByHand_pt                = 0;
    OUT::met_PhotonEnUpByHand_phi               = 0;
    OUT::met_PhotonEnDownByHand_pt              = 0;
    OUT::met_PhotonEnDownByHand_phi             = 0;
    OUT::met_PhotonResUpByHand_pt               = 0;
    OUT::met_PhotonResUpByHand_phi              = 0;
    OUT::met_PhotonResDownByHand_pt             = 0;
    OUT::met_PhotonResDownByHand_phi            = 0;
    OUT::met_ElectronEnUpByHand_pt              = 0;
    OUT::met_ElectronEnUpByHand_phi             = 0;
    OUT::met_ElectronEnDownByHand_pt            = 0;
    OUT::met_ElectronEnDownByHand_phi           = 0;
    OUT::met_ElectronResUpByHand_pt             = 0;
    OUT::met_ElectronResUpByHand_phi            = 0;
    OUT::met_ElectronResDownByHand_pt           = 0;
    OUT::met_ElectronResDownByHand_phi          = 0;
    OUT::met_MuonEnUpByHand_pt                  = 0;
    OUT::met_MuonEnUpByHand_phi                 = 0;
    OUT::met_MuonEnDownByHand_pt                = 0;
    OUT::met_MuonEnDownByHand_phi               = 0;
    OUT::met_MuonResUpByHand_pt             = 0;
    OUT::met_MuonResUpByHand_phi            = 0;
    OUT::met_MuonResDownByHand_pt           = 0;
    OUT::met_MuonResDownByHand_phi          = 0;
    OUT::nu_z_solution_success                  = 0;
   
    OUT::leadjet_pt                             = 0;
    OUT::subljet_pt                             = 0;
    OUT::leaddijet_m                            = 0;
    OUT::leaddijet_pt                           = 0;
    OUT::massdijet_m                            = 0;
    OUT::massdijet_pt                           = 0;

    OUT::isBlinded                              = 0;
    OUT::SampleWeight                           = 0;

    OUT::ph_loose_n                            = 0;
    OUT::ph_medium_n                            = 0;
    OUT::ph_tight_n                            = 0;

    OUT::ph_mediumPassPSV_n                     = 0;
    OUT::ph_mediumFailPSV_n                     = 0;
    OUT::ph_mediumPassCSEV_n                    = 0;
    OUT::ph_mediumFailCSEV_n                    = 0;

    OUT::ph_mediumPassEleOlap_n                 = 0;
    OUT::ph_mediumPassEleOlapPassCSEV_n         = 0;
    OUT::ph_mediumPassEleOlapFailCSEV_n         = 0;

    OUT::ph_mediumNoSIEIE_n                     = 0;
    OUT::ph_mediumNoChIso_n                     = 0;
    OUT::ph_mediumNoNeuIso_n                    = 0;
    OUT::ph_mediumNoPhoIso_n                    = 0;

    OUT::ph_mediumNoSIEIENoChIso_n              = 0;
    OUT::ph_mediumNoSIEIENoNeuIso_n             = 0;
    OUT::ph_mediumNoSIEIENoPhoIso_n             = 0;
    OUT::ph_mediumNoChIsoNoPhoIso_n             = 0;
    OUT::ph_mediumNoChIsoNoNeuIso_n             = 0;
    OUT::ph_mediumNoPhoIsoNoNeuIso_n            = 0;

    OUT::ph_mediumNoSIEIEPassPSV_n              = 0;
    OUT::ph_mediumNoChIsoPassPSV_n              = 0;
    OUT::ph_mediumNoNeuIsoPassPSV_n             = 0;
    OUT::ph_mediumNoPhoIsoPassPSV_n             = 0;

    OUT::ph_mediumNoSIEIEFailPSV_n              = 0;
    OUT::ph_mediumNoChIsoFailPSV_n              = 0;
    OUT::ph_mediumNoNeuIsoFailPSV_n             = 0;
    OUT::ph_mediumNoPhoIsoFailPSV_n             = 0;

    OUT::ph_mediumNoSIEIEPassCSEV_n             = 0;
    OUT::ph_mediumNoChIsoPassCSEV_n             = 0;
    OUT::ph_mediumNoNeuIsoPassCSEV_n            = 0;
    OUT::ph_mediumNoPhoIsoPassCSEV_n            = 0;

    OUT::ph_mediumNoSIEIEFailCSEV_n             = 0;
    OUT::ph_mediumNoChIsoFailCSEV_n             = 0;
    OUT::ph_mediumNoNeuIsoFailCSEV_n            = 0;
    OUT::ph_mediumNoPhoIsoFailCSEV_n            = 0;

    OUT::ptSorted_ph_loose_idx                  = 0;
    OUT::ptSorted_ph_medium_idx                 = 0;
    OUT::ptSorted_ph_tight_idx                  = 0;

    OUT::ptSorted_ph_mediumPassPSV_idx          = 0;
    OUT::ptSorted_ph_mediumFailPSV_idx          = 0;
    OUT::ptSorted_ph_mediumPassCSEV_idx         = 0;
    OUT::ptSorted_ph_mediumFailCSEV_idx         = 0;

    OUT::ptSorted_ph_mediumPassEleOlap_idx         = 0;
    OUT::ptSorted_ph_mediumPassEleOlapPassCSEV_idx = 0;
    OUT::ptSorted_ph_mediumPassEleOlapFailCSEV_idx = 0;

    OUT::ptSorted_ph_mediumNoSIEIE_idx          = 0;
    OUT::ptSorted_ph_mediumNoChIso_idx          = 0;
    OUT::ptSorted_ph_mediumNoNeuIso_idx         = 0;
    OUT::ptSorted_ph_mediumNoPhoIso_idx         = 0;

    OUT::ptSorted_ph_mediumNoSIEIENoChIso_idx   = 0;
    OUT::ptSorted_ph_mediumNoSIEIENoNeuIso_idx  = 0;
    OUT::ptSorted_ph_mediumNoSIEIENoPhoIso_idx  = 0;
    OUT::ptSorted_ph_mediumNoChIsoNoPhoIso_idx  = 0;
    OUT::ptSorted_ph_mediumNoChIsoNoNeuIso_idx  = 0;
    OUT::ptSorted_ph_mediumNoPhoIsoNoNeuIso_idx = 0;

    OUT::ptSorted_ph_mediumNoSIEIEPassPSV_idx   = 0;
    OUT::ptSorted_ph_mediumNoChIsoPassPSV_idx   = 0;
    OUT::ptSorted_ph_mediumNoNeuIsoPassPSV_idx  = 0;
    OUT::ptSorted_ph_mediumNoPhoIsoPassPSV_idx  = 0;

    OUT::ptSorted_ph_mediumNoSIEIEFailPSV_idx   = 0;
    OUT::ptSorted_ph_mediumNoChIsoFailPSV_idx   = 0;
    OUT::ptSorted_ph_mediumNoNeuIsoFailPSV_idx  = 0;
    OUT::ptSorted_ph_mediumNoPhoIsoFailPSV_idx  = 0;

    OUT::ptSorted_ph_mediumNoSIEIEPassCSEV_idx  = 0;
    OUT::ptSorted_ph_mediumNoChIsoPassCSEV_idx  = 0;
    OUT::ptSorted_ph_mediumNoNeuIsoPassCSEV_idx = 0;
    OUT::ptSorted_ph_mediumNoPhoIsoPassCSEV_idx = 0;

    OUT::ptSorted_ph_mediumNoSIEIEFailCSEV_idx  = 0;
    OUT::ptSorted_ph_mediumNoChIsoFailCSEV_idx  = 0;
    OUT::ptSorted_ph_mediumNoNeuIsoFailCSEV_idx = 0;
    OUT::ptSorted_ph_mediumNoPhoIsoFailCSEV_idx = 0;

    OUT::trueph_n                               = 0;
    OUT::trueph_pt                              = 0;
    OUT::trueph_eta                             = 0;
    OUT::trueph_phi                             = 0;
    OUT::trueph_lep_dr                          = 0;
    OUT::trueph_motherPID                       = 0;
    OUT::trueph_status                          = 0;
    OUT::trueph_isPromptFS                      = 0;
    OUT::trueph_FHPFS                           = 0;

    OUT::truelep_n                              = 0;
    OUT::truelep_pt                             = 0;
    OUT::truelep_eta                            = 0;
    OUT::truelep_phi                            = 0;
    OUT::truelep_e                              = 0;
    OUT::truelep_motherPID                      = 0;
    OUT::truelep_status                         = 0;
    OUT::truelep_PID                            = 0;

    OUT::truenu_n                               = 0;
    OUT::truemed_mass                           = 0;

    OUT::trueW_n                               = 0;
    OUT::trueW_pt                              = 0;
    OUT::trueW_eta                             = 0;
    OUT::trueW_phi                             = 0;
    OUT::trueW_m                               = 0;
    OUT::trueW_motherPID                       = 0;
    OUT::trueW_status                          = 0;

    OUT::truelepnu_m                            = 0;
    OUT::truelepnuph_m                          = 0;
    OUT::truelepph_dr                           = 0;
    OUT::truemt_lep_met                         = 0;
    OUT::truemt_lep_met_ph                      = 0;
    OUT::truemt_res                             = 0;
    OUT::truemt_res_l23                             = 0;
    OUT::truemt_res_lO                             = 0;
    OUT::trueht                                 = 0;

    OUT::isWMuDecay                             = 0;
    OUT::isWElDecay                             = 0;
    OUT::isWTauDecay                            = 0;
    OUT::isWTauElDecay                          = 0;
    OUT::isWTauMuDecay                          = 0;
    OUT::WIDStep                                = 0;

    OUT::NLOWeight                              = 1;
    OUT::QCDScaleWeights                        = 0;
    OUT::PDFWeights                             = 0;

    OUT::PUWeight                               = 1;
    OUT::PUWeightUP5                            = 1;
    OUT::PUWeightUP10                           = 1;
    OUT::PUWeightDN5                            = 1;
    OUT::PUWeightDN10                           = 1;

#ifndef EXISTS_prefweight
    OUT::prefweight                             = 1;
    OUT::prefweightup                           = 1;
    OUT::prefweightdown                         = 1;
#endif

    // *************************
    // Declare Branches
    // *************************

    bool build_truth = false;
    BOOST_FOREACH( ModuleConfig & mod_conf, configs ) {
        if( mod_conf.GetName() == "BuildTruth" )  build_truth = true;
    }
    outtree->Branch("mu_pt20_n", &OUT::mu_pt20_n, "mu_pt20_n/I"  );
    outtree->Branch("mu_pt30_n", &OUT::mu_pt30_n, "mu_pt30_n/I"  );
    outtree->Branch("mu_pt_rc", &OUT::mu_pt_rc            );
    outtree->Branch("mu_e_rc", &OUT::mu_e_rc            );
    outtree->Branch("mu_pt_rc_up", &OUT::mu_pt_rc_up            );
    outtree->Branch("mu_e_rc_up", &OUT::mu_e_rc_up            );
    outtree->Branch("mu_pt_rc_down", &OUT::mu_pt_rc_down            );
    outtree->Branch("mu_e_rc_down", &OUT::mu_e_rc_down            );
    outtree->Branch("mu_pt_Sigma_up", &OUT::mu_pt_Sigma_up            );
    outtree->Branch("mu_e_Sigma_up", &OUT::mu_e_Sigma_up            );
    outtree->Branch("mu_pt_Sigma_down", &OUT::mu_pt_Sigma_down            );
    outtree->Branch("mu_e_Sigma_down", &OUT::mu_e_Sigma_down            );
    outtree->Branch("mu_passTight", &OUT::mu_passTight            );
    outtree->Branch("mu_passMedium", &OUT::mu_passMedium           );
    outtree->Branch("mu_passLoose", &OUT::mu_passLoose            );
    outtree->Branch("mu_hasTruthMatchMu", &OUT::mu_hasTruthMatchMu );
    outtree->Branch("mu_truthMatchMu_dr", &OUT::mu_truthMatchMu_dr   );
    outtree->Branch("mu_truthMatchMu_pt", &OUT::mu_truthMatchMu_pt      );
    outtree->Branch("mu_hasTrigMatch", &OUT::mu_hasTrigMatch );
    outtree->Branch("mu_trigMatch_dr", &OUT::mu_trigMatch_dr);

    outtree->Branch("el_pt30_n", &OUT::el_pt30_n, "el_pt30_n/I"  );
    outtree->Branch("el_pt35_n", &OUT::el_pt35_n, "el_pt35_n/I"  );
    outtree->Branch("el_pt40_n", &OUT::el_pt40_n, "el_pt40_n/I"  );
    outtree->Branch("el_passVeryLoose", &OUT::el_passVeryLoose );
    outtree->Branch("el_passLoose", &OUT::el_passLoose );
    outtree->Branch("el_passMedium", &OUT::el_passMedium );
    outtree->Branch("el_passTight", &OUT::el_passTight );
    outtree->Branch("el_hasTrigMatch", &OUT::el_hasTrigMatch );
    outtree->Branch("el_trigMatch_dr", &OUT::el_trigMatch_dr );
    outtree->Branch("el_hasTruthMatchEl", &OUT::el_hasTruthMatchEl );
    outtree->Branch("el_truthMatchEl_dr", &OUT::el_truthMatchEl_dr   );
    outtree->Branch("el_truthMatchEl_pt", &OUT::el_truthMatchEl_pt      );
    outtree->Branch("el_hasTruthMatchPh", &OUT::el_hasTruthMatchPh );
    outtree->Branch("el_truthMatchPh_dr", &OUT::el_truthMatchPh_dr   );
    outtree->Branch("el_truthMatchPh_pt", &OUT::el_truthMatchPh_pt      );

    //outtree->Branch("ph_chIsoCorr", &OUT::ph_chIsoCorr);
    //outtree->Branch("ph_neuIsoCorr", &OUT::ph_neuIsoCorr);
    //outtree->Branch("ph_phoIsoCorr", &OUT::ph_phoIsoCorr);
    outtree->Branch("ph_min_el_dr", &OUT::ph_min_el_dr);
    outtree->Branch("ph_IsEB", &OUT::ph_IsEB );
    outtree->Branch("ph_IsEE", &OUT::ph_IsEE );
    outtree->Branch("ph_passTight", &OUT::ph_passTight );
    outtree->Branch("ph_passMedium", &OUT::ph_passMedium );
    outtree->Branch("ph_passLoose", &OUT::ph_passLoose );
    outtree->Branch("ph_passLooseNoSIEIE", &OUT::ph_passLooseNoSIEIE );
    outtree->Branch("ph_passHOverELoose", &OUT::ph_passHOverELoose );
    outtree->Branch("ph_passHOverEMedium", &OUT::ph_passHOverEMedium );
    outtree->Branch("ph_passHOverETight", &OUT::ph_passHOverETight );
    outtree->Branch("ph_passSIEIELoose", &OUT::ph_passSIEIELoose );
    outtree->Branch("ph_passSIEIEMedium", &OUT::ph_passSIEIEMedium );
    outtree->Branch("ph_passSIEIETight", &OUT::ph_passSIEIETight );
    outtree->Branch("ph_passChIsoCorrLoose", &OUT::ph_passChIsoCorrLoose );
    outtree->Branch("ph_passChIsoCorrMedium", &OUT::ph_passChIsoCorrMedium );
    outtree->Branch("ph_passChIsoCorrTight", &OUT::ph_passChIsoCorrTight );
    outtree->Branch("ph_passNeuIsoCorrLoose", &OUT::ph_passNeuIsoCorrLoose );
    outtree->Branch("ph_passNeuIsoCorrMedium", &OUT::ph_passNeuIsoCorrMedium );
    outtree->Branch("ph_passNeuIsoCorrTight", &OUT::ph_passNeuIsoCorrTight );
    outtree->Branch("ph_passPhoIsoCorrLoose", &OUT::ph_passPhoIsoCorrLoose );
    outtree->Branch("ph_passPhoIsoCorrMedium", &OUT::ph_passPhoIsoCorrMedium );
    outtree->Branch("ph_passPhoIsoCorrTight", &OUT::ph_passPhoIsoCorrTight );
    outtree->Branch("ph_hasTruthMatchPh", &OUT::ph_hasTruthMatchPh );
    outtree->Branch("ph_truthMatchPh_dr", &OUT::ph_truthMatchPh_dr   );
    outtree->Branch("ph_truthMatchPh_pt", &OUT::ph_truthMatchPh_pt      );
    outtree->Branch("ph_hasTruthMatchEl", &OUT::ph_hasTruthMatchEl );
    outtree->Branch("ph_truthMatchEl_dr", &OUT::ph_truthMatchEl_dr   );
    outtree->Branch("ph_truthMatchEl_pt", &OUT::ph_truthMatchEl_pt      );
    outtree->Branch("ph_hasTruthMatchMu", &OUT::ph_hasTruthMatchMu );
    outtree->Branch("ph_truthMatchMu_dr", &OUT::ph_truthMatchMu_dr   );
    outtree->Branch("ph_truthMatchMu_pt", &OUT::ph_truthMatchMu_pt      );


    outtree->Branch("jet_CSVLoose_n", &OUT::jet_CSVLoose_n, "jet_CSVLoose_n/I"  );
    outtree->Branch("jet_CSVMedium_n", &OUT::jet_CSVMedium_n, "jet_CSVMedium_n/I"  );
    outtree->Branch("jet_CSVTight_n", &OUT::jet_CSVTight_n, "jet_CSVTight_n/I"  );
    outtree->Branch("jet_DeepJetLoose_n", &OUT::jet_DeepJetLoose_n, "jet_DeepJetLoose_n/I"  );
    outtree->Branch("jet_DeepJetMedium_n", &OUT::jet_DeepJetMedium_n, "jet_DeepJetMedium_n/I"  );
    outtree->Branch("jet_DeepJetTight_n", &OUT::jet_DeepJetTight_n, "jet_DeepJetTight_n/I"  );
    outtree->Branch("jet_IdLoose", &OUT::jet_IdLoose );
    outtree->Branch("jet_IdTight", &OUT::jet_IdTight );
    outtree->Branch("jet_IdTightLep", &OUT::jet_IdTightLep );
    outtree->Branch("jet_btagged", &OUT::jet_btagged );

    outtree->Branch("m_lep_ph"        , &OUT::m_lep_ph        , "m_lep_ph/F"  );
    outtree->Branch("m_lep_ph_comb_sublLep"        , &OUT::m_lep_ph_comb_sublLep);
    outtree->Branch("m_lep_ph_comb_leadLep"        , &OUT::m_lep_ph_comb_leadLep);
    outtree->Branch("m_lep_met_ph"        , &OUT::m_lep_met_ph        , "m_lep_met_ph/F"  );
    outtree->Branch("m_mt_lep_met_ph"        , &OUT::m_mt_lep_met_ph        , "m_mtlep_met_ph/F"  );
    outtree->Branch("m_mt_lep_met_ph_forcewmass"        , &OUT::m_mt_lep_met_ph_forcewmass        , "m_mt_lep_met_ph_forcewmass/F"  );
    outtree->Branch("mt_w"        , &OUT::mt_w        , "mt_w/F"  );
    outtree->Branch("mt_res"        , &OUT::mt_res        , "mt_res/F"  );
    outtree->Branch("mt_res_JetResUp",          &OUT::mt_res_JetResUp             ,"mt_res_JetResUp/F" );
    outtree->Branch("mt_res_JetResDown",        &OUT::mt_res_JetResDown           ,"mt_res_JetResDown/F" );
    outtree->Branch("mt_res_JetEnUp",           &OUT::mt_res_JetEnUp              ,"mt_res_JetEnUp/F" );
    outtree->Branch("mt_res_JetEnDown",         &OUT::mt_res_JetEnDown            ,"mt_res_JetEnDown/F" );
    outtree->Branch("mt_res_MuonEnUp",          &OUT::mt_res_MuonEnUp             ,"mt_res_MuonEnUp/F" );
    outtree->Branch("mt_res_MuonEnDown",        &OUT::mt_res_MuonEnDown           ,"mt_res_MuonEnDown/F" );
    outtree->Branch("mt_res_MuonResUp",          &OUT::mt_res_MuonResUp             ,"mt_res_MuonResUp/F" );
    outtree->Branch("mt_res_MuonResDown",        &OUT::mt_res_MuonResDown           ,"mt_res_MuonResDown/F" );
    outtree->Branch("mt_res_ElectronEnUp",      &OUT::mt_res_ElectronEnUp         ,"mt_res_ElectronEnUp/F" );
    outtree->Branch("mt_res_ElectronEnDown",    &OUT::mt_res_ElectronEnDown       ,"mt_res_ElectronEnDown/F" );
    outtree->Branch("mt_res_ElectronResUp",      &OUT::mt_res_ElectronResUp         ,"mt_res_ElectronResUp/F" );
    outtree->Branch("mt_res_ElectronResDown",    &OUT::mt_res_ElectronResDown       ,"mt_res_ElectronResDown/F" );
    outtree->Branch("mt_res_PhotonEnUp",        &OUT::mt_res_PhotonEnUp           ,"mt_res_PhotonEnUp/F" );
    outtree->Branch("mt_res_PhotonEnDown",      &OUT::mt_res_PhotonEnDown         ,"mt_res_PhotonEnDown/F" );
    outtree->Branch("mt_res_PhotonResUp",        &OUT::mt_res_PhotonResUp           ,"mt_res_PhotonResUp/F" );
    outtree->Branch("mt_res_PhotonResDown",      &OUT::mt_res_PhotonResDown         ,"mt_res_PhotonResDown/F" );
    outtree->Branch("mt_res_UnclusteredEnUp",   &OUT::mt_res_UnclusteredEnUp      ,"mt_res_UnclusteredEnUp/F" );
    outtree->Branch("mt_res_UnclusteredEnDown", &OUT::mt_res_UnclusteredEnDown    ,"mt_res_UnclusteredEnDown/F" );
    outtree->Branch("met_PhotonEnUpByHand_pt", &OUT::met_PhotonEnUpByHand_pt, "met_PhotonEnUpByHand_pt/F");
    outtree->Branch("met_PhotonEnUpByHand_phi", &OUT::met_PhotonEnUpByHand_phi, "met_PhotonEnUpByHand_phi/F");
    outtree->Branch("met_PhotonEnDownByHand_pt", &OUT::met_PhotonEnDownByHand_pt, "met_PhotonEnDownByHand_pt/F");
    outtree->Branch("met_PhotonEnDownByHand_phi", &OUT::met_PhotonEnDownByHand_phi, "met_PhotonEnDownByHand_phi/F");
    outtree->Branch("met_PhotonResUpByHand_pt", &OUT::met_PhotonResUpByHand_pt, "met_PhotonResUpByHand_pt/F");
    outtree->Branch("met_PhotonResUpByHand_phi", &OUT::met_PhotonResUpByHand_phi, "met_PhotonResUpByHand_phi/F");
    outtree->Branch("met_PhotonResDownByHand_pt", &OUT::met_PhotonResDownByHand_pt, "met_PhotonResDownByHand_pt/F");
    outtree->Branch("met_PhotonResDownByHand_phi", &OUT::met_PhotonResDownByHand_phi, "met_PhotonResDownByHand_phi/F");
    outtree->Branch("met_ElectronEnUpByHand_pt", &OUT::met_ElectronEnUpByHand_pt, "met_ElectronEnUpByHand_pt/F");
    outtree->Branch("met_ElectronEnUpByHand_phi", &OUT::met_ElectronEnUpByHand_phi, "met_ElectronEnUpByHand_phi/F");
    outtree->Branch("met_ElectronEnDownByHand_pt", &OUT::met_ElectronEnDownByHand_pt, "met_ElectronEnDownByHand_pt/F");
    outtree->Branch("met_ElectronEnDownByHand_phi", &OUT::met_ElectronEnDownByHand_phi, "met_ElectronEnDownByHand_phi/F");
    outtree->Branch("met_ElectronResUpByHand_pt", &OUT::met_ElectronResUpByHand_pt, "met_ElectronResUpByHand_pt/F");
    outtree->Branch("met_ElectronResUpByHand_phi", &OUT::met_ElectronResUpByHand_phi, "met_ElectronResUpByHand_phi/F");
    outtree->Branch("met_ElectronResDownByHand_pt", &OUT::met_ElectronResDownByHand_pt, "met_ElectronResDownByHand_pt/F");
    outtree->Branch("met_ElectronResDownByHand_phi", &OUT::met_ElectronResDownByHand_phi, "met_ElectronResDownByHand_phi/F");
    outtree->Branch("met_MuonEnUpByHand_pt", &OUT::met_MuonEnUpByHand_pt, "met_MuonEnUpByHand_pt/F");
    outtree->Branch("met_MuonEnUpByHand_phi", &OUT::met_MuonEnUpByHand_phi, "met_MuonEnUpByHand_phi/F");
    outtree->Branch("met_MuonEnDownByHand_pt", &OUT::met_MuonEnDownByHand_pt, "met_MuonEnDownByHand_pt/F");
    outtree->Branch("met_MuonEnDownByHand_phi", &OUT::met_MuonEnDownByHand_phi, "met_MuonEnDownByHand_phi/F");
    outtree->Branch("met_MuonResUpByHand_pt", &OUT::met_MuonResUpByHand_pt, "met_MuonResUpByHand_pt/F");
    outtree->Branch("met_MuonResUpByHand_phi", &OUT::met_MuonResUpByHand_phi, "met_MuonResUpByHand_phi/F");
    outtree->Branch("met_MuonResDownByHand_pt", &OUT::met_MuonResDownByHand_pt, "met_MuonResDownByHand_pt/F");
    outtree->Branch("met_MuonResDownByHand_phi", &OUT::met_MuonResDownByHand_phi, "met_MuonResDownByHand_phi/F");

    outtree->Branch("mt_lep_ph"        , &OUT::mt_lep_ph        , "mt_lep_ph/F"  );
    outtree->Branch("dphi_lep_ph"        , &OUT::dphi_lep_ph        , "dphi_lep_ph/F"  );
    outtree->Branch("dr_lep_ph"        , &OUT::dr_lep_ph        , "dr_lep_ph/F"  );
    outtree->Branch("dr_lep2_ph"       , &OUT::dr_lep2_ph       , "dr_lep2_ph/F"  );
    outtree->Branch("mt_lep_met"      , &OUT::mt_lep_met      , "mt_lep_met/F" );
    outtree->Branch("m_lep_met"       , &OUT::m_lep_met       , "m_lep_met/F" );
    outtree->Branch("pt_lep_met"      , &OUT::pt_lep_met      , "pt_lep_met/F" );
    outtree->Branch("dphi_lep_met"    , &OUT::dphi_lep_met    , "dphi_lep_met/F" );
    outtree->Branch("mt_lep_met_ph"   , &OUT::mt_lep_met_ph   , "mt_lep_met_ph/F");
    outtree->Branch("mt_lep_met_ph_inv"   , &OUT::mt_lep_met_ph_inv   , "mt_lep_met_ph_inv/F");
    outtree->Branch("dphi_ph_w"   , &OUT::dphi_ph_w   , "dphi_ph_w/F");
    outtree->Branch("pt_lep_met_ph"   , &OUT::pt_lep_met_ph   , "pt_lep_met_ph/F");
    outtree->Branch("RecoWMass"       , &OUT::RecoWMass       , "RecoWMass/F");
    outtree->Branch("recoM_lep_nu_ph" , &OUT::recoM_lep_nu_ph , "recoM_lep_nu_ph/F");
    outtree->Branch("recoMet_eta" , &OUT::recoMet_eta, "recoMet_eta/F");
    outtree->Branch("recoW_pt" , &OUT::recoW_pt, "recoW_pt/F");
    outtree->Branch("recoW_eta" , &OUT::recoW_eta, "recoW_eta/F");
    outtree->Branch("recoW_phi" , &OUT::recoW_phi, "recoW_phi/F");
    outtree->Branch("m_ll" , &OUT::m_ll, "m_ll/F");
    outtree->Branch("m_llph" , &OUT::m_llph, "m_llph/F");
    outtree->Branch("nu_z_solution_success" , &OUT::nu_z_solution_success, "nu_z_solution_success/O");


    outtree->Branch("leadjet_pt", &OUT::leadjet_pt, "leadjet_pt/F" );
    outtree->Branch("subljet_pt", &OUT::subljet_pt, "subljet_pt/F" );
    outtree->Branch("leaddijet_m", &OUT::leaddijet_m, "leaddijet_m/F" );
    outtree->Branch("leaddijet_pt", &OUT::leaddijet_pt, "leaddijet_pt/F" );
    outtree->Branch("massdijet_m", &OUT::massdijet_m, "massdijet_m/F" );
    outtree->Branch("massdijet_pt", &OUT::massdijet_pt, "massdijet_pt/F" );

    outtree->Branch("isBlinded" , &OUT::isBlinded, "isBlinded/O");
    outtree->Branch("SampleWeight" , &OUT::SampleWeight, "SampleWeight/F");

#ifdef MODULE_MakePhotonCountVars

    outtree->Branch("ph_loose_n"           , &OUT::ph_loose_n           , "ph_loose_n/I"           );
    outtree->Branch("ph_medium_n"           , &OUT::ph_medium_n           , "ph_medium_n/I"           );
    outtree->Branch("ph_tight_n"           , &OUT::ph_tight_n           , "ph_tight_n/I"           );

    outtree->Branch("ph_mediumPassPSV_n"           , &OUT::ph_mediumPassPSV_n           , "ph_mediumPassPSV_n/I"           );
    outtree->Branch("ph_mediumFailPSV_n"           , &OUT::ph_mediumFailPSV_n           , "ph_mediumFailPSV_n/I"           );
    outtree->Branch("ph_mediumPassCSEV_n"          , &OUT::ph_mediumPassCSEV_n          , "ph_mediumPassCSEV_n/I"          );
    outtree->Branch("ph_mediumFailCSEV_n"          , &OUT::ph_mediumFailCSEV_n          , "ph_mediumFailCSEV_n/I"          );

    outtree->Branch("ph_mediumPassEleOlap_n"          , &OUT::ph_mediumPassEleOlap_n, "ph_mediumPassEleOlap_n/I"          );
    outtree->Branch("ph_mediumPassEleOlapPassCSEV_n"          , &OUT::ph_mediumPassEleOlapPassCSEV_n, "ph_mediumPassEleOlapPassCSEV_n/I"          );
    outtree->Branch("ph_mediumPassEleOlapFailCSEV_n"          , &OUT::ph_mediumPassEleOlapFailCSEV_n, "ph_mediumPassEleOlapFailCSEV_n/I"          );

    outtree->Branch("ph_mediumNoSIEIE_n"           , &OUT::ph_mediumNoSIEIE_n           , "ph_mediumNoSIEIE_n/I"           );
    outtree->Branch("ph_mediumNoChIso_n"           , &OUT::ph_mediumNoChIso_n           , "ph_mediumNoChIso_n/I"           );
    outtree->Branch("ph_mediumNoNeuIso_n"          , &OUT::ph_mediumNoNeuIso_n          , "ph_mediumNoNeuIso_n/I"          );
    outtree->Branch("ph_mediumNoPhoIso_n"          , &OUT::ph_mediumNoPhoIso_n          , "ph_mediumNoPhoIso_n/I"          );

    outtree->Branch("ph_mediumNoSIEIENoChIso_n", &OUT::ph_mediumNoSIEIENoChIso_n, "ph_mediumNoSIEIENoChIso_n/I" );
    outtree->Branch("ph_mediumNoSIEIENoNeuIso_n", &OUT::ph_mediumNoSIEIENoNeuIso_n, "ph_mediumNoSIEIENoNeuIso_n/I" );
    outtree->Branch("ph_mediumNoSIEIENoPhoIso_n", &OUT::ph_mediumNoSIEIENoPhoIso_n, "ph_mediumNoSIEIENoPhoIso_n/I" );
    outtree->Branch("ph_mediumNoChIsoNoPhoIso_n", &OUT::ph_mediumNoChIsoNoPhoIso_n, "ph_mediumNoChIsoNoPhoIso_n/I" );
    outtree->Branch("ph_mediumNoChIsoNoNeuIso_n", &OUT::ph_mediumNoChIsoNoNeuIso_n, "ph_mediumNoChIsoNoNeuIso_n/I" );
    outtree->Branch("ph_mediumNoPhoIsoNoNeuIso_n", &OUT::ph_mediumNoPhoIsoNoNeuIso_n, "ph_mediumNoPhoIsoNoNeuIso_n/I" );


    outtree->Branch("ph_mediumNoSIEIEPassPSV_n"           , &OUT::ph_mediumNoSIEIEPassPSV_n           , "ph_mediumNoSIEIEPassPSV_n/I"           );
    outtree->Branch("ph_mediumNoChIsoPassPSV_n"           , &OUT::ph_mediumNoChIsoPassPSV_n           , "ph_mediumNoChIsoPassPSV_n/I"           );
    outtree->Branch("ph_mediumNoNeuIsoPassPSV_n"          , &OUT::ph_mediumNoNeuIsoPassPSV_n          , "ph_mediumNoNeuIsoPassPSV_n/I"          );
    outtree->Branch("ph_mediumNoPhoIsoPassPSV_n"          , &OUT::ph_mediumNoPhoIsoPassPSV_n          , "ph_mediumNoPhoIsoPassPSV_n/I"          );

    outtree->Branch("ph_mediumNoSIEIEFailPSV_n"           , &OUT::ph_mediumNoSIEIEFailPSV_n           , "ph_mediumNoSIEIEFailPSV_n/I"           );
    outtree->Branch("ph_mediumNoChIsoFailPSV_n"           , &OUT::ph_mediumNoChIsoFailPSV_n           , "ph_mediumNoChIsoFailPSV_n/I"           );
    outtree->Branch("ph_mediumNoNeuIsoFailPSV_n"          , &OUT::ph_mediumNoNeuIsoFailPSV_n          , "ph_mediumNoNeuIsoFailPSV_n/I"          );
    outtree->Branch("ph_mediumNoPhoIsoFailPSV_n"          , &OUT::ph_mediumNoPhoIsoFailPSV_n          , "ph_mediumNoPhoIsoFailPSV_n/I"          );

    outtree->Branch("ph_mediumNoSIEIEPassCSEV_n"           , &OUT::ph_mediumNoSIEIEPassCSEV_n           , "ph_mediumNoSIEIEPassCSEV_n/I"           );
    outtree->Branch("ph_mediumNoChIsoPassCSEV_n"           , &OUT::ph_mediumNoChIsoPassCSEV_n           , "ph_mediumNoChIsoPassCSEV_n/I"           );
    outtree->Branch("ph_mediumNoNeuIsoPassCSEV_n"          , &OUT::ph_mediumNoNeuIsoPassCSEV_n          , "ph_mediumNoNeuIsoPassCSEV_n/I"          );
    outtree->Branch("ph_mediumNoPhoIsoPassCSEV_n"          , &OUT::ph_mediumNoPhoIsoPassCSEV_n          , "ph_mediumNoPhoIsoPassCSEV_n/I"          );

    outtree->Branch("ph_mediumNoSIEIEFailCSEV_n"           , &OUT::ph_mediumNoSIEIEFailCSEV_n           , "ph_mediumNoSIEIEFailCSEV_n/I"           );
    outtree->Branch("ph_mediumNoChIsoFailCSEV_n"           , &OUT::ph_mediumNoChIsoFailCSEV_n           , "ph_mediumNoChIsoFailCSEV_n/I"           );
    outtree->Branch("ph_mediumNoNeuIsoFailCSEV_n"          , &OUT::ph_mediumNoNeuIsoFailCSEV_n          , "ph_mediumNoNeuIsoFailCSEV_n/I"          );
    outtree->Branch("ph_mediumNoPhoIsoFailCSEV_n"          , &OUT::ph_mediumNoPhoIsoFailCSEV_n          , "ph_mediumNoPhoIsoFailCSEV_n/I"          );

    outtree->Branch("ptSorted_ph_loose_idx"  , &OUT::ptSorted_ph_loose_idx );
    outtree->Branch("ptSorted_ph_medium_idx"  , &OUT::ptSorted_ph_medium_idx );
    outtree->Branch("ptSorted_ph_tight_idx"  , &OUT::ptSorted_ph_tight_idx );

    outtree->Branch("ptSorted_ph_mediumPassPSV_idx"   , &OUT::ptSorted_ph_mediumPassPSV_idx );
    outtree->Branch("ptSorted_ph_mediumFailPSV_idx"   , &OUT::ptSorted_ph_mediumFailPSV_idx );
    outtree->Branch("ptSorted_ph_mediumPassCSEV_idx"  , &OUT::ptSorted_ph_mediumPassCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumFailCSEV_idx"  , &OUT::ptSorted_ph_mediumFailCSEV_idx );

    outtree->Branch("ptSorted_ph_mediumPassEleOlap_idx"  , &OUT::ptSorted_ph_mediumPassEleOlap_idx);
    outtree->Branch("ptSorted_ph_mediumPassEleOlapPassCSEV_idx"  , &OUT::ptSorted_ph_mediumPassEleOlapPassCSEV_idx);
    outtree->Branch("ptSorted_ph_mediumPassEleOlapFailCSEV_idx"  , &OUT::ptSorted_ph_mediumPassEleOlapFailCSEV_idx);

    outtree->Branch("ptSorted_ph_mediumNoSIEIE_idx"   , &OUT::ptSorted_ph_mediumNoSIEIE_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIso_idx"   , &OUT::ptSorted_ph_mediumNoChIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoNeuIso_idx"  , &OUT::ptSorted_ph_mediumNoNeuIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIso_idx"  , &OUT::ptSorted_ph_mediumNoPhoIso_idx );

    outtree->Branch("ptSorted_ph_mediumNoSIEIENoChIso_idx", &OUT::ptSorted_ph_mediumNoSIEIENoChIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoSIEIENoNeuIso_idx", &OUT::ptSorted_ph_mediumNoSIEIENoNeuIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoSIEIENoPhoIso_idx", &OUT::ptSorted_ph_mediumNoSIEIENoPhoIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoNoPhoIso_idx", &OUT::ptSorted_ph_mediumNoChIsoNoPhoIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoNoNeuIso_idx", &OUT::ptSorted_ph_mediumNoChIsoNoNeuIso_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIsoNoNeuIso_idx", &OUT::ptSorted_ph_mediumNoPhoIsoNoNeuIso_idx );

    outtree->Branch("ptSorted_ph_mediumNoSIEIEPassPSV_idx"   , &OUT::ptSorted_ph_mediumNoSIEIEPassPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoPassPSV_idx"   , &OUT::ptSorted_ph_mediumNoChIsoPassPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoNeuIsoPassPSV_idx"  , &OUT::ptSorted_ph_mediumNoNeuIsoPassPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIsoPassPSV_idx"  , &OUT::ptSorted_ph_mediumNoPhoIsoPassPSV_idx );

    outtree->Branch("ptSorted_ph_mediumNoSIEIEFailPSV_idx"   , &OUT::ptSorted_ph_mediumNoSIEIEFailPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoFailPSV_idx"   , &OUT::ptSorted_ph_mediumNoChIsoFailPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoNeuIsoFailPSV_idx"  , &OUT::ptSorted_ph_mediumNoNeuIsoFailPSV_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIsoFailPSV_idx"  , &OUT::ptSorted_ph_mediumNoPhoIsoFailPSV_idx );

    outtree->Branch("ptSorted_ph_mediumNoSIEIEPassCSEV_idx"   , &OUT::ptSorted_ph_mediumNoSIEIEPassCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoPassCSEV_idx"   , &OUT::ptSorted_ph_mediumNoChIsoPassCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoNeuIsoPassCSEV_idx"  , &OUT::ptSorted_ph_mediumNoNeuIsoPassCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIsoPassCSEV_idx"  , &OUT::ptSorted_ph_mediumNoPhoIsoPassCSEV_idx );

    outtree->Branch("ptSorted_ph_mediumNoSIEIEFailCSEV_idx"   , &OUT::ptSorted_ph_mediumNoSIEIEFailCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoChIsoFailCSEV_idx"   , &OUT::ptSorted_ph_mediumNoChIsoFailCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoNeuIsoFailCSEV_idx"  , &OUT::ptSorted_ph_mediumNoNeuIsoFailCSEV_idx );
    outtree->Branch("ptSorted_ph_mediumNoPhoIsoFailCSEV_idx"  , &OUT::ptSorted_ph_mediumNoPhoIsoFailCSEV_idx );

#endif

    if( build_truth ) {
        outtree->Branch("trueph_n"           , &OUT::trueph_n, "trueph_n/I" );
        outtree->Branch("trueph_pt"           , &OUT::trueph_pt                        );
        outtree->Branch("trueph_eta"          , &OUT::trueph_eta                       );
        outtree->Branch("trueph_phi"          , &OUT::trueph_phi                       );
        outtree->Branch("trueph_lep_dr"       , &OUT::trueph_lep_dr                    );
        outtree->Branch("trueph_motherPID"    , &OUT::trueph_motherPID                 );
        outtree->Branch("trueph_status"       , &OUT::trueph_status                    );
        outtree->Branch("trueph_isPromptFS"   , &OUT::trueph_isPromptFS                );
        outtree->Branch("trueph_FHPFS"        , &OUT::trueph_FHPFS                     );

        outtree->Branch("truelep_n"           , &OUT::truelep_n, "truelep_n/I" );
        outtree->Branch("truelep_pt"          , &OUT::truelep_pt                       );
        outtree->Branch("truelep_eta"         , &OUT::truelep_eta                      );
        outtree->Branch("truelep_phi"         , &OUT::truelep_phi                      );
        outtree->Branch("truelep_e"           , &OUT::truelep_e                        );
        outtree->Branch("truelep_motherPID"   , &OUT::truelep_motherPID                );
        outtree->Branch("truelep_status"      , &OUT::truelep_status                   );
        outtree->Branch("truelep_PID"         , &OUT::truelep_PID                      );

        outtree->Branch("truenu_n"            , &OUT::truenu_n, "truenu_n/I"           );
        outtree->Branch("truemed_mass"        , &OUT::truemed_mass, "truemed_mass/F"   );

        outtree->Branch("trueW_n"           , &OUT::trueW_n, "trueW_n/I" );
        outtree->Branch("trueW_pt"           , &OUT::trueW_pt                        );
        outtree->Branch("trueW_eta"          , &OUT::trueW_eta                       );
        outtree->Branch("trueW_phi"          , &OUT::trueW_phi                       );
        outtree->Branch("trueW_m"            , &OUT::trueW_m                         );
        outtree->Branch("trueW_motherPID"    , &OUT::trueW_motherPID                 );
        outtree->Branch("trueW_status"       , &OUT::trueW_status                    );

        outtree->Branch("truelepnu_m"         , &OUT::truelepnu_m, "truelepnu_m/F"     );
        outtree->Branch("truelepnuph_m"       , &OUT::truelepnuph_m, "truelepnuph_m/F" );
        outtree->Branch("truelepph_dr"        , &OUT::truelepph_dr, "truelepph_dr/F"   );
        outtree->Branch("truemt_lep_met"      , &OUT::truemt_lep_met, "truemt_lep_met/F");
        outtree->Branch("truemt_lep_met_ph" , &OUT::truemt_lep_met_ph, "truemt_lep_met_ph/F");
        outtree->Branch("truemt_res" , &OUT::truemt_res, "truemt_res/F");
        outtree->Branch("truemt_res_l23" , &OUT::truemt_res_l23, "truemt_res_l23/F");
        outtree->Branch("truemt_res_lO" , &OUT::truemt_res_lO, "truemt_res_lO/F");
        outtree->Branch("trueht" , &OUT::trueht, "trueht/F");

        outtree->Branch("isWMuDecay"          , &OUT::isWMuDecay, "isWMuDecay/O"       );
        outtree->Branch("isWElDecay"          , &OUT::isWElDecay, "isWElDecay/O"       );
        outtree->Branch("isWTauDecay"         , &OUT::isWTauDecay, "isWTauDecay/O"     );
        outtree->Branch("isWTauElDecay"         , &OUT::isWTauElDecay, "isWTauElDecay/O"     );
        outtree->Branch("isWTauMuDecay"         , &OUT::isWTauMuDecay, "isWTauMuDecay/O"     );
        outtree->Branch("WIDStep"             , &OUT::WIDStep    , "WIDStep/I"     );
    }

    outtree->Branch("NLOWeight", &OUT::NLOWeight, "NLOWeight/F" );
    outtree->Branch("QCDScaleWeights", &OUT::QCDScaleWeights );
    outtree->Branch("PDFWeights", &OUT::PDFWeights );
    outtree->Branch("PUWeight", &OUT::PUWeight, "PUWeight/F" );
    outtree->Branch("PUWeightUP5", &OUT::PUWeightUP5, "PUWeightUP5/F" );
    outtree->Branch("PUWeightUP10", &OUT::PUWeightUP10, "PUWeightUP10/F" );
    outtree->Branch("PUWeightDN5", &OUT::PUWeightDN5, "PUWeightDN5/F" );
    outtree->Branch("PUWeightDN10", &OUT::PUWeightDN10, "PUWeightDN10/F" );

#ifndef EXISTS_prefweight
    outtree->Branch("prefweight", &OUT::prefweight, "prefweight/F" );
    outtree->Branch("prefweightup", &OUT::prefweightup, "prefweightup/F" );
    outtree->Branch("prefweightdown", &OUT::prefweightdown, "prefweightdown/F" );
#endif


    std::map<std::string, std::string>::const_iterator eitr;
    BOOST_FOREACH( ModuleConfig & mod_conf, configs ) {
    
        if( mod_conf.GetName() == "FilterBlind" ) { 

            eitr = mod_conf.GetInitData().find( "isData" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string data = eitr->second;
                std::transform(data.begin(), data.end(), data.begin(), ::tolower);
                if( data=="true") _isData=true;
                else              _isData=false;
            }

        }
        if (mod_conf.GetName() == "BuildEventVars") {
            std::map<std::string, std::string>::const_iterator year =
                mod_conf.GetInitData().find("year");
            if (year != mod_conf.GetInitData().end()) {
                _year = std::stoi(year->second);
                std::cout << "year = " << _year << std::endl;
            }
        }
        if( mod_conf.GetName() == "FilterEvent" ) { 

            for ( int i = 0 ; i < 6 ; i++ ) OUT::QCDScaleWeights->push_back(1.);
            for ( int i = 0 ; i < 101 ; i++ ) OUT::PDFWeights->push_back(1.);

            eitr = mod_conf.GetInitData().find( "evalCutflow" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string evalcutflow = eitr->second;
                std::transform(evalcutflow.begin(), evalcutflow.end(), evalcutflow.begin(), ::tolower);
                if( evalcutflow=="true") _filterevent_cutflow=true;
                else                     _filterevent_cutflow=false;
                std::cout<< "evalCutFlow: "<< _filterevent_cutflow <<std::endl;
            }

        }
        if( mod_conf.GetName() == "FilterMuon" ) { 

            //Rochester input file
            eitr = mod_conf.GetInitData().find( "FilePathRochester" );
            if( eitr != mod_conf.GetInitData().end() ) {
              rc.init((eitr->second).c_str());
            }
            //trigger
             eitr = mod_conf.GetInitData().find( "triggerMatchBits" );
            if( eitr != mod_conf.GetInitData().end() ) {
              std::vector<std::string> trigger_bit_list = Tokenize( eitr->second, "," );
              for( std::vector<std::string>::const_iterator bitr = trigger_bit_list.begin(); bitr != trigger_bit_list.end(); ++bitr ) {
                std::stringstream ss_id( *bitr );
                int trig_id;
                ss_id >> trig_id;
                _muonTrigMatchBits.push_back(trig_id);
              }
            }
            eitr = mod_conf.GetInitData().find( "evalPID" );
            if( eitr != mod_conf.GetInitData().end() ) {
              std::string pid = eitr->second;
              if( pid == "tight"     ) {  _eval_mu_tight       = true; std::cout<< "eval only mu tight"<<std::endl; }
              if( pid == "medium"    ) {  _eval_mu_medium      = true; std::cout<< "eval only mu medium"<<std::endl; }
              if( pid == "loose"     ) {  _eval_mu_loose       = true; std::cout<< "eval only mu loose"<<std::endl; }
            }

        }
        if( mod_conf.GetName() == "FilterElectron" ) { 

	           eitr = mod_conf.GetInitData().find( "triggerMatchBits" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::vector<std::string> trigger_bit_list = Tokenize( eitr->second, "," );
                for( std::vector<std::string>::const_iterator bitr = trigger_bit_list.begin(); bitr != trigger_bit_list.end(); ++bitr ) {
                    std::stringstream ss_id( *bitr );
                    int trig_id;
                    ss_id >> trig_id;
                    _electronTrigMatchBits.push_back(trig_id);
                }
            }
            eitr = mod_conf.GetInitData().find( "evalPID" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string pid = eitr->second;
                if( pid == "tight"     ) _eval_el_tight       = true;
                if( pid == "medium"    ) _eval_el_medium      = true;
                if( pid == "loose"     ) _eval_el_loose       = true;
                if( pid == "veryloose" ) _eval_el_veryloose   = true;
            }

        }
        if( mod_conf.GetName() == "FilterPhoton" ) { 

             eitr = mod_conf.GetInitData().find( "evalPID" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string pid = eitr->second;
                if( pid == "tight"     ) _eval_ph_tight       = true;
                if( pid == "medium"    ) _eval_ph_medium      = true;
                if( pid == "loose"     ) _eval_ph_loose       = true;
            }

        }
        if( mod_conf.GetName() == "FilterJet" ) { 

             eitr = mod_conf.GetInitData().find( "evalBTagID" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string pid = eitr->second;
                if( pid == "tight"     ) _eval_btag_tight       = true;
                if( pid == "medium"    ) _eval_btag_medium      = true;
                if( pid == "loose"     ) _eval_btag_loose       = true;
            }

        }
        if( mod_conf.GetName() == "FilterTrigger" ) { 

             eitr = mod_conf.GetInitData().find( "triggerBits" );
            if( eitr != mod_conf.GetInitData().end() ) {
                // get the mapping of trigger ID to the name
                // This could be taken from the TrigInfoTree
                std::vector<std::string> trigger_bit_list = Tokenize( eitr->second, "," );
                //for( std::vector<std::string>::const_iterator bitr = trigger_bit_list.begin(); bitr != trigger_bit_list.end(); ++bitr ) {
                for ( auto const& bitr : trigger_bit_list ){
                    std::vector<std::string> name_id_map = Tokenize( bitr, ":" );
                    std::stringstream ss_id( name_id_map[0] );
                    // convert the ID to an int
                    int trig_id;
                    ss_id >> trig_id;
                    // make an entry in the output map
                    triggerResults[trig_id] = 0;
                    outtree->Branch(name_id_map[1].c_str(), &(triggerResults[trig_id]), (name_id_map[1]+"/O").c_str() );
                    triggerNames[trig_id] = name_id_map[1];
                    //mod_conf.PassCounter(name_id_map[1], true, 0.0);
                }
            }
            eitr = mod_conf.GetInitData().find( "AuxTreeName" );
            if( eitr != mod_conf.GetInitData().end() ) {
                // if the trigger bits were provided, do not overwrite
                if( triggerResults.size() ) {
                    std::cout << "WARNING -- triggerBits were provided.  Will not get trigger info from the tree.  Please provide either triggerBits or AuxTreeName" << std::endl;
                }
                else {
                    const std::string & trig_tree_name = eitr->second;
                    TFile * fptr = _input_tree->GetFile();
                    TTree * trigTree = dynamic_cast<TTree*>(fptr->Get(trig_tree_name.c_str()));
                    if( !trigTree ) {
                        std::cout << "Did not locate tree with name " + trig_tree_name + ". Will not store trigger results!" << std::endl;
                    }
                    else {
                        Int_t   trigger_ids;
                        Char_t  trigger_names;
                        std::cout << trigTree->GetName() << std::endl;
                        trigTree->SetBranchAddress("trigger_ids", &trigger_ids);
                        trigTree->SetBranchAddress("trigger_names", &trigger_names);

                        for( int tidx = 0; tidx < trigTree->GetEntries(); tidx++  ) {
                            trigTree->GetEntry(tidx);
                            triggerResults[trigger_ids] = 0;
                            std::string tn( &trigger_names);
                            outtree->Branch(tn.c_str(), &(triggerResults[trigger_ids]), (tn+"/O").c_str() );
                        }
                    }
                }
            }

        }
       if( mod_conf.GetName() == "FilterMET" ) {

            // working on met filter. Basically copied from trigger code
             eitr = mod_conf.GetInitData().find( "METFilterBits" );
            if( eitr != mod_conf.GetInitData().end() ) {
                // get the mapping of metfilter index to the name
                // This could be taken from the FilterInfoTree
                std::vector<std::string> metfilter_bit_list = Tokenize( eitr->second, "," );
                for( std::vector<std::string>::const_iterator bitr = metfilter_bit_list.begin(); bitr != metfilter_bit_list.end(); ++bitr ) {
                    std::vector<std::string> name_id_map = Tokenize( *bitr, ":" );
                    std::stringstream ss_id( name_id_map[0] );
                    // convert the ID to an int
                    int metfilter_id;
                    ss_id >> metfilter_id;
                    // make an entry in the output map
                    metfilterResults[metfilter_id] = 0;
                    outtree->Branch(name_id_map[1].c_str(), &(metfilterResults[metfilter_id]), (name_id_map[1]+"/O").c_str() );
                }
            }
            eitr = mod_conf.GetInitData().find( "METAuxTreeName" );
            if( eitr != mod_conf.GetInitData().end() ) {
                // if the metfilter bits were provided, do not overwrite
                if( metfilterResults.size() ) {
                    std::cout << "WARNING -- METFilterBits were provided.  Will not get metfilter info from the tree.  Please provide either metfilterBits or METAuxTreeName" << std::endl;
                }
                else {
                    const std::string & metfilter_tree_name = eitr->second;
                    TFile * fptr = _input_tree->GetFile();
                    TTree * metfilterTree = dynamic_cast<TTree*>(fptr->Get(metfilter_tree_name.c_str()));
                    if( !metfilterTree ) {
                        std::cout << "Did not locate tree with name " + metfilter_tree_name + ". Will not store metfilter results!" << std::endl;
                    }
                    else {
                        Int_t   filter_ids;
                        Char_t  filter_names;
                        std::cout << metfilterTree->GetName() << std::endl;
                        metfilterTree->SetBranchAddress("filter_ids",   &filter_ids);
                        metfilterTree->SetBranchAddress("filter_names", &filter_names);

                        for( int tidx = 0; tidx < metfilterTree->GetEntries(); tidx++  ) {
                            metfilterTree->GetEntry(tidx);
                            metfilterResults[filter_ids] = 0;
                            std::string tn( &filter_names);
                            outtree->Branch(tn.c_str(), &(metfilterResults[filter_ids]), (tn+"/O").c_str() );
                        }
                    }
                }
            }

        }
        if( mod_conf.GetName() == "WeightEvent" ) { 

             eitr = mod_conf.GetInitData().find( "ApplyNLOWeight" );
            if( eitr != mod_conf.GetInitData().end() ) {
                if( eitr->second == "true" ) {
                    _needs_nlo_weght = true;
                }
            }
            std::string sample_filename;
            std::string data_filename;
            std::string sample_histname;
            std::string data_histname;
            std::string pdf_histname;
            std::map<std::string, std::string>::const_iterator sfitr = mod_conf.GetInitData().find( "sample_file" );
            std::map<std::string, std::string>::const_iterator dfitr = mod_conf.GetInitData().find( "data_file" );
            std::map<std::string, std::string>::const_iterator shitr = mod_conf.GetInitData().find( "sample_hist" );
            std::map<std::string, std::string>::const_iterator dhitr = mod_conf.GetInitData().find( "data_hist" );
            std::map<std::string, std::string>::const_iterator phitr = mod_conf.GetInitData().find( "pdf_hist" );

            bool get_weight_hists = true;

            if( sfitr != mod_conf.GetInitData().end() ) {
                sample_filename = sfitr->second;
            }
            else {
                get_weight_hists = false;
            }
            if( dfitr != mod_conf.GetInitData().end() ) {
                data_filename = dfitr->second;
            }
            else {
                get_weight_hists = false;
            }
            if( shitr != mod_conf.GetInitData().end() ) {
                sample_histname = shitr->second;
            }
            else {
                get_weight_hists = false;
            }
            if( dhitr != mod_conf.GetInitData().end() ) {
                data_histname = dhitr->second;
            }
            else {
                get_weight_hists = false;
            }
            if( phitr != mod_conf.GetInitData().end() ) {
                pdf_histname = phitr->second;
            }
            else {
                get_weight_hists = false;
            }

            if( get_weight_hists ) {

                _puweight_sample_file = TFile::Open( sample_filename.c_str(), "READ" );
                _puweight_data_file = TFile::Open( data_filename.c_str(), "READ" );

                _puweight_sample_hist = dynamic_cast<TH1D*>(_puweight_sample_file->Get( sample_histname.c_str() ) ) ;
                _puweight_data_hist = dynamic_cast<TH1D*>(_puweight_data_file->Get( data_histname.c_str() ) );

                _pdfweight_sample_hist = dynamic_cast<TH1D*>(_puweight_sample_file->Get( pdf_histname.c_str() ) ) ;
                if( !_puweight_sample_hist ) {
                    std::cout << "Could not retrieve histogram " << sample_histname << " from " << sample_filename  << std::endl;
                }
                if( !_puweight_data_hist ) {
                    std::cout << "Could not retrieve histogram " << data_histname << " from " << data_filename << std::endl;
                }
                if( !_pdfweight_sample_hist ) {
                    std::cout << "Could not retrieve histogram " << pdf_histname << " from " << sample_filename  << std::endl;
                }
            }
            else {
                std::cout << "WeightEvent::ERROR - Needed histogram does not exist! Will not apply PU weights!" << std::endl;
            }
            
        }
        if( mod_conf.GetName() == "FilterDataQuality" ) { 

            eitr = mod_conf.GetInitData().find( "jsonFile" );
            if( eitr != mod_conf.GetInitData().end() ) {
                std::string jsonFile = eitr->second;

                std::string line;
                std::ifstream infile( jsonFile.c_str() );
                if( infile.is_open() ) {
                    while( getline( infile, line ) ) {
                        std::vector<std::string> key_val_tok = Tokenize( line, ":" );
                        std::vector<std::string> run_number_tok = Tokenize( key_val_tok[0], "\"" );
                        // find first and last brackets
                        std::string::size_type first_bracket = key_val_tok[1].find( "[" );
                        std::string::size_type last_bracket  = key_val_tok[1].rfind( "]" );

                        std::string rm_last = key_val_tok[1].substr( 0, last_bracket );
                        std::string all_vals  = rm_last.substr( first_bracket+1 );

                        int run_number;
                        std::stringstream run_number_ss( run_number_tok[1] );
                        run_number_ss >> run_number;

                        std::cout << "Run = " << run_number_tok[1] << " values = " << all_vals << std::endl;

                        std::vector<std::string> ranges_tok = Tokenize( all_vals, "[" );

                        std::vector<int> full_range;
                        for( std::vector<std::string>::const_iterator itr = ranges_tok.begin(); itr != ranges_tok.end(); ++itr ) {
                            std::string range_str = *itr;
                            std::string::size_type bracket_pos = range_str.find("]");
                            std::string range = range_str.substr( 0, bracket_pos );

                            std::vector<std::string> range_tok = Tokenize( range, "," );
                            if( range_tok.size() != 2 ) {
                                std::cout << "Expected two entries in the range.  String was " << range << std::endl;
                                continue;
                            }

                            int range_begin;
                            int range_end;

                            std::stringstream range_begin_ss( range_tok[0] );
                            std::stringstream range_end_ss  ( range_tok[1] );

                            range_begin_ss >> range_begin;
                            range_end_ss   >> range_end;

                            for( int ls = range_begin ; ls <= range_end; ++ls ) {
                                full_range.push_back(ls);
                            }

                        }
                        _quality_map[run_number] = full_range;
                    }
                }
            }
        }
    }

}

void RunModule::finalize() {
    // Save EventWeight
    _outfile->cd();
    h_EventWeight->Write(); 

}

bool RunModule::execute( std::vector<ModuleConfig> & configs ) {

    // In BranchInit
    CopyInputVarsToOutput();

    // loop over configured modules
    bool save_event = true;
    printevent = false;
    if( IN::eventNumber%10000 == 42 ) printevent = true;
    if( printevent ) std::cout << " eventNumber " << IN::eventNumber << std::endl;
    BOOST_FOREACH( ModuleConfig & mod_conf, configs ) {
        save_event &= ApplyModule( mod_conf );
        if( printevent ) std::cout << " module " << mod_conf.GetName() << " result " << save_event << std::endl;
	
    }
    // Fill EventWeight histogram
    h_EventWeight->Fill(OUT::NLOWeight); 

    return save_event;

}

bool RunModule::ApplyModule( ModuleConfig & config ) {

    bool keep_evt = true;

    if( config.GetName() == "FilterMuon" ) {
        FilterMuon( config );
    }
    if( config.GetName() == "FilterElectron" ) {
        FilterElectron( config );
    }
    if( config.GetName() == "FilterPhoton" ) {
        FilterPhoton( config );
    }
    if( config.GetName() == "FilterJet" ) {
        FilterJet( config );
    }
    if( config.GetName() == "BuildEventVars" ) {
        BuildEventVars( config );
    }
    if( config.GetName() == "MakePhotonCountVars" ) {
        MakePhotonCountVars( config );
    }
    if( config.GetName() == "BuildTruth" ) {
        BuildTruth( config );
    }
    if( config.GetName() == "FilterEvent" ) {
        keep_evt &= FilterEvent( config );
    }
    if( config.GetName() == "FilterTrigger" ) {
        keep_evt &= FilterTrigger( config );
    }
    if( config.GetName() == "FilterMET" ){
        keep_evt &= FilterMET( config );
    }
    if( config.GetName() == "WeightEvent" ) {
        WeightEvent( config );
    }
    if( config.GetName() == "FilterDataQuality" ) {
        keep_evt &= FilterDataQuality( config );
    }
    if( config.GetName() == "FilterBlind" ) {
        keep_evt &= FilterBlind( config );
    }


    return keep_evt;


}

void RunModule::FilterMuon( ModuleConfig & config ) {

    OUT::mu_n                  = 0;
    OUT::mu_pt20_n             = 0;
    OUT::mu_pt30_n             = 0;
    OUT::mu_pt_rc              -> clear();
    OUT::mu_e_rc              -> clear();
    OUT::mu_pt_rc_up              -> clear();
    OUT::mu_e_rc_up              -> clear();
    OUT::mu_pt_rc_down              -> clear();
    OUT::mu_e_rc_down              -> clear();
    OUT::mu_pt_Sigma_up              -> clear();
    OUT::mu_e_Sigma_up              -> clear();
    OUT::mu_pt_Sigma_down              -> clear();
    OUT::mu_e_Sigma_down              -> clear();
    OUT::mu_passTight          -> clear();
    OUT::mu_passMedium         -> clear();
    OUT::mu_passLoose          -> clear();
    OUT::mu_hasTrigMatch       -> clear();
    OUT::mu_trigMatch_dr       -> clear();
    // truth matching is happeing in BuildTruth
    //OUT::mu_hasTruthMatchMu       -> clear();
    //OUT::mu_truthMatchMu_dr    -> clear();
    //OUT::mu_truthMatchMu_pt    -> clear();

    ClearOutputPrefix("mu_");

    for( int idx = 0; idx < IN::mu_n; ++idx ) {

        float pt = IN::mu_pt->at(idx);
        float eta = IN::mu_eta->at(idx);
	float phi = IN::mu_phi->at(idx);
	float Q = IN::mu_charge->at(idx);
	int nl = IN::mu_nTrkLayers->at(idx);
	float rcsfs = 1.; 
	float rcsfserr = 0.;

	//////  Begin Rochester corrections! //////
	if( OUT::isData) {
	  double dtSF = rc.kScaleDT(Q, pt, eta, phi, 0, 0); //data, central correction
	  std::vector<double> dtSF_errs;
	  
	  //get statistical error (set 1)
	  double dtSF_statmean = 0.;
	  double dtSF_statvar = 0;
	  std::vector<double> dtSF_staterrs;
	  for (int i = 0; i < 100; i++)
	    {
	      double f = rc.kScaleDT(Q, pt, eta, phi, 1, i);
	      dtSF_staterrs.push_back(f);
	      dtSF_statmean += f;
	    }
	  dtSF_statmean = dtSF_statmean/100.;
	  for (int i = 0; i < 100; i++)
	    {
	      dtSF_statvar += pow(dtSF_staterrs.at(i) - dtSF_statmean, 2);
	    }
	  dtSF_statvar = dtSF_statvar/100.;
	  dtSF_errs.push_back(sqrt(dtSF_statvar));

	  //get systematic errors (set 2-5)
	  for (int i = 2; i < 6; i++)
	    {
	      double dtSF_witherr = rc.kScaleDT(Q, pt, eta, phi, i, 0);
	      dtSF_errs.push_back(dtSF_witherr - dtSF);
	    }

	  // total error = quadrature sum of stat and sys errors
	  double dtSFerr = 0.;
	  for (int i = 0; i < 5; i++)
	    {
	      dtSFerr += (dtSF_errs.at(i))*(dtSF_errs.at(i));
	    }
	  dtSFerr = sqrt(dtSFerr);

	  rcsfs = dtSF;
	  rcsfserr = dtSFerr;
	}
#ifdef EXISTS_gen_n
	else {
          double mcSF = 1.;
          float genpt = 0.;
          double u = 0.;
          bool matched = false;
          
          // some simple truth matching for rochester corrections
          float mindr = 10;
          TLorentzVector mulv;
          mulv.SetPtEtaPhiE( IN::mu_pt->at(idx),
                             IN::mu_eta->at(idx),
                             IN::mu_phi->at(idx),
                             IN::mu_e->at(idx)
                             );
          for( int gidx = 0; gidx < IN::gen_n; ++gidx ) {
            int id = IN::gen_PID->at(gidx);
            int absid = abs(id);
            if (absid != 13)
                continue;
            int st = IN::gen_status->at(gidx);
            if (st != 1)
                continue;
                
            TLorentzVector tleplv;
            tleplv.SetPtEtaPhiE( IN::gen_pt->at(gidx),
                                 IN::gen_eta->at(gidx),
                                 IN::gen_phi->at(gidx),
                                 IN::gen_e->at(gidx)
                               );
            
            float dr = mulv.DeltaR( tleplv );

            if( dr < mindr && tleplv.Pt()/mulv.Pt() < 2 && mulv.Pt()/tleplv.Pt() < 2) {
                mindr = dr;
                genpt = tleplv.Pt();
                matched = true;
            }
          }
          
          if (matched) {
	    mcSF = rc.kSpreadMC(Q, pt, eta, phi, genpt, 0, 0); //MC scale and extra smearing with matched gen muon
          }
          else {
	    u = gRandom->Rndm();
	    mcSF = rc.kSmearMC(Q, pt, eta, phi, nl, u, 0, 0); //MC scale and extra smearing when matched gen muon is not available
          }
	  std::vector<double> mcSF_errs;
	  //get statistical error (set 1)
	  double mcSF_statmean = 0.;
	  double mcSF_statvar = 0;
	  std::vector<double> mcSF_staterrs;
	  for (int i = 0; i < 100; i++)
	    {
	      double f = 1.;
              if (matched) {
                f = rc.kSpreadMC(Q, pt, eta, phi, genpt, 1, i);
              }
              else {
	        f = rc.kSmearMC(Q, pt, eta, phi, nl, u, 1, i); // matched gen muon not available
              }
	      mcSF_staterrs.push_back(f);
	      mcSF_statmean += f;
	    }
	  mcSF_statmean = mcSF_statmean/100.;
	  for (int i = 0; i < 100; i++)
	    {
	      mcSF_statvar += pow(mcSF_staterrs.at(i) - mcSF_statmean, 2);
	    }
	  mcSF_statvar = mcSF_statvar/100.;
	  mcSF_errs.push_back(sqrt(mcSF_statvar));

	  //get systematic errors (set 2-5)
	  for (int i = 2; i < 6; i++)
	    {
	      double mcSF_witherr = 1.;
              if (matched) {
                mcSF_witherr = rc.kSpreadMC(Q, pt, eta, phi, genpt, i, 0);
              }
              else {
	        mcSF_witherr = rc.kSmearMC(Q, pt, eta, phi, nl, u, i, 0); // matched gen muon not available
              }
	      mcSF_errs.push_back(mcSF_witherr - mcSF);
	    }

	  // total error = quadrature sum of stat and sys errors
	  double mcSFerr = 0.;
	  for (int i = 0; i < 5; i++)
	    {
	      mcSFerr += (mcSF_errs.at(i))*(mcSF_errs.at(i));
	    }
	  mcSFerr = sqrt(mcSFerr);

	  rcsfs = mcSF;
	  rcsfserr = mcSFerr;
	}
#endif
	
	//////  End Rochester corrections! //////

	float ptrc = pt*rcsfs;
        
        if (rcsfs > 3) {
            std::cout << "WARNING: rcsfs = " << rcsfs << std::endl;
        }
        
	      float ptrc_up = pt*(rcsfs + rcsfserr); // vary RC up
	      float ptrc_down = pt*(rcsfs - rcsfserr); // vary RC down



        ///// additional muon energy smearing according to https://twiki.cern.ch/twiki/bin/view/CMS/MuonPOG
        TLorentzVector mulv;
        mulv.SetPtEtaPhiM( ptrc,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );
        float muon_smear_sig = 0;
        float muon_smear_a=0;
        float muon_smear_b=0;
        float muon_smear_c=0;
        float muon_smear_d=0;
        float muon_smear_e=0;
        float ptSig_up = ptrc;
        float ptSig_down = ptrc;
        if( !OUT::isData) {
            if (_year==2018){
                if(fabs(eta)<1.2){
                    muon_smear_a=0.0062; muon_smear_b=0.000096; muon_smear_c=-9.7e-8; muon_smear_d=4.9e-11; muon_smear_e=-9e-15;
                }else if(fabs(eta)<2.1){
                    muon_smear_a=0.0136; muon_smear_b=0.000052; muon_smear_c=-2.4e-8; muon_smear_d=5.0e-12; muon_smear_e=0;
                }else if(fabs(eta)<2.4){
                    muon_smear_a=0.0174; muon_smear_b=0.000087; muon_smear_c=-3.3e-9; muon_smear_d=-1.6e-11; muon_smear_e=5e-15;
                }
            }else if(_year==2017){
                if(fabs(eta)<1.2){
                    muon_smear_a=0.0053; muon_smear_b=0.00011; muon_smear_c=-1.3e-7; muon_smear_d=6.9e-11; muon_smear_e=-1.3e-14;
                }else if(fabs(eta)<2.1){
                    muon_smear_a=0.0136; muon_smear_b=0.000063; muon_smear_c=-2.6e-8; muon_smear_d=-1.3e-12; muon_smear_e=3e-15;
                }else if(fabs(eta)<2.4){
                    muon_smear_a=0.0170; muon_smear_b=0.000084; muon_smear_c=+2.6e-9; muon_smear_d=-2.3e-11; muon_smear_e=8e-15;
                }
            }else if(_year==2016){
                if(fabs(eta)<1.2){
                    muon_smear_a=0.0062; muon_smear_b=0.0001; muon_smear_c=-1.0e-7; muon_smear_d=5.7e-11; muon_smear_e=-1.1e-14;
                }else if(fabs(eta)<2.1){
                    muon_smear_a=0.0134; muon_smear_b=0.000063; muon_smear_c=-4.7e-8; muon_smear_d=2.6e-11; muon_smear_e=-5e-15;
                }else if(fabs(eta)<2.4){
                    muon_smear_a=0.0151; muon_smear_b=0.000114; muon_smear_c=-3.7e-8; muon_smear_d=3.9e-12; muon_smear_e=1e-15;
                }
            }else{
                std::cout<<"Wrong Year setting in Muon smearing!!! "<<std::endl;
            }
            muon_smear_sig = (muon_smear_a + muon_smear_b*mulv.P() + muon_smear_c*pow(mulv.P(),2) + muon_smear_d*pow(mulv.P(),3) + muon_smear_e*pow(mulv.P(),4));
            // Additional 15% smearing for 2017,2018 |eta|>1.2
            ptSig_down = ptrc;
            if(_year!=2016 && fabs(eta)>1.2){
                ptrc = ptrc*(1+rndm.Gaus(0, muon_smear_sig*0.57) );
            }
            // 10% uncertainty and one-sided
            if(ptrc>120){
                ptSig_up = ptrc*(1+rndm.Gaus(0, muon_smear_sig*0.46) );
            }else{
                ptSig_up = ptrc;
            }
        }

        if( !config.PassFloat( "cut_pt", ptrc   ) ) continue;

        if( !config.PassFloat( "cut_eta", fabs(eta) ) ) continue;
        bool isPfMu = IN::mu_isPf->at(idx);
        bool isGloMu = IN::mu_isGlobal->at(idx);
        bool isTkMu = IN::mu_isTracker->at(idx);
        float chi2 = IN::mu_chi2->at(idx);
        int nHits =  IN::mu_nHits->at(idx);
        int nStations = IN::mu_nMuStations->at(idx);
        int nPixHits = IN::mu_nPixHits->at(idx);
        int nTrkLayers = IN::mu_nTrkLayers->at(idx);
        float d0 = IN::mu_d0->at(idx);
        float z0 = IN::mu_dz->at(idx);
        float pfIso = IN::mu_pfIso->at(idx);
        float tkIso = IN::mu_trkIso->at(idx);
        
        bool pass_tight = true;
        bool pass_loose = true;
        //bool pass_medium = true;

        bool use_eval = _eval_mu_tight || _eval_mu_medium || _eval_mu_loose;

        // loose cuts
        if( !use_eval || _eval_mu_loose ) {
            if( !config.PassBool ( "cut_isPf_loose"   , isPfMu ) ) {
                pass_loose = false;
                if( _eval_mu_loose) continue;
            }
            if( !config.PassBool ( "cut_isGlobalOrTk_loose"   , (isGloMu || isTkMu ) ) ) {
                pass_loose = false;
                if( _eval_mu_loose) continue;
            }
        }

        // tight cuts
        //std::cout<< "debug: mu tight eval "<< _eval_mu_tight<<std::endl; 
        if( !use_eval || _eval_mu_tight ) {
            if( !config.PassBool ( "cut_isGlobal_tight"       , isGloMu     ) ) {
                pass_tight = false;
                //std::cout<< "debug: mu tight eval isGlobal"<< _eval_mu_tight<<std::endl; 
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassBool ( "cut_isPf_tight"       , isPfMu     ) ) {
                pass_tight = false;
                std::cout<< "debug: mu tight eval isPf"<< _eval_mu_tight<<std::endl; 
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_chi2_tight"       , chi2  ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_nMuonHits_tight"  , nHits ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_nStations_tight" ,nStations) ){ 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_nPixelHits_tight" , nPixHits) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_nTrkLayers_tight" , nTrkLayers ) ){ 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_d0_tight"         , fabs(d0) ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_z0_tight"         , fabs(z0) ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_trkiso_tight"     , tkIso       ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
            if( !config.PassFloat( "cut_corriso_tight"    , pfIso     ) ) { 
                pass_tight = false;
                if( _eval_mu_tight ) continue;
            }
        }

        if( !config.PassBool( "cut_tight",    pass_tight)              ) continue;

        //
        // use Muon ID instead of offline calculation
        //  
        if( !config.PassBool( "cut_id_Tight",      IN::mu_isTight->at(idx)) ) continue;
        if( !config.PassFloat("cut_pfiso_tight",   IN::mu_pfIso->at(idx))   ) continue;

        //TLorentzVector mulv;
        mulv.SetPtEtaPhiM( ptrc,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );

        TLorentzVector mulv_up;
        mulv_up.SetPtEtaPhiM( ptrc_up,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );

        TLorentzVector mulv_down;
        mulv_down.SetPtEtaPhiM( ptrc_down,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );

        TLorentzVector mulv_Sigup;
        mulv_Sigup.SetPtEtaPhiM( ptSig_up,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );

        TLorentzVector mulv_Sigdown;
        mulv_Sigdown.SetPtEtaPhiM( ptSig_down,
                           IN::mu_eta->at(idx),
                           IN::mu_phi->at(idx),
                           0.1057
                           );

        float mindr = 101.0;
        for( int hltidx = 0 ; hltidx < IN::HLTObj_n; ++hltidx ) {

            std::vector<int> passTrigs = IN::HLTObj_passTriggers->at(hltidx);

            bool foundMatch = false;
            for( std::vector<int>::const_iterator bitr = _muonTrigMatchBits.begin(); bitr != _muonTrigMatchBits.end(); ++bitr ) {

                if( std::find( passTrigs.begin(), passTrigs.end(), (*bitr) ) != 
                              passTrigs.end() ) foundMatch = true;
            }

            if( !foundMatch ) continue;

            TLorentzVector hltlv;
            hltlv.SetPtEtaPhiE( IN::HLTObj_pt->at(hltidx),
                                IN::HLTObj_eta->at(hltidx),
                                IN::HLTObj_phi->at(hltidx),
                                IN::HLTObj_e->at(hltidx)
                              );

            float dr = mulv.DeltaR( hltlv );
            if( dr < mindr ) {
                mindr = dr;
            }
        }


        bool matchTrig = false;
        // change it to 0.1 according to the muon pog
        if( mindr < 0.1 ){ 
            matchTrig = true;
        }
        OUT::mu_hasTrigMatch->push_back(matchTrig);
        OUT::mu_trigMatch_dr->push_back(mindr);

        std::vector<int> matchPID;
        matchPID.push_back(13);
        matchPID.push_back(-13);

        // truth matching is happening in BuildTruth
        //float truthMinDR = 100.0;
        //TLorentzVector matchLV;
        //bool match = HasTruthMatch( mulv, matchPID, 0.2, truthMinDR, matchLV );
        //OUT::mu_hasTruthMatchMu->push_back( match );
        //OUT::mu_truthMatchMu_dr->push_back( truthMinDR );
        //OUT::mu_truthMatchMu_pt->push_back( matchLV.Pt() );
        CopyPrefixIndexBranchesInToOut( "mu_", idx );

        OUT::mu_passTight -> push_back( pass_tight );
        OUT::mu_passLoose -> push_back( pass_loose );

        OUT::mu_n++;
        
        // Write Rochester-corrected pt only after muon is accepted
        OUT::mu_pt_rc->push_back( ptrc );
        OUT::mu_e_rc->push_back( mulv.E() );
        OUT::mu_pt_rc_up->push_back( ptrc_up );
        OUT::mu_e_rc_up->push_back( mulv_up.E() );
        OUT::mu_pt_rc_down->push_back( ptrc_down );
        OUT::mu_e_rc_down->push_back( mulv_down.E() );
        OUT::mu_pt_Sigma_up->push_back( ptSig_up );
        OUT::mu_e_Sigma_up->push_back( mulv_Sigup.E() );
        OUT::mu_pt_Sigma_down->push_back( ptSig_down );
        OUT::mu_e_Sigma_down->push_back( mulv_Sigdown.E() );
        if( ptrc > 20 ) {
            OUT::mu_pt20_n++;
        }
        if( ptrc > 30 ) {
            OUT::mu_pt30_n++;
        }

         config.PassCounter("muon_pass");
    }


}

void RunModule::FilterElectron( ModuleConfig & config ) {

    OUT::el_n          = 0;
    OUT::el_pt30_n          = 0;
    OUT::el_pt35_n          = 0;
    OUT::el_pt40_n          = 0;
    OUT::el_passVeryLoose->clear();
    OUT::el_passLoose->clear();
    OUT::el_passMedium->clear();
    OUT::el_passTight->clear();
    OUT::el_hasTrigMatch->clear();
    OUT::el_trigMatch_dr->clear();
    float rho = IN::rho;

    ClearOutputPrefix("el_");

    float isorho_endcap_tight_a1      =0;
    float isorho_endcap_medium_a1     =0;
    float isorho_endcap_loose_a1      =0;
    float isorho_endcap_veryloose_a1  =0;
    float isorho_barrel_tight_a1      =0;
    float isorho_barrel_medium_a1     =0;
    float isorho_barrel_loose_a1      =0;
    float isorho_barrel_veryloose_a1  =0;
    if (!config.PassBool("cut_isorho_94x", false, false)){
          if (printevent) std::cout<< "94x iso rho cuts coefficient applied"<<std::endl; 
          isorho_endcap_tight_a1      =0.963;
          isorho_endcap_medium_a1     =0.963;
          isorho_endcap_loose_a1      =0.963;
          isorho_endcap_veryloose_a1  =0.963;
          isorho_barrel_tight_a1      =0.506;
          isorho_barrel_medium_a1     =0.506;
          isorho_barrel_loose_a1      =0.506;
          isorho_barrel_veryloose_a1  =0.506;
    }
    float hovere_endcap_tight_a1      =0;
    float hovere_endcap_medium_a1     =0;
    float hovere_endcap_loose_a1      =0;
    float hovere_endcap_veryloose_a1  =0;
    float hovere_endcap_tight_a2      =0;
    float hovere_endcap_medium_a2     =0;
    float hovere_endcap_loose_a2      =0;
    float hovere_endcap_veryloose_a2  =0;
    float hovere_barrel_tight_a1      =0;
    float hovere_barrel_medium_a1     =0;
    float hovere_barrel_loose_a1      =0;
    float hovere_barrel_veryloose_a1  =0;
    float hovere_barrel_tight_a2      =0;
    float hovere_barrel_medium_a2     =0;
    float hovere_barrel_loose_a2      =0;
    float hovere_barrel_veryloose_a2  =0;
    if(!config.PassBool( "cut_hovere_94x", false, false)) {
          if(printevent) std::cout<< "94x H/E cuts coefficient applied"<< std::endl;  
          hovere_endcap_tight_a1      =2.06;
          hovere_endcap_medium_a1     =2.52;
          hovere_endcap_loose_a1      =2.54;
          hovere_endcap_veryloose_a1  =2.54;
          hovere_endcap_tight_a2      =0.183;
          hovere_endcap_medium_a2     =0.183;
          hovere_endcap_loose_a2      =0.183;
          hovere_endcap_veryloose_a2  =0.183;
          hovere_barrel_tight_a1      =1.15;
          hovere_barrel_medium_a1     =1.16;
          hovere_barrel_loose_a1      =1.16;
          hovere_barrel_veryloose_a1  =1.16;
          hovere_barrel_tight_a2      =0.0324;
          hovere_barrel_medium_a2     =0.0324;
          hovere_barrel_loose_a2      =0.0324;
          hovere_barrel_veryloose_a2  =0.0324;
    }
    

    for( int idx = 0; idx < IN::el_n; ++idx ) {

        float pt    = IN::el_pt->at(idx);
        float eta   = IN::el_eta->at(idx);
        float sceta = IN::el_sc_eta->at(idx);

        if( !config.PassFloat( "cut_pt",  pt ) ) continue;
        if( !config.PassFloat( "cut_eta", fabs(eta) ) ) continue;

        // ECAL fudical region cut
        if( fabs(sceta)> 1.4442 && fabs(sceta)<1.566 ) continue;

        float dEtaIn = IN::el_dEtaIn->at(idx);
        float dPhiIn = IN::el_dPhiIn->at(idx);
        float sigmaIEIEFull5x5 = IN::el_sigmaIEIEfull5x5->at(idx);
        float d0 = fabs( IN::el_d0->at(idx) );
        float z0 = fabs( IN::el_dz->at(idx) );
        float hovere = IN::el_hOverE->at(idx);
        float ooEmooP = IN::el_ooEmooP->at(idx);
        float iso_rho = IN::el_pfIsoRho->at(idx);
        float el_esc  = IN::el_sc_e->at(idx); 
        bool passConvVeto = IN::el_passConvVeto->at(idx);
        int misshits = IN::el_expectedMissingInnerHits->at(idx);


        bool use_eval = _eval_el_tight || _eval_el_medium || _eval_el_loose || _eval_el_veryloose;

        bool pass_tight = true;
        bool pass_medium = true;
        bool pass_loose = true;
        bool pass_veryloose = true;

        if( fabs(sceta) < 1.479 ) { // barrel

            // Tight cuts
            if( !use_eval || _eval_el_tight ) {
                if( !config.PassFloat( "cut_absdEtaIn_barrel_tight"    , fabs(dEtaIn)       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_barrel_tight"    , fabs(dPhiIn)       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_tight" , sigmaIEIEFull5x5    ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_d0_barrel_tight"        , d0           ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_z0_barrel_tight"        , z0           ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_hovere_barrel_tight"    , hovere - hovere_barrel_tight_a1/el_esc - hovere_barrel_tight_a2*rho/el_esc ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_barrel_tight"    , ooEmooP       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_barrel_tight"   ,iso_rho - isorho_barrel_tight_a1 / pt ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassInt( "cut_passConvVeto_barrel_tight"   , passConvVeto      ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_barrel_tight"  , misshits     ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
            }
            
            // Medium cuts
            if( !use_eval || _eval_el_medium ) {
                config.PassCounter("electron_barrel");
                config.PassCounter("electron_endcap",true,0.0);
                if( !config.PassFloat( "cut_absdEtaIn_barrel_medium"    , fabs(dEtaIn)        , false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                   config.PassCounter("cut_absdEtaIn_medium");
                }
                if( !config.PassFloat( "cut_absdPhiIn_barrel_medium"    , fabs(dPhiIn)       ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_absdPhiIn_medium");
                }
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_medium" , sigmaIEIEFull5x5    ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_sigmaIEIE_medium");
                }
                if( !config.PassFloat( "cut_d0_barrel_medium"        , d0            ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_d0_medium");
                }
                if( !config.PassFloat( "cut_z0_barrel_medium"        , z0           ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_z0_medium");
                }
                if( !config.PassFloat( "cut_hovere_barrel_medium"    , hovere - hovere_barrel_medium_a1/el_esc - hovere_barrel_medium_a2*rho/el_esc  ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_hovere_medium");
                }
                if( !config.PassFloat( "cut_ooEmooP_barrel_medium"    , ooEmooP       ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_ooEmooP_medium");
                }
                if( !config.PassFloat( "cut_isoRho_barrel_medium"   ,iso_rho - isorho_barrel_medium_a1 / pt ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_isoRho_medium");
                }
                if( !config.PassInt( "cut_passConvVeto_barrel_medium"   , passConvVeto       ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_ConvVeto_medium");
                }
                if( !config.PassInt  ( "cut_misshits_barrel_medium"  , misshits      ,false ) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_misshits_medium");
                }
            }
            
            // Loose cuts
            if( !use_eval || _eval_el_loose ) {
                if( !config.PassFloat( "cut_absdEtaIn_barrel_loose"    , fabs(dEtaIn)       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_barrel_loose"    , fabs(dPhiIn)       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_loose" , sigmaIEIEFull5x5    ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_d0_barrel_loose"        , d0           ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_z0_barrel_loose"        , z0           ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_barrel_loose"    , hovere - hovere_barrel_loose_a1/el_esc - hovere_barrel_loose_a2*rho/el_esc ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_barrel_loose"    , ooEmooP       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_barrel_loose"   ,iso_rho - isorho_barrel_loose_a1 / pt ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassInt( "cut_passConvVeto_barrel_loose"   , passConvVeto      ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_barrel_loose"  , misshits     ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
            }

            // Very Loose cuts
            if( !use_eval || _eval_el_veryloose ) {
                if( !config.PassFloat( "cut_absdEtaIn_barrel_veryloose"    , fabs(dEtaIn)       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_barrel_veryloose"    , fabs(dPhiIn)       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_veryloose" , sigmaIEIEFull5x5    ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_d0_barrel_veryloose"        , d0           ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_z0_barrel_veryloose"        , z0           ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_barrel_veryloose"    , hovere - hovere_barrel_veryloose_a1/el_esc - hovere_barrel_veryloose_a2*rho/el_esc ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_barrel_veryloose"    , ooEmooP       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_barrel_veryloose"   ,iso_rho - isorho_barrel_veryloose_a1 / pt ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassInt( "cut_passConvVeto_barrel_veryloose"   , passConvVeto      ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_barrel_veryloose"  , misshits     ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
            }


        }
        else { // endcap

            // Tight cuts
            if( !use_eval || _eval_el_tight ) {
                if( !config.PassFloat( "cut_absdEtaIn_endcap_tight"    , fabs(dEtaIn)       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_endcap_tight"    , fabs(dPhiIn)       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_tight" , sigmaIEIEFull5x5    ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_d0_endcap_tight"        , d0           ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_z0_endcap_tight"        , z0           ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_hovere_endcap_tight"    , hovere - hovere_endcap_tight_a1/el_esc - hovere_endcap_tight_a2*rho/el_esc ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_endcap_tight"    , ooEmooP       ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_endcap_tight"   ,iso_rho - isorho_endcap_tight_a1 / pt ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }

                if( !config.PassInt( "cut_passConvVeto_endcap_tight"   , passConvVeto      ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_endcap_tight"  , misshits     ) ) {
                    pass_tight=false;
                    if( _eval_el_tight ) continue;
                }
            }
            
            // Medium cuts
            if( !use_eval || _eval_el_medium ) {
                config.PassCounter("electron_barrel",true,0.0);
                config.PassCounter("electron_endcap");
                if( !config.PassFloat( "cut_absdEtaIn_endcap_medium"    , fabs(dEtaIn)      ,false ) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_absdEtaIn_medium");
                }
                if( !config.PassFloat( "cut_absdPhiIn_endcap_medium"    , fabs(dPhiIn)       ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_absdPhiIn_medium");
                }
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_medium" , sigmaIEIEFull5x5    ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_sigmaIEIE_medium");
                }
                if( !config.PassFloat( "cut_d0_endcap_medium"        , d0           ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_d0_medium");
                }
                if( !config.PassFloat( "cut_z0_endcap_medium"        , z0           ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_z0_medium");
                }
                if( !config.PassFloat( "cut_hovere_endcap_medium"    , hovere - hovere_endcap_medium_a1/el_esc - hovere_endcap_medium_a2*rho/el_esc ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_hovere_medium");
                }
                if( !config.PassFloat( "cut_ooEmooP_endcap_medium"    , ooEmooP       ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_ooEmooP_medium");
                }
                if( !config.PassFloat( "cut_isoRho_endcap_medium"   ,iso_rho - isorho_endcap_medium_a1 / pt ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_isoRho_medium");
                }
                if( !config.PassInt( "cut_passConvVeto_endcap_medium"   , passConvVeto      ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_ConvVeto_medium");
                }
                if( !config.PassInt  ( "cut_misshits_endcap_medium"  , misshits      ,false) ) {
                    pass_medium=false;
                    if( _eval_el_medium ) continue;
                } else {
                    config.PassCounter("cut_misshits_medium");
                }
            }
            
            // Loose cuts
            if( !use_eval || _eval_el_loose ) {
                if( !config.PassFloat( "cut_absdEtaIn_endcap_loose"    , fabs(dEtaIn)       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_endcap_loose"    , fabs(dPhiIn)       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_loose" , sigmaIEIEFull5x5    ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_d0_endcap_loose"        , d0           ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_z0_endcap_loose"        , z0           ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_endcap_loose"    , hovere - hovere_endcap_loose_a1/el_esc - hovere_endcap_loose_a2*rho/el_esc ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_endcap_loose"    , ooEmooP       ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_endcap_loose"   ,iso_rho - isorho_endcap_loose_a1 / pt ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassInt( "cut_passConvVeto_endcap_loose"   , passConvVeto      ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_endcap_loose"  , misshits     ) ) {
                    pass_loose=false;
                    if( _eval_el_loose ) continue;
                }
            }

            // Very Loose cuts
            if( !use_eval || _eval_el_veryloose ) {
                if( !config.PassFloat( "cut_absdEtaIn_endcap_veryloose"    , fabs(dEtaIn)       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_absdPhiIn_endcap_veryloose"    , fabs(dPhiIn)       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_veryloose" , sigmaIEIEFull5x5    ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_d0_endcap_veryloose"        , d0           ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_z0_endcap_veryloose"        , z0           ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_endcap_veryloose"    , hovere - hovere_endcap_veryloose_a1/el_esc - hovere_endcap_veryloose_a2*rho/el_esc ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_ooEmooP_endcap_veryloose"    , ooEmooP       ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassFloat( "cut_isoRho_endcap_veryloose"   ,iso_rho - isorho_endcap_veryloose_a1 / pt ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassInt( "cut_passConvVeto_endcap_veryloose"   , passConvVeto      ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
                if( !config.PassInt  ( "cut_misshits_endcap_veryloose"  , misshits     ) ) {
                    pass_veryloose=false;
                    if( _eval_el_veryloose ) continue;
                }
            }

        }

        // add electron impact parameter cut
        if( fabs( IN::el_eta->at(idx) )<1.479 ){
          if( fabs( IN::el_d0->at(idx) )>0.05 || fabs( IN::el_dz->at(idx) )>0.10 ) continue;
        }
        else{
          if( fabs( IN::el_d0->at(idx) )>0.10 || fabs( IN::el_dz->at(idx) )>0.20 ) continue;
        }

        if( !config.PassBool( "cut_tight"     , pass_tight ) ) continue;
        if( !config.PassBool( "cut_medium"    , pass_medium ) ) continue;
        if( !config.PassBool( "cut_vid_tight", IN::el_passVIDTight->at(idx) ) ) continue;
        

        TLorentzVector ellv;
        ellv.SetPtEtaPhiE( IN::el_pt->at(idx), 
                           IN::el_eta->at(idx),
                           IN::el_phi->at(idx),
                           IN::el_e->at(idx)
                           );

        float min_mu_dr = 100.0;
        for( int muidx = 0; muidx < OUT::mu_n; ++muidx ) {

            TLorentzVector mulv;
            mulv.SetPtEtaPhiE( OUT::mu_pt->at(muidx),
                               OUT::mu_eta->at(muidx),
                               OUT::mu_phi->at(muidx),
                               OUT::mu_e->at(muidx)
                             );

            float dr = mulv.DeltaR( ellv );

            if( dr < min_mu_dr ) {
                min_mu_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_muon_dr", min_mu_dr ) ) continue;

        float mindr = 101.0;
        for( int hltidx = 0 ; hltidx < IN::HLTObj_n; ++hltidx ) {

            std::vector<int> passTrigs = IN::HLTObj_passTriggers->at(hltidx);

            bool foundMatch = false;
            for( std::vector<int>::const_iterator bitr = _electronTrigMatchBits.begin(); bitr != _electronTrigMatchBits.end(); ++bitr ) {
                if( std::find( passTrigs.begin(), passTrigs.end(), (*bitr) ) != 
                              passTrigs.end() ) foundMatch = true;
            }

            if( !foundMatch ) continue;

            TLorentzVector hltlv;
            hltlv.SetPtEtaPhiE( IN::HLTObj_pt->at(hltidx),
                                IN::HLTObj_eta->at(hltidx),
                                IN::HLTObj_phi->at(hltidx),
                                IN::HLTObj_e->at(hltidx)
                              );

            float dr = ellv.DeltaR( hltlv );
            if( dr < mindr ) {
                mindr = dr;
            }
        }

        bool matchTrig = false;
        if( mindr < 0.2 ) {
            matchTrig = true;
        }
        OUT::el_hasTrigMatch->push_back(matchTrig);
        OUT::el_trigMatch_dr->push_back(mindr);

        OUT::el_passVeryLoose->push_back( pass_veryloose );
        OUT::el_passLoose->push_back( pass_loose );
        OUT::el_passMedium->push_back( pass_medium );
        OUT::el_passTight->push_back( pass_tight );

        CopyPrefixIndexBranchesInToOut( "el_", idx );
        OUT::el_n++;

        if( IN::el_pt->at(idx) > 30 ) {
            OUT::el_pt30_n++;
        }

        if( IN::el_pt->at(idx) > 35 ) {
            OUT::el_pt35_n++;
        }

        if( IN::el_pt->at(idx) > 40 ) {
            OUT::el_pt40_n++;
        }

        config.PassCounter("electron_pass");
    }

}

void RunModule::FilterPhoton( ModuleConfig & config ) {

    OUT::ph_n          = 0;
    //OUT::ph_chIsoCorr            -> clear();
    //OUT::ph_neuIsoCorr           -> clear();
    //OUT::ph_phoIsoCorr           -> clear();
    OUT::ph_min_el_dr           -> clear();
    OUT::ph_IsEB                 -> clear();
    OUT::ph_IsEE                 -> clear();
    OUT::ph_passTight            -> clear();
    OUT::ph_passMedium           -> clear();
    OUT::ph_passLoose            -> clear();
    OUT::ph_passLooseNoSIEIE     -> clear();
    OUT::ph_passHOverELoose      -> clear();
    OUT::ph_passHOverEMedium     -> clear();
    OUT::ph_passHOverETight      -> clear();
    OUT::ph_passSIEIELoose       -> clear();
    OUT::ph_passSIEIEMedium      -> clear();
    OUT::ph_passSIEIETight       -> clear();
    OUT::ph_passChIsoCorrLoose   -> clear();
    OUT::ph_passChIsoCorrMedium  -> clear();
    OUT::ph_passChIsoCorrTight   -> clear();
    OUT::ph_passNeuIsoCorrLoose  -> clear();
    OUT::ph_passNeuIsoCorrMedium -> clear();
    OUT::ph_passNeuIsoCorrTight  -> clear();
    OUT::ph_passPhoIsoCorrLoose  -> clear();
    OUT::ph_passPhoIsoCorrMedium -> clear();
    OUT::ph_passPhoIsoCorrTight  -> clear();


    ClearOutputPrefix("ph_");

    for( int idx = 0; idx < IN::ph_n; ++idx ) {

        float pt = IN::ph_pt->at(idx);
        float pt_uncalib = IN::ph_ptOrig->at(idx);
        float eta = IN::ph_eta->at(idx);
        float sceta = IN::ph_sc_eta->at(idx);

        bool iseb = false;
        bool isee = false;
        if( fabs(sceta) < 1.479 ) {
            iseb = true;
        }
        if( fabs(sceta) > 1.57 ) {
            isee = true;
        }

        if( !config.PassFloat( "cut_pt", pt ) ) continue;
        if( !config.PassFloat( "cut_eta", fabs(eta) ) ) continue;
        if( !config.PassBool( "cut_eb", iseb ) ) continue;
        if( !config.PassBool( "cut_ee", isee ) ) continue;
        if( !config.PassFloat( "cut_abseta_crack", fabs(sceta)) ) continue;

        float sigmaIEIE = IN::ph_sigmaIEIEFull5x5->at(idx);
        float hovere = IN::ph_hOverE->at(idx);

        float pfChIso  = IN::ph_chIso->at(idx);
        float pfNeuIso = IN::ph_neuIso->at(idx);
        float pfPhoIso = IN::ph_phoIso->at(idx);

        //float pfChIsoRhoCorr = 0.0;
        //float pfNeuIsoRhoCorr = 0.0;
        //float pfPhoIsoRhoCorr = 0.0;
        //calc_corr_iso( pfChIso, pfPhoIso, pfNeuIso, IN::rho, sceta, pfChIsoRhoCorr, pfPhoIsoRhoCorr, pfNeuIsoRhoCorr);

        //float pfChIsoRhoCorr2  = IN::ph_chIsoCorr->at(idx);
        //float pfNeuIsoRhoCorr2 = IN::ph_neuIsoCorr->at(idx);
        //float pfPhoIsoRhoCorr2 = IN::ph_phoIsoCorr->at(idx);
        float pfChIsoRhoCorr  = IN::ph_chIsoCorr->at(idx);
        float pfNeuIsoRhoCorr = IN::ph_neuIsoCorr->at(idx);
        float pfPhoIsoRhoCorr = IN::ph_phoIsoCorr->at(idx);
        
       // std::cout<<"ch  "<< pfChIso  <<" "<< pfChIsoRhoCorr <<" " << pfChIsoRhoCorr2 <<" " 
       //          <<"neu "<< pfNeuIso <<" "<< pfNeuIsoRhoCorr<<" " << pfNeuIsoRhoCorr2<<" " 
       //          <<"pho "<< pfPhoIso <<" "<< pfPhoIsoRhoCorr<<" " << pfPhoIsoRhoCorr2<<" " << std::endl; 
        float p1_neu = 0;
        float p2_neu = 0;
        float p1_pho = 0;
        // taken from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Recommended_Working_points_for_2
        // Updated May 2019 (94X V2 PID)
        if( iseb ) {
            p1_neu = 0.01512;
            p2_neu = 2.259e-5;
            p1_pho = 0.004017;
        }
        else {
            p1_neu = 0.0117;
            p2_neu = 2.3e-5;
            p1_pho = 0.0037;
        }

        float pfChIsoPtRhoCorr  = pfChIsoRhoCorr;
        float pfNeuIsoPtRhoCorr = pfNeuIsoRhoCorr-p1_neu*pt_uncalib-p2_neu*pt_uncalib*pt_uncalib;
        float pfPhoIsoPtRhoCorr = pfPhoIsoRhoCorr-p1_pho*pt_uncalib;

        bool pass_loose         = true;
        bool pass_loose_nosieie = true;
        bool pass_medium        = true;
        bool pass_tight         = true;

        bool pass_sieie_loose   = true;
        bool pass_sieie_medium   = true;
        bool pass_sieie_tight   = true;

        bool pass_chIsoCorr_loose   = true;
        bool pass_chIsoCorr_medium   = true;
        bool pass_chIsoCorr_tight   = true;

        bool pass_neuIsoCorr_loose   = true;
        bool pass_neuIsoCorr_medium   = true;
        bool pass_neuIsoCorr_tight   = true;

        bool pass_phoIsoCorr_loose   = true;
        bool pass_phoIsoCorr_medium   = true;
        bool pass_phoIsoCorr_tight   = true;

        bool pass_hovere_loose   = true;
        bool pass_hovere_medium   = true;
        bool pass_hovere_tight   = true;

        bool use_eval = _eval_ph_tight || _eval_ph_medium || _eval_ph_loose;

        if( fabs(sceta) < 1.479 ) { // barrel

            //loose 
            if( !use_eval || _eval_ph_loose ) {
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_loose"   , sigmaIEIE         ) ) {
                    pass_loose=false;
                    pass_sieie_loose=false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_chIsoCorr_barrel_loose"   , pfChIsoPtRhoCorr  ) ) {
                    pass_loose=false;
                    pass_chIsoCorr_loose = false;
                    pass_loose_nosieie=false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_neuIsoCorr_barrel_loose"  , pfNeuIsoPtRhoCorr ) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_neuIsoCorr_loose = false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_phoIsoCorr_barrel_loose"  , pfPhoIsoPtRhoCorr ) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_phoIsoCorr_loose = false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_barrel_loose"  , hovere) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_hovere_loose = false;
                    if( _eval_ph_loose ) continue;
                }

            }

            // medium
            if( !use_eval || _eval_ph_medium ) {
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_medium"  , sigmaIEIE         ) ) {
                    pass_medium=false;
                    pass_sieie_medium=false;
                    if( _eval_ph_medium ) continue;
                } else {
                    config.PassCounter("cut_sigmaIEIE_medium");
                }
                if( !config.PassFloat( "cut_chIsoCorr_barrel_medium"  , pfChIsoPtRhoCorr   ) ) {
                    pass_medium=false;
                    pass_chIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {
                    config.PassCounter("cut_chIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_neuIsoCorr_barrel_medium" , pfNeuIsoPtRhoCorr  ) ) {
                    pass_medium=false;
                    pass_neuIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {
                    config.PassCounter("cut_neuIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_phoIsoCorr_barrel_medium" , pfPhoIsoPtRhoCorr ) ) {
                    pass_medium=false;
                    pass_phoIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {
                    config.PassCounter("cut_phoIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_hovere_barrel_medium" , hovere ) ) {
                    pass_medium=false;
                    pass_hovere_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {
                    config.PassCounter("cut_hovere_medium");
                }
            }

            // tight
            if( !use_eval || _eval_ph_tight ) {
                if( !config.PassFloat( "cut_sigmaIEIE_barrel_tight"   , sigmaIEIE         ) ) {
                    pass_tight=false;
                    pass_sieie_tight=false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_chIsoCorr_barrel_tight"   , pfChIsoPtRhoCorr  ) ) {
                    pass_tight=false;
                    pass_chIsoCorr_tight = false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_neuIsoCorr_barrel_tight"  , pfNeuIsoPtRhoCorr ) ) {
                    pass_tight=false;
                    pass_neuIsoCorr_tight = false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_phoIsoCorr_barrel_tight"  , pfPhoIsoPtRhoCorr ) ) {
                    pass_tight=false;
                    pass_phoIsoCorr_tight = false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_hovere_barrel_tight"  , hovere ) ) {
                    pass_tight=false;
                    pass_hovere_tight = false;
                    if( _eval_ph_tight ) continue;
                }
            }

        }
        else { // endcap
            // loose
            if( !use_eval || _eval_ph_loose ) {
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_loose"   , sigmaIEIE         ) ) {
                    pass_loose=false;
                    pass_sieie_loose=false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_chIsoCorr_endcap_loose"   , pfChIsoPtRhoCorr  ) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_chIsoCorr_loose = false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_neuIsoCorr_endcap_loose"  , pfNeuIsoPtRhoCorr ) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_neuIsoCorr_loose = false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_phoIsoCorr_endcap_loose"  , pfPhoIsoPtRhoCorr ) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_phoIsoCorr_loose = false;
                    if( _eval_ph_loose ) continue;
                }
                if( !config.PassFloat( "cut_hovere_endcap_loose"  , hovere) ) {
                    pass_loose=false;
                    pass_loose_nosieie=false;
                    pass_hovere_loose = false;
                    if( _eval_ph_loose ) continue;
                }
            }

            // medium
            if( !use_eval || _eval_ph_medium ) {
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_medium"  , sigmaIEIE          ) ) {
                    pass_medium=false;
                    pass_sieie_medium=false;
                    if( _eval_ph_medium ) continue;
                } else {  
                    config.PassCounter("cut_sigmaIEIE_medium");
                }
                if( !config.PassFloat( "cut_chIsoCorr_endcap_medium"  , pfChIsoPtRhoCorr   ) ) {
                    pass_medium=false;
                    pass_chIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {  
                    config.PassCounter("cut_chIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_neuIsoCorr_endcap_medium" , pfNeuIsoPtRhoCorr  ) ) {
                    pass_medium=false;
                    pass_neuIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {  
                    config.PassCounter("cut_neuIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_phoIsoCorr_endcap_medium" , pfPhoIsoPtRhoCorr  ) ) {
                    pass_medium=false;
                    pass_phoIsoCorr_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {  
                    config.PassCounter("cut_phoIsoCorr_medium");
                }
                if( !config.PassFloat( "cut_hovere_endcap_medium" , hovere ) )  {
                    pass_medium=false;
                    pass_hovere_medium = false;
                    if( _eval_ph_medium ) continue;
                } else {  
                    config.PassCounter("cut_hovere_medium");
                }
            }

            // tight
            if( !use_eval || _eval_ph_tight ) {
                if( !config.PassFloat( "cut_sigmaIEIE_endcap_tight"   , sigmaIEIE         ) ) {
                    pass_tight=false;
                    pass_sieie_tight=false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_chIsoCorr_endcap_tight"   , pfChIsoPtRhoCorr  ) ) {
                    pass_tight=false;
                    pass_chIsoCorr_tight = false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_neuIsoCorr_endcap_tight"  , pfNeuIsoPtRhoCorr ) ) {
                    pass_tight=false;
                    pass_neuIsoCorr_tight = false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_phoIsoCorr_endcap_tight"  , pfPhoIsoPtRhoCorr ) ) {
                    pass_phoIsoCorr_tight = false;
                    pass_tight=false;
                    if( _eval_ph_tight ) continue;
                }
                if( !config.PassFloat( "cut_hovere_endcap_tight"  , hovere) ) {
                    pass_hovere_tight = false;
                    pass_tight=false;
                    if( _eval_ph_tight ) continue;
                }
            }
        }

        if( !config.PassBool( "cut_tight"     ,  pass_tight     ) ) continue;
        if( !config.PassBool( "cut_medium"    ,  pass_medium    ) ) continue;
        if( !config.PassBool( "cut_loose"     ,  pass_loose     ) ) continue;
        if( !config.PassBool( "cut_vid_medium",  IN::ph_passVIDMedium->at(idx)) ) continue;
        if( !config.PassBool( "cut_CSEV"      ,  IN::ph_passEleVeto->at(idx) ) ) continue;

        TLorentzVector phlv;
        phlv.SetPtEtaPhiE( IN::ph_pt->at(idx), 
                           IN::ph_eta->at(idx),
                           IN::ph_phi->at(idx),
                           IN::ph_e->at(idx) 
                           );

        float min_mu_dr = 100.0;
        for( int muidx = 0; muidx < OUT::mu_n; ++muidx ) {

            TLorentzVector mulv;
            mulv.SetPtEtaPhiE( OUT::mu_pt->at(muidx),
                               OUT::mu_eta->at(muidx),
                               OUT::mu_phi->at(muidx),
                               OUT::mu_e->at(muidx)
                             );

            float dr = mulv.DeltaR( phlv );

            if( dr < min_mu_dr ) {
                min_mu_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_muon_dr", min_mu_dr ) ) continue;

        float min_el_dr = 100.0;
        for( int elidx = 0; elidx < OUT::el_n; ++elidx ) {

            TLorentzVector ellv;
            ellv.SetPtEtaPhiE( OUT::el_pt->at(elidx),
                               OUT::el_eta->at(elidx),
                               OUT::el_phi->at(elidx),
                               OUT::el_e->at(elidx)
                             );

            float dr = ellv.DeltaR( phlv );

            if( dr < min_el_dr ) {
                min_el_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_electron_dr", min_el_dr ) ) continue;


        //OUT::ph_chIsoCorr            -> push_back(pfChIsoPtRhoCorr);
        //OUT::ph_neuIsoCorr           -> push_back(pfNeuIsoPtRhoCorr);
        //OUT::ph_phoIsoCorr           -> push_back(pfPhoIsoPtRhoCorr);

        OUT::ph_min_el_dr            -> push_back( min_el_dr );

/*          if (pass_tight != IN::ph_passVIDTight->at(idx) && iseb) {
          std::cout<< "sigmaIEIE "<< sigmaIEIE
                    << " chIsoCorr "  << pfChIsoPtRhoCorr
                    << " neuIsoCorr " << pfNeuIsoPtRhoCorr
                    << " phoIsoCorr " << pfPhoIsoPtRhoCorr
                    << " hovere " << hovere<<std::endl; 

        }
  */
        OUT::ph_passTight            -> push_back(pass_tight);
        OUT::ph_passMedium           -> push_back(pass_medium);
        OUT::ph_passLoose            -> push_back(pass_loose);
        OUT::ph_passLooseNoSIEIE     -> push_back(pass_loose_nosieie);
        OUT::ph_passHOverELoose      -> push_back(pass_hovere_loose);
        OUT::ph_passHOverEMedium     -> push_back(pass_hovere_medium);
        OUT::ph_passHOverETight      -> push_back(pass_hovere_tight);
        OUT::ph_passSIEIELoose       -> push_back(pass_sieie_loose);
        OUT::ph_passSIEIEMedium      -> push_back(pass_sieie_medium);
        OUT::ph_passSIEIETight       -> push_back(pass_sieie_tight);
        OUT::ph_passChIsoCorrLoose   -> push_back(pass_chIsoCorr_loose);
        OUT::ph_passChIsoCorrMedium  -> push_back(pass_chIsoCorr_medium);
        OUT::ph_passChIsoCorrTight   -> push_back(pass_chIsoCorr_tight);
        OUT::ph_passNeuIsoCorrLoose  -> push_back(pass_neuIsoCorr_loose);
        OUT::ph_passNeuIsoCorrMedium -> push_back(pass_neuIsoCorr_medium);
        OUT::ph_passNeuIsoCorrTight  -> push_back(pass_neuIsoCorr_tight);
        OUT::ph_passPhoIsoCorrLoose  -> push_back(pass_phoIsoCorr_loose);
        OUT::ph_passPhoIsoCorrMedium -> push_back(pass_phoIsoCorr_medium);
        OUT::ph_passPhoIsoCorrTight  -> push_back(pass_phoIsoCorr_tight);

        OUT::ph_IsEB -> push_back( iseb );
        OUT::ph_IsEE -> push_back( isee );


        CopyPrefixIndexBranchesInToOut( "ph_", idx );
        OUT::ph_n++;

    config.PassCounter("photon_pass");
    }
}

void RunModule::calc_corr_iso( float chIso, float phoIso, float neuIso, float rho, float eta, float &chIsoCorr, float &phoIsoCorr, float &neuIsoCorr )  const
{

    // from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Selection_implementation_details
    // updated May 2019 (94X V2 PID)
    
    float ea_ch=0.0;
    float ea_pho=0.0;
    float ea_neu=0.0;

    if( fabs( eta ) < 1.0 ) {
        ea_ch = 0.0112;
        ea_neu = 0.0668;
        ea_pho = 0.1113;
    }
    else if( fabs(eta) >= 1.0 && fabs( eta ) < 1.479 ) {
        ea_ch = 0.0108;
        ea_neu = 0.1054;
        ea_pho = 0.0953;
    }
    else if( fabs(eta) >= 1.479 && fabs( eta ) < 2.0 ) {
        ea_ch = 0.0106;
        ea_neu = 0.0786;
        ea_pho = 0.0619;
    }
    else if( fabs(eta) >= 2.0 && fabs( eta ) < 2.2 ) {
        ea_ch = 0.01002;
        ea_neu = 0.0233;
        ea_pho = 0.0837;
    }
    else if( fabs(eta) >= 2.2 && fabs( eta ) < 2.3 ) {
        ea_ch = 0.0098;
        ea_neu = 0.0078;
        ea_pho = 0.1070;
    }
    else if( fabs(eta) >= 2.3 && fabs( eta ) < 2.4 ) {
        ea_ch = 0.0089;
        ea_neu = 0.0028;
        ea_pho = 0.1212;
    }
    else if( fabs(eta) >= 2.4 ) {
        ea_ch = 0.0087;
        ea_neu = 0.0137;
        ea_pho = 0.1466;
    }

    chIsoCorr  = chIso  - rho*ea_ch;
    phoIsoCorr = phoIso - rho*ea_pho;
    neuIsoCorr = neuIso - rho*ea_neu;

    if( chIsoCorr < 0 ) {
        chIsoCorr = 0;
    }
    if( phoIsoCorr < 0 ) {
        phoIsoCorr = 0;
    }
    if( neuIsoCorr < 0 ) {
        neuIsoCorr = 0;
    }

}


void RunModule::FilterJet( ModuleConfig & config ) const {

    OUT::jet_n          = 0;
    OUT::jet_CSVLoose_n          = 0;
    OUT::jet_CSVMedium_n          = 0;
    OUT::jet_CSVTight_n          = 0;
    OUT::jet_DeepJetLoose_n          = 0;
    OUT::jet_DeepJetMedium_n          = 0;
    OUT::jet_DeepJetTight_n          = 0;
    OUT::jet_IdLoose        -> clear();
    OUT::jet_IdTight        -> clear();
    OUT::jet_IdTightLep     -> clear();
    OUT::jet_btagged        -> clear();
    ClearOutputPrefix("jet_");

    for( int idx = 0; idx < IN::jet_n; ++idx ) {

        float pt = IN::jet_pt->at(idx);
        float eta = IN::jet_eta->at(idx);

        if( !config.PassFloat( "cut_pt", pt ) ) continue;
        if( !config.PassFloat( "cut_eta", fabs(eta)) ) continue;

        float nhf        = IN::jet_nhf->at(idx);
        float chf        = IN::jet_chf->at(idx);
        float muf        = IN::jet_muf->at(idx);
        float cemf       = IN::jet_cemf->at(idx);
        float nemf       = IN::jet_nemf->at(idx);
        int   cmult      = IN::jet_cmult->at(idx);
        int   nmult      = IN::jet_nmult->at(idx);
        int   nconst     = cmult + nmult;

        bool pass_loose    = true;
        bool pass_tight    = true;
        bool pass_tightlep = true;
        bool btagged = false;

        if( fabs( eta ) <= 2.7 ) {
            if (_year!=2018){
                if( !config.PassFloat( "cut_jet_nhf_central_loose" , nhf  ) ) pass_loose = false;
                if( !config.PassFloat( "cut_jet_nemf_central_loose", nemf ) ) pass_loose = false;
                if( !config.PassInt  ( "cut_jet_nconst_central_loose", nconst ) ) pass_loose = false;
    
                if( !config.PassFloat( "cut_jet_nhf_central_tight" , nhf  ) ) pass_tight = false;
                if( !config.PassFloat( "cut_jet_nemf_central_tight", nemf ) ) pass_tight = false;
                if( !config.PassInt  ( "cut_jet_nconst_central_tight", nconst ) ) pass_tight = false;
    
                if( !config.PassFloat( "cut_jet_nhf_central_tightlep" , nhf  ) ) pass_tightlep = false;
                if( !config.PassFloat( "cut_jet_nemf_central_tightlep", nemf ) ) pass_tightlep = false;
                if( !config.PassInt  ( "cut_jet_nconst_central_tightlep", nconst ) ) pass_tightlep = false;
                if( !config.PassFloat  ( "cut_jet_muf_central_tightlep", muf ) ) pass_tightlep = false;
                if( fabs( eta ) < 2.4 ) {
                    if( !config.PassFloat( "cut_jet_chf_central_loose" , chf  ) ) pass_loose = false;
                    if( !config.PassInt( "cut_jet_cmult_central_loose" , cmult  ) ) pass_loose = false;
                    if( !config.PassFloat( "cut_jet_cemf_central_loose" , cemf ) ) pass_loose = false;
    
                    if( !config.PassFloat( "cut_jet_chf_central_tight" , chf  ) ) pass_tight = false;
                    if( !config.PassInt( "cut_jet_cmult_central_tight" , cmult  ) ) pass_tight = false;
                    if( !config.PassFloat( "cut_jet_cemf_central_tight" , cemf ) ) pass_tight = false;
    
                    if( !config.PassFloat( "cut_jet_chf_central_tightlep" , chf  ) ) pass_tightlep = false;
                    if( !config.PassInt( "cut_jet_cmult_central_tightlep" , cmult  ) ) pass_tightlep = false;
                    if( !config.PassFloat( "cut_jet_cemf_central_tightlep" , cemf ) ) pass_tightlep = false;
                }
            }else{
                if( fabs( eta ) > 2.6 ) {
                    if( !config.PassFloat( "cut_jet_nhf_central2_tight" , nhf  ) ) pass_tight = false;
                    if( !config.PassFloat( "cut_jet_nemf_central2_tight", nemf ) ) pass_tight = false;
                    if( !config.PassInt( "cut_jet_cmult_central2_tight" , cmult  ) ) pass_tight = false;

                    if( !config.PassFloat  ( "cut_jet_muf_central2_tightlep", muf ) ) pass_tightlep = false;
                    if( !config.PassFloat( "cut_jet_cemf_central_tightlep" , cemf ) ) pass_tightlep = false;
                }else{
                    if( !config.PassFloat( "cut_jet_nhf_central_tight" , nhf  ) ) pass_tight = false;
                    if( !config.PassFloat( "cut_jet_nemf_central_tight", nemf ) ) pass_tight = false;
                    if( !config.PassInt  ( "cut_jet_nconst_central_tight", nconst ) ) pass_tight = false;
                    if( !config.PassFloat( "cut_jet_chf_central_tight" , chf  ) ) pass_tight = false;
                    if( !config.PassInt( "cut_jet_cmult_central_tight" , cmult  ) ) pass_tight = false;
    
                    if( !config.PassFloat  ( "cut_jet_muf_central_tightlep", muf ) ) pass_tightlep = false;
                    if( !config.PassFloat( "cut_jet_cemf_central_tightlep" , cemf ) ) pass_tightlep = false;
                }
            }
        }
        else if( fabs( eta ) < 3.0 ) {
            if( !config.PassFloat( "cut_jet_nhf_transition_loose", nhf ) ) pass_loose = false;
            if( !config.PassFloat( "cut_jet_nemf_transition_loose", nemf ) ) pass_loose = false;
            if( !config.PassInt( "cut_jet_nmult_transition_loose", nmult ) ) pass_loose = false;

            if( !config.PassFloat( "cut_jet_nhf_transition_tight", nhf ) ) pass_tight = false;
            if( !config.PassFloat( "cut_jet_nemf_transition_tight", nemf ) ) pass_tight = false;
            //if( !config.PassFloat( "cut_jet_nemf_transition_tight2", nemf ) ) pass_tight = false;
            if( !config.PassInt( "cut_jet_nmult_transition_tight", nmult ) ) pass_tight = false;

        }
        else {
            if( !config.PassFloat( "cut_jet_nemf_forward_loose", nemf ) ) pass_loose = false;
            if( !config.PassInt( "cut_jet_nmult_forward_loose", nmult ) ) pass_loose = false;

            if( !config.PassFloat( "cut_jet_nhf_forward_tight", nhf ) ) pass_tight = false;
            if( !config.PassFloat( "cut_jet_nemf_forward_tight", nemf ) ) pass_tight = false;
            if( !config.PassInt( "cut_jet_nmult_forward_tight", nmult ) ) pass_tight = false;
        }

        if( !config.PassBool( "cut_tight", pass_tight) ) continue; 
        //if( !config.PassBool( "cut_loose", pass_loose) ) continue;
        //if( !config.PassBool( "cut_tightlep", pass_tightlep) ) continue;

        TLorentzVector jetlv;
        jetlv.SetPtEtaPhiE( IN::jet_pt->at(idx), 
                            IN::jet_eta->at(idx),
                            IN::jet_phi->at(idx),
                            IN::jet_e->at(idx) 
                            );

        float min_mu_dr = 100.0;
        for( int muidx = 0; muidx < OUT::mu_n; ++muidx ) {

            TLorentzVector mulv;
            mulv.SetPtEtaPhiE( OUT::mu_pt->at(muidx),
                               OUT::mu_eta->at(muidx),
                               OUT::mu_phi->at(muidx),
                               OUT::mu_e->at(muidx)
                             );

            float dr = mulv.DeltaR( jetlv );

            if( dr < min_mu_dr ) {
                min_mu_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_muon_dr", min_mu_dr ) ) continue;

        float min_el_dr = 100.0;
        for( int elidx = 0; elidx < OUT::el_n; ++elidx ) {

            TLorentzVector ellv;
            ellv.SetPtEtaPhiE( OUT::el_pt->at(elidx),
                               OUT::el_eta->at(elidx),
                               OUT::el_phi->at(elidx),
                               OUT::el_e->at(elidx)
                             );

            float dr = ellv.DeltaR( jetlv );

            if( dr < min_el_dr ) {
                min_el_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_electron_dr", min_el_dr ) ) continue;

        float min_ph_dr = 100.0;
        for( int phidx = 0; phidx < OUT::ph_n; ++phidx ) {

            TLorentzVector phlv;
            phlv.SetPtEtaPhiE( OUT::ph_pt->at(phidx),
                               OUT::ph_eta->at(phidx),
                               OUT::ph_phi->at(phidx),
                               OUT::ph_e->at(phidx)
                             );

            float dr = phlv.DeltaR( jetlv );

            if( dr < min_ph_dr ) {
                min_ph_dr = dr;
            }
        }

        if( !config.PassFloat( "cut_photon_dr", min_ph_dr ) ) continue;

        CopyPrefixIndexBranchesInToOut( "jet_", idx );
        OUT::jet_n++;

        OUT::jet_IdLoose        -> push_back(pass_loose );
        OUT::jet_IdTight        -> push_back(pass_tight );
        OUT::jet_IdTightLep     -> push_back(pass_tightlep );

        float jet_csv = IN::jet_bTagCisvV2->at(idx);

        if( config.PassFloat( "cut_jet_CSV_Loose",  jet_csv ) ) {
            OUT::jet_CSVLoose_n++;
        }
        if( config.PassFloat( "cut_jet_CSV_Medium",  jet_csv ) ) {
            OUT::jet_CSVMedium_n++;
        }
        if( config.PassFloat( "cut_jet_CSV_Tight",  jet_csv ) ) {
            OUT::jet_CSVTight_n++;
        }

        float jet_deepb = IN::jet_bTagDeepb->at(idx);

        btagged = false;

        if( config.PassFloat( "cut_jet_DeepJet_Loose",  jet_deepb ) ) {
            OUT::jet_DeepJetLoose_n++;
            if (_eval_btag_loose) btagged=true;
        }
        if( config.PassFloat( "cut_jet_DeepJet_Medium",  jet_deepb ) ) {
            OUT::jet_DeepJetMedium_n++;
            if (_eval_btag_medium) btagged=true;
        }
        if( config.PassFloat( "cut_jet_DeepJet_Tight",  jet_deepb ) ) {
            OUT::jet_DeepJetTight_n++;
            if (_eval_btag_tight) btagged=true;
        }
        
        OUT::jet_btagged      -> push_back( btagged );

    }
}

bool RunModule::FilterEvent( ModuleConfig & config ) const {

    bool keep_event = true;
    bool pass_event = true;

    for( auto const& itr: triggerNames){
      int triggerid = itr.first ;
      std::string triggername = itr.second ;
      bool pass = triggerResults.at(triggerid);

      config.PassCounter(triggername, pass );
    
    }
    if( printevent ) std::cout << " passtrigger FilterEvent "     << passtrigger     << std::endl;
    if ( passtrigger ) {
        config.PassCounter("trigger_pass");
    } else {
        if (_filterevent_cutflow) return false; 
    } 
    if( !config.PassInt( "cut_el_n"     , OUT::el_n        ) ) { 
        if (_filterevent_cutflow) return false;
        keep_event=false; 
        //if( printevent ) std::cout << " fail cut_el_n "      << OUT::el_n      << std::endl;
    } 
    if( !config.PassInt( "cut_el_pt35_n", OUT::el_pt35_n   ) ) {
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_el_pt35_n " << OUT::el_pt35_n << std::endl;
    } 
    if( !config.PassInt( "cut_mu_n"     , OUT::mu_n        ) ) {
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_mu_n "      << OUT::mu_n      << std::endl;
    } 
    if( !config.PassInt( "cut_mu_pt30_n", OUT::mu_pt30_n   ) ) { 
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_mu_pt30_n " << OUT::mu_pt30_n << std::endl;
    } 
    if( !config.PassInt( "cut_mu_pt20_n", OUT::mu_pt20_n   ) ) { 
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_mu_pt20_n " << OUT::mu_pt20_n << std::endl;
    } 
    if( !config.PassInt( "cut_ph_n"     , OUT::ph_n   ) )      { 
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_ph_n "      << OUT::ph_n      << std::endl;
    } 
    if( !config.PassInt( "cut_jet_n"    , OUT::jet_n  ) )      { 
        if (_filterevent_cutflow) return false;
        keep_event=false;
        //if( printevent ) std::cout << " fail cut_jet_n "     << OUT::jet_n     << std::endl;
    } 
    
    if (keep_event) config.PassCounter("event_kept");
    pass_event=keep_event;
    if (OUT::ph_n==1 && OUT::mu_n==1 && OUT::mu_pt30_n==1 && OUT::el_n==0){ // muon channel
       config.PassCounter("mu_basicsel");
      if (pass_event && OUT::ph_IsEB->at(0))                              config.PassCounter("phEB" ); else pass_event=false;
      if (pass_event && OUT::ph_passMedium->at(0))                        config.PassCounter("phID");  else pass_event=false;
      if (pass_event && OUT::ph_pt->at(0)>80)                             config.PassCounter("phpt");  else pass_event=false;
      if (pass_event && OUT::ph_hasPixSeed->at(0)==0)                     config.PassCounter("phPix"); else pass_event=false;
      if (pass_event && OUT::mu_passTight->at(0))                         config.PassCounter("muID");  else pass_event=false;
      if (pass_event && OUT::met_pt>25)                                   config.PassCounter("met");   else pass_event=false;
      if (pass_event) config.PassCounter("event_pass");
    }
    if (OUT::ph_n==1 && OUT::el_n==1 && OUT::el_pt35_n==1 && OUT::mu_n==0){ // electron channel
       config.PassCounter("el_basicsel");
      if (pass_event && OUT::ph_passMedium->at(0))                        config.PassCounter("phID");  else pass_event=false;
      if (pass_event && OUT::ph_pt->at(0)>80)                             config.PassCounter("phpt");  else pass_event=false;
      if (pass_event && OUT::ph_hasPixSeed->at(0)==0)                     config.PassCounter("phPix"); else pass_event=false;
      if (pass_event && OUT::el_passTight->at(0))                         config.PassCounter("elID");  else pass_event=false;
      if (pass_event && OUT::el_pt->at(0)>40 && fabs(OUT::el_eta->at(0))<2.1) config.PassCounter("elpte"); else pass_event=false;
      if (pass_event && OUT::met_pt>25)                                   config.PassCounter("met");   else pass_event=false;
      if (pass_event && fabs(OUT::m_lep_ph-_m_z)>15)                      config.PassCounter("invZ");  else pass_event=false;
      if (pass_event) config.PassCounter("event_pass");
    }


    //if( !config.PassBool( "cut_trig_Ele27_eta2p1_tight", IN::passTrig_HLT_Ele27_eta2p1_WPTight_Gsf) ) keep_event=false;
    //if( !config.PassBool( "cut_trig_Mu27_IsoORIsoTk", (IN::passTrig_HLT_IsoMu27 | IN::passTrig_HLT_IsoTkMu27) ) ) keep_event=false;
    //if( !config.PassBool( "cut_trig_Mu24_IsoORIsoTk", (IN::passTrig_HLT_IsoMu24 | IN::passTrig_HLT_IsoTkMu24) ) ) keep_event=false;
    //if( !config.PassBool( "cut_trig_Mu24_IsoORIsoTk", (IN::passTrig_HLT_IsoMu24 | IN::passTrig_HLT_IsoTkMu24) ) ) keep_event=false;
    //if( !config.PassBool( "cut_trig_Mu17_Photon30", (IN::passTrig_HLT_Mu17_Photon30_CaloIdL_L1ISO) ) ) keep_event=false;

    return keep_event;
    
}

bool RunModule::FilterTrigger( ModuleConfig & config ) {
   

    std::vector<int> passed_ids;
    // for each configured trigger, get its decision and store the result 
    // in the output branch
    //for( std::map<int, bool>::iterator itr = triggerResults.begin();
    //        itr != triggerResults.end() ; ++itr ) {
    for( auto& itr : triggerResults ) {

        if( std::find( IN::passedTriggers->begin(), IN::passedTriggers->end(), itr.first ) 
                != IN::passedTriggers->end() ) {
            itr.second = true;
            passed_ids.push_back( itr.first );
        }
        else {
            itr.second = false;
        }
    }

    bool keep_event = true;

    if( !config.PassAnyIntVector( "cut_bits", passed_ids ) ) keep_event = false;

    // pass the result to FilterEvent for cutflow
    passtrigger = keep_event;
    if( printevent ) std::cout << " passtrigger FilterTrigger "     << passtrigger     << std::endl;

    return keep_event;
    
}

bool RunModule::FilterMET( ModuleConfig & config ) {

    bool keep_event = true;

#ifdef EXISTS_passedFilters
    std::vector<int> passed_ids;
    // for each configured metfilter, get its decision and store the result
    // in the output branch
    for( std::map<int, bool>::iterator itr = metfilterResults.begin();
            itr != metfilterResults.end() ; ++itr ) {

        if( std::find( IN::passedFilters->begin(), IN::passedFilters->end(), itr->first )
                != IN::passedFilters->end() ) {
            itr->second = true;
            passed_ids.push_back( itr->first );
        }
        else {
            itr->second = false;
        }
    }



    if( !config.PassAnyIntVector( "cut_metfilter_bits", passed_ids ) ) keep_event = false;
#endif
    return keep_event;

}

void RunModule::BuildEventVars( ModuleConfig & config ) const {


    OUT::m_lep_ph = 0;
    OUT::m_lep_ph_comb_leadLep->clear();
    OUT::m_lep_ph_comb_sublLep->clear();
    OUT::m_lep_met_ph = 0;
    OUT::m_mt_lep_met_ph = 0;
    OUT::m_mt_lep_met_ph_forcewmass = 0;
    OUT::mt_w = 0;
    OUT::mt_res = 0;
    OUT::mt_res_JetResUp =0;
    OUT::mt_res_JetResDown =0;
    OUT::mt_res_JetEnUp =0;
    OUT::mt_res_JetEnDown =0;
    OUT::mt_res_MuonEnUp =0;
    OUT::mt_res_MuonEnDown =0;
    OUT::mt_res_MuonResUp =0;
    OUT::mt_res_MuonResDown =0;
    OUT::mt_res_ElectronEnUp =0;
    OUT::mt_res_ElectronEnDown =0;
    OUT::mt_res_ElectronResUp =0;
    OUT::mt_res_ElectronResDown =0;
    OUT::mt_res_PhotonEnUp =0;
    OUT::mt_res_PhotonEnDown =0;
    OUT::mt_res_PhotonResUp =0;
    OUT::mt_res_PhotonResDown =0;
    OUT::mt_res_UnclusteredEnUp =0;
    OUT::mt_res_UnclusteredEnDown =0;
    OUT::mt_lep_ph = 0;
    OUT::dphi_lep_ph = 0;
    OUT::dr_lep_ph = 0;
    OUT::dr_lep2_ph = 0;
    OUT::m_lep_met = 0;
    OUT::mt_lep_met = 0;
    OUT::pt_lep_met = 0;
    OUT::dphi_lep_met = 0;
    OUT::mt_lep_met_ph = 0;
    OUT::mt_lep_met_ph_inv = 0;
    OUT::RecoWMass = 0;
    OUT::recoM_lep_nu_ph = 0;
    OUT::recoMet_eta= 0;
    OUT::recoW_pt= 0;
    OUT::recoW_eta= 0;
    OUT::recoW_phi= 0;
    OUT::m_ll = 0;
    OUT::m_llph = 0;
    OUT::met_PhotonEnUpByHand_pt = 0;
    OUT::met_PhotonEnUpByHand_phi = 0;
    OUT::met_PhotonEnDownByHand_pt = 0;
    OUT::met_PhotonEnDownByHand_phi = 0;
    OUT::met_PhotonResUpByHand_pt = 0;
    OUT::met_PhotonResUpByHand_phi = 0;
    OUT::met_PhotonResDownByHand_pt = 0;
    OUT::met_PhotonResDownByHand_phi = 0;
    OUT::met_ElectronEnUpByHand_pt = 0;
    OUT::met_ElectronEnUpByHand_phi = 0;
    OUT::met_ElectronEnDownByHand_pt = 0;
    OUT::met_ElectronEnDownByHand_phi = 0;
    OUT::met_ElectronResUpByHand_pt = 0;
    OUT::met_ElectronResUpByHand_phi = 0;
    OUT::met_ElectronResDownByHand_pt = 0;
    OUT::met_ElectronResDownByHand_phi = 0;
    OUT::met_MuonEnUpByHand_pt = 0;
    OUT::met_MuonEnUpByHand_phi = 0;
    OUT::met_MuonEnDownByHand_pt = 0;
    OUT::met_MuonEnDownByHand_phi = 0;
    OUT::met_MuonResUpByHand_pt = 0;
    OUT::met_MuonResUpByHand_phi = 0;
    OUT::met_MuonResDownByHand_pt = 0;
    OUT::met_MuonResDownByHand_phi = 0;

    OUT::leadjet_pt = 0;
    OUT::subljet_pt = 0;
    OUT::leaddijet_m = 0;
    OUT::leaddijet_pt = 0;
    OUT::massdijet_m = 0;
    OUT::massdijet_pt = 0;

    // MET XY corrections
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
    std::tie(OUT::met_pt,                   OUT::met_phi                  )
        = METXYCorr_Met_MetPhi(OUT::met_pt,                   OUT::met_phi,                   OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_JetEnUp_pt,           OUT::met_JetEnUp_phi          )
        = METXYCorr_Met_MetPhi(OUT::met_JetEnUp_pt,           OUT::met_JetEnUp_phi,           OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_JetEnDown_pt,         OUT::met_JetEnDown_phi        )
        = METXYCorr_Met_MetPhi(OUT::met_JetEnDown_pt,         OUT::met_JetEnDown_phi,         OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_JetResUp_pt,          OUT::met_JetResUp_phi         )
        = METXYCorr_Met_MetPhi(OUT::met_JetResUp_pt,          OUT::met_JetResUp_phi,          OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_JetResDown_pt,        OUT::met_JetResDown_phi       )
        = METXYCorr_Met_MetPhi(OUT::met_JetResDown_pt,        OUT::met_JetResDown_phi,        OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_UnclusteredEnUp_pt,   OUT::met_UnclusteredEnUp_phi  )
        = METXYCorr_Met_MetPhi(OUT::met_UnclusteredEnUp_pt,   OUT::met_UnclusteredEnUp_phi,   OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_UnclusteredEnDown_pt, OUT::met_UnclusteredEnDown_phi)
        = METXYCorr_Met_MetPhi(OUT::met_UnclusteredEnDown_pt, OUT::met_UnclusteredEnDown_phi, OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_MuonEnUp_pt,          OUT::met_MuonEnUp_phi         )
        = METXYCorr_Met_MetPhi(OUT::met_MuonEnUp_pt,          OUT::met_MuonEnUp_phi,          OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_MuonEnDown_pt,        OUT::met_MuonEnDown_phi       )
        = METXYCorr_Met_MetPhi(OUT::met_MuonEnDown_pt,        OUT::met_MuonEnDown_phi,        OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_ElectronEnUp_pt,      OUT::met_ElectronEnUp_phi     )
        = METXYCorr_Met_MetPhi(OUT::met_ElectronEnUp_pt,      OUT::met_ElectronEnUp_phi,      OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_ElectronEnDown_pt,    OUT::met_ElectronEnDown_phi   )
        = METXYCorr_Met_MetPhi(OUT::met_ElectronEnDown_pt,    OUT::met_ElectronEnDown_phi,    OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_PhotonEnUp_pt,        OUT::met_PhotonEnUp_phi       )
        = METXYCorr_Met_MetPhi(OUT::met_PhotonEnUp_pt,        OUT::met_PhotonEnUp_phi,        OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);
    std::tie(OUT::met_PhotonEnDown_pt,      OUT::met_PhotonEnDown_phi     )
        = METXYCorr_Met_MetPhi(OUT::met_PhotonEnDown_pt,      OUT::met_PhotonEnDown_phi,      OUT::runNumber, _year, !OUT::isData, OUT::vtx_n);

    TLorentzVector metlv;
    metlv.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    TLorentzVector metlvOrig( metlv );

    TLorentzVector metlv_JetResUp;
    TLorentzVector metlv_JetResDown;
    TLorentzVector metlv_JetEnUp;
    TLorentzVector metlv_JetEnDown;
    TLorentzVector metlv_UnclusteredEnUp;
    TLorentzVector metlv_UnclusteredEnDown;
    TLorentzVector metlv_MuonEnUp;
    TLorentzVector metlv_MuonEnDown;
    TLorentzVector metlv_MuonResUp;
    TLorentzVector metlv_MuonResDown;
    TLorentzVector metlv_ElectronEnUp;
    TLorentzVector metlv_ElectronEnDown;
    TLorentzVector metlv_ElectronResUp;
    TLorentzVector metlv_ElectronResDown;
    TLorentzVector metlv_PhotonEnUp;
    TLorentzVector metlv_PhotonEnDown;
    TLorentzVector metlv_PhotonResUp;
    TLorentzVector metlv_PhotonResDown;
    // Take MET uncertainties from jets and unclustered energy from MiniAod
    metlv_JetResUp.SetPtEtaPhiM( OUT::met_JetResUp_pt,    0.0, 
                                  OUT::met_JetResUp_phi,   0.0 );
    metlv_JetResDown.SetPtEtaPhiM( OUT::met_JetResDown_pt,    0.0, 
                                    OUT::met_JetResDown_phi,   0.0 );
    metlv_JetEnUp.SetPtEtaPhiM( OUT::met_JetEnUp_pt,    0.0, 
                                 OUT::met_JetEnUp_phi,   0.0 );
    metlv_JetEnDown.SetPtEtaPhiM( OUT::met_JetEnDown_pt,    0.0, 
                                   OUT::met_JetEnDown_phi,   0.0 );
    metlv_UnclusteredEnUp.SetPtEtaPhiM( OUT::met_UnclusteredEnUp_pt,  0.0, 
                                         OUT::met_UnclusteredEnUp_phi, 0.0 );
    metlv_UnclusteredEnDown.SetPtEtaPhiM( OUT::met_UnclusteredEnDown_pt, 0.0, 
                                    OUT::met_UnclusteredEnDown_phi,   0.0 );
    // Recalculate MET uncertainties from leptons and photons
    // Will use 4vecs for convenience, set mass=0 again later
    metlv_MuonEnUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_MuonEnDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_MuonResUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_MuonResDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_ElectronEnUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_ElectronEnDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_ElectronResUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_ElectronResDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_PhotonEnUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_PhotonEnDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_PhotonResUp.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );
    metlv_PhotonResDown.SetPtEtaPhiM( OUT::met_pt, 0.0, OUT::met_phi, 0.0 );

    std::vector<TLorentzVector> leptons;
    std::vector<TLorentzVector> leptons_MuEn_up;
    std::vector<TLorentzVector> leptons_MuEn_down;
    std::vector<TLorentzVector> leptons_MuRes_up;
    std::vector<TLorentzVector> leptons_MuRes_down;
    std::vector<TLorentzVector> leptons_ElEn_up;
    std::vector<TLorentzVector> leptons_ElEn_down;
    std::vector<TLorentzVector> leptons_ElRes_up;
    std::vector<TLorentzVector> leptons_ElRes_down;
    std::vector<TLorentzVector> photons;
    std::vector<TLorentzVector> photons_PhEn_up;
    std::vector<TLorentzVector> photons_PhEn_down;
    std::vector<TLorentzVector> photons_PhRes_up;
    std::vector<TLorentzVector> photons_PhRes_down;

    for( int idx = 0; idx < OUT::mu_n; ++idx ) {
        TLorentzVector tlv;
        TLorentzVector tlv_up;
        TLorentzVector tlv_down;
        TLorentzVector tlv_res_up;
        TLorentzVector tlv_res_down;
        tlv.SetPtEtaPhiE( OUT::mu_pt_rc->at(idx), 
                          OUT::mu_eta->at(idx), 
                          OUT::mu_phi->at(idx), 
                          OUT::mu_e_rc->at(idx) );
        tlv_up.SetPtEtaPhiE( OUT::mu_pt_rc_up->at(idx), 
                          OUT::mu_eta->at(idx), 
                          OUT::mu_phi->at(idx), 
                          OUT::mu_e_rc_up->at(idx) );
        tlv_down.SetPtEtaPhiE( OUT::mu_pt_rc_down->at(idx), 
                          OUT::mu_eta->at(idx), 
                          OUT::mu_phi->at(idx), 
                          OUT::mu_e_rc_down->at(idx) );
        tlv_res_up.SetPtEtaPhiE( OUT::mu_pt_Sigma_up->at(idx),
                          OUT::mu_eta->at(idx),
                          OUT::mu_phi->at(idx),
                          OUT::mu_e_Sigma_up->at(idx) );
        tlv_res_down.SetPtEtaPhiE( OUT::mu_pt_Sigma_down->at(idx),
                          OUT::mu_eta->at(idx),
                          OUT::mu_phi->at(idx),
                          OUT::mu_e_Sigma_down->at(idx) );
        leptons.push_back(tlv);
        leptons_MuEn_up.push_back(tlv_up);
        leptons_MuEn_down.push_back(tlv_down);
        leptons_MuRes_up.push_back(tlv_res_up);
        leptons_MuRes_down.push_back(tlv_res_down);
        leptons_ElEn_up.push_back(tlv);
        leptons_ElEn_down.push_back(tlv);
        leptons_ElRes_up.push_back(tlv);
        leptons_ElRes_down.push_back(tlv);
        metlv_MuonEnUp   = metlv_MuonEnUp   + tlv - tlv_up;
        metlv_MuonEnDown = metlv_MuonEnDown + tlv - tlv_down;
        metlv_MuonResUp   = metlv_MuonResUp   + tlv - tlv_res_up;
        metlv_MuonResDown = metlv_MuonResDown + tlv - tlv_res_down;

    }
    metlv_MuonEnUp.SetPtEtaPhiM(   metlv_MuonEnUp.Pt(),   0.0, metlv_MuonEnUp.Phi(),   0.0 );
    metlv_MuonEnDown.SetPtEtaPhiM( metlv_MuonEnDown.Pt(), 0.0, metlv_MuonEnDown.Phi(), 0.0 );
    metlv_MuonResUp.SetPtEtaPhiM(   metlv_MuonResUp.Pt(),   0.0, metlv_MuonResUp.Phi(),   0.0 );
    metlv_MuonResDown.SetPtEtaPhiM( metlv_MuonResDown.Pt(), 0.0, metlv_MuonResDown.Phi(), 0.0 );
    OUT::met_MuonEnUpByHand_pt = metlv_MuonEnUp.Pt();
    OUT::met_MuonEnUpByHand_phi = metlv_MuonEnUp.Phi();
    OUT::met_MuonEnDownByHand_pt = metlv_MuonEnDown.Pt();
    OUT::met_MuonEnDownByHand_phi = metlv_MuonEnDown.Phi();
    OUT::met_MuonResUpByHand_pt = metlv_MuonResUp.Pt();
    OUT::met_MuonResUpByHand_phi = metlv_MuonResUp.Phi();
    OUT::met_MuonResDownByHand_pt = metlv_MuonResDown.Pt();
    OUT::met_MuonResDownByHand_phi = metlv_MuonResDown.Phi();

    for( int idx = 0; idx < OUT::el_n; ++idx ) {
        TLorentzVector tlv;
        TLorentzVector tlv_up;
        TLorentzVector tlv_down;
        TLorentzVector tlv_res_up;
        TLorentzVector tlv_res_down;
        tlv.SetPtEtaPhiE( OUT::el_pt->at(idx), 
                          OUT::el_eta->at(idx), 
                          OUT::el_phi->at(idx), 
                          OUT::el_e->at(idx) );
        tlv_up.SetPtEtaPhiE( OUT::el_pt_ScaleUp->at(idx), 
                          OUT::el_eta->at(idx), 
                          OUT::el_phi->at(idx), 
                          OUT::el_e_ScaleUp->at(idx) );
        tlv_down.SetPtEtaPhiE( OUT::el_pt_ScaleDown->at(idx), 
                          OUT::el_eta->at(idx), 
                          OUT::el_phi->at(idx), 
                          OUT::el_e_ScaleDown->at(idx) );
        tlv_res_up.SetPtEtaPhiE( OUT::el_pt_SigmaUp->at(idx), 
                          OUT::el_eta->at(idx), 
                          OUT::el_phi->at(idx), 
                          OUT::el_e_SigmaUp->at(idx) );
        tlv_res_down.SetPtEtaPhiE( OUT::el_pt_SigmaDown->at(idx), 
                          OUT::el_eta->at(idx), 
                          OUT::el_phi->at(idx), 
                          OUT::el_e_SigmaDown->at(idx) );

        leptons.push_back(tlv);
        leptons_MuEn_up.push_back(tlv);
        leptons_MuEn_down.push_back(tlv);
        leptons_MuRes_up.push_back(tlv);
        leptons_MuRes_down.push_back(tlv);
        leptons_ElEn_up.push_back(tlv_up);
        leptons_ElEn_down.push_back(tlv_down);
        leptons_ElRes_up.push_back(tlv_res_up);
        leptons_ElRes_down.push_back(tlv_res_down);

        metlv_ElectronEnUp   = metlv_ElectronEnUp   + tlv - tlv_up;
        metlv_ElectronEnDown = metlv_ElectronEnDown + tlv - tlv_down;
        metlv_ElectronResUp   = metlv_ElectronResUp   + tlv - tlv_res_up;
        metlv_ElectronResDown = metlv_ElectronResDown + tlv - tlv_res_down;
    }
    metlv_ElectronEnUp.SetPtEtaPhiM(   metlv_ElectronEnUp.Pt(),   0.0, metlv_ElectronEnUp.Phi(),   0.0 );
    metlv_ElectronEnDown.SetPtEtaPhiM( metlv_ElectronEnDown.Pt(), 0.0, metlv_ElectronEnDown.Phi(), 0.0 );
    OUT::met_ElectronEnUpByHand_pt = metlv_ElectronEnUp.Pt();
    OUT::met_ElectronEnUpByHand_phi = metlv_ElectronEnUp.Phi();
    OUT::met_ElectronEnDownByHand_pt = metlv_ElectronEnDown.Pt();
    OUT::met_ElectronEnDownByHand_phi = metlv_ElectronEnDown.Phi();
    metlv_ElectronResUp.SetPtEtaPhiM(   metlv_ElectronResUp.Pt(),   0.0, metlv_ElectronResUp.Phi(),   0.0 );
    metlv_ElectronResDown.SetPtEtaPhiM( metlv_ElectronResDown.Pt(), 0.0, metlv_ElectronResDown.Phi(), 0.0 );
    OUT::met_ElectronResUpByHand_pt    = metlv_ElectronResUp.Pt();
    OUT::met_ElectronResUpByHand_phi   = metlv_ElectronResUp.Phi();
    OUT::met_ElectronResDownByHand_pt  = metlv_ElectronResDown.Pt();
    OUT::met_ElectronResDownByHand_phi = metlv_ElectronResDown.Phi();


    for( int idx = 0; idx < OUT::ph_n; ++idx ) {
        TLorentzVector tlv;
        TLorentzVector tlv_up;
        TLorentzVector tlv_down;
        TLorentzVector tlv_res_up;
        TLorentzVector tlv_res_down;
        tlv.SetPtEtaPhiE( OUT::ph_pt->at(idx), 
                          OUT::ph_eta->at(idx), 
                          OUT::ph_phi->at(idx), 
                          OUT::ph_e->at(idx) );
        tlv_up.SetPtEtaPhiE( OUT::ph_pt_ScaleUp->at(idx), 
                          OUT::ph_eta->at(idx), 
                          OUT::ph_phi->at(idx), 
                          OUT::ph_e_ScaleUp->at(idx) );
        tlv_down.SetPtEtaPhiE( OUT::ph_pt_ScaleDown->at(idx), 
                          OUT::ph_eta->at(idx), 
                          OUT::ph_phi->at(idx), 
                          OUT::ph_e_ScaleDown->at(idx) );
        tlv_res_up.SetPtEtaPhiE( OUT::ph_pt_SigmaUp->at(idx), 
                          OUT::ph_eta->at(idx), 
                          OUT::ph_phi->at(idx), 
                          OUT::ph_e_SigmaUp->at(idx) );
        tlv_res_down.SetPtEtaPhiE( OUT::ph_pt_SigmaDown->at(idx), 
                          OUT::ph_eta->at(idx), 
                          OUT::ph_phi->at(idx), 
                          OUT::ph_e_SigmaDown->at(idx) );

        photons.push_back(tlv);
        photons_PhEn_up.push_back(tlv_up);
        photons_PhEn_down.push_back(tlv_down);
        photons_PhRes_up.push_back(tlv_res_up);
        photons_PhRes_down.push_back(tlv_res_down);

        metlv_PhotonEnUp   = metlv_PhotonEnUp   + tlv - tlv_up;
        metlv_PhotonEnDown = metlv_PhotonEnDown + tlv - tlv_down;
        metlv_PhotonResUp   = metlv_PhotonResUp   + tlv - tlv_res_up;
        metlv_PhotonResDown = metlv_PhotonResDown + tlv - tlv_res_down;
    }
    metlv_PhotonEnUp.SetPtEtaPhiM(   metlv_PhotonEnUp.Pt(),   0.0, metlv_PhotonEnUp.Phi(),   0.0 );
    metlv_PhotonEnDown.SetPtEtaPhiM( metlv_PhotonEnDown.Pt(), 0.0, metlv_PhotonEnDown.Phi(), 0.0 );
    OUT::met_PhotonEnUpByHand_pt = metlv_PhotonEnUp.Pt();
    OUT::met_PhotonEnUpByHand_phi = metlv_PhotonEnUp.Phi();
    OUT::met_PhotonEnDownByHand_pt = metlv_PhotonEnDown.Pt();
    OUT::met_PhotonEnDownByHand_phi = metlv_PhotonEnDown.Phi();
    metlv_PhotonResUp.SetPtEtaPhiM(   metlv_PhotonResUp.Pt(),   0.0, metlv_PhotonResUp.Phi(),   0.0 );
    metlv_PhotonResDown.SetPtEtaPhiM( metlv_PhotonResDown.Pt(), 0.0, metlv_PhotonResDown.Phi(), 0.0 );
    OUT::met_PhotonResUpByHand_pt    = metlv_PhotonResUp.Pt();
    OUT::met_PhotonResUpByHand_phi   = metlv_PhotonResUp.Phi();
    OUT::met_PhotonResDownByHand_pt  = metlv_PhotonResDown.Pt();
    OUT::met_PhotonResDownByHand_phi = metlv_PhotonResDown.Phi();

    if( leptons.size() > 0 ) {
        OUT::mt_lep_met = Utils::calc_mt( leptons[0], metlvOrig );
        OUT::m_lep_met = (leptons[0]+metlvOrig).M();
        OUT::pt_lep_met = (leptons[0]+metlvOrig).Pt();
        OUT::dphi_lep_met = leptons[0].DeltaPhi( metlvOrig );

        bool success = get_constriained_nu_pz( leptons[0], metlv );
        OUT::nu_z_solution_success = success;

        OUT::RecoWMass = ( leptons[0] + metlv ).M();

        if( leptons.size() > 1 ) {
            OUT::m_ll = (leptons[0] + leptons[1]).M();
        }

    }

    if( photons.size() > 0 ) {

        if( leptons.size() > 0 ) {

            OUT::m_lep_ph = ( leptons[0] + photons[0] ).M();
            OUT::m_lep_met_ph = ( leptons[0] + photons[0] + metlvOrig ).M();
            OUT::dphi_lep_ph = leptons[0].DeltaPhi(photons[0] );
            OUT::dr_lep_ph = leptons[0].DeltaR(photons[0] );
	    if (leptons.size() > 1) {
	      OUT::m_llph = (leptons[0] + leptons[1] + photons[0]).M();
	      OUT::dr_lep2_ph = leptons[1].DeltaR(photons[0] );
	    }
            OUT::recoM_lep_nu_ph = ( leptons[0] + metlv + photons[0] ).M();
            OUT::recoMet_eta = metlv.Eta() ;
            OUT::mt_lep_met_ph = Utils::calc_mt( leptons[0] + metlvOrig, photons[0]);
            OUT::mt_lep_met_ph_inv = Utils::calc_mt( leptons[0] + photons[0], metlvOrig);
            OUT::dphi_ph_w = ( metlvOrig + leptons[0] ).DeltaPhi( photons[0] );
            OUT::pt_lep_met_ph = ( metlvOrig + leptons[0] + photons[0]).Pt();

            TLorentzVector recoW = leptons[0] + metlv;

            OUT::recoW_pt = recoW.Pt() ;
            OUT::recoW_eta = recoW.Eta() ;
            OUT::recoW_phi = recoW.Phi() ;

            float mt = Utils::calc_mt( leptons[0], metlvOrig );

            TLorentzVector wlv;
            wlv.SetXYZM( leptons[0].Px() + metlvOrig.Px(), leptons[0].Py() + metlvOrig.Py(), leptons[0].Pz(), mt );

            TLorentzVector wlv_force;
            wlv_force.SetXYZM( leptons[0].Px() + metlvOrig.Px(), leptons[0].Py() + metlvOrig.Py(), leptons[0].Pz(), _m_w );
            OUT::m_mt_lep_met_ph = ( wlv + photons[0] ).M();
            OUT::m_mt_lep_met_ph_forcewmass = ( wlv_force + photons[0] ).M();
            OUT::mt_w = mt;

            
            TLorentzVector lep_trans; 
            TLorentzVector lep_trans_MuEn_up; 
            TLorentzVector lep_trans_MuEn_down; 
            TLorentzVector lep_trans_MuRes_up;
            TLorentzVector lep_trans_MuRes_down;
            TLorentzVector lep_trans_ElEn_up; 
            TLorentzVector lep_trans_ElEn_down; 
            TLorentzVector lep_trans_ElRes_up;
            TLorentzVector lep_trans_ElRes_down;
            TLorentzVector ph_trans; 
            TLorentzVector ph_trans_PhEn_up; 
            TLorentzVector ph_trans_PhEn_down; 
            TLorentzVector ph_trans_PhRes_up;
            TLorentzVector ph_trans_PhRes_down;

            lep_trans.SetPtEtaPhiM( leptons[0].Pt(), 0.0, leptons[0].Phi(), leptons[0].M() );
            lep_trans_MuEn_up  .SetPtEtaPhiM( leptons_MuEn_up[0].Pt(),   0.0, leptons_MuEn_up[0].Phi(),   leptons_MuEn_up[0].M() );
            lep_trans_MuEn_down.SetPtEtaPhiM( leptons_MuEn_down[0].Pt(), 0.0, leptons_MuEn_down[0].Phi(), leptons_MuEn_down[0].M() );
            lep_trans_MuRes_up  .SetPtEtaPhiM( leptons_MuRes_up[0].Pt(),   0.0, leptons_MuRes_up[0].Phi(),   leptons_MuRes_up[0].M() );
            lep_trans_MuRes_down.SetPtEtaPhiM( leptons_MuRes_down[0].Pt(), 0.0, leptons_MuRes_down[0].Phi(), leptons_MuRes_down[0].M() );

            lep_trans_ElEn_up  .SetPtEtaPhiM( leptons_ElEn_up[0].Pt(),   0.0, leptons_ElEn_up[0].Phi(),   leptons_ElEn_up[0].M() );
            lep_trans_ElEn_down.SetPtEtaPhiM( leptons_ElEn_down[0].Pt(), 0.0, leptons_ElEn_down[0].Phi(), leptons_ElEn_down[0].M() );
            lep_trans_ElRes_up  .SetPtEtaPhiM( leptons_ElRes_up[0].Pt(),   0.0, leptons_ElRes_up[0].Phi(),   leptons_ElRes_up[0].M() );
            lep_trans_ElRes_down.SetPtEtaPhiM( leptons_ElRes_down[0].Pt(), 0.0, leptons_ElRes_down[0].Phi(), leptons_ElRes_down[0].M() );

            ph_trans.SetPtEtaPhiM( photons[0].Pt(), 0.0, photons[0].Phi(), photons[0].M() );
            ph_trans_PhEn_up.SetPtEtaPhiM( photons_PhEn_up[0].Pt(), 0.0, photons_PhEn_up[0].Phi(), photons_PhEn_up[0].M() );
            ph_trans_PhEn_down.SetPtEtaPhiM( photons_PhEn_down[0].Pt(), 0.0, photons_PhEn_down[0].Phi(), photons_PhEn_down[0].M() );
            ph_trans_PhRes_up.SetPtEtaPhiM  ( photons_PhRes_up[0].Pt(),   0.0, photons_PhRes_up[0].Phi(),   photons_PhRes_up[0].M() );
            ph_trans_PhRes_down.SetPtEtaPhiM( photons_PhRes_down[0].Pt(), 0.0, photons_PhRes_down[0].Phi(), photons_PhRes_down[0].M() );

            // Transverse resonance mass
            OUT::mt_res = ( lep_trans + ph_trans + metlvOrig ).M();

            // met systematics
            OUT::mt_res_JetResUp   = ( lep_trans + ph_trans + metlv_JetResUp   ).M();
            OUT::mt_res_JetResDown = ( lep_trans + ph_trans + metlv_JetResDown ).M();
            OUT::mt_res_JetEnUp    = ( lep_trans + ph_trans + metlv_JetEnUp   ).M();
            OUT::mt_res_JetEnDown  = ( lep_trans + ph_trans + metlv_JetEnDown ).M();
            OUT::mt_res_ElectronEnUp   = ( lep_trans_ElEn_up   + ph_trans + metlv_ElectronEnUp   ).M();
            OUT::mt_res_ElectronEnDown = ( lep_trans_ElEn_down + ph_trans + metlv_ElectronEnDown ).M();
            OUT::mt_res_ElectronResUp   = ( lep_trans_ElRes_up   + ph_trans + metlv_ElectronResUp   ).M();
            OUT::mt_res_ElectronResDown = ( lep_trans_ElRes_down + ph_trans + metlv_ElectronResDown ).M();
            OUT::mt_res_MuonEnUp     = ( lep_trans_MuEn_up + ph_trans + metlv_MuonEnUp   ).M();
            OUT::mt_res_MuonEnDown   = ( lep_trans_MuEn_down + ph_trans + metlv_MuonEnDown ).M();
            OUT::mt_res_MuonResUp   = ( lep_trans_MuRes_up   + ph_trans + metlv_MuonResUp   ).M();
            OUT::mt_res_MuonResDown = ( lep_trans_MuRes_down + ph_trans + metlv_MuonResDown ).M();
            OUT::mt_res_PhotonEnUp   = ( lep_trans + ph_trans_PhEn_up + metlv_PhotonEnUp   ).M();
            OUT::mt_res_PhotonEnDown = ( lep_trans + ph_trans_PhEn_down + metlv_PhotonEnDown ).M();
            OUT::mt_res_PhotonResUp   = ( lep_trans + ph_trans_PhRes_up   + metlv_PhotonResUp   ).M();
            OUT::mt_res_PhotonResDown = ( lep_trans + ph_trans_PhRes_down + metlv_PhotonResDown ).M();
            OUT::mt_res_UnclusteredEnUp   = ( lep_trans + ph_trans + metlv_UnclusteredEnUp   ).M();
            OUT::mt_res_UnclusteredEnDown = ( lep_trans + ph_trans + metlv_UnclusteredEnDown ).M();

            OUT::mt_lep_ph = ( lep_trans + ph_trans ).M();

            for( std::vector<TLorentzVector>::const_iterator phitr = photons.begin();
                    phitr != photons.end(); ++phitr ) {

                float mass = ( *phitr + leptons[0] ).M();

                OUT::m_lep_ph_comb_leadLep->push_back( mass );
            }

            if( leptons.size() == 2 ) {
                for( std::vector<TLorentzVector>::const_iterator phitr = photons.begin();
                        phitr != photons.end(); ++phitr ) {

                    float mass = ( *phitr + leptons[1] ).M();

                    OUT::m_lep_ph_comb_sublLep->push_back( mass );
                }
            }
        }
    }

    std::vector<std::pair<float, int> > sorted_jets;
    std::vector<TLorentzVector> jet_lvs;

    for( int jeti = 0; jeti < OUT::jet_n; ++jeti ) {
        sorted_jets.push_back( std::make_pair( OUT::jet_pt->at(jeti ), jeti ) );

        TLorentzVector lv;
        lv.SetPtEtaPhiE( OUT::jet_pt->at(jeti),
                        OUT::jet_eta->at(jeti),
                        OUT::jet_phi->at(jeti),
                        OUT::jet_e->at(jeti)
                );
        jet_lvs.push_back( lv );
    }


    std::sort(sorted_jets.rbegin(), sorted_jets.rend());

    if( OUT::jet_n > 0 ) {
        OUT::leadjet_pt = OUT::jet_pt->at(0);

        if( OUT::jet_n > 1 ) {
            OUT::leadjet_pt = OUT::jet_pt->at(sorted_jets[0].second);
            OUT::subljet_pt = OUT::jet_pt->at(sorted_jets[1].second);

            OUT::leaddijet_m  = (jet_lvs[sorted_jets[0].second]+jet_lvs[sorted_jets[1].second]).M();
            OUT::leaddijet_pt = (jet_lvs[sorted_jets[0].second]+jet_lvs[sorted_jets[1].second]).Pt();

            float min_mass = 100000000.;
            int min_idx1 = -1;
            int min_idx2 = -1;
            for( unsigned i = 0 ; i < jet_lvs.size(); ++i ) {
                for( unsigned j = i+1 ; j < jet_lvs.size(); ++j ) {

                    float mass = ( jet_lvs[i] + jet_lvs[j] ).M();
                    float diff = fabs( _m_z - mass );

                    if( diff < min_mass ) {
                        min_mass = diff;
                        min_idx1 = i;
                        min_idx2 = j;
                    }
                }
            }

            OUT::massdijet_m  = ( jet_lvs[min_idx1] + jet_lvs[min_idx2] ).M();
            OUT::massdijet_pt = ( jet_lvs[min_idx1] + jet_lvs[min_idx2] ).Pt();
        }

    }

}

void RunModule::MakePhotonCountVars( ModuleConfig & config ) const { 

    OUT::ph_loose_n = 0;
    OUT::ph_medium_n = 0;
    OUT::ph_tight_n = 0;

    OUT::ph_mediumPassPSV_n = 0;
    OUT::ph_mediumFailPSV_n = 0;
    OUT::ph_mediumPassCSEV_n = 0;
    OUT::ph_mediumFailCSEV_n = 0;

    OUT::ph_mediumPassEleOlap_n = 0;
    OUT::ph_mediumPassEleOlapPassCSEV_n = 0;
    OUT::ph_mediumPassEleOlapFailCSEV_n = 0;

    OUT::ph_mediumNoSIEIE_n = 0;
    OUT::ph_mediumNoChIso_n = 0;
    OUT::ph_mediumNoNeuIso_n = 0;
    OUT::ph_mediumNoPhoIso_n = 0;

    OUT::ph_mediumNoSIEIENoChIso_n = 0;
    OUT::ph_mediumNoSIEIENoNeuIso_n = 0;
    OUT::ph_mediumNoSIEIENoPhoIso_n = 0;
    OUT::ph_mediumNoChIsoNoPhoIso_n = 0;
    OUT::ph_mediumNoChIsoNoNeuIso_n = 0;
    OUT::ph_mediumNoPhoIsoNoNeuIso_n = 0;

    OUT::ph_mediumNoSIEIEPassPSV_n = 0;
    OUT::ph_mediumNoChIsoPassPSV_n = 0;
    OUT::ph_mediumNoNeuIsoPassPSV_n = 0;
    OUT::ph_mediumNoPhoIsoPassPSV_n = 0;

    OUT::ph_mediumNoSIEIEFailPSV_n = 0;
    OUT::ph_mediumNoChIsoFailPSV_n = 0;
    OUT::ph_mediumNoNeuIsoFailPSV_n = 0;
    OUT::ph_mediumNoPhoIsoFailPSV_n = 0;

    OUT::ph_mediumNoSIEIEPassCSEV_n = 0;
    OUT::ph_mediumNoChIsoPassCSEV_n = 0;
    OUT::ph_mediumNoNeuIsoPassCSEV_n = 0;
    OUT::ph_mediumNoPhoIsoPassCSEV_n = 0;

    OUT::ph_mediumNoSIEIEFailCSEV_n = 0;
    OUT::ph_mediumNoChIsoFailCSEV_n = 0;
    OUT::ph_mediumNoNeuIsoFailCSEV_n = 0;
    OUT::ph_mediumNoPhoIsoFailCSEV_n = 0;

    OUT::ptSorted_ph_loose_idx->clear();
    OUT::ptSorted_ph_medium_idx->clear();
    OUT::ptSorted_ph_tight_idx->clear();

    OUT::ptSorted_ph_mediumPassPSV_idx->clear();
    OUT::ptSorted_ph_mediumFailPSV_idx->clear();
    OUT::ptSorted_ph_mediumPassCSEV_idx->clear();
    OUT::ptSorted_ph_mediumFailCSEV_idx->clear();

    OUT::ptSorted_ph_mediumPassEleOlap_idx->clear();
    OUT::ptSorted_ph_mediumPassEleOlapPassCSEV_idx->clear();
    OUT::ptSorted_ph_mediumPassEleOlapFailCSEV_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIE_idx->clear();
    OUT::ptSorted_ph_mediumNoChIso_idx->clear();
    OUT::ptSorted_ph_mediumNoNeuIso_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIso_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIENoChIso_idx->clear();
    OUT::ptSorted_ph_mediumNoSIEIENoNeuIso_idx->clear();
    OUT::ptSorted_ph_mediumNoSIEIENoPhoIso_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoNoPhoIso_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoNoNeuIso_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIsoNoNeuIso_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIEPassPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoPassPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoNeuIsoPassPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIsoPassPSV_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIEFailPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoFailPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoNeuIsoFailPSV_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIsoFailPSV_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIEPassCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoPassCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoNeuIsoPassCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIsoPassCSEV_idx->clear();

    OUT::ptSorted_ph_mediumNoSIEIEFailCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoChIsoFailCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoNeuIsoFailCSEV_idx->clear();
    OUT::ptSorted_ph_mediumNoPhoIsoFailCSEV_idx->clear();


    std::vector<std::pair<float, int> > sorted_ph_loose;
    std::vector<std::pair<float, int> > sorted_ph_medium;
    std::vector<std::pair<float, int> > sorted_ph_tight;

    std::vector<std::pair<float, int> > sorted_ph_mediumPassPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumFailPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumPassCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumFailCSEV;

    std::vector<std::pair<float, int> > sorted_ph_mediumPassEleOlap;
    std::vector<std::pair<float, int> > sorted_ph_mediumPassEleOlapPassCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumPassEleOlapFailCSEV;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIE;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoNeuIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIso;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIENoChIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIENoNeuIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIENoPhoIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoNoPhoIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoNoNeuIso;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIsoNoNeuIso;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIEPassPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoPassPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoNeuIsoPassPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIsoPassPSV;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIEFailPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoFailPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoNeuIsoFailPSV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIsoFailPSV;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIEPassCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoPassCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoNeuIsoPassCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIsoPassCSEV;

    std::vector<std::pair<float, int> > sorted_ph_mediumNoSIEIEFailCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoChIsoFailCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoNeuIsoFailCSEV;
    std::vector<std::pair<float, int> > sorted_ph_mediumNoPhoIsoFailCSEV;


    std::vector<TLorentzVector> photons;
    std::vector<std::pair<float, int> > sorted_photons;
    for( int idx = 0; idx < OUT::ph_n; ++idx ) {
        TLorentzVector phot;
        phot.SetPtEtaPhiM(  OUT::ph_pt->at(idx), 
                            OUT::ph_eta->at(idx),
                            OUT::ph_phi->at(idx),
                            0.0
                        );
        photons.push_back(phot);

        std::pair<float, int> sort_pair = std::make_pair( phot.Pt(), idx );

        sorted_photons.push_back( sort_pair );

        bool passHOverEMedium = OUT::ph_passHOverEMedium->at(idx);
        bool passSIEIEMedium  = OUT::ph_passSIEIEMedium->at(idx);
        bool passChIsoMedium  = OUT::ph_passChIsoCorrMedium->at(idx);
        bool passNeuIsoMedium = OUT::ph_passNeuIsoCorrMedium->at(idx);
        bool passPhoIsoMedium = OUT::ph_passPhoIsoCorrMedium->at(idx);
        bool passEleOlap      = (OUT::ph_min_el_dr->at(idx) > 0.4);
        bool passLoose        = OUT::ph_passLoose->at(idx);
        bool passMedium        = OUT::ph_passMedium->at(idx);
        bool passTight        = OUT::ph_passTight->at(idx);

        bool eleVeto          = OUT::ph_passEleVeto->at(idx);
        bool hasPixSeed       = OUT::ph_hasPixSeed->at(idx);

        if( passHOverEMedium ) {

            if( passNeuIsoMedium && passPhoIsoMedium ) {
                sorted_ph_mediumNoSIEIENoChIso.push_back( sort_pair );
            }
            if( passChIsoMedium && passPhoIsoMedium ) {
                sorted_ph_mediumNoSIEIENoNeuIso.push_back( sort_pair );
            }
            if( passChIsoMedium && passNeuIsoMedium ) {
                sorted_ph_mediumNoSIEIENoPhoIso.push_back( sort_pair );
            }
            if( passSIEIEMedium && passNeuIsoMedium ) {
                sorted_ph_mediumNoChIsoNoPhoIso.push_back( sort_pair );
            }
            if( passSIEIEMedium && passPhoIsoMedium ) {
                sorted_ph_mediumNoChIsoNoNeuIso.push_back( sort_pair );
            }
            if( passSIEIEMedium && passChIsoMedium ) {
                sorted_ph_mediumNoPhoIsoNoNeuIso.push_back( sort_pair );
            }
            
            if( passChIsoMedium && 
                passNeuIsoMedium &&  
                passPhoIsoMedium    ) {
                sorted_ph_mediumNoSIEIE.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumNoSIEIEPassCSEV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoSIEIEFailCSEV.push_back(sort_pair);
                }
                if( hasPixSeed ) {
                    sorted_ph_mediumNoSIEIEFailPSV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoSIEIEPassPSV.push_back(sort_pair);
                }
            }
            if( passSIEIEMedium && 
                passNeuIsoMedium &&  
                passPhoIsoMedium    ) {
                sorted_ph_mediumNoChIso.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumNoChIsoPassCSEV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoChIsoFailCSEV.push_back(sort_pair);
                }
                if( hasPixSeed ) {
                    sorted_ph_mediumNoChIsoFailPSV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoChIsoPassPSV.push_back(sort_pair);
                }
            }
            if( passSIEIEMedium && 
                passChIsoMedium &&  
                passPhoIsoMedium    ) {
                sorted_ph_mediumNoNeuIso.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumNoNeuIsoPassCSEV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoNeuIsoFailCSEV.push_back(sort_pair);
                }
                if( hasPixSeed ) {
                    sorted_ph_mediumNoNeuIsoFailPSV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoNeuIsoPassPSV.push_back(sort_pair);
                }
            }
            if( passSIEIEMedium && 
                passChIsoMedium &&  
                passNeuIsoMedium    ) {
                sorted_ph_mediumNoPhoIso.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumNoPhoIsoPassCSEV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoPhoIsoFailCSEV.push_back(sort_pair);
                }
                if( hasPixSeed ) {
                    sorted_ph_mediumNoPhoIsoFailPSV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumNoPhoIsoPassPSV.push_back(sort_pair);
                }
            }
            if( passSIEIEMedium && 
                passChIsoMedium &&  
                passNeuIsoMedium &&
                passPhoIsoMedium     ) {
                sorted_ph_medium.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumPassCSEV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumFailCSEV.push_back(sort_pair);
                }
                if( hasPixSeed ) {
                    sorted_ph_mediumFailPSV.push_back(sort_pair);
                }
                else {
                    sorted_ph_mediumPassPSV.push_back(sort_pair);
                }
            }
        }
        if( passLoose )  {
            sorted_ph_loose.push_back( sort_pair );
        }
        if( passMedium )  {
            if( passEleOlap ) {
                sorted_ph_mediumPassEleOlap.push_back( sort_pair );
                if( eleVeto ) {
                    sorted_ph_mediumPassEleOlapPassCSEV.push_back( sort_pair );
                }
                else {
                    sorted_ph_mediumPassEleOlapFailCSEV.push_back( sort_pair );
                }
            }
        }
        if( passTight )  {
            sorted_ph_tight.push_back( sort_pair );
        }
    }

    std::sort(sorted_ph_loose.rbegin(),sorted_ph_loose.rend()) ;
    std::sort(sorted_ph_medium.rbegin(),sorted_ph_medium.rend()) ;
    std::sort(sorted_ph_tight.rbegin(),sorted_ph_tight.rend()) ;

    std::sort(sorted_ph_mediumPassPSV.rbegin(),sorted_ph_mediumPassPSV.rend()) ;
    std::sort(sorted_ph_mediumFailPSV.rbegin(),sorted_ph_mediumFailPSV.rend()) ;
    std::sort(sorted_ph_mediumPassCSEV.rbegin(),sorted_ph_mediumPassCSEV.rend()) ;
    std::sort(sorted_ph_mediumFailCSEV.rbegin(),sorted_ph_mediumFailCSEV.rend()) ;

    std::sort(sorted_ph_mediumPassEleOlap.rbegin(),sorted_ph_mediumPassEleOlap.rend());
    std::sort(sorted_ph_mediumPassEleOlapPassCSEV.rbegin(),sorted_ph_mediumPassEleOlapPassCSEV.rend());
    std::sort(sorted_ph_mediumPassEleOlapFailCSEV.rbegin(),sorted_ph_mediumPassEleOlapFailCSEV.rend());


    std::sort(sorted_ph_mediumNoSIEIE.rbegin(),sorted_ph_mediumNoSIEIE.rend()) ;
    std::sort(sorted_ph_mediumNoChIso.rbegin(),sorted_ph_mediumNoChIso.rend()) ;
    std::sort(sorted_ph_mediumNoNeuIso.rbegin(),sorted_ph_mediumNoNeuIso.rend()) ;
    std::sort(sorted_ph_mediumNoPhoIso.rbegin(),sorted_ph_mediumNoPhoIso.rend()) ;

    std::sort(sorted_ph_mediumNoSIEIENoChIso.rbegin(),sorted_ph_mediumNoSIEIENoChIso.rend());
    std::sort(sorted_ph_mediumNoSIEIENoNeuIso.rbegin(),sorted_ph_mediumNoSIEIENoNeuIso.rend());
    std::sort(sorted_ph_mediumNoSIEIENoPhoIso.rbegin(),sorted_ph_mediumNoSIEIENoPhoIso.rend());
    std::sort(sorted_ph_mediumNoChIsoNoPhoIso.rbegin(),sorted_ph_mediumNoChIsoNoPhoIso.rend());
    std::sort(sorted_ph_mediumNoChIsoNoNeuIso.rbegin(),sorted_ph_mediumNoChIsoNoNeuIso.rend());
    std::sort(sorted_ph_mediumNoPhoIsoNoNeuIso.rbegin(),sorted_ph_mediumNoPhoIsoNoNeuIso.rend());

    std::sort(sorted_ph_mediumNoSIEIEPassPSV.rbegin(),sorted_ph_mediumNoSIEIEPassPSV.rend()) ;
    std::sort(sorted_ph_mediumNoChIsoPassPSV.rbegin(),sorted_ph_mediumNoChIsoPassPSV.rend()) ;
    std::sort(sorted_ph_mediumNoNeuIsoPassPSV.rbegin(),sorted_ph_mediumNoNeuIsoPassPSV.rend()) ;
    std::sort(sorted_ph_mediumNoPhoIsoPassPSV.rbegin(),sorted_ph_mediumNoPhoIsoPassPSV.rend()) ;

    std::sort(sorted_ph_mediumNoSIEIEFailPSV.rbegin(),sorted_ph_mediumNoSIEIEFailPSV.rend()) ;
    std::sort(sorted_ph_mediumNoChIsoFailPSV.rbegin(),sorted_ph_mediumNoChIsoFailPSV.rend()) ;
    std::sort(sorted_ph_mediumNoNeuIsoFailPSV.rbegin(),sorted_ph_mediumNoNeuIsoFailPSV.rend()) ;
    std::sort(sorted_ph_mediumNoPhoIsoFailPSV.rbegin(),sorted_ph_mediumNoPhoIsoFailPSV.rend()) ;

    std::sort(sorted_ph_mediumNoSIEIEPassCSEV.rbegin(),sorted_ph_mediumNoSIEIEPassCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoChIsoPassCSEV.rbegin(),sorted_ph_mediumNoChIsoPassCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoNeuIsoPassCSEV.rbegin(),sorted_ph_mediumNoNeuIsoPassCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoPhoIsoPassCSEV.rbegin(),sorted_ph_mediumNoPhoIsoPassCSEV.rend()) ;

    std::sort(sorted_ph_mediumNoSIEIEFailCSEV.rbegin(),sorted_ph_mediumNoSIEIEFailCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoChIsoFailCSEV.rbegin(),sorted_ph_mediumNoChIsoFailCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoNeuIsoFailCSEV.rbegin(),sorted_ph_mediumNoNeuIsoFailCSEV.rend()) ;
    std::sort(sorted_ph_mediumNoPhoIsoFailCSEV.rbegin(),sorted_ph_mediumNoPhoIsoFailCSEV.rend()) ;


    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_loose.begin(); itr != sorted_ph_loose.end(); ++itr ) {
        OUT::ptSorted_ph_loose_idx->push_back( itr->second );
        OUT::ph_loose_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_medium.begin(); itr != sorted_ph_medium.end(); ++itr ) {
        OUT::ptSorted_ph_medium_idx->push_back( itr->second );
        OUT::ph_medium_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_tight.begin(); itr != sorted_ph_tight.end(); ++itr ) {
        OUT::ptSorted_ph_tight_idx->push_back( itr->second );
        OUT::ph_tight_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumPassPSV.begin(); itr != sorted_ph_mediumPassPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumPassPSV_idx->push_back( itr->second );
        OUT::ph_mediumPassPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumFailPSV.begin(); itr != sorted_ph_mediumFailPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumFailPSV_idx->push_back( itr->second );
        OUT::ph_mediumFailPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumPassCSEV.begin(); itr != sorted_ph_mediumPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumFailCSEV.begin(); itr != sorted_ph_mediumFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumFailCSEV_n++;
    }

    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumPassEleOlap.begin(); itr != sorted_ph_mediumPassEleOlap.end(); ++itr ) {
        OUT::ptSorted_ph_mediumPassEleOlap_idx->push_back( itr->second );
        OUT::ph_mediumPassEleOlap_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumPassEleOlapPassCSEV.begin(); itr != sorted_ph_mediumPassEleOlapPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumPassEleOlapPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumPassEleOlapPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumPassEleOlapFailCSEV.begin(); itr != sorted_ph_mediumPassEleOlapFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumPassEleOlapFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumPassEleOlapFailCSEV_n++;
    }


    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIE.begin(); itr != sorted_ph_mediumNoSIEIE.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIE_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIE_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIso.begin(); itr != sorted_ph_mediumNoChIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIso_idx->push_back( itr->second );
        OUT::ph_mediumNoChIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoNeuIso.begin(); itr != sorted_ph_mediumNoNeuIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoNeuIso_idx->push_back( itr->second );
        OUT::ph_mediumNoNeuIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIso.begin(); itr != sorted_ph_mediumNoPhoIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIso_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIENoChIso.begin(); itr != sorted_ph_mediumNoSIEIENoChIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIENoChIso_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIENoChIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIENoNeuIso.begin(); itr != sorted_ph_mediumNoSIEIENoNeuIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIENoNeuIso_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIENoNeuIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIENoPhoIso.begin(); itr != sorted_ph_mediumNoSIEIENoPhoIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIENoPhoIso_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIENoPhoIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoNoPhoIso.begin(); itr != sorted_ph_mediumNoChIsoNoPhoIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoNoPhoIso_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoNoPhoIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoNoNeuIso.begin(); itr != sorted_ph_mediumNoChIsoNoNeuIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoNoNeuIso_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoNoNeuIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIsoNoNeuIso.begin(); itr != sorted_ph_mediumNoPhoIsoNoNeuIso.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIsoNoNeuIso_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIsoNoNeuIso_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIEPassPSV.begin(); itr != sorted_ph_mediumNoSIEIEPassPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIEPassPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIEPassPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoPassPSV.begin(); itr != sorted_ph_mediumNoChIsoPassPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoPassPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoPassPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoNeuIsoPassPSV.begin(); itr != sorted_ph_mediumNoNeuIsoPassPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoNeuIsoPassPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoNeuIsoPassPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIsoPassPSV.begin(); itr != sorted_ph_mediumNoPhoIsoPassPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIsoPassPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIsoPassPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIEFailPSV.begin(); itr != sorted_ph_mediumNoSIEIEFailPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIEFailPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIEFailPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoFailPSV.begin(); itr != sorted_ph_mediumNoChIsoFailPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoFailPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoFailPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoNeuIsoFailPSV.begin(); itr != sorted_ph_mediumNoNeuIsoFailPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoNeuIsoFailPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoNeuIsoFailPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIsoFailPSV.begin(); itr != sorted_ph_mediumNoPhoIsoFailPSV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIsoFailPSV_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIsoFailPSV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIEPassCSEV.begin(); itr != sorted_ph_mediumNoSIEIEPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIEPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIEPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoPassCSEV.begin(); itr != sorted_ph_mediumNoChIsoPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoNeuIsoPassCSEV.begin(); itr != sorted_ph_mediumNoNeuIsoPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoNeuIsoPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoNeuIsoPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIsoPassCSEV.begin(); itr != sorted_ph_mediumNoPhoIsoPassCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIsoPassCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIsoPassCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoSIEIEFailCSEV.begin(); itr != sorted_ph_mediumNoSIEIEFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoSIEIEFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoSIEIEFailCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoChIsoFailCSEV.begin(); itr != sorted_ph_mediumNoChIsoFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoChIsoFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoChIsoFailCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoNeuIsoFailCSEV.begin(); itr != sorted_ph_mediumNoNeuIsoFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoNeuIsoFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoNeuIsoFailCSEV_n++;
    }
    for( std::vector<std::pair<float, int> >::const_iterator itr = sorted_ph_mediumNoPhoIsoFailCSEV.begin(); itr != sorted_ph_mediumNoPhoIsoFailCSEV.end(); ++itr ) {
        OUT::ptSorted_ph_mediumNoPhoIsoFailCSEV_idx->push_back( itr->second );
        OUT::ph_mediumNoPhoIsoFailCSEV_n++;
    }


}

void RunModule::BuildTruth( ModuleConfig & config ) const {

#ifdef EXISTS_gen_n

    OUT::trueph_n = 0;
    OUT::trueph_pt->clear();
    OUT::trueph_eta->clear();
    OUT::trueph_phi->clear();
    OUT::trueph_lep_dr->clear();
    OUT::trueph_motherPID->clear();
    OUT::trueph_status->clear();
    OUT::trueph_isPromptFS->clear();
    OUT::trueph_FHPFS->clear();

    OUT::truelep_n = 0;
    OUT::truelep_pt->clear();
    OUT::truelep_eta->clear();
    OUT::truelep_phi->clear();
    OUT::truelep_e->clear();
    OUT::truelep_motherPID->clear();
    OUT::truelep_status->clear();
    OUT::truelep_PID->clear();

    OUT::truenu_n = 0;
    OUT::truemed_mass = 0.0;

    OUT::trueW_n = 0;
    OUT::trueW_pt->clear();
    OUT::trueW_eta->clear();
    OUT::trueW_phi->clear();
    OUT::trueW_m->clear();
    OUT::trueW_motherPID->clear();
    OUT::trueW_status->clear();

    OUT::truelepnu_m = 0.0;
    OUT::truelepnuph_m = 0.0;
    OUT::truelepph_dr = 0.0;
    OUT::truemt_lep_met = 0.0;
    OUT::truemt_lep_met_ph = 0;
    OUT::truemt_res = 0;
    OUT::truemt_res_l23 = 0;
    OUT::truemt_res_lO = 0;

    OUT::trueht = 0;

    OUT::isWMuDecay = false;
    OUT::isWElDecay = false;
    OUT::isWTauDecay = false;
    OUT::isWTauElDecay = false;
    OUT::isWTauMuDecay = false;
    OUT::WIDStep = 0;

    OUT::ph_hasTruthMatchPh->clear();
    OUT::ph_truthMatchPh_pt->clear();
    OUT::ph_truthMatchPh_dr->clear();
    OUT::ph_hasTruthMatchEl->clear();
    OUT::ph_truthMatchEl_pt->clear();
    OUT::ph_truthMatchEl_dr->clear();
    OUT::ph_hasTruthMatchMu->clear();
    OUT::ph_truthMatchMu_pt->clear();
    OUT::ph_truthMatchMu_dr->clear();

    OUT::el_hasTruthMatchEl->clear();
    OUT::el_truthMatchEl_pt->clear();
    OUT::el_truthMatchEl_dr->clear();
    OUT::el_hasTruthMatchPh->clear();
    OUT::el_truthMatchPh_pt->clear();
    OUT::el_truthMatchPh_dr->clear();

    OUT::mu_hasTruthMatchMu->clear();
    OUT::mu_truthMatchMu_pt->clear();
    OUT::mu_truthMatchMu_dr->clear();

    std::vector<TLorentzVector> trueleps;
    std::vector<TLorentzVector> nulvs;
    std::vector<TLorentzVector> photlvs;
    std::vector<std::pair<float, unsigned> >  sorted_tleps;
    std::vector<std::pair<float, unsigned> >  sorted_tphots;

    TLorentzVector nu_sum;

    for( int gidx = 0; gidx < IN::gen_n; ++gidx ) {

        int id = IN::gen_PID->at(gidx);
        int absid = abs(id);
        int st = IN::gen_status->at(gidx);

        // calculate HT at truth level
        if( ( absid < 6 || id == 21 )  && st == 23 ) {
            OUT::trueht += IN::gen_pt->at(gidx);
        }

        if( absid >= 11 && absid <= 16) {

            // keep all leptons at this point
            // so they can be used later
            sorted_tleps.push_back( std::make_pair( IN::gen_pt->at(gidx), gidx ) );

        }
        // neutrinos
        if( absid == 12 || absid == 14 || absid == 16 ) {

            TLorentzVector nulv;
            nulv.SetPtEtaPhiM( IN::gen_pt->at(gidx),
                               IN::gen_eta->at(gidx),
                               IN::gen_phi->at(gidx),
                               0.0
                             );

            nu_sum += nulv;
            nulvs.push_back( nulv );

            OUT::truenu_n++;
        }


        if ( absid == 9000007 ) {
          // mediators
          TLorentzVector xlv;
          xlv.SetPtEtaPhiE( IN::gen_pt->at(gidx),
                            IN::gen_eta->at(gidx),
                            IN::gen_phi->at(gidx),
                            IN::gen_e->at(gidx)
                            );
          if ( OUT::truemed_mass<1. && abs(IN::gen_eta->at(gidx))<100) OUT::truemed_mass = xlv.M();
        
        }

        // photons
        if( absid == 22 ) {

            bool pass_ph_cuts = true;

            //if( !config.PassInt( "cut_ph_mother", abs(IN::gen_motherPID->at(gidx) ) ) ) pass_ph_cuts = false;
            //std::cout<<" ph_mother "<<IN::gen_motherPID->at(gidx)  <<" " << pass_ph_cuts;
            //if( !config.PassInt( "cut_ph_status", IN::gen_status->at(gidx) ) ) pass_ph_cuts = false;
            //std::cout<<" ph_status "<<IN::gen_status->at(gidx)  <<" " << pass_ph_cuts;
            //if( !config.PassBool( "cut_ph_IsPromptFinalState", IN::gen_isPromptFinalState->at(gidx) ) ) pass_ph_cuts = false;
            //std::cout<<" ph_prompt "<< IN::gen_isPromptFinalState->at(gidx) <<" " << pass_ph_cuts;
            //if( !config.PassBool( "cut_ph_FromHardProcessFinalState", IN::gen_fromHardProcessFinalState->at(gidx) ) ) pass_ph_cuts = false;
            //std::cout<<" ph_FHPFS "<<IN::gen_fromHardProcessFinalState->at(gidx) <<" " << pass_ph_cuts <<std::endl ;


            if( pass_ph_cuts ) {

                sorted_tphots.push_back( std::make_pair( IN::gen_pt->at(gidx), gidx ) );
            }
        }
 
        // W boson
        if( absid == 24 ) {

          bool pass_W_cuts = true;

          if( IN::gen_motherPID->at(gidx) == id ) pass_W_cuts = false;
          if( pass_W_cuts || IN::gen_status->at( gidx ) == 62 ){ // first and last W
          
            TLorentzVector wlv;
            wlv.SetPtEtaPhiE( IN::gen_pt ->at(gidx),
                              IN::gen_eta->at(gidx),
                              IN::gen_phi->at(gidx),
                              IN::gen_e  ->at(gidx)
                            );

              OUT::trueW_n++;
              OUT::trueW_pt->push_back( IN::gen_pt->at( gidx ) );
              OUT::trueW_eta->push_back( IN::gen_eta->at( gidx ) );
              OUT::trueW_phi->push_back( IN::gen_phi->at( gidx ) );
              OUT::trueW_m->push_back( wlv.M() );
              OUT::trueW_motherPID->push_back( IN::gen_motherPID->at( gidx ) );
              OUT::trueW_status->push_back( IN::gen_status->at( gidx ) );

          }
        }
    }
    // Handle events with off-shell W bosons (not in event record)
    // But they can also be just W-less background events
    if (OUT::trueW_n == 0) {
        // search LHE leptons
        int clidx = -1;
        int nuidx = -1;
        for( int gidx = 0; gidx < IN::gen_n; ++gidx ) {
            int id = IN::gen_PID->at(gidx);
            int absid = abs(id);
            int st = IN::gen_status->at(gidx);
            
            if (st != 23)
                continue;
            
            if( IN::gen_motherPID->at(gidx) == id )
                continue;
            
            if( absid == 11 || absid == 13 || absid == 15 ) {
                clidx = gidx;
            }
            else if( absid == 12 || absid == 14 || absid == 16 ) {
                nuidx = gidx;
            }
        }
        // LHE off-shell W candidate
        if (clidx > -1 && nuidx > -1) {
            if (abs(IN::gen_PID->at(clidx) + IN::gen_PID->at(nuidx)) != 1) {
                std::cout << "Caution, pdgIds for off-shell W construction do not match: " << IN::gen_PID->at(clidx) << "," << IN::gen_PID->at(nuidx) << std::endl;
            }
            TLorentzVector cllv;
            cllv.SetPtEtaPhiE( IN::gen_pt ->at(clidx),
                               IN::gen_eta->at(clidx),
                               IN::gen_phi->at(clidx),
                               IN::gen_e  ->at(clidx)
                             );
            TLorentzVector nulv;
            nulv.SetPtEtaPhiM( IN::gen_pt ->at(nuidx),
                               IN::gen_eta->at(nuidx),
                               IN::gen_phi->at(nuidx),
                               0.0
                             );
            TLorentzVector wlv = cllv + nulv;
            
            OUT::trueW_n++;
            OUT::trueW_pt ->push_back( wlv.Pt()  );
            if (wlv.Pt() > 0.001)
                OUT::trueW_eta->push_back( wlv.Eta() );
            else
                OUT::trueW_eta->push_back( wlv.Pz() * 1e3 );
            OUT::trueW_phi->push_back( wlv.Phi() );
            OUT::trueW_m  ->push_back( wlv.M()   );
            OUT::trueW_motherPID->push_back( 0 );
            OUT::trueW_status   ->push_back( 22 );
            
            // status-62 W candidate
            int clidx62 = -1;
            int nuidx62 = -1;
            int wcharge =  0;
            for( int gidx = 0; gidx < IN::gen_n; ++gidx ) {
                int id = IN::gen_PID->at(gidx);
                //int absid = abs(id);
                int st = IN::gen_status->at(gidx);
                
                // daughters of status-62 W are stable (1) or decayed taus (2)
                if (st != 1 && st != 2)
                    continue;
                // find last one in chain
                if( IN::gen_motherPID->at(gidx) != id )
                    continue;
                
                if( id == IN::gen_PID->at(clidx) ) {
                    clidx62 = gidx;
                    if (id > 0) { // e/mu/tau-
                        wcharge = -1;
                    }
                    else if (id < 0) { // e/mu/tau+
                        wcharge = +1;
                    }
                }
                else if( id == IN::gen_PID->at(nuidx) ) {
                    nuidx62 = gidx;
                }
            }
            if (clidx62 > -1 && nuidx62 > -1) {
                TLorentzVector cllv;
                cllv.SetPtEtaPhiE( IN::gen_pt ->at(clidx62),
                                   IN::gen_eta->at(clidx62),
                                   IN::gen_phi->at(clidx62),
                                   IN::gen_e  ->at(clidx62)
                                   );
                TLorentzVector nulv;
                nulv.SetPtEtaPhiM( IN::gen_pt ->at(nuidx62),
                                   IN::gen_eta->at(nuidx62),
                                   IN::gen_phi->at(nuidx62),
                                   0.0
                                   );
                TLorentzVector wlv = cllv + nulv;
                
                OUT::trueW_n++;
                OUT::trueW_pt ->push_back( wlv.Pt()  );
                OUT::trueW_eta->push_back( wlv.Eta() );
                OUT::trueW_phi->push_back( wlv.Phi() );
                OUT::trueW_m  ->push_back( wlv.M()   );
                OUT::trueW_motherPID->push_back( 24 * wcharge );
                OUT::trueW_status   ->push_back( 62 );
            } 
        }
    }

    // sort leptons so the highest pT leptons come first
    std::sort(sorted_tleps.rbegin(), sorted_tleps.rend());
    // sort photons so the highest pT leptons come first
    std::sort(sorted_tphots.rbegin(), sorted_tphots.rend());

    // now fill outputs using the sorted objects
    for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
            sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

        unsigned lidx = litr->second;

        int pid = IN::gen_PID->at(lidx);
        // should be an electron or muon or taon
        if( !( abs(pid) == 11 || abs(pid) == 13 || abs(pid) == 15 ) ) continue;

        //if( IN::gen_motherPID->at(lidx) == pid ) continue;

        if( !config.PassInt( "cut_lep_mother", IN::gen_motherPID->at(lidx) ) ) continue;
        if( !config.PassInt( "cut_lep_status", IN::gen_status->at(lidx) ) ) continue;

        OUT::truelep_n++;
        OUT::truelep_pt->push_back( IN::gen_pt->at( lidx ) );
        OUT::truelep_eta->push_back( IN::gen_eta->at( lidx ) );
        OUT::truelep_phi->push_back( IN::gen_phi->at( lidx ) );
        OUT::truelep_e->push_back( IN::gen_e->at( lidx ) );
        OUT::truelep_motherPID->push_back( IN::gen_motherPID->at( lidx ) );
        OUT::truelep_status->push_back( IN::gen_status->at( lidx ) );
        OUT::truelep_PID->push_back( IN::gen_PID->at( lidx ) );

        TLorentzVector leplv;
        leplv.SetPtEtaPhiE( IN::gen_pt->at(lidx),
                            IN::gen_eta->at(lidx),
                            IN::gen_phi->at(lidx),
                            IN::gen_e->at(lidx)
                          );

        trueleps.push_back( leplv );

    }

    for( std::vector<std::pair<float, unsigned> >::const_iterator pitr = 
            sorted_tphots.begin(); pitr != sorted_tphots.end(); ++pitr ) {

        unsigned pidx = pitr->second;

        OUT::trueph_n++;
        OUT::trueph_pt->push_back( IN::gen_pt->at( pidx ) );
        OUT::trueph_eta->push_back( IN::gen_eta->at( pidx ) );
        OUT::trueph_phi->push_back( IN::gen_phi->at( pidx ) );
        OUT::trueph_motherPID->push_back( IN::gen_motherPID->at( pidx ) );
        OUT::trueph_status->push_back( IN::gen_status->at( pidx ) );
        OUT::trueph_isPromptFS->push_back( IN::gen_isPromptFinalState->at( pidx ) );
        OUT::trueph_FHPFS->push_back( IN::gen_fromHardProcessFinalState->at( pidx ) );

        TLorentzVector phlv;
        phlv.SetPtEtaPhiM( IN::gen_pt->at(pidx),
                           IN::gen_eta->at(pidx),
                           IN::gen_phi->at(pidx),
                           0.0
                         );

        photlvs.push_back( phlv );
        
        float minDR = 99.;
        for (TLorentzVector truelep : trueleps) {
            float dR = phlv.DeltaR(truelep);
            if (dR < minDR) {
                minDR = dR;
            }
        }
        OUT::trueph_lep_dr->push_back( minDR );
    }

    //photon truth match
    for( int phidx = 0; phidx < OUT::ph_n; ++phidx ) {

        TLorentzVector phlv;
        phlv.SetPtEtaPhiE( OUT::ph_pt->at(phidx),
                           OUT::ph_eta->at(phidx),
                           OUT::ph_phi->at(phidx),
                           OUT::ph_e->at(phidx)
                         );


        float mindr = 101.;
        float matchPt = 0;
        for( int tphidx = 0; tphidx < OUT::trueph_n; ++tphidx ) {

            TLorentzVector tphlv;
            tphlv.SetPtEtaPhiM( OUT::trueph_pt->at(tphidx),
                                OUT::trueph_eta->at(tphidx),
                                OUT::trueph_phi->at(tphidx),
                                0.0
                              );


            float dr = phlv.DeltaR( tphlv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tphlv.Pt();
            }

        }

        OUT::ph_hasTruthMatchPh->push_back( ( mindr < 0.4 ) );
        OUT::ph_truthMatchPh_pt->push_back( matchPt );
        OUT::ph_truthMatchPh_dr->push_back( mindr );

        // photon matched to a gen electron
        mindr = 101.;
        matchPt = 0;
        for( int tlepidx = 0; tlepidx < OUT::truelep_n; ++tlepidx ) {

            if( abs( OUT::truelep_PID->at(tlepidx) ) != 11 ) continue;

            TLorentzVector tleplv;
            tleplv.SetPtEtaPhiE( OUT::truelep_pt->at(tlepidx),
                                 OUT::truelep_eta->at(tlepidx),
                                 OUT::truelep_phi->at(tlepidx),
                                 OUT::truelep_e->at(tlepidx)
                               );

            float dr = phlv.DeltaR( tleplv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tleplv.Pt();
            }

        }

        OUT::ph_hasTruthMatchEl->push_back( ( mindr < 0.4 ) );
        OUT::ph_truthMatchEl_pt->push_back( matchPt );
        OUT::ph_truthMatchEl_dr->push_back( mindr );


        // photon matched to a gen muon
        mindr = 101.;
        matchPt = 0;
        for( int tlepidx = 0; tlepidx < OUT::truelep_n; ++tlepidx ) {

            if( abs( OUT::truelep_PID->at(tlepidx) ) != 13 ) continue;

            TLorentzVector tleplv;
            tleplv.SetPtEtaPhiE( OUT::truelep_pt->at(tlepidx),
                                 OUT::truelep_eta->at(tlepidx),
                                 OUT::truelep_phi->at(tlepidx),
                                 OUT::truelep_e->at(tlepidx)
                               );

            float dr = phlv.DeltaR( tleplv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tleplv.Pt();
            }

        }

        OUT::ph_hasTruthMatchMu->push_back( ( mindr < 0.4 ) );
        OUT::ph_truthMatchMu_pt->push_back( matchPt );
        OUT::ph_truthMatchMu_dr->push_back( mindr );

    }

    // muon truth match
    for( int muidx = 0; muidx < OUT::mu_n; ++muidx ) {

        TLorentzVector mulv;
        mulv.SetPtEtaPhiE( OUT::mu_pt->at(muidx),
                           OUT::mu_eta->at(muidx),
                           OUT::mu_phi->at(muidx),
                           OUT::mu_e->at(muidx)
                         );


        float mindr = 101.;
        float matchPt = 0;
        for( int tlepidx = 0; tlepidx < OUT::truelep_n; ++tlepidx ) {

            if( abs( OUT::truelep_PID->at(tlepidx) ) != 13 ) continue;

            TLorentzVector tleplv;
            tleplv.SetPtEtaPhiE( OUT::truelep_pt->at(tlepidx),
                                 OUT::truelep_eta->at(tlepidx),
                                 OUT::truelep_phi->at(tlepidx),
                                 OUT::truelep_e->at(tlepidx)
                               );

            float dr = mulv.DeltaR( tleplv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tleplv.Pt();
            }

        }

        OUT::mu_hasTruthMatchMu->push_back( ( mindr < 0.4 ) );
        OUT::mu_truthMatchMu_pt->push_back( matchPt );
        OUT::mu_truthMatchMu_dr->push_back( mindr );

    }

    // electron truth match
    for( int elidx = 0; elidx < OUT::el_n; ++elidx ) {

        TLorentzVector ellv;
        ellv.SetPtEtaPhiE( OUT::el_pt->at(elidx),
                           OUT::el_eta->at(elidx),
                           OUT::el_phi->at(elidx),
                           OUT::el_e->at(elidx)
                         );


        float mindr = 101.;
        float matchPt = 0;
        for( int tlepidx = 0; tlepidx < OUT::truelep_n; ++tlepidx ) {

            if( abs( OUT::truelep_PID->at(tlepidx) ) != 11 ) continue;

            TLorentzVector tleplv;
            tleplv.SetPtEtaPhiE( OUT::truelep_pt->at(tlepidx),
                                 OUT::truelep_eta->at(tlepidx),
                                 OUT::truelep_phi->at(tlepidx),
                                 OUT::truelep_e->at(tlepidx)
                               );

            float dr = ellv.DeltaR( tleplv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tleplv.Pt();
            }

        }

        OUT::el_hasTruthMatchEl->push_back( ( mindr < 0.4 ) );
        OUT::el_truthMatchEl_pt->push_back( matchPt );
        OUT::el_truthMatchEl_dr->push_back( mindr );

        mindr = 101.;
        matchPt = 0;
        for( int tphidx = 0; tphidx < OUT::trueph_n; ++tphidx ) {

            TLorentzVector tphlv;
            tphlv.SetPtEtaPhiM( OUT::trueph_pt->at(tphidx),
                                OUT::trueph_eta->at(tphidx),
                                OUT::trueph_phi->at(tphidx),
                                0.0
                              );


            float dr = ellv.DeltaR( tphlv );

            if( dr < mindr ) {
                mindr = dr;
                matchPt = tphlv.Pt();
            }

        }

        OUT::el_hasTruthMatchPh->push_back( ( mindr < 0.4 ) );
        OUT::el_truthMatchPh_pt->push_back( matchPt );
        OUT::el_truthMatchPh_dr->push_back( mindr );

    }

    if( trueleps.size() > 0 && OUT::trueph_n>0) {
   
        TLorentzVector lep_trans;
        TLorentzVector ph_trans;
        lep_trans.SetPtEtaPhiM( trueleps[0].Pt(), 0.0, trueleps[0].Phi(), trueleps[0].M() );
        ph_trans.SetPtEtaPhiM( OUT::trueph_pt->at(0), 0.0, OUT::trueph_phi->at(0), 0.0 );

        TLorentzVector metlv;
        metlv.SetPtEtaPhiM( nu_sum.Pt(), 0.0, nu_sum.Phi(), 0.0 );

        OUT::truemt_res = ( lep_trans + ph_trans + metlv ).M();
        //std::cout << "Truelep pt = " << trueleps[0].Pt() << " nu pt = " << nu_sum.Pt() << std::endl;
        OUT::truemt_lep_met_ph = Utils::calc_mt( trueleps[0], nu_sum);

        if( trueleps.size() == 2  ) { 

            TLorentzVector l23_trans;
            TLorentzVector lO_trans;
            for( int lidx = 0; lidx != OUT::truelep_n ; ++lidx ) {

                TLorentzVector leplv = trueleps[lidx];

                if( OUT::truelep_status->at(lidx) == 23 ) {
                    l23_trans.SetPtEtaPhiM( leplv.Pt(), 0.0, leplv.Phi(), leplv.M() );
                }
                else {
                    lO_trans.SetPtEtaPhiM( leplv.Pt(), 0.0, leplv.Phi(), leplv.M() );
                }
            }
            OUT::truemt_res_l23 = ( l23_trans + ph_trans + metlv ).M();
            OUT::truemt_res_lO = ( lO_trans + ph_trans + metlv ).M();
        }
    }


    OUT::truelepnu_m = 0.0;
    OUT::truelepnuph_m = 0.0;
    OUT::truelepph_dr = 0.0;

    if( OUT::truelep_n >= 1 ) {
        TLorentzVector wlv( trueleps[0] );

        for( unsigned i = 0 ; i < nulvs.size(); ++i) {
            wlv = wlv + nulvs[i];
        }
        OUT::truelepnu_m = wlv.M();

        if( photlvs.size() > 0 ) {
            OUT::truelepnuph_m = (wlv + photlvs[0]).M();
            OUT::truelepph_dr =  trueleps[0].DeltaR( photlvs[0] );
        }
    }

    OUT::truemt_lep_met = 0.0;

    if( trueleps.size() > 0 && nulvs.size() > 0 ) {

        TLorentzVector true_metlv;
        true_metlv.SetPtEtaPhiM( nulvs[0].Pt(), 0.0, nulvs[0].Phi(), 0.0 );

        OUT::truemt_lep_met = Utils::calc_mt( trueleps[0], true_metlv);
    }

    // in dermining the W decay mode use the original
    // vector of leptons.  Do not use trueleps which
    // may have additional cuts
    OUT::isWMuDecay = false;
    OUT::isWElDecay = false;
    OUT::isWTauDecay = false;
    OUT::isWTauElDecay = false;
    OUT::isWTauMuDecay = false;
    OUT::WIDStep = 0;
    bool found_w_mother = false;
    for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
            sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

        if( abs(IN::gen_motherPID->at(litr->second)) != 24 ) continue;
        if( IN::gen_status->at(litr->second) != 3 ) continue;

        found_w_mother = true;

        int lep_id = IN::gen_PID->at(litr->second);

        if( abs(lep_id) == 11 || abs(lep_id) == 12 ) {
            OUT::isWElDecay=true;
        }
        if( abs(lep_id) == 13 || abs(lep_id) == 14 ) {
            OUT::isWMuDecay=true;
        }
        if( abs(lep_id) == 15 || abs(lep_id) == 16 ) {
            OUT::isWTauDecay=true;
        }
    }

    if( !found_w_mother ) {
        OUT::WIDStep++;
        for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
                sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

            if( abs(IN::gen_motherPID->at(litr->second)) != 24 ) continue;
            if( IN::gen_status->at(litr->second) != 23 ) continue;

            found_w_mother = true;

            int lep_id = IN::gen_PID->at(litr->second);

            if( abs(lep_id) == 11 || abs(lep_id) == 12 ) {
                OUT::isWElDecay=true;
            }
            if( abs(lep_id) == 13 || abs(lep_id) == 14 ) {
                OUT::isWMuDecay=true;
            }
            if( abs(lep_id) == 15 || abs(lep_id) == 16 ) {
                OUT::isWTauDecay=true;
            }
        }
    }

    if( !found_w_mother ) {
        OUT::WIDStep++;
        for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
                sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

            if( abs(IN::gen_motherPID->at(litr->second)) != 24 ) continue;
            if( IN::gen_status->at(litr->second) != 2 ) continue;

            found_w_mother = true;

            int lep_id = IN::gen_PID->at(litr->second);

            if( abs(lep_id) == 11 || abs(lep_id) == 12 ) {
                OUT::isWElDecay=true;
            }
            if( abs(lep_id) == 13 || abs(lep_id) == 14 ) {
                OUT::isWMuDecay=true;
            }
            if( abs(lep_id) == 15 || abs(lep_id) == 16 ) {
                OUT::isWTauDecay=true;
            }
        }
    }

    if( !found_w_mother ) {
        OUT::WIDStep++;
        for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
                sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

            if( abs(IN::gen_motherPID->at(litr->second)) != 24 ) continue;
            if( IN::gen_status->at(litr->second) != 1 ) continue;

            found_w_mother = true;

            int lep_id = IN::gen_PID->at(litr->second);
            if( abs(lep_id) == 11 || abs(lep_id) == 12 ) {
                OUT::isWElDecay=true;
            }
            if( abs(lep_id) == 13 || abs(lep_id) == 14 ) {
                OUT::isWMuDecay=true;
            }
            if( abs(lep_id) == 15 || abs(lep_id) == 16 ) {
                OUT::isWTauDecay=true;
            }
        }
    }
    if( !found_w_mother ) {
        OUT::WIDStep++;
        int n_el    = 0;
        int n_mu    = 0;
        int n_tau   = 0;
        int n_elnu  = 0;
        int n_munu  = 0;
        int n_taunu = 0;
        for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
                sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

            if( IN::gen_status->at(litr->second) != 1 ) continue;

            int momId = abs(IN::gen_motherPID->at(litr->second));
            int Id    = abs(IN::gen_PID      ->at(litr->second));

            if( ( Id == momId) || ( momId == 15 ) ) {

                if( Id == 11 ) n_el++;
                if( Id == 12 ) n_elnu++;
                if( Id == 13 ) n_mu++;
                if( Id == 14 ) n_munu++;
                if( Id == 15 ) n_tau++;
                if( Id == 16 ) n_taunu++;

            }
        }

        // if anything tau-like is present then its a tau decay
        if( n_taunu > 0 || n_tau > 0 ) {
            found_w_mother = true;
            OUT::isWTauDecay = true;
        }
        // otherwise the final state objects should be consistent with a W decay
        else if( n_munu > 0 && n_mu > 0 ) {
            OUT::isWMuDecay = true;
        }
        // otherwise the final state objects should be consistent with a W decay
        else if( n_elnu > 0 && n_el > 0 ) {
            OUT::isWElDecay = true;
        }
    }

    if( OUT::isWTauDecay ) {
        std::vector<unsigned> found_leptons;
        for( std::vector<std::pair<float, unsigned> >::const_iterator litr = 
                sorted_tleps.begin(); litr != sorted_tleps.end(); ++litr ) {

            if( IN::gen_status->at(litr->second) != 1 ) continue;

            int momId = abs(IN::gen_motherPID->at(litr->second));
            int Id    = abs(IN::gen_PID      ->at(litr->second));

            if( fabs(momId) == 15 && (fabs( Id ) == 11 || fabs(Id) == 13) ) {

                found_leptons.push_back( litr->second );
            }
        }

        if( found_leptons.size() > 0 ) {
            int lep_id = IN::gen_PID->at(found_leptons[0]);
            if( fabs(lep_id) == 11 ) {
                OUT::isWTauElDecay=true;
            }
            if( fabs(lep_id) == 13 ) {
                OUT::isWTauMuDecay =true;
            }
        }
    }
#endif

}

bool RunModule::FilterDataQuality( ModuleConfig & config) const {

    bool pass_quality = false;
    OUT::PassQuality = -1;

    bool isData = IN::isData;
    int run = IN::runNumber;
    int ls  = IN::lumiSection;

    if( _quality_map.size() > 0 && isData ) {

        std::map<int, std::vector<int> >::const_iterator mitr = _quality_map.find( run );
        if( mitr != _quality_map.end() ) {

            std::vector<int>::const_iterator vitr = std::find( mitr->second.begin(), mitr->second.end(), ls );

            if( vitr != mitr->second.end() ) {
                pass_quality = true;
                OUT::PassQuality = 1;
            }
            else {
                pass_quality = false;
                OUT::PassQuality = 0;
            }
        }
        else {
            pass_quality = false;
            OUT::PassQuality = 0;
        }
    }
    else {
        pass_quality = true;
    }

    return pass_quality;

}


bool RunModule::FilterBlind( ModuleConfig & config ) const {

    bool keep_event = true;

    bool pass_blind = true;
    if( OUT::ph_n > 0 ) {
        if( !config.PassFloat( "cut_ph_pt_lead", OUT::ph_pt->at(0)) ) pass_blind=false;
        if( !config.PassBool(  "cut_ph_CSEV", OUT::ph_passEleVeto->at(0) )  // Blinded when it is strictly in signal region defined by both kinds of vetos
            && !config.PassBool(  "cut_ph_Pixel", OUT::ph_hasPixSeed->at(0) ) 
            && !config.PassFloat(  "cut_met", OUT::met_pt ) )         pass_blind=false;
    }
    if( !config.PassFloat( "cut_mt_lep_met_ph", OUT::mt_lep_met_ph) ) pass_blind=false;
    if( !config.PassFloat( "cut_mt_res", OUT::mt_res) ) pass_blind=false;

    if( OUT::jet_n > 1 ) {
        if( !config.PassFloat( "cut_abs_dijet_m_from_z", fabs(OUT::leaddijet_m-_m_z)) ) pass_blind=false;
    }

    if( !pass_blind ) {
        OUT::isBlinded=true;
        if( OUT::isData ) keep_event=false;
    }
    else {
        OUT::isBlinded=false;
    }

    return keep_event;

}


bool RunModule::get_constriained_nu_pz( const TLorentzVector lepton, TLorentzVector &metlv ) const {

    float solved_pz = -1;

    bool desc_pos = calc_constrained_nu_momentum( lepton, metlv, solved_pz );
    if( desc_pos ) {
        metlv.SetXYZM( metlv.Px(), metlv.Py(), solved_pz, 0.0 );
    }
    else {
        //std::cout << "DISCRIMINANT IS NEGATIVE" << std::endl;
        // require the discriminant to be zero
        // solve a second quadratic equation to 
        // rescale MET so that there is a solution

        float alpha = ( lepton.Px()*metlv.Px() + lepton.Py()*metlv.Py() )/ metlv.Pt();
        float delta = ( _m_w*_m_w - lepton.M()*lepton.M() );

        float Aval = 4*lepton.Pz()*lepton.Pz() - 4*lepton.E()*lepton.E() +4*alpha*alpha;
        float Bval = 4*alpha*delta;
        float Cval = delta*delta;

        float solution1=-1;
        float solution2=-1;

        bool success2 = solve_quadratic( Aval, Bval, Cval, solution1, solution2 );

        if( !success2 ) {
            std::cout << "SECOND FAILURE" << std::endl;
        }

        float scale1 = solution1/metlv.Pt();
        float scale2 = solution2/metlv.Pt();

        TLorentzVector metlv_sol1;
        TLorentzVector metlv_sol2;
        metlv_sol1.SetPtEtaPhiM( OUT::met_pt*scale1, 0.0, OUT::met_phi, 0.0 );
        metlv_sol2.SetPtEtaPhiM( OUT::met_pt*scale2, 0.0, OUT::met_phi, 0.0 );

        float pz_sol1 = -1;
        float pz_sol2 = -1;
        bool success_sol1 = calc_constrained_nu_momentum( lepton, metlv_sol1, pz_sol1 );
        bool success_sol2 = calc_constrained_nu_momentum( lepton, metlv_sol2, pz_sol2 );

        if( !success_sol1 ) {
            //std::cout << "FAILURE SOLUTION 1" << std::endl;
            metlv.SetPtEtaPhiM(-1, 0, 0, 0);
            return false;
        }

        if( !success_sol2 ) {
            //std::cout << "FAILURE SOLUTION 2" << std::endl;
            metlv.SetPtEtaPhiM(-1, 0, 0, 0);
            return false;
        }

        TVector3 solved_met3v_sol1;
        TVector3 solved_met3v_sol2;
        solved_met3v_sol1.SetXYZ(metlv_sol1.Px(), metlv_sol1.Py(), pz_sol1);
        solved_met3v_sol2.SetXYZ(metlv_sol2.Px(), metlv_sol2.Py(), pz_sol2);
        TLorentzVector solved_metlv_sol1;
        TLorentzVector solved_metlv_sol2;
        solved_metlv_sol1.SetVectM( solved_met3v_sol1 , 0.0 );
        solved_metlv_sol2.SetVectM( solved_met3v_sol2 , 0.0 );

        float wmass_sol1 = ( lepton + solved_metlv_sol1 ).M();
        float wmass_sol2 = ( lepton + solved_metlv_sol2 ).M();

        if( fabs( wmass_sol1 - _m_w ) < fabs( wmass_sol2 - _m_w ) ) {
            solved_pz = pz_sol1;
            metlv = metlv_sol1;
        }
        else {
            solved_pz = pz_sol2;
            metlv = metlv_sol2;
        }
        
    }
    return desc_pos;
}

bool RunModule::calc_constrained_nu_momentum( const TLorentzVector lepton, const TLorentzVector met, float & result ) const {

   float little_a = _m_w*_m_w - lepton.M()*lepton.M() + 2*( lepton.Px()*met.Px() + lepton.Py()*met.Py() );

   float Aval = ( 4*lepton.E()*lepton.E() ) - ( 4*lepton.Pz()*lepton.Pz() );
   float Bval = -4 * little_a * lepton.Pz();

   float Cval = 4*lepton.E()*lepton.E()*met.Pt()*met.Pt() - little_a*little_a;

   float solution1=-1;
   float solution2=-1;
   bool success = solve_quadratic( Aval, Bval, Cval, solution1, solution2 );

   if ( success ) {
      if( fabs(solution1 - lepton.Pz() ) < fabs( solution2 - lepton.Pz() ) ) {
          result = solution1;
      }
      else {
          result = solution2;
      }
   }
   return success;
}

bool RunModule::solve_quadratic( float Aval, float Bval, float Cval, float & solution1, float &solution2 ) const {
   float discriminant = Bval*Bval - 4*Aval*Cval;

   //std::cout << "DISCRIMINANT = " << discriminant << std::endl;

   if ( discriminant >= 0 ) {
      solution1 = ( -1*Bval + sqrt( discriminant ) ) / ( 2 * Aval ) ; 
      solution2 = ( -1*Bval - sqrt( discriminant ) ) / ( 2 * Aval ) ; 
      return true;
   }
   else {
       return false;
   }
}
void RunModule::WeightEvent( ModuleConfig & config ) const {

#ifdef EXISTS_EventWeights

    OUT::NLOWeight = 1.0;
    if( !IN::isData && _needs_nlo_weght ) {
        if( IN::EventWeights->at(0) < 0 ) {
            OUT::NLOWeight = -1.0;
        }
    }
    if (IN::EventWeights->size()>=10){
      OUT::QCDScaleWeights->clear();
      /* MadGraphChargedResonance weights
        *        0 *              Central scale variation: mur=1 muf=1  *
        *        1 *              Central scale variation: mur=1 muf=2  *
        *        2 *            Central scale variation: mur=1 muf=0.5  *
        *        3 *              Central scale variation: mur=2 muf=1  *
        *        4 *              Central scale variation: mur=2 muf=2  *
        *        5 *            Central scale variation: mur=2 muf=0.5  * <- discard
        *        6 *            Central scale variation: mur=0.5 muf=1  *
        *        7 *            Central scale variation: mur=0.5 muf=2  * <- discard
        *        8 *          Central scale variation: mur=0.5 muf=0.5  *
        *        9 *                 NNPDF30_lo_as_0130.LHgrid:Member 0 * <- 100 NNPDF replicas
        ...
        *      109 *               NNPDF30_lo_as_0130.LHgrid:Member 100 *
      */
      for ( int i : { 1, 2, 3, 4, 6, 8 } ){
        if (printevent) std::cout<<i<<"  "
        <<(IN::EventWeights->at(i)/IN::EventWeights->at(0)/_pdfweight_sample_hist->GetBinContent(i+1))<<" "
        <<IN::EventWeights->at(i)/IN::EventWeights->at(0)<<"  "<<_pdfweight_sample_hist->GetBinContent(i+1)<<std::endl;
        OUT::QCDScaleWeights->push_back(IN::EventWeights->at(i)/IN::EventWeights->at(0)/_pdfweight_sample_hist->GetBinContent(i+1));
      }
    }
    if (IN::EventWeights->size()>=110){
      OUT::PDFWeights->clear();
      for ( int i = 9; i < 110; ++i ){
        if (printevent) std::cout<<i<<"  "
        <<(IN::EventWeights->at(i)/IN::EventWeights->at(0)/_pdfweight_sample_hist->GetBinContent(i+1))<<" "
        <<IN::EventWeights->at(i)/IN::EventWeights->at(0)<<"  "<<_pdfweight_sample_hist->GetBinContent(i+1)<<std::endl;
        OUT::PDFWeights->push_back(IN::EventWeights->at(i)/IN::EventWeights->at(0)/_pdfweight_sample_hist->GetBinContent(i+1));
      }
    }

#endif 
#ifdef EXISTS_truepu_n
    if( !_puweight_data_hist || !_puweight_sample_hist ) {
        OUT::PUWeight = 1.0;
        return;
    }
    float puval = -1;
    puval = IN::truepu_n;
    //#ifdef EXISTS_nVtx
    //puval = IN::nVtx;
    //#endif

    OUT::PUWeight = calc_pu_weight( puval );

    OUT::PUWeightUP5 = calc_pu_weight( puval, 1.05 );
    OUT::PUWeightUP10 = calc_pu_weight( puval, 1.10 );

    OUT::PUWeightDN5 = calc_pu_weight( puval, 0.95 );
    OUT::PUWeightDN10 = calc_pu_weight( puval, 0.9 );

#endif
    
}

float RunModule::calc_pu_weight( float puval, float mod ) const {

    float tot_data   = _puweight_data_hist->Integral();
    float tot_sample = _puweight_sample_hist->Integral();

    int bin_sample = _puweight_sample_hist->FindBin(puval);
    int bin_data = _puweight_data_hist->FindBin(mod*puval);

    float val_data = _puweight_data_hist->GetBinContent( bin_data );
    float val_sample = _puweight_sample_hist->GetBinContent( bin_sample );

    float num = val_data*mod/tot_data;
    float den = val_sample/tot_sample;

    float weight = num/den;

    if ( weight < 0.00000005 || den==0 || printevent ) std::cout << "puval = " << puval << " data_val = " << val_data << " sample_val = " << val_sample << " num = " << num << " den = " << den << " weight = " << weight << std::endl;

    if( weight < 0.00000005 || den == 0) {

        int bin_min_sample = _puweight_sample_hist->FindBin(puval-2.5);
        int bin_max_sample = _puweight_sample_hist->FindBin(puval+2.5);
        int bin_min_data = _puweight_data_hist->FindBin(puval*mod-2.5);
        int bin_max_data = _puweight_data_hist->FindBin(puval*mod+2.5);

        val_data = _puweight_data_hist->Integral(bin_min_data, bin_max_data);
        val_sample = _puweight_sample_hist->Integral(bin_min_sample, bin_max_sample);

        num = val_data/tot_data;
        den = val_sample/tot_sample;

        weight = num/den;
        std::cout << "corrected puval = " << puval << " data_val = " << val_data << " sample_val = " << val_sample << " num = " << num << " den = " << den << " weight = " << weight << std::endl;

    }
    return weight;
}



bool RunModule::HasTruthMatch( const TLorentzVector & objlv, const std::vector<int> & matchPID, float maxDR ) const {
    
    float minDR = 100.0;
    TLorentzVector matchLV;
    return HasTruthMatch( objlv, matchPID, maxDR, minDR, matchLV );

}

bool RunModule::HasTruthMatch( const TLorentzVector & objlv, const std::vector<int> & matchPID, float maxDR, float &minDR ) const {
    
    TLorentzVector matchLV;
    return HasTruthMatch( objlv, matchPID, maxDR, minDR, matchLV );

}

bool RunModule::HasTruthMatch( const TLorentzVector & objlv, const std::vector<int> & matchPID, float maxDR, float &minDR, TLorentzVector &matchLV ) const {
    
    int motherPID = 0;
    int parentage = -1;
    return HasTruthMatch( objlv, matchPID, maxDR, minDR, matchLV, motherPID, parentage );

}

bool RunModule::HasTruthMatch( const TLorentzVector & objlv, const std::vector<int> & matchPID, float maxDR, float & minDR, TLorentzVector & matchLV, int &matchMotherPID, int &matchParentage  ) const {
   
    minDR = 100.0;
    matchLV.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    bool match=false;
#ifdef EXISTS_Gen_pt

    // store commom formats for all
    // information 
    std::vector<int> match_pid;
    std::vector<TLorentzVector> match_tlv;
    std::vector<int> match_momPID;
    std::vector<int> match_parentage;
    BOOST_FOREACH( int mpid, matchPID ) {

        // use photons
        if( mpid == 22 ) {

            for( int gidx = 0; gidx < OUT::trueph_n; ++gidx ) {

                TLorentzVector mclv;
                mclv.SetPtEtaPhiM( OUT::trueph_pt->at(gidx), 
                                   OUT::trueph_eta->at(gidx), 
                                   OUT::trueph_phi->at(gidx), 
                                   0. );

                match_pid.push_back( mpid );
                match_tlv.push_back( mclv );
                match_momPID.push_back( OUT::trueph_motherPID->at(gidx) );
                match_parentage.push_back( 0 );
            }
        }
        else if( mpid==11 || mpid==13 || mpid==-11 || mpid==-13 ) {
            for( int lidx = 0; lidx < OUT::truelep_n; ++lidx ) {

                // only use this lepton type
                if( mpid != OUT::truelep_PID->at(lidx) ) continue;

                TLorentzVector mclv;
                mclv.SetPtEtaPhiE( OUT::truelep_pt->at(lidx), 
                                   OUT::truelep_eta->at(lidx), 
                                   OUT::truelep_phi->at(lidx), 
                                   OUT::truelep_e->at(lidx) );

                match_pid.push_back( mpid );
                match_tlv.push_back( mclv );
                match_momPID.push_back( OUT::truelep_motherPID->at(lidx) );
                match_parentage.push_back( 0 );
            }
        }
        //else {
        //    for( unsigned lidx = 0; lidx < IN::GJetAk04Pt->size(); ++lidx ) {
        //        TLorentzVector mclv;
        //        mclv.SetPtEtaPhiE( IN::GJetAk04Pt->at(lidx), IN::GJetAk04Eta->at(lidx), IN::GJetAk04Phi->at(lidx), IN::GJetAk04E->at(lidx) );

        //        match_pid.push_back( mpid );
        //        match_tlv.push_back( mclv );
        //        // FIX should be filled with the originator quark/gluon
        //        match_momPID.push_back( 1 );
        //        match_parentage.push_back( 0 );
        //    }
        //}


    }

    for( unsigned idx = 0; idx < match_tlv.size(); ++idx ) {

        float dr = match_tlv[idx].DeltaR( objlv );
        //std::cout << "dr = " << dr << std::endl;
        if( dr < maxDR) {
            match = true;
            matchMotherPID = match_momPID[idx];
            matchParentage = match_parentage[idx]; 
        }
        // store the minimum delta R
        if( dr < minDR ) {
            minDR = dr;
            matchLV = match_tlv[idx];
        }
    }

#endif
    return match;

}


RunModule::RunModule() {
    _m_w = 80.385;
    _m_z = 91.2;
    _year = 0;
    _isData = false;
    _filterevent_cutflow = false;
    passtrigger = true;
    _eval_mu_loose    =false;
    _eval_mu_medium   =false;
    _eval_mu_tight    =false;
    _eval_ph_tight    =false;
    _eval_ph_medium   =false;
    _eval_ph_loose    =false;
    _eval_btag_tight  =false;
    _eval_btag_medium =false;
    _eval_btag_loose  =false;
    _eval_el_tight    =false;
    _eval_el_medium   =false;
    _eval_el_loose    =false;
    _eval_el_veryloose=false;
    _needs_nlo_weght  =false;
}
