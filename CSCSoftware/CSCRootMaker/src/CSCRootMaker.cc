// -*- C++ -*-
//
// Package:    CSCRootMaker
// Class:      CSCRootMaker
// 
//
// Description: This edAnalyzer makes root tuples to be used in the CSC analysis
//
//
//
// Original Author:  Matthew Snowball
//         Created:  Tue Jun 18 10:26:09 EDT 2013
//
// Last Updated: Apr. 30, 2014
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigi.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutRecord.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "EventFilter/CSCRawToDigi/interface/CSCDCCEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDCCExaminer.h"
#include "EventFilter/CSCRawToDigi/interface/CSCEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCALCTHeader.h"
#include "EventFilter/CSCRawToDigi/interface/CSCAnodeData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCLCTData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCTMBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCTMBHeader.h"
#include "EventFilter/CSCRawToDigi/interface/CSCRPCData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDCCExaminer.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDCCEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBTimeSlice.h"
#include "CondFormats/CSCObjects/interface/CSCCrateMap.h"
#include "CondFormats/DataRecord/interface/CSCCrateMapRcd.h"
#include <EventFilter/CSCRawToDigi/interface/CSCMonitorInterface.h>
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Common/interface/TriggerNames.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
//#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/TrackingTools/interface/SegmentsTrackAssociator.h"


#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "DataFormats/MuonDetId/interface/CSCIndexer.h"

#include "CondFormats/CSCObjects/interface/CSCDBGains.h"
#include "CondFormats/DataRecord/interface/CSCDBGainsRcd.h"
#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCDBNoiseMatrixRcd.h"
#include "CondFormats/CSCObjects/interface/CSCDBCrosstalk.h"
#include "CondFormats/DataRecord/interface/CSCDBCrosstalkRcd.h"
#include "CondFormats/CSCObjects/interface/CSCDBPedestals.h"
#include "CondFormats/DataRecord/interface/CSCDBPedestalsRcd.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include "RecoLocalMuon/CSCValidation/src/CSCValHists.h"

#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
//#include "RecoLuminosity/LumiProducer/interface/LumiCorrectionParam.h"

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

using namespace std;

//
// class declaration
//

class CSCRootMaker : public edm::EDAnalyzer {
public:
  explicit CSCRootMaker(const edm::ParameterSet&);
  ~CSCRootMaker();
  
//  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

//  void doMuons(edm::Handle<reco::Muon> muons, edm::Handle<reco::TrackCollection> saMuons, edm::Handle<CSCSegmentCollection> cscSegments, edm::Handle<CSCRecHit2DCollection> recHits,
 //              const reco::Vertex *&PV, const edm::Event& iEvent, const edm::EventSetup& iSetup, edm::ESHandle<GlobalTrackingGeometry> theGeom, edm::ESHandle<CSCGeometry> cscGeom);

  void doMuons(edm::Handle<reco::MuonCollection> muons, edm::Handle<reco::TrackCollection> saMuons, edm::Handle<CSCSegmentCollection> cscSegments, edm::Handle<CSCRecHit2DCollection> recHits,
	       const reco::Vertex *&PV, const edm::Event& iEvent, const edm::EventSetup& iSetup, edm::ESHandle<GlobalTrackingGeometry> theGeom, edm::ESHandle<CSCGeometry> cscGeom);
  void doTracks(edm::Handle<reco::TrackCollection> genTracks);
  void doRecHits(edm::Handle<CSCRecHit2DCollection> recHits, edm::Handle<edm::PSimHitContainer> simHits, edm::Handle<reco::TrackCollection> saMuons, 
		 edm::Handle<reco::MuonCollection> muons, edm::ESHandle<CSCGeometry> cscGeom, const edm::Event& iEvent);
  double getthisSignal(const CSCStripDigiCollection& stripdigis, CSCDetId idRH, int centerStrip);
  void doSegments(edm::Handle<CSCSegmentCollection> cscSegments, edm::ESHandle<CSCGeometry> cscGeom);
  void doTrigger(edm::Handle<L1MuGMTReadoutCollection> pCollection, edm::Handle<edm::TriggerResults> hlt);
  void doStripDigis(edm::Handle<CSCStripDigiCollection> strips, edm::ESHandle<CSCGeometry> cscGeom);
  void doWireDigis(edm::Handle<CSCWireDigiCollection> wires, edm::ESHandle<CSCGeometry> cscGeom);
  void doCompTiming(const CSCComparatorDigiCollection& compars);
  void doLCTDigis(edm::Handle<CSCALCTDigiCollection> alcts, edm::Handle<CSCCLCTDigiCollection> clcts,
		  edm::Handle<CSCCorrelatedLCTDigiCollection> correlatedlcts,
		  edm::Handle<L1MuGMTReadoutCollection> pCollection, edm::ESHandle<CSCGeometry> cscGeom, 
		  const edm::EventSetup& eventSetup, const edm::Event &event);
  void doCalibrations(const edm::EventSetup& eventSetup);
  double fitX(CLHEP::HepMatrix points, CLHEP::HepMatrix errors);
  void doNonAssociatedRecHits(edm::Handle<CSCSegmentCollection> cscSegments, edm::ESHandle<CSCGeometry> cscGeom,  edm::Handle<CSCStripDigiCollection> strips);
  int chamberSerial( CSCDetId id );
  int ringSerial( CSCDetId id );
  int getWidth(const CSCStripDigiCollection& stripdigis, CSCDetId idRH, int centerStrip);
  void doGasGain(const CSCWireDigiCollection& wirecltn,  const CSCStripDigiCollection&   strpcltn, const CSCRecHit2DCollection& rechitcltn);  
  bool withinSensitiveRegion(LocalPoint localPos, const std::array<const double, 4> & layerBounds, int station, int ring, double shiftFromEdge, double shiftFromDeadZone);

  std::vector<CSCSegment> findMuonSegments(edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry, const reco::Track& Track, 
					   edm::Handle<CSCSegmentCollection> cscSegments, edm::Handle<CSCRecHit2DCollection> recHits, 
					   edm::ESHandle<CSCGeometry> cscGeom);
  

  // register to the TFileService 
  edm::Service<TFileService> fs;  
  std::map<std::string,TH1F*> histContainer_;

  //---- Functions ----//
  void bookTree(TTree *tree);

  //---- Variables ----//
  edm::EDGetTokenT<reco::MuonCollection> muonSrc;
//  edm::InputTag muonSrc;
  edm::EDGetTokenT<reco::VertexCollection> vertexSrc;
  edm::EDGetTokenT<reco::TrackCollection> standAloneMuonsSrc;
  edm::EDGetTokenT<CSCRecHit2DCollection> cscRecHitTagSrc;
  edm::EDGetTokenT<CSCSegmentCollection> cscSegTagSrc;
  edm::EDGetTokenT<L1MuGMTReadoutCollection> level1TagSrc;
  edm::EDGetTokenT<edm::TriggerResults> hltTagSrc;
  edm::EDGetTokenT<CSCWireDigiCollection> wireDigiTagSrc;
  edm::EDGetTokenT<CSCStripDigiCollection> stripDigiTagSrc;
  edm::EDGetTokenT<CSCComparatorDigiCollection> compDigiTagSrc;
  edm::EDGetTokenT<CSCALCTDigiCollection> alctDigiTagSrc;
  edm::EDGetTokenT<CSCCLCTDigiCollection> clctDigiTagSrc;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> corrlctDigiTagSrc;
  edm::EDGetTokenT<edm::PSimHitContainer> simHitTagSrc;
  edm::EDGetTokenT<FEDRawDataCollection> fedRawTagSrc;

  SegmentsTrackAssociator* theSegmentsAssociator;
  edm::ParameterSet parameters;

  bool isFullRECO, isLocalRECO, isGEN, isSIM, isRAW, isDIGI, isDATA;
  bool addMuons, addTracks, addRecHits, addSegments, addTrigger, addDigis, addTimeMonitoring;
  bool addCalibrations;

  TTree *tree;
  ULong64_t Run, Event, LumiSect;
  int BunchCrossing;
  int nEventsTotal;
  unsigned int timeSecond;

  // Luminosity
/*
  double avgInstantLumi;
  double rawbxlumi;
  double correctedAvgInstantLumi;
  double bx_B1[3564];
  double bx_B2[3564];
  double bx_LUMI[3564];  
*/

  int counter;

  // Struct for map
  struct ltrh
  {
    bool operator()(const CSCRecHit2D rh1, const CSCRecHit2D rh2) const
    {
      return ((rh1.localPosition()).x()-(rh2.localPosition()).x()) < 0;
    }
  };
  

  // Global Maps
  std::multimap<CSCDetId , CSCRecHit2D> AllRechits;
  std::multimap<CSCDetId , CSCRecHit2D> SegRechits;
  std::multimap<CSCDetId , CSCRecHit2D> NonAssociatedRechits;
  //std::map<CSCRecHit2D , double,ltrh> distRHmap;
  std::map<int, int>   m_single_wire_layer;
  std::map<int, std::vector<int> >   m_wire_hvsegm;
  std::vector<int>     nmbhvsegm;
  std::vector<double> distRHvec;

  //Vertices
  int vertex_nVertex;
  
  // Muons
  int       muons_nMuons;
  bool      muons_isStandAloneMuon[1000], muons_isGlobalMuon[1000], muons_isPFMuon[1000], muons_isCaloMuon[1000], muons_isTrackerMuon[1000];
  bool      muons_isEnergyValid[1000];
  int       muons_numberOfChambers[1000], muons_numberOfMatches[1000], muons_numberOfSegments[1000];
  double    muons_calEnergyTower[1000], muons_calEnergyEm[1000], muons_calEnergyHad[1000];
  int       muons_charge[1000], muons_nRecHits[1000];
  double    muons_energy[1000],  muons_px[1000], muons_py[1000], muons_pz[1000], muons_pt[1000];
  double    muons_et[1000], muons_p[1000], muons_phi[1000], muons_eta[1000], muons_theta[1000];
  double    muons_vx[1000], muons_vy[1000], muons_vz[1000];
  double    muons_globalTrackNormalizedChi2[1000];
  int       muons_globalTrackNumberOfValidMuonHits[1000], muons_trackNumberOfValidHits[1000], muons_trackNumberOfLostHits[1000];  
  double    muons_isoNH04[1000], muons_isoCH04[1000], muons_isoPhot04[1000], muons_isoPU04[1000];
  double    muons_isoNH03[1000], muons_isoCH03[1000], muons_isoPhot03[1000], muons_isoPU03[1000];
  double    muons_dxy[1000], muons_dz[1000];
  std::vector< std::vector<double> > muons_cscSegmentRecord_nRecHits;
  std::vector< std::vector<double> > muons_cscSegmentRecord_ring;
  std::vector< std::vector<double> > muons_cscSegmentRecord_station;
  std::vector< std::vector<double> > muons_cscSegmentRecord_chamber;
  std::vector< std::vector<double> > muons_cscSegmentRecord_endcap;
  std::vector< std::vector<double> > muons_cscSegmentRecord_localY;
  std::vector< std::vector<double> > muons_cscSegmentRecord_localX;


  // Tracks
  int       tracks_nTracks;
  int       tracks_charge[2000];
  double    tracks_px[2000], tracks_py[2000], tracks_pz[2000];
  double    tracks_pt[2000], tracks_p[2000], tracks_eta[2000], tracks_phi[2000];

  // SimHits
  int simHits_nSimHits;
  int simHits_particleType[10000];
  double simHits_localX[10000], simHits_localY[10000], simHits_globalX[10000], simHits_globalY[10000];
  int    simHits_ID_endcap[10000], simHits_ID_ring[10000], simHits_ID_station[10000], simHits_ID_chamber[10000], simHits_ID_layer[10000];
  int    simHits_ID_chamberSerial[10000], simHits_ID_ringSerial[10000], simHits_ID_processType[10000];
  double simHits_momentum[10000], simHits_phi[10000], simHits_theta[10000];

  // CSCRecHits2D
  int       recHits2D_nRecHits2D;
  int       recHits2D_ID_endcap[10000], recHits2D_ID_ring[10000], recHits2D_ID_station[10000], recHits2D_ID_chamber[10000], recHits2D_ID_layer[10000];
  double    recHits2D_localX[10000], recHits2D_localY[10000];
  double    recHits2D_localXXerr[10000], recHits2D_localYYerr[10000], recHits2D_localXYerr[10000];
  double    recHits2D_stripPosition[10000], recHits2D_stripError[10000];
  double    recHits2D_SumQ[10000], recHits2D_SumQSides[10000];
  double    recHits2D_Time[10000];
  double    recHits2D_globalX[10000], recHits2D_globalY[10000], recHits2D_ADCSignal[10000];
  int       recHits2D_belongsToSaMuon[10000], recHits2D_belongsToMuon[10000];
  int       recHits2D_ID_chamberSerial[10000], recHits2D_ID_ringSerial[10000];
  double    recHits2D_simHit_localX[10000], recHits2D_simHit_localY[10000];
  int       recHits2D_simHit_particleTypeID[10000], recHits2D_nearestStrip[10000], recHits2D_nearestWire[10000], recHits2D_nearestWireGroup[10000];
  double    recHits2D_localStripWireIntersectionX[10000], recHits2D_localStripWireIntersectionY[10000], recHits2D_localStripWireGroupIntersectionX[10000];
  double    recHits2D_localStripWireGroupIntersectionY[10000], recHits2D_stripWidthAtHit[10000];
  double    recHits2D_positionWithinStrip[10000], recHits2D_wireTime[10000];
  int       recHits2D_hitWire[10000], recHits2D_wgroupsBX[10000], recHits2D_nWireGroups[10000];

  
  // CSCSegments
  int       cscSegments_nSegments;
  double    cscSegments_localX[10000], cscSegments_localY[10000], cscSegments_globalX[10000], cscSegments_globalY[10000];
  double    cscSegments_globalTheta[10000], cscSegments_globalPhi[10000];
  double    cscSegments_localTheta[10000], cscSegments_chi2[10000], cscSegments_nRecHits[10000];
  int       cscSegments_nDOF[10000];
  int       cscSegments_ID_endcap[10000], cscSegments_ID_ring[10000], cscSegments_ID_station[10000], cscSegments_ID_chamber[10000];
  double    cscSegments_distToIP[10000], cscSegments_segmentTime[10000];
  std::vector< std::vector<double> > cscSegments_recHitRecord_endcap;
  std::vector< std::vector<double> > cscSegments_recHitRecord_ring;
  std::vector< std::vector<double> > cscSegments_recHitRecord_station;
  std::vector< std::vector<double> > cscSegments_recHitRecord_chamber;
  std::vector< std::vector<double> > cscSegments_recHitRecord_layer;
  std::vector< std::vector<double> > cscSegments_recHitRecord_localY;
  std::vector< std::vector<double> > cscSegments_recHitRecord_localX;
  int       cscSegments_ID_chamberSerial[10000], cscSegments_ID_ringSerial[10000];
  double    cscSegments_Resolution_pull[10000], cscSegments_Resolution_residual[10000];

  // L1 GMT
  bool l1Trigger_CSC, l1Trigger_DT, l1Trigger_RPCForward, l1Trigger_RPCBarrel, l1Trigger_beamHalo;
  int l1GMT_BXN;
  
  // HLT Trigger
  int hltTrigger_nBits;
  int hltTrigger_bits[10000];

  // Standalone Muons
  int standaloneMuons_nMuons;
  double standaloneMuons_p[10000], standaloneMuons_pt[10000];
  int standaloneMuons_nRecHits[10000];
  double standaloneMuons_chi2[10000], standaloneMuons_normChi2[10000];
  int standaloneMuons_nDTHits[10000], standaloneMuons_nCSCHits[10000], standaloneMuons_nCSCHitsP[10000];
  int standaloneMuons_nCSCHitsM[10000], standaloneMuons_nRPCHits[10000], standaloneMuons_nRPCHitsP[10000];
  int standaloneMuons_nRPCHitsM[10000], standaloneMuons_nHitsP[10000], standaloneMuons_nHitsM[10000];
  double standaloneMuons_crudeLength[10000], standaloneMuons_deltaPhi[10000];
  double standaloneMuons_innerGlobalPolarAngle[10000], standaloneMuons_outerGlobalPolarAngle[10000];

  // Strip Digis
  int firedStripDigis_nStripDigis;
  int firedStripDigis_ID_endcap[10000], firedStripDigis_ID_station[10000], firedStripDigis_ID_layer[10000];
  int firedStripDigis_ID_chamber[10000], firedStripDigis_ID_strip[10000], firedStripDigis_ID_ring[10000];
  int firedStripDigis_tbinMax[10000];
  double firedStripDigis_ADCTotal[10000], firedStripDigis_ADCMax[10000], firedStripDigis_localX[10000];

  // Wire Digis
  int firedWireDigis_nWireDigis;
  int firedWireDigis_ID_endcap[10000], firedWireDigis_ID_station[10000], firedWireDigis_ID_layer[10000], firedWireDigis_chamberSerial[10000];
  int firedWireDigis_ID_chamber[10000], firedWireDigis_ID_wire[10000], firedWireDigis_timeBin[10000], firedWireDigis_ID_ring[10000];
  int firedWireDigis_AFEB[10000], firedWireDigis_numberWireTimeBins[10000];
  double firedWireDigis_localY[10000];

  // Comparator Digis
  int comparatorDigis_nDigis;
  int comparatorDigis_ID_endcap[10000], comparatorDigis_ID_station[10000], comparatorDigis_ID_layer[10000], comparatorDigis_cfeb[10000];
  int comparatorDigis_ID_chamber[10000], comparatorDigis_ID_strip[10000], comparatorDigis_ID_halfStrip[10000], comparatorDigis_timeBin[10000], comparatorDigis_ID_ring[10000];

  // ALCTs
  int alct_nAlcts;
  int alct_BX[10000], alct_fullBX[10000], alct_ID_chamber[10000], alct_ID_endcap[10000], alct_ID_station[10000];
  int alct_ID_ring[10000], alct_ID_layer[10000], alct_ID_chamberID[10000], alct_ID_chamberSerial[10000], alct_ID_ringSerial[10000];

  // CLCTs
  int clct_nClcts;
  int clct_BX[10000], clct_fullBX[10000], clct_ID_chamber[10000], clct_ID_endcap[10000], clct_ID_station[10000];
  int clct_ID_ring[10000], clct_ID_layer[10000], clct_ID_chamberID[10000], clct_ID_chamberSerial[10000], clct_ID_ringSerial[10000];

  // Correlated LCTs
  int correlatedLct_nLcts;
  int correlatedLct_BX[10000], correlatedLct_trkNumber[10000], correlatedLct_quality[10000], correlatedLct_keyWG[10000];
  int correlatedLct_strip[10000], correlatedLct_pattern[10000], correlatedLct_bend[10000], correlatedLct_CLCTPattern[10000];
  int correlatedLct_ID_chamber[10000], correlatedLct_ID_ring[10000], correlatedLct_ID_station[10000];
  int correlatedLct_ID_endcap[10000], correlatedLct_ID_layer[10000], correlatedLct_ID_chamberSerial[10000], correlatedLct_ID_ringSerial[10000];
  unsigned int correlatedLct_cscID[10000], correlatedLct_BX0[10000], correlatedLct_syncErr[10000];

  // TMB
  int tmb_nTmb;
  int tmb_BXNCount[10000], tmb_ALCTMatchTime[10000], tmb_ID_chamberID[10000], tmb_ID_chamber[10000], tmb_ID_endcap[10000];
  int tmb_ID_station[10000], tmb_ID_ring[10000], tmb_ID_layer[10000], tmb_ID_chamberSerial[10000], tmb_ID_ringSerial[10000];
  int tmb_alct0key[10000], tmb_alctRelL1A[10000];

  // Calibrations
  int calibrations_nCalib;
  double calibrations_Gain_slope[400], calibrations_XT_slope_left[400], calibrations_XT_slope_right[400];
  double calibrations_XT_intercept_left[400], calibrations_XT_intercept_right[400], calibrations_Pedestals_ped[400];
  double calibrations_Pedestals_rms[400], calibrations_NoiseMatrix_33[400], calibrations_NoiseMatrix_34[400];
  double calibrations_NoiseMatrix_35[400], calibrations_NoiseMatrix_44[400], calibrations_NoiseMatrix_45[400];
  double calibrations_NoiseMatrix_46[400], calibrations_NoiseMatrix_55[400], calibrations_NoiseMatrix_56[400];
  double calibrations_NoiseMatrix_57[400], calibrations_NoiseMatrix_66[400], calibrations_NoiseMatrix_67[400];
  double calibrations_NoiseMatrix_77[400];

  // Non Associated RecHits
  int nonAssocRecHits_nNonAssocRH;
  int nonAssocRecHits_codeBroad[10000], nonAssocRecHits_codeNarrow[10000], nonAssocRecHits_ID_layer[10000];
  int nonAssocRecHits_ID_station[10000], nonAssocRecHits_ID_ring[10000], nonAssocRecHits_ID_chamber[10000];
  int nonAssocRecHits_ID_endcap[10000], nonAssocRecHits_ID_centerStrip[10000], nonAssocRecHits_width[10000];
  double nonAssocRecHits_globalX[10000], nonAssocRecHits_globalY[10000], nonAssocRecHits_localX[10000];
  double nonAssocRecHits_localY[10000], nonAssocRecHits_sumQ[10000], nonAssocRecHits_ratioSumQ[10000];
  double nonAssocRecHits_Time[10000],  nonAssocRecHits_distToGoodRH[10000];

  // Associated RecHits
  int assocRecHits_nAssocRH;
  int assocRecHits_codeBroad[10000], assocRecHits_codeNarrow[10000], assocRecHits_ID_layer[10000];
  int assocRecHits_ID_station[10000], assocRecHits_ID_ring[10000], assocRecHits_ID_chamber[10000];
  int assocRecHits_ID_endcap[10000], assocRecHits_ID_centerStrip[10000], assocRecHits_width[10000];
  double assocRecHits_globalX[10000], assocRecHits_globalY[10000], assocRecHits_localX[10000];
  double assocRecHits_localY[10000], assocRecHits_sumQ[10000], assocRecHits_ratioSumQ[10000];
  double assocRecHits_Time[10000];

  // Gas Gain
  int gasGain_nGasGain;
  int gasGain_chamberType[10000], gasGain_HVSegNumber[10000], gasGain_NmbHVSegments[10000];
  int gasGain_location[10000], gasGain_chamber[10000], gasGain_ring[10000], gasGain_station[10000];
  int gasGain_endcap[10000], gasGain_layer[10000]; 
  double gasGain_ADC3x3Sum[10000];

};


//Constructor
CSCRootMaker::CSCRootMaker(const edm::ParameterSet& iConfig) :
  histContainer_(),
  muonSrc(consumes<reco::MuonCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc"))),

//  muonSrc(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  vertexSrc(consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc"))),
  standAloneMuonsSrc(consumes<reco::TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("standAloneMuonsSrc"))),
  cscRecHitTagSrc(consumes<CSCRecHit2DCollection>(iConfig.getUntrackedParameter<edm::InputTag>("cscRecHitTagSrc"))),
  cscSegTagSrc(consumes<CSCSegmentCollection>(iConfig.getUntrackedParameter<edm::InputTag>("cscSegTagSrc"))),
  level1TagSrc(consumes<L1MuGMTReadoutCollection>(iConfig.getUntrackedParameter<edm::InputTag>("level1TagSrc"))),
  hltTagSrc(consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("hltTagSrc"))),
  wireDigiTagSrc(consumes<CSCWireDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("wireDigiTagSrc"))),
  stripDigiTagSrc(consumes<CSCStripDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("stripDigiTagSrc"))),
  compDigiTagSrc(consumes<CSCComparatorDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("compDigiTagSrc"))),
  alctDigiTagSrc(consumes<CSCALCTDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("alctDigiTagSrc"))),
  clctDigiTagSrc(consumes<CSCCLCTDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("clctDigiTagSrc"))),
  corrlctDigiTagSrc(consumes<CSCCorrelatedLCTDigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("corrlctDigiTagSrc"))),
  simHitTagSrc(consumes<edm::PSimHitContainer>(iConfig.getUntrackedParameter<edm::InputTag>("simHitTagSrc"))),
  fedRawTagSrc(consumes<FEDRawDataCollection>(iConfig.getUntrackedParameter<edm::InputTag>("fedRawTagSrc"))),

  isFullRECO(iConfig.getUntrackedParameter<bool>("isFullRECO",false)),
  isLocalRECO(iConfig.getUntrackedParameter<bool>("isLocalRECO",false)),
  isGEN(iConfig.getUntrackedParameter<bool>("isGEN",false)),
  isSIM(iConfig.getUntrackedParameter<bool>("isSIM",false)),
  isRAW(iConfig.getUntrackedParameter<bool>("isRAW",false)),
  isDIGI(iConfig.getUntrackedParameter<bool>("isDIGI",false)),
  isDATA(iConfig.getUntrackedParameter<bool>("isDATA",false)),
  addMuons(iConfig.getUntrackedParameter<bool>("addMuons",false)),
  addTracks(iConfig.getUntrackedParameter<bool>("addTracks",true)),
  addRecHits(iConfig.getUntrackedParameter<bool>("addRecHits",true)),
  addSegments(iConfig.getUntrackedParameter<bool>("addSegments",true)),
  addTrigger(iConfig.getUntrackedParameter<bool>("addTrigger",true)),
  addDigis(iConfig.getUntrackedParameter<bool>("addDigis",false)),
  addTimeMonitoring(iConfig.getUntrackedParameter<bool>("addTimeMonitoring",false)),
  addCalibrations(iConfig.getUntrackedParameter<bool>("addCalibrations",false))
{
  histContainer_["NEVENTS"]=fs->make<TH1F>("nEvents","nEvents in Sample",2,0,2);
  nEventsTotal = 0;
  counter = 0;

  tree = new TTree("Events","Events");
  

}


//Destructor
CSCRootMaker::~CSCRootMaker()
{
   // do anything here that needs to be done at desctruction time

}



// ------------ method called for each event  ------------
void CSCRootMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   /// Time in seconds since January 1, 1970.
   timeSecond = iEvent.time().unixTime();

   //Luminosity
//   edm::Handle<LumiDetails> LumiDet;
//   if(isFullRECO && isDATA) iEvent.getLuminosityBlock().getByToken("lumiProducer",LumiDet); 

   //general tracks
//   edm::Handle<reco::TrackCollection> genTracks;
//   if(isFullRECO) iEvent.getByToken("generalTracks",genTracks);

   //muons
   edm::Handle<reco::MuonCollection> muons;
//   edm::Handle<reco::Muon> muons;   
   if(isFullRECO) iEvent.getByToken(muonSrc,muons);


   //vertex
   const reco::Vertex *PV = 0;
   edm::Handle<reco::VertexCollection> vertex;
   if(isFullRECO) 
     {
       iEvent.getByToken(vertexSrc,vertex);
       if(!vertex->empty() && vertex->size() > 0) PV = &(vertex->at(0));
       vertex_nVertex = (int) vertex->size();
     }


   // get the standalone muon collection
   edm::Handle<reco::TrackCollection> saMuons;
   if(isFullRECO) iEvent.getByToken(standAloneMuonsSrc,saMuons);


   edm::ESHandle<CSCGeometry> cscGeom;
   iSetup.get<MuonGeometryRecord>().get(cscGeom);   

   edm::ESHandle<GlobalTrackingGeometry> geometry_;
   iSetup.get<GlobalTrackingGeometryRecord>().get(geometry_);
   
   edm::Handle<CSCRecHit2DCollection> recHits;
   if(isLocalRECO || isFullRECO) iEvent.getByToken(cscRecHitTagSrc,recHits);

   // get CSC segment collection
   edm::Handle<CSCSegmentCollection> cscSegments;
   if(isLocalRECO || isFullRECO) iEvent.getByToken(cscSegTagSrc, cscSegments);
 
   // get the trigger collection
   edm::Handle<L1MuGMTReadoutCollection> pCollection;
   iEvent.getByToken(level1TagSrc,pCollection);

   edm::Handle<edm::TriggerResults> hlt;
   iEvent.getByToken(hltTagSrc,hlt);
   
   // get the digi collections
   edm::Handle<CSCWireDigiCollection> wires;
   edm::Handle<CSCStripDigiCollection> strips;
   edm::Handle<CSCComparatorDigiCollection> compars;
   edm::Handle<CSCALCTDigiCollection> alcts;
   edm::Handle<CSCCLCTDigiCollection> clcts;
   edm::Handle<CSCCorrelatedLCTDigiCollection> correlatedlcts;
   if (isDIGI){
     iEvent.getByToken(wireDigiTagSrc, wires);
     iEvent.getByToken(stripDigiTagSrc, strips);
     iEvent.getByToken(compDigiTagSrc, compars);
     iEvent.getByToken(alctDigiTagSrc, alcts);
     iEvent.getByToken(clctDigiTagSrc, clcts);
     iEvent.getByToken(corrlctDigiTagSrc, correlatedlcts);
   }

   edm::Handle<edm::PSimHitContainer> simHits;
   if (isSIM) iEvent.getByToken(simHitTagSrc, simHits);


   ////////////////////////////////////////////////////////////////////////////////
   nEventsTotal++;

   Run = iEvent.id().run();
   Event = iEvent.id().event();
   LumiSect = iEvent.id().luminosityBlock();
   BunchCrossing = iEvent.bunchCrossing();


   //Lumi Details
/*   if (isDATA && isFullRECO && LumiDet.isValid()){
     rawbxlumi = LumiDet->lumiValue(LumiDetails::kOCC1,iEvent.bunchCrossing());
     for (int i=0;i<3564;++i)
       {
	 bx_B1[i]=LumiDet->lumiBeam1Intensity(i);
	 bx_B2[i]=LumiDet->lumiBeam2Intensity(i);
	 bx_LUMI[i]=LumiDet->lumiValue(LumiDetails::kOCC1,i);
	 //calibrated here but not corrected, in Hz/ub 
       }
   }
   else{
     rawbxlumi = -999;
     for (int i=0;i<3564;++i)
       {
	 bx_B1[i]=-1;
	 bx_B2[i]=-1;
	 bx_LUMI[i]=-1;
       }
   }
 */  


   if(addMuons && isFullRECO) doMuons(muons,saMuons,cscSegments,recHits,PV,iEvent,iSetup,geometry_,cscGeom);
//   if(addTracks && isFullRECO) doTracks(genTracks);
   if(addRecHits &&  (isFullRECO || isLocalRECO)) doRecHits(recHits,simHits,saMuons,muons,cscGeom,iEvent);
   if(addSegments && (isFullRECO || isLocalRECO)) doSegments(cscSegments,cscGeom);
//   if(addTrigger && (isFullRECO || isLocalRECO || isRAW)) doTrigger(pCollection,hlt);
   if(addDigis && isDIGI)
     {
       doStripDigis(strips, cscGeom);
       doWireDigis(wires, cscGeom);
       doCompTiming(*compars);
       if(addTimeMonitoring) doLCTDigis(alcts, clcts, correlatedlcts, pCollection, cscGeom,iSetup, iEvent);
       if(isLocalRECO) doGasGain(*wires, *strips, *recHits);
     }
   if(addRecHits && isDIGI && (isLocalRECO || isFullRECO) ) doNonAssociatedRecHits(cscSegments,cscGeom,strips);
   if(addCalibrations && nEventsTotal == 1) doCalibrations(iSetup);

   //Fill the tree
//cout << "nRHs: " << recHits2D_nRecHits2D << endl;
   if((addRecHits && recHits2D_nRecHits2D > 0 && (isFullRECO || isLocalRECO) ) /*|| !addRecHits*/) {tree->Fill();}


   //clear some vectors 
   cscSegments_recHitRecord_endcap.clear();
   cscSegments_recHitRecord_ring.clear();
   cscSegments_recHitRecord_station.clear();
   cscSegments_recHitRecord_chamber.clear();
   cscSegments_recHitRecord_layer.clear();
   cscSegments_recHitRecord_localY.clear();
   cscSegments_recHitRecord_localX.clear();

   muons_cscSegmentRecord_nRecHits.clear();
   muons_cscSegmentRecord_ring.clear();
   muons_cscSegmentRecord_station.clear();
   muons_cscSegmentRecord_chamber.clear();
   muons_cscSegmentRecord_endcap.clear();
   muons_cscSegmentRecord_localY.clear();
   muons_cscSegmentRecord_localX.clear();



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByToken("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif


}


// ------------ method called once each job just before starting event loop  ------------
void CSCRootMaker::beginJob()
{
  bookTree(tree);
}

// ------------ method called once each job just after ending the event loop  ------------
void CSCRootMaker::endJob() 
{
  histContainer_["NEVENTS"]->SetBinContent(1,nEventsTotal);
}

// ------------ method called when starting to processes a run  ------------
void CSCRootMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void CSCRootMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void CSCRootMaker::beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& iSetup)
{
  using namespace edm;
  using namespace std;

  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/LumiCalc
//  edm::Handle<LumiSummary> l;
/*  if(isDATA && isFullRECO)
    {
      lumi.getByToken("lumiProducer", l); 
      if (!l.isValid()){ avgInstantLumi = -999;}
      else{ avgInstantLumi = l->avgInsDelLumi();}
*/      
      //Corrected Lumi
/*      edm::ESHandle<LumiCorrectionParam> datahandle;
      iSetup.getData(datahandle);
      int corrfac = -1;
      if(datahandle.isValid())
	{
	  const LumiCorrectionParam* mydata = datahandle.product();
	  corrfac = mydata->getCorrection(avgInstantLumi);  
	}
*/
//      correctedAvgInstantLumi = avgInstantLumi*corrfac; //final lumi value=correction*raw lumi value
//    }

}

// ------------ method called when ending the processing of a luminosity block  ------------
void CSCRootMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
/*void CSCRootMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) 
{
  using namespace edm;
  ParameterSetDescription desc;




  desc.addUntracked<InputTag>("muonSrc",edm::InputTag("muons"))->setComment("Muon Input Tag. default: muons");
  desc.addUntracked<InputTag>("vertexSrc",edm::InputTag("goodOfflinePrimaryVertices"))->setComment("Vertex Input Tag. default: goodOfflinePrimaryVertices");
  desc.addUntracked<InputTag>("standAloneMuonsSrc",edm::InputTag("standAloneMuons"))->setComment("StandAlone Muons Input Tag. default: standAloneMuons");
  desc.addUntracked<InputTag>("cscRecHitTagSrc",edm::InputTag("csc2DRecHits"))->setComment("2D RecHits Input Tag. default: csc2DRecHits");
  desc.addUntracked<InputTag>("cscSegTagSrc",edm::InputTag("cscSegments"))->setComment("Segments Input Tag. default: cscSegments");
  desc.addOptionalUntracked<InputTag>("level1TagSrc",edm::InputTag("gtDigis"))->setComment("L1 Digi Input Tag. default: gtDigis");
  desc.addOptionalUntracked<InputTag>("hltTagSrc",edm::InputTag("TriggerResults::HLT"))->setComment("HLT Input Tag. default: TriggerResults::HLT");
  desc.addOptionalUntracked<InputTag>("stripDigiTagSrc",edm::InputTag("simMuonCSCDigis:MuonCSCStripDigi"))->setComment("Strip Digi Input Tag. default: simMuonCSCDigis:MuonCSCStripDigi");
  desc.addOptionalUntracked<InputTag>("wireDigiTagSrc",edm::InputTag("simMuonCSCDigis:MuonCSCWireDigi"))->setComment("Wire Digi Input Tag. default: simMuonCSCDigis:MuonCSCWireDigi");
  desc.addOptionalUntracked<InputTag>("compDigiTagSrc",edm::InputTag("simMuonCSCDigis:MuonCSCComparatorDigi"))->setComment("Comparator Digi Input Tag. default: simMuonCSCDigis:MuonCSCComparatorDigi");
  desc.addOptionalUntracked<InputTag>("alctDigiTagSrc",edm::InputTag("muonCSCDigis:MuonCSCALCTDigi"))->setComment("ALCT Digi Input Tag. default: muonCSCDigis:MuonCSCALCTDigi");
  desc.addOptionalUntracked<InputTag>("clctDigiTagSrc",edm::InputTag("muonCSCDigis:MuonCSCCLCTDigi"))->setComment("CLCT Digi Input Tag. default: muonCSCDigis:MuonCSCCLCTDigi");
  desc.addOptionalUntracked<InputTag>("corrlctDigiTagSrc",edm::InputTag("muonCSCDigis:MuonCSCCorrelatedLCTDigi"))->setComment("Correlated Digi Input Tag. default: muonCSCDigis:MuonCSCCorrelatedLCTDigi");
  desc.addOptionalUntracked<InputTag>("simHitTagSrc",edm::InputTag("g4SimHits:MuonCSCHits"))->setComment("SimHit InputTag. default: g4SimHits:MuonCSCHits");
  desc.addOptionalUntracked<InputTag>("fedRawTagSrc",edm::InputTag("rawDataCollector"))->setComment("FED Raw InputTag. default: rawDataCollector");
  desc.addOptionalUntracked<bool>("isFullRECO",false)->setComment("Is full RECO info present in dataset");
  desc.addOptionalUntracked<bool>("isLocalRECO",false)->setComment("Is local muon RECO info present in dataset");
  desc.addOptionalUntracked<bool>("isGEN",false)->setComment("Is GEN info present in dataset");
  desc.addOptionalUntracked<bool>("isSIM",false)->setComment("Is SIM info present in dataset");
  desc.addOptionalUntracked<bool>("isRAW",false)->setComment("Is RAW info present in dataset");
  desc.addOptionalUntracked<bool>("isDIGI",false)->setComment("Is DIGI info present in dataset");
  desc.addUntracked<bool>("isDATA",false)->setComment("Is dataset real data");
  desc.addOptionalUntracked<bool>("addMuons",false)->setComment("Add info for muons into tree - requires isFullRECO and RECO dataset");
  desc.addOptionalUntracked<bool>("addTracks",false)->setComment("Add info for tracks into tree - requires isFullRECO and RECO dataset");
  desc.addOptionalUntracked<bool>("addRecHits",false)->setComment("Add info for RecHits into tree - requires isLocalRECO and RECO dataset");
  desc.addOptionalUntracked<bool>("addSegments",false)->setComment("Add info for segments into tree - requires isLocalRECO and RECO dataset");
  desc.addOptionalUntracked<bool>("addTrigger",false)->setComment("Add info for trigger into tree - requires isDIGI and DIGI dataset");
  desc.addOptionalUntracked<bool>("addDigis",false)->setComment("Add info for digis into tree - requires isDIGI and DIGI dataset");
  desc.addOptionalUntracked<bool>("addTimeMonitoring",false)->setComment("Add timing info into tree - requires isDIGI and DIGI dataset");
  desc.addOptionalUntracked<bool>("addCalibrations",false)->setComment("Add calibration info into tree");
  descriptions.add("cscRootMaker",desc);
}
*/


void CSCRootMaker::doMuons(edm::Handle<reco::MuonCollection> muons, edm::Handle<reco::TrackCollection> saMuons, edm::Handle<CSCSegmentCollection> cscSegments, edm::Handle<CSCRecHit2DCollection> recHits,
			     const reco::Vertex *&PV, const edm::Event& iEvent, const edm::EventSetup& iSetup, edm::ESHandle<GlobalTrackingGeometry> theGeom, edm::ESHandle<CSCGeometry> cscGeom)
{

  //Muons
  counter = 0;
  for(reco::MuonCollection::const_iterator mu = muons->begin(); mu != muons->end(); mu++ ) 
    {
      
      muons_isStandAloneMuon[counter] = mu->isStandAloneMuon();
      muons_isGlobalMuon[counter] = mu->isGlobalMuon();
      muons_isPFMuon[counter] = mu->isPFMuon();
      muons_isCaloMuon[counter] = mu->isCaloMuon();
      muons_isTrackerMuon[counter] = mu->isTrackerMuon();
      muons_isEnergyValid[counter] = mu->isEnergyValid();
      muons_numberOfChambers[counter] = mu->numberOfChambers();
      muons_numberOfMatches[counter] = mu->numberOfMatches();
      muons_calEnergyTower[counter] = mu->calEnergy().tower;
      muons_calEnergyEm[counter] = mu->calEnergy().em;
      muons_calEnergyHad[counter] = mu->calEnergy().had;
      muons_charge[counter] = mu->charge();
      muons_energy[counter] = mu->energy();
      muons_px[counter] = mu->px();
      muons_py[counter] = mu->py();
      muons_pz[counter] = mu->pz();
      muons_pt[counter] = mu->pt();
      muons_et[counter] = mu->et();
      muons_p[counter] = mu->p();
      muons_phi[counter] = mu->phi();
      muons_eta[counter] = mu->eta();
      muons_theta[counter] = mu->theta();
      muons_vx[counter] = mu->vx();
      muons_vy[counter] = mu->vy();
      muons_vz[counter] = mu->vz();

      std::vector<double> cscSegmentRecord_nRecHits, cscSegmentRecord_ring, cscSegmentRecord_station, cscSegmentRecord_chamber, cscSegmentRecord_endcap;
      std::vector<double> cscSegmentRecord_localY, cscSegmentRecord_localX, cscSegmentRecord_theta;

           
      if (mu->isGlobalMuon())
	{
	  muons_globalTrackNormalizedChi2[counter] = (double)mu->globalTrack()->chi2()/mu->globalTrack()->ndof();
	  muons_globalTrackNumberOfValidMuonHits[counter] = mu->globalTrack()->hitPattern().numberOfValidMuonHits();
	}  
      else{
	muons_globalTrackNormalizedChi2[counter] = -999;
	muons_globalTrackNumberOfValidMuonHits[counter] = -999;
      }
      if(mu->track().isNonnull())
	{
	  muons_trackNumberOfValidHits[counter] = mu->track()->numberOfValidHits();
	  muons_trackNumberOfLostHits[counter] = mu->track()->numberOfLostHits();  
	  muons_dxy[counter] = mu->track()->dxy(PV->position());
	  muons_dz[counter] = mu->track()->dz(PV->position());

	  muons_nRecHits[counter] = -1;
	  muons_numberOfSegments[counter] = 0;
	  int tmpRHCounter = 0, tmpSegCounter = 0;
	  
	  if(mu->outerTrack().isNonnull() && (mu->isStandAloneMuon() || mu->isGlobalMuon()) )
	    {
	      std::vector<CSCSegment> mySavedSegments = findMuonSegments(theGeom, *mu->outerTrack(), cscSegments, recHits, cscGeom);
	      for (int j = 0; j < (int)mySavedSegments.size(); j++)
		{
		  CSCDetId cscSegId  = (CSCDetId)mySavedSegments[j].cscDetId();
		  tmpSegCounter++;
		  cscSegmentRecord_nRecHits.push_back(mySavedSegments[j].recHits().size());
		  cscSegmentRecord_ring.push_back(cscSegId.ring());
		  cscSegmentRecord_station.push_back(cscSegId.station());
		  cscSegmentRecord_chamber.push_back(cscSegId.chamber());
		  cscSegmentRecord_endcap.push_back(cscSegId.endcap());
		  LocalPoint localP = mySavedSegments[j].localPosition();
		  cscSegmentRecord_localY.push_back(localP.y());
		  cscSegmentRecord_localX.push_back(localP.x());
		  tmpRHCounter += mySavedSegments[j].recHits().size();
		}
	      
	    }
	  //cout << tmpSegCounter << "   " << tmpRHCounter << endl;
	  muons_nRecHits[counter] = tmpRHCounter;
	  muons_numberOfSegments[counter] = tmpSegCounter;
	}
      else
	{
	  muons_trackNumberOfValidHits[counter] = -999;
	  muons_trackNumberOfLostHits[counter] = -999;
	  muons_dxy[counter] = -999;
	  muons_dz[counter] = -999;
	}
      if(mu->isPFIsolationValid())
	{
	  muons_isoNH03[counter] = mu->pfIsolationR03().sumNeutralHadronEt;
	  muons_isoCH03[counter] = mu->pfIsolationR03().sumChargedHadronPt;
	  muons_isoPhot03[counter] = mu->pfIsolationR03().sumPhotonEt;
	  muons_isoPU03[counter] = mu->pfIsolationR03().sumPUPt;
	  
	  muons_isoNH04[counter] = mu->pfIsolationR04().sumNeutralHadronEt;
	  muons_isoCH04[counter] = mu->pfIsolationR04().sumChargedHadronPt;
	  muons_isoPhot04[counter] = mu->pfIsolationR04().sumPhotonEt;
	  muons_isoPU04[counter] = mu->pfIsolationR04().sumPUPt;
	}
      counter++;

      muons_cscSegmentRecord_nRecHits.push_back(cscSegmentRecord_nRecHits);
      muons_cscSegmentRecord_ring.push_back(cscSegmentRecord_ring); 
      muons_cscSegmentRecord_station.push_back(cscSegmentRecord_station);
      muons_cscSegmentRecord_chamber.push_back(cscSegmentRecord_chamber);
      muons_cscSegmentRecord_endcap.push_back(cscSegmentRecord_endcap);
      muons_cscSegmentRecord_localY.push_back(cscSegmentRecord_localY);
      muons_cscSegmentRecord_localX.push_back(cscSegmentRecord_localX);
    }
  muons_nMuons = counter;
  



  // Standalone Muon RecHits
  counter = 0;
  for(reco::TrackCollection::const_iterator muon = saMuons->begin(); muon != saMuons->end(); ++muon ) {
    standaloneMuons_p[counter] = muon->p();
    standaloneMuons_pt[counter] = muon->pt();
    standaloneMuons_nRecHits[counter] = muon->recHitsSize();
    standaloneMuons_chi2[counter] = muon->chi2();
    standaloneMuons_normChi2[counter] = muon->normalizedChi2();
    
    // loop over hits
    int nDTHits = 0;
    int nCSCHits = 0;
    int nCSCHitsp = 0;
    int nCSCHitsm = 0;
    int nRPCHits = 0;
    int nRPCHitsp = 0;
    int nRPCHitsm = 0;
    int np = 0;
    int nm = 0;
    int recHitCounter = 0;
    std::vector<CSCDetId> staChambers;
    for (trackingRecHit_iterator hit = muon->recHitsBegin(); hit != muon->recHitsEnd(); hit++ ) {
      const DetId detId( (*hit)->geographicalId() );
      if (detId.det() == DetId::Muon) {
	if (detId.subdetId() == MuonSubdetId::RPC) {
	  RPCDetId rpcId(detId.rawId());
	  nRPCHits++;
	  if (rpcId.region() == 1){ nRPCHitsp++; np++;}
	  if (rpcId.region() == -1){ nRPCHitsm++; nm++;}
	}
	if (detId.subdetId() == MuonSubdetId::DT) {
	  nDTHits++;
	}
	else if (detId.subdetId() == MuonSubdetId::CSC) {
	  CSCDetId cscId(detId.rawId());
	  staChambers.push_back(detId.rawId());
	  nCSCHits++;
	  if (cscId.endcap() == 1){ nCSCHitsp++; np++;}
	  if (cscId.endcap() == 2){ nCSCHitsm++; nm++;}
	}
      }
      recHitCounter++;
    }
    
    standaloneMuons_nDTHits[counter] = nDTHits;
    standaloneMuons_nCSCHits[counter] = nCSCHits;
    standaloneMuons_nCSCHitsP[counter] = nCSCHitsp;
    standaloneMuons_nCSCHitsM[counter] = nCSCHitsm;
    standaloneMuons_nRPCHits[counter] = nRPCHits;
    standaloneMuons_nRPCHitsP[counter] = nRPCHitsp;
    standaloneMuons_nRPCHitsM[counter] = nRPCHitsm;
    standaloneMuons_nHitsP[counter] = np;
    standaloneMuons_nHitsM[counter] = nm;
    
    
    GlobalPoint  innerPnt(muon->innerPosition().x(),muon->innerPosition().y(),muon->innerPosition().z());
    GlobalPoint  outerPnt(muon->outerPosition().x(),muon->outerPosition().y(),muon->outerPosition().z());
    GlobalVector innerKin(muon->innerMomentum().x(),muon->innerMomentum().y(),muon->innerMomentum().z());
    GlobalVector outerKin(muon->outerMomentum().x(),muon->outerMomentum().y(),muon->outerMomentum().z());
    GlobalVector deltaPnt = innerPnt - outerPnt;
    standaloneMuons_crudeLength[counter] = deltaPnt.mag();
    standaloneMuons_deltaPhi[counter] = innerPnt.phi() - outerPnt.phi();
    standaloneMuons_innerGlobalPolarAngle[counter] = innerKin.theta();
    standaloneMuons_outerGlobalPolarAngle[counter] = outerKin.theta();
    
    counter++;
  }
  standaloneMuons_nMuons = counter;
}

void 
CSCRootMaker::doTracks(edm::Handle<reco::TrackCollection> genTracks)
{

   // Tracks   
   counter = 0;
   for(reco::TrackCollection::const_iterator genTrack = genTracks->begin(); genTrack != genTracks->end(); genTrack++){
     
     tracks_charge[counter] = genTrack->charge();
     tracks_px[counter] = genTrack->px();
     tracks_py[counter] = genTrack->py();
     tracks_pz[counter] = genTrack->pz();
     tracks_pt[counter] = genTrack->pt();
     tracks_p[counter] = genTrack->p();
     tracks_eta[counter] = genTrack->eta();
     tracks_phi[counter] = genTrack->phi();

     counter++;
   }
   tracks_nTracks = counter;

}


void
CSCRootMaker::doRecHits(edm::Handle<CSCRecHit2DCollection> recHits, edm::Handle<edm::PSimHitContainer> simHits, edm::Handle<reco::TrackCollection> saMuons,  
			  edm::Handle<reco::MuonCollection> muons, edm::ESHandle<CSCGeometry> cscGeom, const edm::Event& iEvent)
{

  edm::Handle<CSCStripDigiCollection> myStrips;
  if(isDIGI) iEvent.getByToken(stripDigiTagSrc, myStrips);

   // RecHits2D
   counter = 0;
   for (int i = 0; i < 10000; i++){
     recHits2D_SumQ[i] = 0;
     recHits2D_SumQSides[i] = 0;
   }


   for (CSCRecHit2DCollection::const_iterator dRHIter = recHits->begin(); dRHIter != recHits->end(); dRHIter++) {

     // Find chamber with rechits in CSC 
     CSCDetId idrec = (CSCDetId)(*dRHIter).cscDetId();
     recHits2D_ID_endcap[counter]  = idrec.endcap();
     recHits2D_ID_ring[counter]    = idrec.ring();
     recHits2D_ID_station[counter] = idrec.station();
     recHits2D_ID_chamber[counter] = idrec.chamber();
     recHits2D_ID_layer[counter]   = idrec.layer();
     recHits2D_ID_chamberSerial[counter] = chamberSerial(idrec);
     recHits2D_ID_ringSerial[counter] = ringSerial(idrec);
     
     AllRechits.insert(std::pair<CSCDetId , CSCRecHit2D>(idrec,*dRHIter));

     // Store rechit as a Local Point:
     LocalPoint rhitlocal = (*dRHIter).localPosition();  
     recHits2D_localX[counter] = rhitlocal.x();
     recHits2D_localY[counter] = rhitlocal.y();
     LocalError rerrlocal = (*dRHIter).localPositionError();  
     //these errors are squared!
     recHits2D_localXXerr[counter] = rerrlocal.xx();
     recHits2D_localYYerr[counter] = rerrlocal.yy();
     recHits2D_localXYerr[counter] = rerrlocal.xy();
     // errors in strip units
     recHits2D_stripPosition[counter] = (*dRHIter).positionWithinStrip();
     recHits2D_stripError[counter] = (*dRHIter).errorWithinStrip();
     
     //Check association with sa muons
     recHits2D_belongsToSaMuon[counter] = -1;
     recHits2D_belongsToMuon[counter] = -1;

     if(isFullRECO)
       {
	 
	 int saMuCounter = 0;
	 double xTolerance = 0.05;
	 double yTolerance = 0.10;
	 for(reco::TrackCollection::const_iterator muon = saMuons->begin(); muon != saMuons->end(); muon++ ) 
	   {
	     bool found = false;
	     for (trackingRecHit_iterator hit = muon->recHitsBegin(); hit != muon->recHitsEnd(); hit++ ) 
	       {	     
		 const DetId detId( (*hit)->geographicalId() );
		 if (detId.det() == DetId::Muon) 
		   {
		     if (detId.subdetId() == MuonSubdetId::CSC) 
		       {
			 CSCDetId cscId(detId.rawId());
			 LocalPoint rhitlocalSA = (*hit)->localPosition();
			 
			 //cout << saMuCounter << endl
			 //<< "x : " << rhitlocal.x() << "  " << rhitlocalSA.x() << endl
			 //<< "y : " << rhitlocal.y() << "  " << rhitlocalSA.y() << endl
			 //<< "ec: " << idrec.endcap() << "  " << cscId.endcap() << endl
			 //<< "rg: " << idrec.ring() << "  " << cscId.ring() << endl
			 //<< "cb: " << idrec.chamber() << "  " << cscId.chamber() << endl
			 //<< "lr: " << idrec.layer() << "  " << cscId.layer() << endl
			 //<< "st: " << idrec.station() << "  " << cscId.station() << endl
			 //<< "found : " << found << endl;
			 
			 if( (fabs(rhitlocal.x() - rhitlocalSA.x()) < fabs(xTolerance*rhitlocal.x()) || fabs(rhitlocal.x() - rhitlocalSA.x()) < fabs(xTolerance*rhitlocalSA.x())) && !found)
			   {
			     if( (fabs(rhitlocal.y() - rhitlocalSA.y()) < fabs(yTolerance*rhitlocal.y()) || fabs(rhitlocal.y() - rhitlocalSA.y()) < fabs(yTolerance*rhitlocalSA.y())) && !found)
			       {
				 if(idrec.endcap() == cscId.endcap() && idrec.ring() == cscId.ring() && idrec.station() == cscId.station() && !found)
				   {
				     if(idrec.chamber() == cscId.chamber() && !found)// && idrec.layer() == cscId.layer() && !found)
				       {
					 recHits2D_belongsToSaMuon[counter] = saMuCounter;
					 found = true;
					 //cout << "FOUND!!!! " << counter << "   " << saMuCounter << endl; 
					 
				       } 
				     
				   }
			       }
			   }
			 
		       }
		   }
	       }
	     
	     saMuCounter++;
	   }


	 int muCounter = 0;
	 xTolerance = 0.05;
	 yTolerance = 0.10;
	 for(reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); muon++ ) 
	   {
	     bool found = false;
	     if (muon->outerTrack().isNull()) continue;
	     for (trackingRecHit_iterator hit = muon->outerTrack()->recHitsBegin(); hit != muon->outerTrack()->recHitsEnd(); hit++ ) 
	       {	     
		 const DetId detId( (*hit)->geographicalId() );
		 if (detId.det() == DetId::Muon) 
		   {
		     if (detId.subdetId() == MuonSubdetId::CSC) 
		       {
			 CSCDetId cscId(detId.rawId());
			 LocalPoint rhitlocalMu = (*hit)->localPosition();
			 
			 //cout << saMuCounter << endl
			 //<< "x : " << rhitlocal.x() << "  " << rhitlocalSA.x() << endl
			 //<< "y : " << rhitlocal.y() << "  " << rhitlocalSA.y() << endl
			 //<< "ec: " << idrec.endcap() << "  " << cscId.endcap() << endl
			 //<< "rg: " << idrec.ring() << "  " << cscId.ring() << endl
			 //<< "cb: " << idrec.chamber() << "  " << cscId.chamber() << endl
			 //<< "lr: " << idrec.layer() << "  " << cscId.layer() << endl
			 //<< "st: " << idrec.station() << "  " << cscId.station() << endl
			 //<< "found : " << found << endl;
			 
			 if( (fabs(rhitlocal.x() - rhitlocalMu.x()) < fabs(xTolerance*rhitlocal.x()) || fabs(rhitlocal.x() - rhitlocalMu.x()) < fabs(xTolerance*rhitlocalMu.x())) && !found)
			   {
			     if( (fabs(rhitlocal.y() - rhitlocalMu.y()) < fabs(yTolerance*rhitlocal.y()) || fabs(rhitlocal.y() - rhitlocalMu.y()) < fabs(yTolerance*rhitlocalMu.y())) && !found)
			       {
				 if(idrec.endcap() == cscId.endcap() && idrec.ring() == cscId.ring() && idrec.station() == cscId.station() && !found)
				   {
				     if(idrec.chamber() == cscId.chamber() && !found)// && idrec.layer() == cscId.layer() && !found)
				       {
					 recHits2D_belongsToMuon[counter] = muCounter;
					 found = true;
				       } 
				   }
			       }
			   }
			 
		       }
		   }
	       }
	     
	     muCounter++;
	   }


       }

     // Find the charge associated with this hit
     //double rHSumQ = 0;
     //double sumsides=0.;
     //int adcsize=dRHIter->nStrips()*dRHIter->nTimeBins();
     for ( unsigned int i=0; i< dRHIter->nStrips(); i++) {
       for ( unsigned int j=0; j< dRHIter->nTimeBins()-1; j++) {
	 recHits2D_SumQ[counter]+=dRHIter->adcs(i,j); 
	 //cout << i << "  " << j << "  " <<  dRHIter->adcs(i,j) << endl;
	 if (i!=1) recHits2D_SumQSides[counter]+=dRHIter->adcs(i,j);
       }
     }
     //cout << counter << "  " << recHits2D_SumQ[counter] << endl;
     // Get the signal timing of this hit
     recHits2D_Time[counter] = 0;
     recHits2D_Time[counter] = (*dRHIter).tpeak()/50.;
     
     // Get pointer to the layer:
     const CSCLayer* csclayer = cscGeom->layer( idrec );
     
     // Transform hit position from local chamber geometry to global CMS geom
     GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
     recHits2D_globalX[counter]   =  rhitglobal.x();
     recHits2D_globalY[counter]   =  rhitglobal.y();
     recHits2D_ADCSignal[counter] = -99;

     if(isDIGI)
       {
	 int centerid     =  (*dRHIter).nStrips()/2;
	 int centerStrip =  (*dRHIter).channels(centerid);
	 double  rHsignal = getthisSignal(*myStrips, idrec, centerStrip);
	 recHits2D_ADCSignal[counter] = rHsignal;
       }	 
	 
     CSCLayerGeometry *thegeom = const_cast<CSCLayerGeometry*>(csclayer->geometry());
     
     recHits2D_nearestStrip[counter] = thegeom->nearestStrip(rhitlocal);
     recHits2D_nearestWire[counter] = thegeom->nearestWire(rhitlocal);
     recHits2D_nearestWireGroup[counter] = thegeom->wireGroup(recHits2D_nearestWire[counter]);
     recHits2D_localStripWireIntersectionX[counter] = thegeom->stripWireIntersection(thegeom->nearestStrip(rhitlocal),thegeom->nearestWire(rhitlocal)).x();
     recHits2D_localStripWireIntersectionY[counter] = thegeom->stripWireIntersection(thegeom->nearestStrip(rhitlocal),thegeom->nearestWire(rhitlocal)).y();
     recHits2D_localStripWireGroupIntersectionX[counter] = thegeom->stripWireGroupIntersection(thegeom->nearestStrip(rhitlocal),thegeom->wireGroup(recHits2D_nearestWire[counter])).x();
     recHits2D_localStripWireGroupIntersectionY[counter] = thegeom->stripWireGroupIntersection(thegeom->nearestStrip(rhitlocal),thegeom->wireGroup(recHits2D_nearestWire[counter])).y();
     recHits2D_stripWidthAtHit[counter] = thegeom->stripPitch(rhitlocal);

     recHits2D_positionWithinStrip[counter] = (*dRHIter).positionWithinStrip();
     recHits2D_nWireGroups[counter] =  (*dRHIter).nWireGroups();
     recHits2D_wireTime[counter] =(*dRHIter).wireTime();
     recHits2D_hitWire[counter] = (*dRHIter).hitWire();
     recHits2D_wgroupsBX[counter] = (*dRHIter).wgroupsBX();
     
     recHits2D_simHit_localX[counter] = -999;
     recHits2D_simHit_localY[counter] = -999;
     recHits2D_simHit_particleTypeID[counter] = -1;

     if (isSIM)
       {

	 double mindiff = 1000;
	 double mindiffX = 99;
	 // If MC, find closest muon simHit to check resolution:
	 edm::PSimHitContainer::const_iterator dSHsimIter;
	 for (dSHsimIter = simHits->begin(); dSHsimIter != simHits->end(); dSHsimIter++){
	   // Get DetID for this simHit:
	   CSCDetId sId = (CSCDetId)(*dSHsimIter).detUnitId();
	   // Check if the simHit detID matches that of current recHit
	   if (sId == idrec ){
	     // Get the position of this simHit in local coordinate system:
	     LocalPoint sHitlocal = (*dSHsimIter).localPosition();
	     // Now we need to make reasonably sure that this simHit is
	     // responsible for this recHit:
	     if ( sqrt((sHitlocal.x() - rhitlocal.x())*(sHitlocal.x() - rhitlocal.x())
		       + (sHitlocal.y() - rhitlocal.y())*(sHitlocal.y() - rhitlocal.y())) < mindiff 
		  && (sHitlocal.x() - rhitlocal.x()) < mindiffX 
		  && (sHitlocal.y() - rhitlocal.y()) < 10.0)
	       {
		 recHits2D_simHit_localX[counter] = sHitlocal.x();
		 recHits2D_simHit_localY[counter] = sHitlocal.y();
		 recHits2D_simHit_particleTypeID[counter] = (*dSHsimIter).particleType();
		 mindiff = sqrt((sHitlocal.x() - rhitlocal.x())*(sHitlocal.x() - rhitlocal.x()) 
				+ (sHitlocal.y() - rhitlocal.y())*(sHitlocal.y() - rhitlocal.y()));
		 mindiffX = (sHitlocal.x() - rhitlocal.x());
	       }
	   }
	 }
       }
     
     
     counter++;
   }
   recHits2D_nRecHits2D = counter;

//cout << "nRH: " << counter << endl;
   
   counter = 0;
   //SimHits
   if (isSIM)
     {
       edm::PSimHitContainer::const_iterator dSHsimIter;
       for (dSHsimIter = simHits->begin(); dSHsimIter != simHits->end(); dSHsimIter++)
	 {
	   // Get DetID for this simHit:
	   CSCDetId sId = (CSCDetId)(*dSHsimIter).detUnitId();
	   LocalPoint sHitlocal = (*dSHsimIter).localPosition();

	   simHits_particleType[counter] = (*dSHsimIter).particleType();
	   simHits_localX[counter] = sHitlocal.x();
	   simHits_localY[counter] = sHitlocal.y();
	   
	   const CSCLayer* shlayer = cscGeom->layer( sId );
	   GlobalPoint shitglobal= shlayer->toGlobal(sHitlocal);
	   simHits_globalX[counter]   =  shitglobal.x();
	   simHits_globalY[counter]   =  shitglobal.y();
	  
	   simHits_ID_endcap[counter]  = sId.endcap();
	   simHits_ID_ring[counter]    = sId.ring();
	   simHits_ID_station[counter] = sId.station();
	   simHits_ID_chamber[counter] = sId.chamber();
	   simHits_ID_layer[counter]   = sId.layer();
	   simHits_ID_chamberSerial[counter] = chamberSerial(sId);
	   simHits_ID_ringSerial[counter] = ringSerial(sId);

	   simHits_ID_processType[counter] = (*dSHsimIter).processType();
	   simHits_momentum[counter] = (*dSHsimIter).pabs();
	   simHits_phi[counter] = (*dSHsimIter).phiAtEntry();
	   simHits_theta[counter] = (*dSHsimIter).thetaAtEntry();
	   
	   counter++;
	 }
     }
   simHits_nSimHits = counter;
   

}


double CSCRootMaker::getthisSignal(const CSCStripDigiCollection& stripdigis, CSCDetId idRH, int centerStrip){
	// Loop over strip digis responsible for this recHit
	CSCStripDigiCollection::DigiRangeIterator sIt;
	double thisADC = 0.;
	//bool foundRHid = false;
	// std::cout<<"iD   S/R/C/L = "<<idRH<<"    "<<idRH.station()<<"/"<<idRH.ring()<<"/"<<idRH.chamber()<<"/"<<idRH.layer()<<std::endl;
	for (sIt = stripdigis.begin(); sIt != stripdigis.end(); sIt++){
		CSCDetId id = (CSCDetId)(*sIt).first;
		//std::cout<<"STRIPS: id    S/R/C/L = "<<id<<"     "<<id.station()<<"/"<<id.ring()<<"/"<<id.chamber()<<"/"<<id.layer()<<std::endl;
		if (id == idRH){
			//foundRHid = true;
			vector<CSCStripDigi>::const_iterator digiItr = (*sIt).second.first;
			vector<CSCStripDigi>::const_iterator last = (*sIt).second.second;
			//if(digiItr == last ) {std::cout << " Attention1 :: Size of digi collection is zero " << std::endl;}
			int St = idRH.station();
			int Rg    = idRH.ring();
			if (St == 1 && Rg == 4){
				while(centerStrip> 16) centerStrip -= 16;
			}
			for ( ; digiItr != last; ++digiItr ) {
				int thisStrip = digiItr->getStrip();
				//std::cout<<" thisStrip = "<<thisStrip<<" centerStrip = "<<centerStrip<<std::endl;
				std::vector<int> myADCVals = digiItr->getADCCounts();
				double thisPedestal = 0.5*(double)(myADCVals[0]+myADCVals[1]);
				double Signal = (double) myADCVals[3];
				if (thisStrip == (centerStrip)){
					thisADC = Signal-thisPedestal;
					//if(thisADC >= 0. && thisADC <2.) {std::cout << " Attention2 :: The Signal is equal to the pedestal " << std::endl;
					//}
					//if(thisADC < 0.) {std::cout << " Attention3 :: The Signal is less than the pedestal " << std::endl;
					//}
				}
				if (thisStrip == (centerStrip+1)){
					std::vector<int> myADCVals = digiItr->getADCCounts();
				}
				if (thisStrip == (centerStrip-1)){
					std::vector<int> myADCVals = digiItr->getADCCounts();
				}
			}
		}
	}
	//if(!foundRHid){std::cout << " Attention4 :: Did not find a matching RH id in the Strip Digi collection " << std::endl;}
	return thisADC;
}


void
CSCRootMaker::doSegments(edm::Handle<CSCSegmentCollection> cscSegments, edm::ESHandle<CSCGeometry> cscGeom)
{
  
   // CSC Segments
   counter = 0;
   for(CSCSegmentCollection::const_iterator dSiter=cscSegments->begin(); dSiter != cscSegments->end(); dSiter++) {

     std::vector<double> recHitRecord_localX, recHitRecord_localY;
     std::vector<double> recHitRecord_layer, recHitRecord_chamber, recHitRecord_station, recHitRecord_ring, recHitRecord_endcap;

     CSCDetId id  = (CSCDetId)(*dSiter).cscDetId();
     int nRH = (*dSiter).nRecHits();
     cscSegments_ID_endcap[counter]  = id.endcap();
     cscSegments_ID_ring[counter]    = id.ring();
     cscSegments_ID_station[counter] = id.station();
     cscSegments_ID_chamber[counter] = id.chamber();
     cscSegments_ID_chamberSerial[counter] = chamberSerial(id);
     cscSegments_ID_ringSerial[counter] = ringSerial(id);
    
     cscSegments_chi2[counter]      = (*dSiter).chi2();
     cscSegments_nRecHits[counter]  = nRH;
     cscSegments_nDOF[counter]      = 2*(*dSiter).nRecHits()-4;
     //double chisqProb = ChiSquaredProbability( (double)chisq, nDOF );
     LocalPoint localPos = (*dSiter).localPosition();
     cscSegments_localX[counter]     = localPos.x();
     cscSegments_localY[counter]     = localPos.y();
     LocalVector segDir = (*dSiter).localDirection();
     cscSegments_localTheta[counter] = segDir.theta();
     
     // global transformation
     cscSegments_globalX[counter] = 0.;
     cscSegments_globalY[counter] = 0.;
     cscSegments_globalTheta[counter] = 0.;
     cscSegments_globalPhi[counter]   = 0.;
     const CSCChamber* cscchamber = cscGeom->chamber(id);
     GlobalPoint globalPosition = GlobalPoint(0.0, 0.0, 0.0);
     if (cscchamber) 
       {
	 globalPosition = cscchamber->toGlobal(localPos);
	 cscSegments_globalX[counter] = globalPosition.x();
	 cscSegments_globalY[counter] = globalPosition.y();
	 GlobalVector globalDirection = cscchamber->toGlobal(segDir);
	 cscSegments_globalTheta[counter] = globalDirection.theta();
	 cscSegments_globalPhi[counter]   = globalDirection.phi();
       }

     cscSegments_distToIP[counter] = -1;
     cscSegments_segmentTime[counter] = -1;
     // try to get the CSC recHits that contribute to this segment.
     std::vector<CSCRecHit2D> theseRecHits = (*dSiter).specificRecHits();
     if (nRH >= 1 )
       {
	 //Store the recHit times of a segment in a vector for later sorting
	 vector<double> non_zero;	
	 CLHEP::HepMatrix sp(6,1);
	 CLHEP::HepMatrix se(6,1);
	 for ( vector<CSCRecHit2D>::const_iterator iRH = theseRecHits.begin(); iRH != theseRecHits.end(); iRH++) {
	   non_zero.push_back( iRH->tpeak());
	   CSCDetId idrec = (CSCDetId)(*iRH).cscDetId();
	   LocalPoint rhitlocal = (*iRH).localPosition();
	   recHitRecord_localX.push_back(rhitlocal.x());
	   recHitRecord_localY.push_back(rhitlocal.y());
	   recHitRecord_endcap.push_back(idrec.endcap());
	   recHitRecord_layer.push_back(idrec.layer());
	   recHitRecord_chamber.push_back(idrec.chamber());
	   recHitRecord_station.push_back(idrec.station());
	   recHitRecord_ring.push_back(idrec.ring());

	   //RESOLUTION
	   // Find the strip containing this hit
	   int centerid     =  iRH->nStrips()/2;
	   int centerStrip =  iRH->channels(centerid);
	   
	   // If this segment has 6 hits, find the position of each hit on the strip in units of stripwidth and store values
	   if (nRH == 6)
	     {
	       int kRing    = idrec.ring();
	       int kStation = idrec.station();
	       int kLayer   = idrec.layer();

	       double stpos = (*iRH).positionWithinStrip();
	       se(kLayer,1) = (*iRH).errorWithinStrip();
	       // Take into account half-strip staggering of layers (ME1/1 has no staggering)
	       if (kStation == 1 && (kRing == 1 || kRing == 4)) sp(kLayer,1) = stpos + centerStrip;
	       else{
		 if (kLayer == 1 || kLayer == 3 || kLayer == 5) sp(kLayer,1) = stpos + centerStrip;
		 if (kLayer == 2 || kLayer == 4 || kLayer == 6) sp(kLayer,1) = stpos - 0.5 + centerStrip;
	       }
	     }
	   
	 }// end rechit loop
	 
	 //Sort the vector of hit times for this segment and average the center two
	 sort(non_zero.begin(),non_zero.end());
	 int middle_index = non_zero.size()/2;
	 double average_two = (non_zero.at(middle_index-1) + non_zero.at(middle_index))/2.;
	 if(non_zero.size()%2) average_two = non_zero.at(middle_index);
	 
	 double distToIP = sqrt(globalPosition.x()*globalPosition.x()+globalPosition.y()*globalPosition.y()+globalPosition.z()*globalPosition.z());
	 cscSegments_distToIP[counter] = distToIP;
	 cscSegments_segmentTime[counter] = average_two;

	 cscSegments_Resolution_residual[counter] = -99;
	 cscSegments_Resolution_pull[counter] = -99;
	 // Fit all points except layer 3, then compare expected value for layer 3 to reconstructed value
	 if (nRH == 6)
	   {
	     double expected = fitX(sp,se);
	     cscSegments_Resolution_residual[counter] = expected - sp(3,1);
	     cscSegments_Resolution_pull[counter] = cscSegments_Resolution_residual[counter]/se(3,1);
	   }
	 
       }
     counter++;
     cscSegments_recHitRecord_endcap.push_back(recHitRecord_endcap);
     cscSegments_recHitRecord_ring.push_back(recHitRecord_ring);
     cscSegments_recHitRecord_station.push_back(recHitRecord_station);
     cscSegments_recHitRecord_chamber.push_back(recHitRecord_chamber);
     cscSegments_recHitRecord_layer.push_back(recHitRecord_layer);
     cscSegments_recHitRecord_localY.push_back(recHitRecord_localY);
     cscSegments_recHitRecord_localX.push_back(recHitRecord_localX);
   }
   cscSegments_nSegments = counter;
//cout << "nSeg: " << counter << endl;
}



void CSCRootMaker::doNonAssociatedRecHits(edm::Handle<CSCSegmentCollection> cscSegments, edm::ESHandle<CSCGeometry> cscGeom,  edm::Handle<CSCStripDigiCollection> strips){


  //Add Segments to Map
  for(CSCSegmentCollection::const_iterator it=cscSegments->begin(); it != cscSegments->end(); it++) {

    std::vector<CSCRecHit2D> theseRecHits = (*it).specificRecHits();
    for ( std::vector<CSCRecHit2D>::const_iterator iRH = theseRecHits.begin(); iRH != theseRecHits.end(); iRH++) {
      CSCDetId idRH = (CSCDetId)(*iRH).cscDetId();
      LocalPoint lpRH = (*iRH).localPosition();
      double xrec = lpRH.x();
      double yrec = lpRH.y();
      double zrec = lpRH.z();
      bool RHalreadyinMap = false;
      //Store the rechits associated with segments into a Map
      multimap<CSCDetId , CSCRecHit2D>::iterator segRHit;
      segRHit = SegRechits.find(idRH);
      if (segRHit != SegRechits.end()){
	for( ; segRHit != SegRechits.upper_bound(idRH); ++segRHit){
	  //for( segRHit = SegRechits.begin(); segRHit != SegRechits.end() ;++segRHit){
	  LocalPoint lposRH = (segRHit->second).localPosition();
	  double xpos = lposRH.x();
	  double ypos = lposRH.y();
	  double zpos = lposRH.z();
	  if ( xrec == xpos && yrec == ypos && zrec == zpos){
	  RHalreadyinMap = true;
	  //std::cout << " Already exists " <<std ::endl;
	  break;}
	}
      }
      if(!RHalreadyinMap){ SegRechits.insert(std::pair<CSCDetId , CSCRecHit2D>(idRH,*iRH));}
    }
  }

  //Search for Non-Associated
  for(std::multimap<CSCDetId , CSCRecHit2D>::iterator allRHiter =  AllRechits.begin();allRHiter != AllRechits.end(); ++allRHiter){
    
    CSCDetId idRH = allRHiter->first;
    LocalPoint lpRH = (allRHiter->second).localPosition();
    double xrec = lpRH.x();
    double yrec = lpRH.y();
    double zrec = lpRH.z();
    
    bool foundmatch = false;
    double dclose =1000.;
    double d      = 0.;
    multimap<CSCDetId , CSCRecHit2D>::iterator segRHit;
    segRHit = SegRechits.find(idRH);
    if (segRHit != SegRechits.end())
      {
	for( ; segRHit != SegRechits.upper_bound(idRH); ++segRHit)
	  {
	    
	    LocalPoint lposRH = (segRHit->second).localPosition();
	    double xpos = lposRH.x();
	    double ypos = lposRH.y();
	    double zpos = lposRH.z();
	    
	    if ( xrec == xpos && yrec == ypos && zrec == zpos){foundmatch = true;}
	    d = sqrt(pow(xrec-xpos,2)+pow(yrec-ypos,2)+pow(zrec-zpos,2));
	    if (d < dclose) dclose = d;
	  }
      }
    if(!foundmatch)
      {
	NonAssociatedRechits.insert(std::pair<CSCDetId , CSCRecHit2D>(idRH,allRHiter->second));
	distRHvec.push_back(dclose);
      }
  }
  
  counter = 0;
  for(unsigned int kk = 0; kk < distRHvec.size(); kk++){
    nonAssocRecHits_distToGoodRH[kk] = distRHvec[kk];
    counter++;
  }
  int tmpCounterRH = counter;
  

  counter=0;
  for(std::multimap<CSCDetId , CSCRecHit2D>::iterator iter =  NonAssociatedRechits.begin();iter != NonAssociatedRechits.end(); ++iter){
    CSCDetId idrec = iter->first;
    int kEndcap  = idrec.endcap();
    int cEndcap  = idrec.endcap();
    if (kEndcap == 2)cEndcap = -1;
    int kRing    = idrec.ring();
    int kStation = idrec.station();
    int kChamber = idrec.chamber();
    int kLayer   = idrec.layer();

    // Store rechit as a Local Point:
    LocalPoint rhitlocal = (iter->second).localPosition();  
    double xreco = rhitlocal.x();
    double yreco = rhitlocal.y();
    
    // Find the strip containing this hit
    int centerid    =  (iter->second).nStrips()/2;
    int centerStrip =  (iter->second).channels(centerid);

    // Find the charge associated with this hit
    double rHSumQ = 0;
    double sumsides=0.;
    int adcsize=(iter->second).nStrips()*(iter->second).nTimeBins();
    for ( unsigned int i=0; i< (iter->second).nStrips(); i++) {
      for ( unsigned int j=0; j< (iter->second).nTimeBins()-1; j++) {
	rHSumQ+=(iter->second).adcs(i,j);
	if (i!=1) sumsides+=(iter->second).adcs(i,j);
      }
    }

    double rHratioQ = sumsides/rHSumQ;
    if (adcsize != 12) rHratioQ = -99;

    // Get the signal timing of this hit
    double rHtime = (iter->second).tpeak()/50;

    // Get the width of this hit
    int rHwidth = getWidth(*strips, idrec, centerStrip);

    // Get pointer to the layer:
    const CSCLayer* csclayer = cscGeom->layer( idrec );

    // Transform hit position from local chamber geometry to global CMS geom
    GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
    double grecx   =  rhitglobal.x();
    double grecy   =  rhitglobal.y();

   // Simple occupancy variables
    int kCodeBroad  = cEndcap * ( 4*(kStation-1) + kRing) ;
    int kCodeNarrow = cEndcap * ( 100*(kRing-1) + kChamber) ;
    
    nonAssocRecHits_codeBroad[counter] = kCodeBroad;
    nonAssocRecHits_codeNarrow[counter] = kCodeNarrow;
    nonAssocRecHits_ID_layer[counter] = kLayer;
    nonAssocRecHits_ID_station[counter] = kStation;
    nonAssocRecHits_ID_ring[counter] = kRing;
    nonAssocRecHits_ID_chamber[counter] = kChamber;
    nonAssocRecHits_ID_endcap[counter] = kEndcap;
    nonAssocRecHits_globalX[counter] = grecx;
    nonAssocRecHits_globalY[counter] = grecy;
    nonAssocRecHits_localX[counter] = xreco;
    nonAssocRecHits_localY[counter] = yreco;
    nonAssocRecHits_ID_centerStrip[counter] = centerStrip;
    nonAssocRecHits_sumQ[counter] = rHSumQ;
    nonAssocRecHits_ratioSumQ[counter] = rHratioQ;
    nonAssocRecHits_Time[counter] = rHtime;
    nonAssocRecHits_width[counter] = rHwidth;
    counter++;
  }
  nonAssocRecHits_nNonAssocRH = counter;
  if (nonAssocRecHits_nNonAssocRH != tmpCounterRH) std::cout << "Non Assoc RH: nonAssocRH != distanceNonAssocRH  " << nonAssocRecHits_nNonAssocRH << "   " 
							     << tmpCounterRH << std::endl;  

  counter=0;
  for(std::multimap<CSCDetId , CSCRecHit2D>::iterator iter =  SegRechits.begin();iter != SegRechits.end(); ++iter){
    CSCDetId idrec = iter->first;
    int kEndcap  = idrec.endcap();
    int cEndcap  = idrec.endcap();
    if (kEndcap == 2)cEndcap = -1;
    int kRing    = idrec.ring();
    int kStation = idrec.station();
    int kChamber = idrec.chamber();
    int kLayer   = idrec.layer();
    
    // Store rechit as a Local Point:
    LocalPoint rhitlocal = (iter->second).localPosition();  
    double xreco = rhitlocal.x();
    double yreco = rhitlocal.y();
    
    // Find the strip containing this hit
    int centerid    =  (iter->second).nStrips()/2;
    int centerStrip =  (iter->second).channels(centerid);
    
    // Find the charge associated with this hit
    
    double rHSumQ = 0;
    double sumsides=0.;
    int adcsize=(iter->second).nStrips()*(iter->second).nTimeBins();
    for ( unsigned int i=0; i< (iter->second).nStrips(); i++) {
      for ( unsigned int j=0; j< (iter->second).nTimeBins()-1; j++) {
	rHSumQ+=(iter->second).adcs(i,j);
	if (i!=1) sumsides+=(iter->second).adcs(i,j);
      }
    }
    
    double rHratioQ = sumsides/rHSumQ;
    if (adcsize != 12) rHratioQ = -99;
    
    // Get the signal timing of this hit
    double rHtime = (iter->second).tpeak()/50;
    
    // Get the width of this hit
    int rHwidth = getWidth(*strips, idrec, centerStrip);
    
    
    // Get pointer to the layer:
    const CSCLayer* csclayer = cscGeom->layer( idrec );
    
    // Transform hit position from local chamber geometry to global CMS geom
    GlobalPoint rhitglobal= csclayer->toGlobal(rhitlocal);
    double grecx   =  rhitglobal.x();
    double grecy   =  rhitglobal.y();
    
    // Simple occupancy variables
    int kCodeBroad  = cEndcap * ( 4*(kStation-1) + kRing) ;
    int kCodeNarrow = cEndcap * ( 100*(kRing-1) + kChamber) ;
    
    
    assocRecHits_codeBroad[counter] = kCodeBroad;
    assocRecHits_codeNarrow[counter] = kCodeNarrow;
    assocRecHits_ID_layer[counter] = kLayer;
    assocRecHits_ID_station[counter] = kStation;
    assocRecHits_ID_ring[counter] = kRing;
    assocRecHits_ID_chamber[counter] = kChamber;
    assocRecHits_ID_endcap[counter] = kEndcap;
    assocRecHits_globalX[counter] = grecx;
    assocRecHits_globalY[counter] = grecy;
    assocRecHits_localX[counter] = xreco;
    assocRecHits_localY[counter] = yreco;
    assocRecHits_ID_centerStrip[counter] = centerStrip;
    assocRecHits_sumQ[counter] = rHSumQ;
    assocRecHits_ratioSumQ[counter] = rHratioQ;
    assocRecHits_Time[counter] = rHtime;
    assocRecHits_width[counter] = rHwidth;
    counter++;
    	   
   }
  assocRecHits_nAssocRH = counter;
  
  distRHvec.clear();
  AllRechits.clear();
  SegRechits.clear();
  NonAssociatedRechits.clear();



}




//-------------------------------------------------------------------------------------
// Fits a straight line to a set of 5 points with errors.  Functions assumes 6 points
// and removes hit in layer 3.  It then returns the expected position value in layer 3
// based on the fit.
//-------------------------------------------------------------------------------------
double CSCRootMaker::fitX(CLHEP::HepMatrix points, CLHEP::HepMatrix errors){

  double S   = 0;
  double Sx  = 0;
  double Sy  = 0;
  double Sxx = 0;
  double Sxy = 0;
  double sigma2 = 0;

  for (int i=1;i<7;i++){
    if (i != 3){
      sigma2 = errors(i,1)*errors(i,1);
      S = S + (1/sigma2);
      Sy = Sy + (points(i,1)/sigma2);
      Sx = Sx + ((i)/sigma2);
      Sxx = Sxx + (i*i)/sigma2;
      Sxy = Sxy + (((i)*points(i,1))/sigma2);
    }
  }

  double delta = S*Sxx - Sx*Sx;
  double intercept = (Sxx*Sy - Sx*Sxy)/delta;
  double slope = (S*Sxy - Sx*Sy)/delta;

  //double chi = 0;
  //double chi2 = 0;

  // calculate chi2 (not currently used)
  //for (int i=1;i<7;i++){
  //  chi = (points(i,1) - intercept - slope*i)/(errors(i,1));
  //  chi2 = chi2 + chi*chi;
  //}

  return (intercept + slope*3);

}


void
CSCRootMaker::doTrigger(edm::Handle<L1MuGMTReadoutCollection> pCollection, edm::Handle<edm::TriggerResults> hlt)
{

  // L1 Global Muon Trigger
  std::vector<L1MuGMTReadoutRecord> L1Mrec = pCollection->getRecords();
  std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
  
  l1Trigger_CSC  = false;
  l1Trigger_DT   = false;
  l1Trigger_RPCForward = false;
  l1Trigger_RPCBarrel = false;
  l1Trigger_beamHalo = false;
  l1GMT_BXN = -1;

  
  for(igmtrr=L1Mrec.begin(); igmtrr!=L1Mrec.end(); igmtrr++) {
    std::vector<L1MuRegionalCand>::const_iterator iter1;
    std::vector<L1MuRegionalCand> rmc;
    
    // CSC
    int icsc = 0;
    rmc = igmtrr->getCSCCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
	icsc++;
	int kQuality = (*iter1).quality();   // kQuality = 1 means beam halo
	if (kQuality == 1) l1Trigger_beamHalo = true;
      }
    }
    if (igmtrr->getBxInEvent() == 0 && icsc>0)
      {
	l1GMT_BXN = igmtrr->getBxNr();
	if (icsc>0) l1Trigger_CSC = true;
      }
    
    // DT
    int idt = 0;
    rmc = igmtrr->getDTBXCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
	idt++;
      }
    }
    if(igmtrr->getBxInEvent()==0 && idt>0) l1Trigger_DT = true;
    
    // RPC Barrel
    int irpcb = 0;
    rmc = igmtrr->getBrlRPCCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
	irpcb++;
      }
    }
    if(igmtrr->getBxInEvent()==0 && irpcb>0) l1Trigger_RPCBarrel = true;
    
    // RPC Forward
    int irpcf = 0;
    rmc = igmtrr->getFwdRPCCands();
    for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
      if ( !(*iter1).empty() ) {
	irpcf++;
      }
    }
    if(igmtrr->getBxInEvent()==0 && irpcf>0) l1Trigger_RPCForward = true;
    
  }
  
  // HLT Trigger
  counter = 0;
  int hltSize = hlt->size();
  for (int i = 0; i < hltSize; ++i)
    {
      if (hlt->accept(i)){
	hltTrigger_bits[counter] = i;
	counter++;
      }
    }
  hltTrigger_nBits = counter;



}


void 
CSCRootMaker::doStripDigis(edm::Handle<CSCStripDigiCollection> strips, edm::ESHandle<CSCGeometry> cscGeom)
{
  int nStripsFired = 0;
  for (CSCStripDigiCollection::DigiRangeIterator dSDiter=strips->begin(); dSDiter!=strips->end(); dSDiter++) {
    CSCDetId id = (CSCDetId)(*dSDiter).first;
    const CSCLayer* csclayer = cscGeom->layer( id );
    CSCLayerGeometry *thegeom = const_cast<CSCLayerGeometry*>(csclayer->geometry());

    std::vector<CSCStripDigi>::const_iterator stripIter = (*dSDiter).second.first;
    std::vector<CSCStripDigi>::const_iterator lStrip = (*dSDiter).second.second;
    for( ; stripIter != lStrip; ++stripIter) 
      {
	int myStrip = stripIter->getStrip();
	std::vector<int> myADCVals = stripIter->getADCCounts();
	bool thisStripFired = false;
	double thisPedestal = 0.5*(double)(myADCVals[0]+myADCVals[1]);
	double threshold = 13.3 ;
	double diff = 0.;
	double thisSignal = (1./6)*(myADCVals[2]+myADCVals[3]+myADCVals[4]+myADCVals[5]+myADCVals[6]+myADCVals[7]);
	
	if(id.station() == 1 && id.ring() == 4)
	  {
	    if(myStrip <= 16) myStrip += 64; 
	  }
	
	int tracker = 0;
	for (unsigned int iCount = 0; iCount < myADCVals.size(); iCount++) {
	  diff = (double)myADCVals[iCount]-thisPedestal;
	  if (diff > threshold) { thisStripFired = true; }
	if (iCount > 0 && diff > (myADCVals[iCount-1]-thisPedestal)) {tracker = iCount;}
	
	}
	if (thisStripFired) {
	  
	  firedStripDigis_ID_endcap[nStripsFired] = id.endcap();
	  firedStripDigis_ID_ring[nStripsFired] = id.ring();
	  firedStripDigis_ID_station[nStripsFired] = id.station();
	  firedStripDigis_ID_layer[nStripsFired] = id.layer();
	  firedStripDigis_ID_chamber[nStripsFired] = id.chamber();
	  firedStripDigis_ID_strip[nStripsFired] = myStrip;
	  double ADC = thisSignal - thisPedestal;
	  firedStripDigis_ADCTotal[nStripsFired] = ADC;
	  firedStripDigis_ADCMax[nStripsFired] = myADCVals[tracker];
	  firedStripDigis_tbinMax[nStripsFired] = tracker;
	  firedStripDigis_localX[nStripsFired] = thegeom->xOfStrip(myStrip);

	  nStripsFired++;
	}
      }

  } // end strip loop
  
  if (nStripsFired == 0) nStripsFired = -1;
  firedStripDigis_nStripDigis = nStripsFired;
//cout << "nStrip: "   << nStripsFired << endl;
}


void 
CSCRootMaker::doWireDigis(edm::Handle<CSCWireDigiCollection> wires, edm::ESHandle<CSCGeometry> cscGeom)
{

  int nWireGroupsTotal = 0;
  for (CSCWireDigiCollection::DigiRangeIterator dWDiter=wires->begin(); dWDiter!=wires->end(); dWDiter++) {
    CSCDetId id = (CSCDetId)(*dWDiter).first;
    const CSCLayer* csclayer = cscGeom->layer( id );
    CSCLayerGeometry *thegeom = const_cast<CSCLayerGeometry*>(csclayer->geometry());
    std::vector<CSCWireDigi>::const_iterator wireIter = (*dWDiter).second.first;
    std::vector<CSCWireDigi>::const_iterator lWire = (*dWDiter).second.second;
    for( ; wireIter != lWire; ++wireIter) {
      int myWire = wireIter->getWireGroup();
      int myTBin = wireIter->getTimeBin();
      firedWireDigis_ID_endcap[nWireGroupsTotal] = id.endcap();
      firedWireDigis_ID_station[nWireGroupsTotal] = id.station();
      firedWireDigis_ID_ring[nWireGroupsTotal] = id.ring();
      firedWireDigis_ID_layer[nWireGroupsTotal] = id.layer();
      firedWireDigis_ID_chamber[nWireGroupsTotal] = id.chamber();

      firedWireDigis_ID_wire[nWireGroupsTotal] = myWire;
      firedWireDigis_timeBin[nWireGroupsTotal] = myTBin;
      firedWireDigis_chamberSerial[nWireGroupsTotal] = chamberSerial(id);
      firedWireDigis_localY[nWireGroupsTotal] = thegeom->yOfWireGroup(myWire);


      //AFEB Timing
      int nmbwiretbin = wireIter->getTimeBinsOn().size();
      int afeb = 3*((myWire-1)/8)+(id.layer()+1)/2;
      firedWireDigis_numberWireTimeBins[nWireGroupsTotal] = nmbwiretbin;
      firedWireDigis_AFEB[nWireGroupsTotal] = afeb;

      nWireGroupsTotal++;
    }
  } // end wire loop

  // this way you can zero suppress but still store info on # events with no digis
  if (nWireGroupsTotal == 0) nWireGroupsTotal = -1;
  firedWireDigis_nWireDigis = nWireGroupsTotal;
//cout << "nwire: " << nWireGroupsTotal << endl;
}




//---------------------------------------------------------------------------
// Module for looking at Comparitor Timing
// Author N. Terentiev
//---------------------------------------------------------------------------
void CSCRootMaker::doCompTiming(const CSCComparatorDigiCollection& compars) {

  int strip,tbin,cfeb;//,idlayer,idchamber;
  //int channel=0; // for  CSCIndexer::dbIndex(id, channel); irrelevant here
  CSCIndexer indexer;
  counter = 0;
  if(compars.begin() != compars.end())  {
    
    //std::cout<<std::endl;
    //std::cout<<"Event "<<nEventsAnalyzed<<std::endl;
    //std::cout<<std::endl;
    
       // cycle on comparators collection for all CSC
    CSCComparatorDigiCollection::DigiRangeIterator compdetUnitIt;
    for(compdetUnitIt=compars.begin();compdetUnitIt!=compars.end(); ++compdetUnitIt) 
      {
	const CSCDetId id = (*compdetUnitIt).first;
	//idlayer=indexer.dbIndex(id, channel); // channel irrelevant here
	//idchamber=idlayer/10;
	
	// looping in the layer of given CSC
	const CSCComparatorDigiCollection::Range& range = (*compdetUnitIt).second;
	for(CSCComparatorDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt)
	  {
	    strip=(*digiIt).getStrip();
	    /*
	      if(id.station()==1 && (id.ring()==1 || id.ring()==4))
	      std::cout<<idchamber<<" "<<id.station()<<" "<<id.ring()<<" "
	      <<strip <<std::endl;  
	    */
	    indexer.dbIndex(id, strip); // strips 1-16 of ME1/1a 
	    // become strips 65-80 of ME1/1 
	    tbin=(*digiIt).getTimeBin();
	    cfeb=(strip-1)/16+1;
	    
	    comparatorDigis_ID_endcap[counter] = id.endcap();
	    comparatorDigis_ID_chamber[counter] = id.chamber();
	    comparatorDigis_ID_station[counter] = id.station();
	    comparatorDigis_ID_strip[counter] = strip;
            comparatorDigis_ID_halfStrip[counter] = (*digiIt).getHalfStrip();
	    comparatorDigis_ID_layer[counter] = id.layer(); //idlayer?
	    comparatorDigis_ID_ring[counter] = id.ring();
	    comparatorDigis_cfeb[counter] = cfeb;
	    comparatorDigis_timeBin[counter] = tbin;
	    counter++;
	    
	  }     // end of digis loop in layer
      } // end of collection loop
  } // end of      if(compars.begin() !=compars.end())
  comparatorDigis_nDigis = counter;     
}


//---------------------------------------------------------------------------------------
// Construct histograms for monitoring the trigger and offline timing
// Author: A. Deisher
//---------------------------------------------------------------------------------------

void CSCRootMaker::doLCTDigis( edm::Handle<CSCALCTDigiCollection> alcts, edm::Handle<CSCCLCTDigiCollection> clcts,
			        edm::Handle<CSCCorrelatedLCTDigiCollection> correlatedlcts,
			        edm::Handle<L1MuGMTReadoutCollection> pCollection, edm::ESHandle<CSCGeometry> cscGeom, 
			        const edm::EventSetup& eventSetup, const edm::Event &event){


  // *************************************************
  // *** ALCT Digis **********************************
  // *************************************************
  
  int n_alcts = 0;
  map<CSCDetId, int > ALCT_KeyWG_map; //structure for storing the key wire group for the first ALCT for each chamber
  for (CSCALCTDigiCollection::DigiRangeIterator j=alcts->begin(); j!=alcts->end(); j++) {
    const CSCALCTDigiCollection::Range& range =(*j).second;
    const CSCDetId& idALCT = (*j).first;
    for (CSCALCTDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt){
      // Valid digi in the chamber (or in neighbouring chamber)  
      if((*digiIt).isValid()){
	alct_BX[n_alcts] = (*digiIt).getBX();
	alct_fullBX[n_alcts] = (*digiIt).getFullBX();

	//if we don't already have digi information stored for this chamber, then we fill it
	if (ALCT_KeyWG_map.find(idALCT.chamberId()) == ALCT_KeyWG_map.end()){
	  ALCT_KeyWG_map[idALCT.chamberId()] = (*digiIt).getKeyWG();
	  //printf("I did fill ALCT info for Chamber %d %d %d %d \n",idALCT.chamberId().endcap(), idALCT.chamberId().station(), idALCT.chamberId().ring(), idALCT.chamberId().chamber());
	}
	alct_ID_chamber[n_alcts] = idALCT.chamber();
	alct_ID_endcap[n_alcts] = idALCT.endcap();
	alct_ID_station[n_alcts] = idALCT.station();
	alct_ID_ring[n_alcts] = idALCT.ring();
	alct_ID_layer[n_alcts] = idALCT.layer();
	alct_ID_chamberID[n_alcts] = idALCT.chamberId();
	alct_ID_chamberSerial[n_alcts] = chamberSerial(idALCT);
	alct_ID_ringSerial[n_alcts] = ringSerial(idALCT);
	n_alcts++;
      }
    }
  }
  alct_nAlcts = n_alcts;

  // *************************************************
  // *** CLCT Digis **********************************
  // *************************************************
  int n_clcts = 0;
  map<CSCDetId, int > CLCT_getFullBx_map; //structure for storing the pretrigger bxn for the first CLCT for each chamber
  for (CSCCLCTDigiCollection::DigiRangeIterator j=clcts->begin(); j!=clcts->end(); j++) {
    const CSCCLCTDigiCollection::Range& range =(*j).second;
    const CSCDetId& idCLCT = (*j).first;
    for (CSCCLCTDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt){
      // Valid digi in the chamber (or in neighbouring chamber) 
      if((*digiIt).isValid()){
        clct_BX[n_clcts] = (*digiIt).getBX();
        clct_fullBX[n_clcts] = (*digiIt).getFullBX();

 	//if we don't already have digi information stored for this chamber, then we fill it
	if (CLCT_getFullBx_map.find(idCLCT.chamberId()) == CLCT_getFullBx_map.end()){
	  CLCT_getFullBx_map[idCLCT.chamberId()] = (*digiIt).getFullBX();
	  //printf("I did fill CLCT info for Chamber %d %d %d %d \n",idCLCT.chamberId().endcap(), idCLCT.chamberId().station(), idCLCT.chamberId().ring(), idCLCT.chamberId().chamber());
	}
        clct_ID_chamber[n_clcts] = idCLCT.chamber();
        clct_ID_endcap[n_clcts] = idCLCT.endcap();
        clct_ID_station[n_clcts] = idCLCT.station();
        clct_ID_ring[n_clcts] = idCLCT.ring();
        clct_ID_layer[n_clcts] = idCLCT.layer();
        clct_ID_chamberID[n_clcts] = idCLCT.chamberId();
	clct_ID_chamberSerial[n_clcts] = chamberSerial(idCLCT);
        clct_ID_ringSerial[n_clcts] = ringSerial(idCLCT);
	n_clcts++;
      }
    }
  }
  clct_nClcts = n_clcts;  
  // *************************************************
  // *** CorrelatedLCT Digis *************************
  // *************************************************
  int n_correlatedlcts = 0;
  for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator j=correlatedlcts->begin(); j!=correlatedlcts->end(); j++) {
    const CSCCorrelatedLCTDigiCollection::Range& range =(*j).second;
    const CSCDetId& idLCT = (*j).first;
    for (CSCCorrelatedLCTDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt){
      if((*digiIt).isValid()){
	
	correlatedLct_BX[n_correlatedlcts] = (*digiIt).getBX();
	correlatedLct_BX0[n_correlatedlcts] = (*digiIt).getBX0();
	correlatedLct_trkNumber[n_correlatedlcts] = (*digiIt).getTrknmb();
	correlatedLct_quality[n_correlatedlcts] = (*digiIt).getQuality();
	correlatedLct_strip[n_correlatedlcts] = (*digiIt).getStrip();
	correlatedLct_pattern[n_correlatedlcts] = (*digiIt).getPattern();
	correlatedLct_bend[n_correlatedlcts] = (*digiIt).getBend();
	correlatedLct_keyWG[n_correlatedlcts] = (*digiIt).getKeyWG();
	correlatedLct_CLCTPattern[n_correlatedlcts] = (*digiIt).getCLCTPattern();
	correlatedLct_cscID[n_correlatedlcts] = (*digiIt).getCSCID();
	correlatedLct_syncErr[n_correlatedlcts] = (*digiIt).getSyncErr();

	correlatedLct_ID_chamber[n_correlatedlcts] = idLCT.chamber();
	correlatedLct_ID_ring[n_correlatedlcts] = idLCT.ring();
	correlatedLct_ID_station[n_correlatedlcts] = idLCT.station();
	correlatedLct_ID_endcap[n_correlatedlcts] = idLCT.endcap();
	correlatedLct_ID_layer[n_correlatedlcts] = idLCT.layer();
        correlatedLct_ID_chamberSerial[n_correlatedlcts] = chamberSerial(idLCT);
        correlatedLct_ID_ringSerial[n_correlatedlcts] = ringSerial(idLCT);

	n_correlatedlcts++;
      }
    }
  }
  correlatedLct_nLcts = n_correlatedlcts;

  // *******************************************************************
  // Get information from the TMB header.  
  // Can this eventually come out of the digis?
  // Taking code from EventFilter/CSCRawToDigis/CSCDCCUnpacker.cc
  // *******************************************************************
  
  edm::ESHandle<CSCCrateMap> hcrate;
  eventSetup.get<CSCCrateMapRcd>().get(hcrate); 
  const CSCCrateMap* pcrate = hcrate.product();
  
  /// Get a handle to the FED data collection
  edm::Handle<FEDRawDataCollection> rawdata;
  event.getByToken(fedRawTagSrc, rawdata);
  bool goodEvent = false;
  // If set selective unpacking mode 
  // hardcoded examiner mask below to check for DCC and DDU level errors will be used first
  // then examinerMask for CSC level errors will be used during unpacking of each CSC block
  unsigned long dccBinCheckMask = 0x06080016;
  unsigned int examinerMask = 0x1FEBF3F6; 
  unsigned int errorMask = 0x0;

  counter=0;
  for (int id=FEDNumbering::MINCSCFEDID; id<=FEDNumbering::MAXCSCFEDID; ++id) {
    // loop over DCCs
    /// uncomment this for regional unpacking
    /// if (id!=SOME_ID) continue;
    
    /// Take a reference to this FED's data
    const FEDRawData& fedData = rawdata->FEDData(id);
    unsigned long length =  fedData.size();
    
    if (length>=32){ ///if fed has data then unpack it
      CSCDCCExaminer* examiner = NULL;
      std::stringstream examiner_out, examiner_err;
      goodEvent = true;
      ///examine event for integrity
      //CSCDCCExaminer examiner;
      examiner = new CSCDCCExaminer();
//      examiner->output1().redirect(examiner_out);
//      examiner->output2().redirect(examiner_err);
      if( examinerMask&0x40000 ) examiner->crcCFEB(1);
      if( examinerMask&0x8000  ) examiner->crcTMB (1);
      if( examinerMask&0x0400  ) examiner->crcALCT(1);
//      examiner->output1().show();
//      examiner->output2().show();
      examiner->setMask(examinerMask);
      const short unsigned int *data = (short unsigned int *)fedData.data();
     
      if( examiner->check(data,long(fedData.size()/2)) < 0 )	{
  	goodEvent=false;
      } 
      else {	  
  	goodEvent=!(examiner->errors()&dccBinCheckMask);
      }  
      
      if (goodEvent) {
  	///get a pointer to data and pass it to constructor for unpacking
  	CSCDCCExaminer * ptrExaminer = examiner;
  	CSCDCCEventData dccData((short unsigned int *) fedData.data(),ptrExaminer);
     	
  	///get a reference to dduData
  	const std::vector<CSCDDUEventData> & dduData = dccData.dduData();
     	
  	/// set default detid to that for E=+z, S=1, R=1, C=1, L=1
  	CSCDetId layer(1, 1, 1, 1, 1);
  	
  	for (unsigned int iDDU=0; iDDU<dduData.size(); ++iDDU) {  // loop over DDUs
  	  /// skip the DDU if its data has serious errors
  	  /// define a mask for serious errors 
  	  if (dduData[iDDU].trailer().errorstat()&errorMask) {
  	    LogTrace("CSCDCCUnpacker|CSCRawToDigi") << "DDU# " << iDDU << " has serious error - no digis unpacked! " <<
  	      std::hex << dduData[iDDU].trailer().errorstat();
  	    continue; // to next iteration of DDU loop
  	  }
  	  
  	  ///get a reference to chamber data
  	  const std::vector<CSCEventData> & cscData = dduData[iDDU].cscData();
  	  for (unsigned int iCSC=0; iCSC<cscData.size(); ++iCSC) { // loop over CSCs
  	    
  	    int vmecrate = cscData[iCSC].dmbHeader()->crateID();
  	    int dmb = cscData[iCSC].dmbHeader()->dmbID();
  	    
  	    ///adjust crate numbers for MTCC data
  	    // SKIPPING MTCC redefinition of vmecrate
  	    
  	    int icfeb = 0;  /// default value for all digis not related to cfebs
  	    int ilayer = 0; /// layer=0 flags entire chamber
  	    
  	    if ((vmecrate>=1)&&(vmecrate<=60) && (dmb>=1)&&(dmb<=10)&&(dmb!=6)) {
  	      layer = pcrate->detId(vmecrate, dmb,icfeb,ilayer );
  	    } 
  	    else{
	      LogTrace ("CSCTimingAlignment|CSCDCCUnpacker|CSCRawToDigi") << " detID input out of range!!! ";
              LogTrace ("CSCTimingAlignment|CSCDCCUnpacker|CSCRawToDigi")
                << " skipping chamber vme= " << vmecrate << " dmb= " << dmb;
	      continue; // to next iteration of iCSC loop
	    }
	    
  	    /// check alct data integrity 
  	    int nalct = cscData[iCSC].dmbHeader()->nalct();
  	    bool goodALCT=false;
  	    //if (nalct&&(cscData[iCSC].dataPresent>>6&0x1)==1) {
  	    if (nalct&&cscData[iCSC].alctHeader()) {  
  	      if (cscData[iCSC].alctHeader()->check()){
  		goodALCT=true;
  	      }
  	    }
  	    
  	    ///check tmb data integrity
  	    int nclct = cscData[iCSC].dmbHeader()->nclct();
  	    bool goodTMB=false;
  	    if (nclct&&cscData[iCSC].tmbData()) {
  	      if (cscData[iCSC].tmbHeader()->check()){
  		if (cscData[iCSC].clctData()->check()) goodTMB=true; 
  	      }
  	    }  
      	      
  	    if (goodTMB && goodALCT) { 

	      if (ALCT_KeyWG_map.find(layer) == ALCT_KeyWG_map.end()) {
		//printf("no ALCT info for Chamber %d %d %d %d \n",layer.endcap(), layer.station(), layer.ring(), layer.chamber());
		continue;
	      }
	      if (CLCT_getFullBx_map.find(layer) == CLCT_getFullBx_map.end()) {
		//printf("no CLCT info for Chamber %d %d %d %d \n",layer.endcap(), layer.station(), layer.ring(), layer.chamber());
		continue;
	      }
	      int ALCT0Key = ALCT_KeyWG_map.find(layer)->second;
	      int CLCTPretrigger = CLCT_getFullBx_map.find(layer)->second;

  	      const CSCTMBHeader *tmbHead = cscData[iCSC].tmbHeader();

	      tmb_BXNCount[counter] = tmbHead->BXNCount();
	      tmb_ALCTMatchTime[counter] = tmbHead->ALCTMatchTime();
	      tmb_ID_chamberID[counter] = layer.chamberId();
	      tmb_ID_chamber[counter] = layer.chamber();
	      tmb_ID_endcap[counter] = layer.endcap();
	      tmb_ID_station[counter] = layer.station();
	      tmb_ID_ring[counter] = layer.ring();
	      tmb_ID_layer[counter] = layer.layer();
	      tmb_ID_chamberSerial[counter] = chamberSerial(layer.chamberId());
	      tmb_ID_ringSerial[counter] = ringSerial(layer.chamberId());
	      tmb_alct0key[counter] = ALCT0Key;

	      //Attempt to make a few sum plots
	      int TMB_ALCT_rel_L1A = tmbHead->BXNCount()-(CLCTPretrigger+2+tmbHead->ALCTMatchTime());
	      if (TMB_ALCT_rel_L1A > 3563)
		TMB_ALCT_rel_L1A = TMB_ALCT_rel_L1A - 3564;
	      if (TMB_ALCT_rel_L1A < 0)
		TMB_ALCT_rel_L1A = TMB_ALCT_rel_L1A + 3564;

	      tmb_alctRelL1A[counter] = TMB_ALCT_rel_L1A;
	      counter++;
	    }

  	  } // end CSCData loop
  	} // end ddu data loop
      } // end if goodEvent
      if (examiner!=NULL) delete examiner;
    }// end if non-zero fed data
  } // end DCC loop for NON-REFERENCE
  tmb_nTmb = counter;

}




// ==============================================
//
// look at Calibrations
//
// ==============================================

void CSCRootMaker::doCalibrations(const edm::EventSetup& eventSetup){

  // Only do this for the first event
  // get the gains
  edm::ESHandle<CSCDBGains> hGains;
  eventSetup.get<CSCDBGainsRcd>().get( hGains );
  const CSCDBGains* pGains = hGains.product();
  // get the crosstalks
  edm::ESHandle<CSCDBCrosstalk> hCrosstalk;
  eventSetup.get<CSCDBCrosstalkRcd>().get( hCrosstalk );
  const CSCDBCrosstalk* pCrosstalk = hCrosstalk.product();
  // get the noise matrix
  edm::ESHandle<CSCDBNoiseMatrix> hNoiseMatrix;
  eventSetup.get<CSCDBNoiseMatrixRcd>().get( hNoiseMatrix );
  const CSCDBNoiseMatrix* pNoiseMatrix = hNoiseMatrix.product();
  // get pedestals
  edm::ESHandle<CSCDBPedestals> hPedestals;
  eventSetup.get<CSCDBPedestalsRcd>().get( hPedestals );
  const CSCDBPedestals* pPedestals = hPedestals.product();

  for (int i = 0; i < 400; i++)
    {
      calibrations_Gain_slope[i] = pGains->gains[i].gain_slope;
      calibrations_XT_slope_left[i] = pCrosstalk->crosstalk[i].xtalk_slope_left;
      calibrations_XT_slope_right[i] = pCrosstalk->crosstalk[i].xtalk_slope_right;
      calibrations_XT_intercept_left[i] = pCrosstalk->crosstalk[i].xtalk_intercept_left;
      calibrations_XT_intercept_right[i] = pCrosstalk->crosstalk[i].xtalk_intercept_right;
      calibrations_Pedestals_ped[i] = pPedestals->pedestals[i].ped;
      calibrations_Pedestals_rms[i] = pPedestals->pedestals[i].rms;
      calibrations_NoiseMatrix_33[i] = pNoiseMatrix->matrix[i].elem33;
      calibrations_NoiseMatrix_34[i] = pNoiseMatrix->matrix[i].elem34;
      calibrations_NoiseMatrix_35[i] = pNoiseMatrix->matrix[i].elem35;
      calibrations_NoiseMatrix_44[i] = pNoiseMatrix->matrix[i].elem44;
      calibrations_NoiseMatrix_45[i] = pNoiseMatrix->matrix[i].elem45;
      calibrations_NoiseMatrix_46[i] = pNoiseMatrix->matrix[i].elem46;
      calibrations_NoiseMatrix_55[i] = pNoiseMatrix->matrix[i].elem55;
      calibrations_NoiseMatrix_56[i] = pNoiseMatrix->matrix[i].elem56;
      calibrations_NoiseMatrix_57[i] = pNoiseMatrix->matrix[i].elem57;
      calibrations_NoiseMatrix_66[i] = pNoiseMatrix->matrix[i].elem66;
      calibrations_NoiseMatrix_67[i] = pNoiseMatrix->matrix[i].elem67;
      calibrations_NoiseMatrix_77[i] = pNoiseMatrix->matrix[i].elem77;

    }
  calibrations_nCalib = 400;


}




//---------------------------------------------------------------------------
// Module for looking at gas gains
// Author N. Terentiev
//---------------------------------------------------------------------------
void CSCRootMaker::doGasGain(const CSCWireDigiCollection& wirecltn,  const CSCStripDigiCollection&   strpcltn,
			       const CSCRecHit2DCollection& rechitcltn) {
  int channel=0,mult,wire,layer,idlayer;//,idchamber;
  int wire_strip_rechit_present;
  std::string name,title,endcapstr;
  ostringstream ss;
  CSCIndexer indexer;
  std::map<int,int>::iterator intIt;
  
  m_single_wire_layer.clear();
  
  if(nEventsTotal == 1) {
    
    // HV segments, their # and location in terms of wire groups
    
    m_wire_hvsegm.clear();
    std::map<int,std::vector<int> >::iterator intvecIt;
    //                    ME1a ME1b ME1/2 ME1/3 ME2/1 ME2/2 ME3/1 ME3/2 ME4/1 ME4/2 
    int csctype[10]=     {1,   2,   3,    4,    5,    6,    7,    8,    9,    10};
    int hvsegm_layer[10]={1,   1,   3,    3,    3,    5,    3,    5,    3,    5};
    int id;
    nmbhvsegm.clear();
    for(int i=0;i<10;i++) nmbhvsegm.push_back(hvsegm_layer[i]);
    // For ME1/1a
    std::vector<int> zer_1_1a(49,0);
    id=csctype[0];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_1_1a;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=48;wire++)  intvecIt->second[wire]=1;  // Segment 1
    
    // For ME1/1b
    std::vector<int> zer_1_1b(49,0);
    id=csctype[1];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_1_1b;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=48;wire++)  intvecIt->second[wire]=1;  // Segment 1
    
    // For ME1/2
    std::vector<int> zer_1_2(65,0);
    id=csctype[2];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_1_2;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=24;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=25;wire<=48;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=49;wire<=64;wire++) intvecIt->second[wire]=3;  // Segment 3
    
    // For ME1/3
    std::vector<int> zer_1_3(33,0);
    id=csctype[3];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_1_3;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=12;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=13;wire<=22;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=23;wire<=32;wire++) intvecIt->second[wire]=3;  // Segment 3
    
    // For ME2/1
    std::vector<int> zer_2_1(113,0);
    id=csctype[4];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_2_1;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=44;wire++)   intvecIt->second[wire]=1;  // Segment 1
    for(int wire=45;wire<=80;wire++)  intvecIt->second[wire]=2;  // Segment 2
    for(int wire=81;wire<=112;wire++) intvecIt->second[wire]=3;  // Segment 3
    
    // For ME2/2
    std::vector<int> zer_2_2(65,0);
    id=csctype[5];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_2_2;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=16;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=17;wire<=28;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=29;wire<=40;wire++) intvecIt->second[wire]=3;  // Segment 3
    for(int wire=41;wire<=52;wire++) intvecIt->second[wire]=4;  // Segment 4
    for(int wire=53;wire<=64;wire++) intvecIt->second[wire]=5;  // Segment 5
    
    // For ME3/1
    std::vector<int> zer_3_1(97,0);
    id=csctype[6];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_3_1;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=32;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=33;wire<=64;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=65;wire<=96;wire++) intvecIt->second[wire]=3;  // Segment 3
    
    // For ME3/2
    std::vector<int> zer_3_2(65,0);
    id=csctype[7];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_3_2;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=16;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=17;wire<=28;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=29;wire<=40;wire++) intvecIt->second[wire]=3;  // Segment 3
    for(int wire=41;wire<=52;wire++) intvecIt->second[wire]=4;  // Segment 4
    for(int wire=53;wire<=64;wire++) intvecIt->second[wire]=5;  // Segment 5
    
    // For ME4/1
    std::vector<int> zer_4_1(97,0);
    id=csctype[8];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_4_1;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=32;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=33;wire<=64;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=65;wire<=96;wire++) intvecIt->second[wire]=3;  // Segment 3
    
    // For ME4/2
    std::vector<int> zer_4_2(65,0);
    id=csctype[9];
    if(m_wire_hvsegm.find(id) == m_wire_hvsegm.end()) m_wire_hvsegm[id]=zer_4_2;
    intvecIt=m_wire_hvsegm.find(id);
    for(int wire=1;wire<=16;wire++)  intvecIt->second[wire]=1;  // Segment 1
    for(int wire=17;wire<=28;wire++) intvecIt->second[wire]=2;  // Segment 2
    for(int wire=29;wire<=40;wire++) intvecIt->second[wire]=3;  // Segment 3
    for(int wire=41;wire<=52;wire++) intvecIt->second[wire]=4;  // Segment 4
    for(int wire=53;wire<=64;wire++) intvecIt->second[wire]=5;  // Segment 5
    
  } // end of if(nEventsAnalyzed==1)
  
  
  // are wires, strips and rechits present?
  wire_strip_rechit_present=0;
  if(wirecltn.begin() != wirecltn.end()) wire_strip_rechit_present = wire_strip_rechit_present+1;
  if(strpcltn.begin() != strpcltn.end()) wire_strip_rechit_present = wire_strip_rechit_present+2;
  if(rechitcltn.begin() != rechitcltn.end()) wire_strip_rechit_present = wire_strip_rechit_present+4;
  
  if(wire_strip_rechit_present==7) {
    
    
    // cycle on wire collection for all CSC to select single wire hit layers
    CSCWireDigiCollection::DigiRangeIterator wiredetUnitIt;
    for(wiredetUnitIt=wirecltn.begin();wiredetUnitIt!=wirecltn.end(); ++wiredetUnitIt) 
      {
	const CSCDetId id = (*wiredetUnitIt).first;
	idlayer=indexer.dbIndex(id, channel);
	//idchamber=idlayer/10;
	layer=id.layer();
	// looping in the layer of given CSC
	mult=0; wire=0; 
	const CSCWireDigiCollection::Range& range = (*wiredetUnitIt).second;
	for(CSCWireDigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt){
	  wire=(*digiIt).getWireGroup();
	  mult++;
	}     // end of digis loop in layer
	
	// select layers with single wire hit
	if(mult==1) {
	  if(m_single_wire_layer.find(idlayer) == m_single_wire_layer.end())
	    m_single_wire_layer[idlayer]=wire;
	} // end of if(mult==1)
      }   // end of cycle on detUnit
    
    // Looping thru rechit collection
    CSCRecHit2DCollection::const_iterator recIt;
    CSCRecHit2D::ADCContainer m_adc;
    
    for(recIt = rechitcltn.begin(); recIt != rechitcltn.end(); ++recIt) 
      {
	CSCDetId id = (CSCDetId)(*recIt).cscDetId();
	idlayer=indexer.dbIndex(id, channel);
	//idchamber=idlayer/10;
	layer=id.layer();
	// select layer with single wire rechit
	if(m_single_wire_layer.find(idlayer) != m_single_wire_layer.end()) 
	  {
	    
	    if(recIt->nStrips()==3)  {        
	      // get 3X3 ADC Sum
	      unsigned int binmx=0;
	      double adcmax=0.0;
	      
	      for(unsigned int i=0;i<recIt->nStrips();i++) 
		for(unsigned int j=0;j<recIt->nTimeBins();j++)
		  if(recIt->adcs(i,j)>adcmax) {
		    adcmax=recIt->adcs(i,j); 
		    binmx=j;
		  }
	      
	      double adc_3_3_sum=0.0;
	      //well, this really only works for 3 strips in readout - not sure the right fix for general case
	      for(unsigned int i=0;i<recIt->nStrips();i++) 
		for(unsigned int j=binmx-1;j<=binmx+1;j++) 
		  adc_3_3_sum+=recIt->adcs(i,j);
	      
	      
	      if(adc_3_3_sum > 0.0 &&  adc_3_3_sum < 2000.0) {
		
		// temporary fix for ME1/1a to avoid triple entries
		int flag=0;
		if(id.station()==1 && id.ring()==4 &&  recIt->channels(1)>16)  flag=1;
		// end of temporary fix
		if(flag==0) {
		  
		  wire= m_single_wire_layer[idlayer];
		  int chambertype=id.iChamberType(id.station(),id.ring());
		  int hvsgmtnmb=m_wire_hvsegm[chambertype][wire];
		  int nmbofhvsegm=nmbhvsegm[chambertype-1];
		  int location= (layer-1)*nmbofhvsegm+hvsgmtnmb;
		  		  
		  gasGain_chamberType[counter] = chambertype;
		  gasGain_HVSegNumber[counter] = hvsgmtnmb;
		  gasGain_NmbHVSegments[counter] = nmbofhvsegm;
		  gasGain_location[counter] = location;
		  gasGain_ADC3x3Sum[counter] = adc_3_3_sum;
		  gasGain_chamber[counter] = id.chamber();
		  gasGain_ring[counter] = id.ring();
		  gasGain_station[counter] = id.station();
		  gasGain_endcap[counter] = id.endcap();
		  gasGain_layer[counter] = id.layer();
		  counter++;

		  /*
		    std::cout<<idchamber<<"   "<<id.station()<<" "<<id.ring()<<" "
		    <<id.chamber()<<"    "<<layer<<" "<< wire<<" "<<m_strip[1]<<" "<<
		    chambertype<<" "<< hvsgmtnmb<<" "<< nmbofhvsegm<<" "<< 
		    location<<"   "<<adc_3_3_sum<<std::endl;
		  */
		} // end of if flag==0
	      } // end if(adcsum>0.0 && adcsum<2000.0)
	    } // end of if if(m_strip.size()==3
	  } // end of if single wire
      } // end of looping thru rechit collection
  }   // end of if wire and strip and rechit present 
  gasGain_nGasGain = counter;


}











//--------------------------------------------------------------
// Compute a serial number for the chamber.
// This is useful when filling histograms and working with arrays.
//--------------------------------------------------------------
int CSCRootMaker::chamberSerial( CSCDetId id ) {
  int st = id.station();
  int ri = id.ring();
  int ch = id.chamber();
  int ec = id.endcap();
  int kSerial = ch;
  if (st == 1 && ri == 1) kSerial = ch;
  if (st == 1 && ri == 2) kSerial = ch + 36;
  if (st == 1 && ri == 3) kSerial = ch + 72;
  if (st == 1 && ri == 4) kSerial = ch;
  if (st == 2 && ri == 1) kSerial = ch + 108;
  if (st == 2 && ri == 2) kSerial = ch + 126;
  if (st == 3 && ri == 1) kSerial = ch + 162;
  if (st == 3 && ri == 2) kSerial = ch + 180;
  if (st == 4 && ri == 1) kSerial = ch + 216;
  if (st == 4 && ri == 2) kSerial = ch + 234;  // one day...
  if (ec == 2) kSerial = kSerial + 300;
  return kSerial;
}

//--------------------------------------------------------------
// Compute a serial number for the ring.
// This is useful when filling histograms and working with arrays.
//--------------------------------------------------------------
int CSCRootMaker::ringSerial( CSCDetId id ) {
  int st = id.station();
  int ri = id.ring();
  int ec = id.endcap();
  int kSerial = 0 ;
  if (st == 1 && ri == 1) kSerial = ri;
  if (st == 1 && ri == 2) kSerial = ri ;
  if (st == 1 && ri == 3) kSerial = ri ;
  if (st == 1 && ri == 4) kSerial = 1;
  if (st == 2 ) kSerial = ri + 3;
  if (st == 3 ) kSerial = ri + 5;
  if (st == 4 ) kSerial = ri + 7;
  if (ec == 2) kSerial = kSerial * (-1);
  return kSerial;
}


int CSCRootMaker::getWidth(const CSCStripDigiCollection& stripdigis, CSCDetId idRH, int centerStrip){

  int width = 1;
  int widthpos = 0;
  int widthneg = 0;

  // Loop over strip digis responsible for this recHit and sum charge
  CSCStripDigiCollection::DigiRangeIterator sIt;

  for (sIt = stripdigis.begin(); sIt != stripdigis.end(); sIt++){
	  CSCDetId id = (CSCDetId)(*sIt).first;
	  if (id == idRH){
		  std::vector<CSCStripDigi>::const_iterator digiItr = (*sIt).second.first;
		  std::vector<CSCStripDigi>::const_iterator first = (*sIt).second.first;
		  std::vector<CSCStripDigi>::const_iterator last = (*sIt).second.second;
		  std::vector<CSCStripDigi>::const_iterator it = (*sIt).second.first;
		  std::vector<CSCStripDigi>::const_iterator itr = (*sIt).second.first;
		  //std::cout << " IDRH " << id <<std::endl;
		  int St = idRH.station();
		  int Rg    = idRH.ring();
		  if (St == 1 && Rg == 4){
			  while(centerStrip> 16) centerStrip -= 16;
		  }
		  for ( ; digiItr != last; ++digiItr ) {
			  int thisStrip = digiItr->getStrip();
			  if (thisStrip == (centerStrip)){
				  it = digiItr;
				  for( ; it != last; ++it ) {
					  int strip = it->getStrip();
					  std::vector<int> myADCVals = it->getADCCounts();
					  double thisPedestal = 0.5*(double)(myADCVals[0]+myADCVals[1]);
					  if(((double)myADCVals[3]-thisPedestal) < 6 || widthpos == 10 || it==last){break;}
					   if(strip != centerStrip){ widthpos += 1;
					   }
				  }
				  itr = digiItr;
				  for( ; itr != first; --itr) {
					  int strip = itr->getStrip();
					  std::vector<int> myADCVals = itr->getADCCounts();
					  double thisPedestal = 0.5*(double)(myADCVals[0]+myADCVals[1]);
					  if(((double)myADCVals[3]-thisPedestal) < 6 || widthneg == 10 || itr==first){break;}	 
					  if(strip != centerStrip) {widthneg += 1 ; 
					  }
				  }
			  }
		  }
	  }
  }
  //std::cout << "Widthneg - " <<  widthneg << "Widthpos + " <<  widthpos << std::endl;
  width =  width + widthneg +  widthpos ;
  //std::cout << "Width " <<  width << std::endl;
  return width;
}



bool CSCRootMaker::withinSensitiveRegion(LocalPoint localPos, const std::array<const double, 4> & layerBounds, int station, int ring, double shiftFromEdge, double shiftFromDeadZone){
//---- check if it is in a good local region (sensitive area - geometrical and HV boundaries excluded) 
  bool pass = false;

  double y_center = 0.;
  double yUp = layerBounds[3] + y_center;
  double yDown = - layerBounds[3] + y_center;
  double xBound1Shifted = layerBounds[0] - shiftFromEdge;//
  double xBound2Shifted = layerBounds[1] - shiftFromEdge;//
  double lineSlope = (yUp - yDown)/(xBound2Shifted-xBound1Shifted);
  double lineConst = yUp - lineSlope*xBound2Shifted;
  double yBorder =  lineSlope*abs(localPos.x()) + lineConst;
      
  //bool withinChamberOnly = false;// false = "good region"; true - boundaries only
  std::vector <double> deadZoneCenter(6);
  double cutZone = shiftFromDeadZone;//cm
  //---- hardcoded... not good
  if(station>1 && station<5){
    if(2==ring){
      deadZoneCenter[0]= -162.48 ;
      deadZoneCenter[1] = -81.8744;
      deadZoneCenter[2] = -21.18165;
      deadZoneCenter[3] = 39.51105;
      deadZoneCenter[4] = 100.2939;
      deadZoneCenter[5] = 160.58;
      
      if(localPos.y() >yBorder &&
	 ((localPos.y()> deadZoneCenter[0] + cutZone && localPos.y()< deadZoneCenter[1] - cutZone) ||
	  (localPos.y()> deadZoneCenter[1] + cutZone && localPos.y()< deadZoneCenter[2] - cutZone) ||
	  (localPos.y()> deadZoneCenter[2] + cutZone && localPos.y()< deadZoneCenter[3] - cutZone) ||
	  (localPos.y()> deadZoneCenter[3] + cutZone && localPos.y()< deadZoneCenter[4] - cutZone) ||
	  (localPos.y()> deadZoneCenter[4] + cutZone && localPos.y()< deadZoneCenter[5] - cutZone))){
	pass = true;
      }
    }
    else if(1==ring){
      if(2==station){
	deadZoneCenter[0]= -95.80 ;
	deadZoneCenter[1] = -27.47;
	deadZoneCenter[2] = 33.67;
	deadZoneCenter[3] = 90.85;
        }
      else if(3==station){
	deadZoneCenter[0]= -89.305 ;
	deadZoneCenter[1] = -39.705;
	deadZoneCenter[2] = 20.195;
	deadZoneCenter[3] = 77.395;
      }
      else if(4==station){
	deadZoneCenter[0]= -75.645;
	deadZoneCenter[1] = -26.055;
	deadZoneCenter[2] = 23.855;
	deadZoneCenter[3] = 70.575;
      }
      if(localPos.y() >yBorder &&
	 ((localPos.y()> deadZoneCenter[0] + cutZone && localPos.y()< deadZoneCenter[1] - cutZone) ||
	  (localPos.y()> deadZoneCenter[1] + cutZone && localPos.y()< deadZoneCenter[2] - cutZone) ||
	  (localPos.y()> deadZoneCenter[2] + cutZone && localPos.y()< deadZoneCenter[3] - cutZone))){
	pass = true;
      }
    }
  }
  else if(1==station){
    if(3==ring){
      deadZoneCenter[0]= -83.155 ;
      deadZoneCenter[1] = -22.7401;
      deadZoneCenter[2] = 27.86665;
      deadZoneCenter[3] = 81.005;
      if(localPos.y() > yBorder &&
	 ((localPos.y()> deadZoneCenter[0] + cutZone && localPos.y()< deadZoneCenter[1] - cutZone) ||
	  (localPos.y()> deadZoneCenter[1] + cutZone && localPos.y()< deadZoneCenter[2] - cutZone) ||
	  (localPos.y()> deadZoneCenter[2] + cutZone && localPos.y()< deadZoneCenter[3] - cutZone))){
	pass = true;
      }
    }
    else if(2==ring){
      deadZoneCenter[0]= -86.285 ;
      deadZoneCenter[1] = -32.88305;
      deadZoneCenter[2] = 32.867423;
      deadZoneCenter[3] = 88.205;
      if(localPos.y() > (yBorder) &&
	 ((localPos.y()> deadZoneCenter[0] + cutZone && localPos.y()< deadZoneCenter[1] - cutZone) ||
	  (localPos.y()> deadZoneCenter[1] + cutZone && localPos.y()< deadZoneCenter[2] - cutZone) ||
	  (localPos.y()> deadZoneCenter[2] + cutZone && localPos.y()< deadZoneCenter[3] - cutZone))){
	pass = true;
      }
    }
    else{
      deadZoneCenter[0]= -81.0;
      deadZoneCenter[1] = 81.0;
      if(localPos.y() > (yBorder) &&
	 (localPos.y()> deadZoneCenter[0] + cutZone && localPos.y()< deadZoneCenter[1] - cutZone )){
	pass = true;
      }
    }
  }
  return pass;
}


std::vector<CSCSegment> CSCRootMaker::findMuonSegments(edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry, const reco::Track& Track, 
							 edm::Handle<CSCSegmentCollection> cscSegments, edm::Handle<CSCRecHit2DCollection> recHits, 
							 edm::ESHandle<CSCGeometry> cscGeom)
{

  std::vector<CSCSegment> savedSegments;
  std::vector<GlobalPoint> savedPoints;

  for(trackingRecHit_iterator recHit =  Track.recHitsBegin(); recHit != Track.recHitsEnd(); ++recHit)
    {
      
      if(!(*recHit)->isValid()) continue;

      //cout << "Valid RH from Track" << endl;
      
      //get the detector Id
      DetId idRivHit = (*recHit)->geographicalId();

      // CSC recHits
      if (idRivHit.det() == 2 /*DetId::Muon*/ && idRivHit.subdetId() == 2 /*MuonSubdetId::CSC*/ ) 
	{
	  //cout << "CSC RH" << endl;

	  // get the RecHit Local Position
	  LocalPoint posTrackRecHit = (*recHit)->localPosition(); 
	  // get the chamber Id
	  CSCDetId tempchamberId(idRivHit.rawId());
	  int ring = tempchamberId.ring();
	  int station = tempchamberId.station();
	  int endcap = tempchamberId.endcap();
	  int chamber = tempchamberId.chamber();    
	  CSCDetId chamberId(endcap, station, ring, chamber, 0);
	  int trackRHChamberSerial = chamberSerial(chamberId);
	  
	  for(CSCSegmentCollection::const_iterator dSiter=cscSegments->begin(); dSiter != cscSegments->end(); dSiter++) 
	    {

	      CSCDetId id  = (CSCDetId)(*dSiter).cscDetId();
	      //int nRH = (*dSiter).nRecHits();
	      int allSegChamberSerial = chamberSerial(id);

	      //cout << "NEW SEG  " << allSegChamberSerial << "   " << trackRHChamberSerial << endl;
	      if (allSegChamberSerial != trackRHChamberSerial) continue;
	      LocalPoint localPos = (*dSiter).localPosition();
	      const CSCChamber* cscchamber = cscGeom->chamber(id);
	      GlobalPoint globalPosition = GlobalPoint(0.0, 0.0, 0.0);
	      if (cscchamber) globalPosition = cscchamber->toGlobal(localPos);
	      

	      //For some reason, the track recHit localPosition is actually the LP for the segment it belongs to...
	      //So this is not needed.
	      /*
	      bool selected = false;
	      std::vector<CSCRecHit2D> theseRecHits = (*dSiter).specificRecHits();
	      for ( vector<CSCRecHit2D>::const_iterator iRH = theseRecHits.begin(); iRH != theseRecHits.end(); iRH++) 
		{
		  LocalPoint rhitlocal = (*iRH).localPosition();
		  cout << "RH from Seg   (" <<rhitlocal.x()<<","<<rhitlocal.y()<<","<<rhitlocal.z()<<")"<< "   ("
		       << posTrackRecHit.x() << "," << posTrackRecHit.y() << "," << posTrackRecHit.z() << ")     S(" 
		       << localPos.x() << "," << localPos.y() << "," << localPos.z() << ") " << endl;
		    

		  if (rhitlocal.x() != posTrackRecHit.x()) continue;
		  if (rhitlocal.y() != posTrackRecHit.y()) continue;
		  if (rhitlocal.z() != posTrackRecHit.z()) continue;
		  selected = true;
		}
	      */
	      if (localPos.x() != posTrackRecHit.x()) continue;                                                                                                                                            
	      if (localPos.y() != posTrackRecHit.y()) continue;                                                                                                                                            
	      if (localPos.z() != posTrackRecHit.z()) continue; 
	      
	      //cout << "Selected!" << endl;
	      if(savedSegments.empty())
		{
		  savedSegments.push_back(*dSiter);
		  savedPoints.push_back(globalPosition);
		}
	      else{
		bool alreadySaved = false;
		for(int n = 0; n< int(savedSegments.size()); n++)
		  {
		    if(savedPoints[n] == globalPosition) alreadySaved = true;
		  }
		if(!alreadySaved)
		  {
		    savedSegments.push_back(*dSiter);
		    savedPoints.push_back(globalPosition);
		  }
	      }		
	      //cout << "BREAKING LOOP " << endl;
	      break;//break loop over segments since you already found one for this track recHit
	    	      
	    }//Segment loop
		
	}//if ID == MuonSystem && ID == CSC

    }//Loop over track recHits

  //cout << endl << endl << endl;

  return savedSegments;
  

}









































void 
CSCRootMaker::bookTree(TTree *tree) 
{

  using namespace edm;
  using namespace std;

  
  tree->Branch("Run",&Run,"Run/l");
  tree->Branch("Event",&Event,"Event/l");
  tree->Branch("LumiSect",&LumiSect,"LumiSect/l");
  tree->Branch("BunchCrossing",&BunchCrossing,"BunchCrossing/I");
  tree->Branch("timeSecond",&timeSecond,"timeSecond/i");

  //Vertex
  tree->Branch("vertex_nVertex",&vertex_nVertex,"vertex_nVertex/I");

  //Lumi
/*
  tree->Branch("avgInstantLumi",&avgInstantLumi,"avgInstantLumi/D");
  tree->Branch("correctedAvgInstantLumi",&correctedAvgInstantLumi,"correctedAvgInstantLumi/D");
  tree->Branch("rawbxlumi",&rawbxlumi,"rawbxlumi/D");
  tree->Branch("bx_B1",  bx_B1,  "bx_B1[3564]/D");
  tree->Branch("bx_B2",  bx_B2,  "bx_B2[3564]/D");
  tree->Branch("bx_LUMI",  bx_LUMI,  "bx_LUMI[3564]/D");

  // Tracks
  tree->Branch("tracks_nTracks", &tracks_nTracks, "tracks_nTracks/I");
  tree->Branch("tracks_charge",  tracks_charge,  "tracks_charge[tracks_nTracks]/I");
  tree->Branch("tracks_px",  tracks_px,  "tracks_px[tracks_nTracks]/D");
  tree->Branch("tracks_py",  tracks_py,  "tracks_py[tracks_nTracks]/D");
  tree->Branch("tracks_pz",  tracks_pz,  "tracks_pz[tracks_nTracks]/D");
  tree->Branch("tracks_pt",  tracks_pt,  "tracks_pt[tracks_nTracks]/D");
  tree->Branch("tracks_p",  tracks_p,  "tracks_p[tracks_nTracks]/D");
  tree->Branch("tracks_eta",  tracks_eta,  "tracks_eta[tracks_nTracks]/D");
  tree->Branch("tracks_phi",  tracks_phi,  "tracks_phi[tracks_nTracks]/D");
  // SimHits
  tree->Branch("simHits_nSimHits", &simHits_nSimHits, "simHits_nSimHits/I");
  tree->Branch("simHits_particleType", simHits_particleType,  "simHits_particleType[simHits_nSimHits]/I");
  tree->Branch("simHits_localX", simHits_localX,  "simHits_localX[simHits_nSimHits]/D");
  tree->Branch("simHits_localY", simHits_localY,  "simHits_localY[simHits_nSimHits]/D");
  tree->Branch("simHits_globalX", simHits_globalX,  "simHits_globalX[simHits_nSimHits]/D");
  tree->Branch("simHits_globalY", simHits_globalY,  "simHits_globalY[simHits_nSimHits]/D");
  tree->Branch("simHits_ID_endcap", simHits_ID_endcap,  "simHits_ID_endcap[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_ring", simHits_ID_ring,  "simHits_ID_ring[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_station", simHits_ID_station,  "simHits_ID_station[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_chamber", simHits_ID_chamber,  "simHits_ID_chamber[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_layer", simHits_ID_layer,  "simHits_ID_layer[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_chamberSerial", simHits_ID_chamberSerial,  "simHits_ID_chamberSerial[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_ringSerial", simHits_ID_ringSerial,  "simHits_ID_ringSerial[simHits_nSimHits]/I");
  tree->Branch("simHits_ID_processType", simHits_ID_processType,  "simHits_ID_processType[simHits_nSimHits]/I");
  tree->Branch("simHits_momentum", simHits_momentum,  "simHits_momentum[simHits_nSimHits]/D");
  tree->Branch("simHits_phi", simHits_phi,  "simHits_phi[simHits_nSimHits]/D");
  tree->Branch("simHits_theta", simHits_theta,  "simHits_theta[simHits_nSimHits]/D");
*/

  // CSCRecHits2D
  tree->Branch("recHits2D_nRecHits2D", &recHits2D_nRecHits2D,"recHits2D_nRecHits2D/I");
  tree->Branch("recHits2D_ID_endcap",  recHits2D_ID_endcap,   "recHits2D_ID_endcap[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_ring",  recHits2D_ID_ring,   "recHits2D_ID_ring[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_station",  recHits2D_ID_station,   "recHits2D_ID_station[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_chamber",  recHits2D_ID_chamber,   "recHits2D_ID_chamber[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_layer",  recHits2D_ID_layer,   "recHits2D_ID_layer[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_localX",  recHits2D_localX,   "recHits2D_localX[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localY",  recHits2D_localY,   "recHits2D_localY[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localXXerr",  recHits2D_localXXerr,   "recHits2D_localXXerr[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localYYerr",  recHits2D_localYYerr,   "recHits2D_localYYer[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localXYerr",  recHits2D_localXYerr,   "recHits2D_localXYerr[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_stripPosition",  recHits2D_stripPosition,   "recHits2D_stripPosition[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_stripError",  recHits2D_stripError,   "recHits2D_stripError[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_SumQ",  recHits2D_SumQ,   "recHits2D_SumQ[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_SumQSides",  recHits2D_SumQSides,   "recHits2D_SumQSides[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_Time",  recHits2D_Time,   "recHits2D_Time[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_globalX",  recHits2D_globalX,   "recHits2D_globalX[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_globalY",  recHits2D_globalY,   "recHits2D_globalY[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_belongsToSaMuon",  recHits2D_belongsToSaMuon,   "recHits2D_belongsToSaMuon[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_belongsToMuon",  recHits2D_belongsToMuon,   "recHits2D_belongsToMuon[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_chamberSerial",  recHits2D_ID_chamberSerial,   "recHits2D_ID_chamberSerial[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_ID_ringSerial",  recHits2D_ID_ringSerial,   "recHits2D_ID_ringSerial[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_simHit_particleTypeID",  recHits2D_simHit_particleTypeID,   "recHits2D_simHit_particleTypeID[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_simHit_localX",  recHits2D_simHit_localX,   "recHits2D_simHit_localX[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_simHit_localY",  recHits2D_simHit_localY,   "recHits2D_simHit_localY[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_ADCSignal",  recHits2D_ADCSignal,   "recHits2D_ADCSignal[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_nearestStrip",  recHits2D_nearestStrip,   "recHits2D_nearestStrip[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_nearestWire",  recHits2D_nearestWire,   "recHits2D_nearestWire[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_nearestWireGroup",  recHits2D_nearestWireGroup,   "recHits2D_nearestWireGroup[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_localStripWireIntersectionX",  recHits2D_localStripWireIntersectionX,   "recHits2D_localStripWireIntersectionX[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localStripWireIntersectionY",  recHits2D_localStripWireIntersectionY,   "recHits2D_localStripWireIntersectionY[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localStripWireGroupIntersectionX",  recHits2D_localStripWireGroupIntersectionX,   "recHits2D_localStripWireGroupIntersectionX[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_localStripWireGroupIntersectionY",  recHits2D_localStripWireGroupIntersectionY,   "recHits2D_localStripWireGroupIntersectionY[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_stripWidthAtHit",  recHits2D_stripWidthAtHit,   "recHits2D_stripWidthAtHit[recHits2D_nRecHits2D]/D");

  tree->Branch("recHits2D_positionWithinStrip",  recHits2D_positionWithinStrip,   "recHits2D_positionWithinStrip[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_wireTime",  recHits2D_wireTime,   "recHits2D_wireTime[recHits2D_nRecHits2D]/D");
  tree->Branch("recHits2D_hitWire",  recHits2D_hitWire,   "recHits2D_hitWire[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_wgroupsBX",  recHits2D_wgroupsBX,   "recHits2D_wgroupsBX[recHits2D_nRecHits2D]/I");
  tree->Branch("recHits2D_nWireGroups",  recHits2D_nWireGroups,   "recHits2D_nWireGroups[recHits2D_nRecHits2D]/I");

  // CSCSegments
  tree->Branch("cscSegments_nSegments", &cscSegments_nSegments,"cscSegments_nSegments/I");
  tree->Branch("cscSegments_localX",  cscSegments_localX,  "cscSegments_localX[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_localY",  cscSegments_localY,  "cscSegments_localY[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_globalX",  cscSegments_globalX,"cscSegments_globalX[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_globalY",  cscSegments_globalY,"cscSegments_globalY[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_globalTheta",  cscSegments_globalTheta,"cscSegments_globalTheta[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_globalPhi",  cscSegments_globalPhi,"cscSegments_globalPhi[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_localTheta",  cscSegments_localTheta,"cscSegments_localTheta[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_chi2",  cscSegments_chi2,"cscSegments_chi2[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_nRecHits",  cscSegments_nRecHits,"cscSegments_nRecHits[cscSegments_nSegments]/D");
  tree->Branch("cscSegments_nDOF",  cscSegments_nDOF,"cscSegments_nDOF[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_ID_endcap",   cscSegments_ID_endcap,"cscSegments_ID_endcap[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_ID_ring",   cscSegments_ID_ring,"cscSegments_ID_ring[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_ID_station",   cscSegments_ID_station,"cscSegments_ID_station[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_ID_chamber",   cscSegments_ID_chamber,"cscSegments_ID_chamber[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_recHitRecord_endcap", &cscSegments_recHitRecord_endcap);
  tree->Branch("cscSegments_recHitRecord_ring", &cscSegments_recHitRecord_ring);
  tree->Branch("cscSegments_recHitRecord_station", &cscSegments_recHitRecord_station);
  tree->Branch("cscSegments_recHitRecord_chamber", &cscSegments_recHitRecord_chamber);
  tree->Branch("cscSegments_recHitRecord_layer", &cscSegments_recHitRecord_layer);
  tree->Branch("cscSegments_recHitRecord_localY", &cscSegments_recHitRecord_localY);
  tree->Branch("cscSegments_recHitRecord_localX", &cscSegments_recHitRecord_localX);
  tree->Branch("cscSegments_ID_chamberSerial",   cscSegments_ID_chamberSerial,"cscSegments_ID_chamberSerial[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_ID_ringSerial",   cscSegments_ID_ringSerial,"cscSegments_ID_ringSerial[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_Resolution_residual",  cscSegments_Resolution_residual ,"cscSegments_Resolution_residual[cscSegments_nSegments]/I");
  tree->Branch("cscSegments_Resolution_pull",  cscSegments_Resolution_pull ,"cscSegments_Resolution_pull[cscSegments_nSegments]/I");

  // Muons
  tree->Branch("muons_nMuons", &muons_nMuons, "muons_nMuons/I");
  tree->Branch("muons_isPFMuon",   muons_isPFMuon,   "muons_isPFMuon[muons_nMuons]/O");
  tree->Branch("muons_isCaloMuon",  muons_isCaloMuon,   "muons_isCaloMuon[muons_nMuons]/O");
  tree->Branch("muons_isTrackerMuon",   muons_isTrackerMuon,   "muons_isTrackerMuon[muons_nMuons]/O");
  tree->Branch("muons_isStandAloneMuon",   muons_isStandAloneMuon,   "muons_isStandAloneMuon[muons_nMuons]/O");
  tree->Branch("muons_isGlobalMuon",   muons_isGlobalMuon,   "muons_isGlobalMuon[muons_nMuons]/O");
  tree->Branch("muons_numberOfChambers",  muons_numberOfChambers,   "muons_numberOfChambers[muons_nMuons]/I");
  tree->Branch("muons_numberOfSegments",  muons_numberOfSegments,   "muons_numberOfSegments[muons_nMuons]/I");
  tree->Branch("muons_numberOfMatches",   muons_numberOfMatches,   "muons_numberOfMatches[muons_nMuons]/I");
  tree->Branch("muons_isEnergyValid",   muons_isEnergyValid,   "muons_isEnergyValid[muons_nMuons]/O");
  tree->Branch("muons_calEnergyTower",   muons_calEnergyTower,   "muons_calEnergyTower[muons_nMuons]/D");
  tree->Branch("muons_calEnergyEm",   muons_calEnergyEm,   "muons_calEnergyEm[muons_nMuons]/D");
  tree->Branch("muons_calEnergyHad",  muons_calEnergyHad,   "muons_calEnergyHad[muons_nMuons]/D");
  tree->Branch("muons_charge",   muons_charge,   "muons_charge[muons_nMuons]/I");
  tree->Branch("muons_energy",   muons_energy,   "muons_energy[muons_nMuons]/D");
  tree->Branch("muons_px",   muons_px,   "muons_px[muons_nMuons]/D");
  tree->Branch("muons_py",   muons_py,   "muons_py[muons_nMuons]/D");
  tree->Branch("muons_pz",   muons_pz,   "muons_pz[muons_nMuons]/D");
  tree->Branch("muons_pt",   muons_pt,   "muons_pt[muons_nMuons]/D");
  tree->Branch("muons_et",   muons_et,   "muons_et[muons_nMuons]/D");
  tree->Branch("muons_p",   muons_p,   "muons_p[muons_nMuons]/D");
  tree->Branch("muons_phi",   muons_phi,   "muons_phi[muons_nMuons]/D");
  tree->Branch("muons_eta",   muons_eta,   "muons_eta[muons_nMuons]/D");
  tree->Branch("muons_theta",   muons_theta,   "muons_theta[muons_nMuons]/D");
  tree->Branch("muons_vx",   muons_vx,   "muons_vx[muons_nMuons]/D");
  tree->Branch("muons_vy",   muons_vy,   "muons_vy[muons_nMuons]/D");
  tree->Branch("muons_vz",   muons_vz,   "muons_vz[muons_nMuons]/D");
  tree->Branch("muons_globalTrackNormalizedChi2",   muons_globalTrackNormalizedChi2   ,   "muons_globalTrackNormalizedChi2[muons_nMuons]/D");
  tree->Branch("muons_globalTrackNumberOfValidMuonHits",   muons_globalTrackNumberOfValidMuonHits, "muons_globalTrackNumberOfValidMuonHits[muons_nMuons]/I");
  tree->Branch("muons_trackNumberOfValidHits",   muons_trackNumberOfValidHits,    "muons_trackNumberOfValidHits[muons_nMuons]/I");
  tree->Branch("muons_trackNumberOfLostHits",   muons_trackNumberOfLostHits,    "muons_trackNumberOfLostHits[muons_nMuons]/I");
  tree->Branch("muons_isoNH03",   muons_isoNH03,   "muons_isoNH03[muons_nMuons]/D");
  tree->Branch("muons_isoCH03",   muons_isoCH03,   "muons_isoCH03[muons_nMuons]/D");
  tree->Branch("muons_isoPhot03",   muons_isoPhot03,   "muons_isoPhot03[muons_nMuons]/D");
  tree->Branch("muons_isoPU03",   muons_isoPU03,   "muons_isoPU03[muons_nMuons]/D");
  tree->Branch("muons_isoCH04",   muons_isoCH04,   "muons_isoCH04[muons_nMuons]/D");
  tree->Branch("muons_isoPhot04",   muons_isoPhot04,   "muons_isoPhot04[muons_nMuons]/D");
  tree->Branch("muons_isoNH04",   muons_isoNH04,   "muons_isoNH04[muons_nMuons]/D");
  tree->Branch("muons_isoPU04",   muons_isoPU04,   "muons_isoPU04[muons_nMuons]/D");
  tree->Branch("muons_dz",   muons_dz,   "muons_dz[muons_nMuons]/D");
  tree->Branch("muons_dxy",   muons_dxy,   "muons_dxy[muons_nMuons]/D");
  tree->Branch("muons_nRecHits",   muons_nRecHits,   "muons_nRecHits[muons_nMuons]/I");
  tree->Branch("muons_cscSegmentRecord_nRecHits", &muons_cscSegmentRecord_nRecHits);
  tree->Branch("muons_cscSegmentRecord_ring", &muons_cscSegmentRecord_ring);
  tree->Branch("muons_cscSegmentRecord_station", &muons_cscSegmentRecord_station);
  tree->Branch("muons_cscSegmentRecord_chamber", &muons_cscSegmentRecord_chamber);
  tree->Branch("muons_cscSegmentRecord_endcap", &muons_cscSegmentRecord_endcap);
  tree->Branch("muons_cscSegmentRecord_localY", &muons_cscSegmentRecord_localY);
  tree->Branch("muons_cscSegmentRecord_localX", &muons_cscSegmentRecord_localX);


/*
  // L1 GMT
  tree->Branch("l1Trigger_CSC",  &l1Trigger_CSC,  "l1Trigger_CSC/O");
  tree->Branch("l1Trigger_DT",  &l1Trigger_DT,  "l1Trigger_DT/O");
  tree->Branch("l1Trigger_RPCForward",  &l1Trigger_RPCForward,  "l1Trigger_RPCForward/O");
  tree->Branch("l1Trigger_RPCBarrel",  &l1Trigger_RPCBarrel,  "l1Trigger_RPCBarrel/O");
  tree->Branch("l1Trigger_beamHalo",  &l1Trigger_beamHalo,  "l1Trigger_beamHalo/O");

  // HLT Trigger
  tree->Branch("hltTrigger_nBits", &hltTrigger_nBits , "hltTrigger_nBits/I");
  tree->Branch("hltTrigger_bits",  hltTrigger_bits,  "hltTrigger_bits[hltTrigger_nBits]/I");
*/
  // Standalone Muons
  tree->Branch("standaloneMuons_nMuons", &standaloneMuons_nMuons , "standaloneMuons_nMuons/I");
  tree->Branch("standaloneMuons_p", standaloneMuons_p , "standaloneMuons_p[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_pt", standaloneMuons_pt , "standaloneMuons_pt[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_nRecHits", standaloneMuons_nRecHits , "standaloneMuons_nRecHits[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_chi2", standaloneMuons_chi2 , "standaloneMuons_chi2[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_normChi2", standaloneMuons_normChi2 , "standaloneMuons_normChi2[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_nDTHits", standaloneMuons_nDTHits , "standaloneMuons_nDTHits[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nCSCHits", standaloneMuons_nCSCHits , "standaloneMuons_nCSCHits[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nCSCHitsP", standaloneMuons_nCSCHitsP , "standaloneMuons_nCSCHitsP[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nCSCHitsM", standaloneMuons_nCSCHitsM , "standaloneMuons_nCSCHitsM[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nRPCHits", standaloneMuons_nRPCHits , "standaloneMuons_nRPCHits[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nRPCHitsP", standaloneMuons_nRPCHitsP , "standaloneMuons_nRPCHitsP[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nRPCHitsM", standaloneMuons_nRPCHitsM , "standaloneMuons_nRPCHitsM[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nHitsM", standaloneMuons_nHitsM , "standaloneMuons_nHitsM[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_nHitsP", standaloneMuons_nHitsP , "standaloneMuons_nHitsP[standaloneMuons_nMuons]/I");
  tree->Branch("standaloneMuons_crudeLength", standaloneMuons_crudeLength , "standaloneMuons_crudeLength[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_deltaPhi", standaloneMuons_deltaPhi , "standaloneMuons_deltaPhi[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_innerGlobalPolarAngle",standaloneMuons_innerGlobalPolarAngle,"standaloneMuons_innerGlobalPolarAngle[standaloneMuons_nMuons]/D");
  tree->Branch("standaloneMuons_outerGlobalPolarAngle",standaloneMuons_outerGlobalPolarAngle,"standaloneMuons_outerGlobalPolarAngle[standaloneMuons_nMuons]/D");

  //Strip Digis
  tree->Branch("firedStripDigis_nStripDigis",&firedStripDigis_nStripDigis,"firedStripDigis_nStripDigis/I");
  tree->Branch("firedStripDigis_ID_endcap",firedStripDigis_ID_endcap,"firedStripDigis_ID_endcap[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ID_chamber",firedStripDigis_ID_chamber,"firedStripDigis_ID_chamber[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ID_station",firedStripDigis_ID_station,"firedStripDigis_ID_station[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ID_layer",firedStripDigis_ID_layer,"firedStripDigis_ID_layer[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ID_strip",firedStripDigis_ID_strip,"firedStripDigis_ID_strip[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ID_ring",firedStripDigis_ID_ring,"firedStripDigis_ID_ring[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_ADCTotal",firedStripDigis_ADCTotal,"firedStripDigis_ADCTotal[firedStripDigis_nStripDigis]/D");
  tree->Branch("firedStripDigis_ADCMax",firedStripDigis_ADCMax,"firedStripDigis_ADCMax[firedStripDigis_nStripDigis]/D");
  tree->Branch("firedStripDigis_tbinMax",firedStripDigis_tbinMax,"firedStripDigis_tbinMax[firedStripDigis_nStripDigis]/I");
  tree->Branch("firedStripDigis_localX",firedStripDigis_localX,"firedStripDigis_localX[firedStripDigis_nStripDigis]/D");

  //Wire Digis
  tree->Branch("firedWireDigis_nWireDigis",&firedWireDigis_nWireDigis,"firedWireDigis_nWireDigis/I");
  tree->Branch("firedWireDigis_ID_endcap", firedWireDigis_ID_endcap,"firedWireDigis_ID_endcap[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_ID_chamber", firedWireDigis_ID_chamber,"firedWireDigis_ID_chamber[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_ID_station", firedWireDigis_ID_station,"firedWireDigis_ID_station[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_ID_wire", firedWireDigis_ID_wire,"firedWireDigis_ID_wire[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_ID_layer", firedWireDigis_ID_layer,"firedWireDigis_ID_layer[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_ID_ring", firedWireDigis_ID_ring,"firedWireDigis_ID_ring[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_timeBin", firedWireDigis_timeBin,"firedWireDigis_timeBin[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_chamberSerial", firedWireDigis_chamberSerial,"firedWireDigis_chamberSerial[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_numberWireTimeBins", firedWireDigis_numberWireTimeBins,"firedWireDigis_numberWireTimeBins[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_AFEB", firedWireDigis_AFEB,"firedWireDigis_AFEB[firedWireDigis_nWireDigis]/I");
  tree->Branch("firedWireDigis_localY", firedWireDigis_localY,"firedWireDigis_localY[firedWireDigis_nWireDigis]/D");



  //Comparator Digis
  tree->Branch("comparatorDigis_nDigis",&comparatorDigis_nDigis,"comparatorDigis_nDigis/I");
  tree->Branch("comparatorDigis_ID_endcap", comparatorDigis_ID_endcap,"comparatorDigis_ID_endcap[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_chamber", comparatorDigis_ID_chamber,"comparatorDigis_ID_chamber[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_station", comparatorDigis_ID_station,"comparatorDigis_ID_station[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_strip", comparatorDigis_ID_strip,"comparatorDigis_ID_strip[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_layer", comparatorDigis_ID_layer,"comparatorDigis_ID_layer[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_ring", comparatorDigis_ID_ring,"comparatorDigis_ID_ring[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_timeBin", comparatorDigis_timeBin,"comparatorDigis_timeBin[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_cfeb", comparatorDigis_cfeb,"comparatorDigis_cfeb[comparatorDigis_nDigis]/I");
  tree->Branch("comparatorDigis_ID_halfStrip", comparatorDigis_ID_halfStrip,"comparatorDigis_ID_halfStrip[comparatorDigis_nDigis]/I");

  //ALCTs
  tree->Branch("alct_nAlcts",&alct_nAlcts,"alct_nAlcts/I");
  tree->Branch("alct_BX", alct_BX,"alct_BX[alct_nAlcts]/I");
  tree->Branch("alct_fullBX", alct_fullBX,"alct_fullBX[alct_nAlcts]/I");
  tree->Branch("alct_ID_ring", alct_ID_ring,"alct_ID_ring[alct_nAlcts]/I");
  tree->Branch("alct_ID_layer", alct_ID_layer,"alct_ID_layer[alct_nAlcts]/I");
  tree->Branch("alct_ID_chamberID", alct_ID_chamberID,"alct_ID_chamberID[alct_nAlcts]/I");
  tree->Branch("alct_ID_chamber", alct_ID_chamber,"alct_ID_chamber[alct_nAlcts]/I");
  tree->Branch("alct_ID_endcap", alct_ID_endcap,"alct_ID_endcap[alct_nAlcts]/I");
  tree->Branch("alct_ID_station", alct_ID_station,"alct_ID_station[alct_nAlcts]/I");
  tree->Branch("alct_ID_chamberSerial", alct_ID_chamberSerial,"alct_ID_chamberSerial[alct_nAlcts]/I");
  tree->Branch("alct_ID_ringSerial", alct_ID_ringSerial,"alct_ID_ringSerial[alct_nAlcts]/I");
 
  // CLCTs
  tree->Branch("clct_nClcts",&clct_nClcts,"clct_nClcts/I");
  tree->Branch("clct_BX", clct_BX,"clct_BX[clct_nClcts]/I");
  tree->Branch("clct_fullBX", clct_fullBX,"clct_fullBX[clct_nClcts]/I");
  tree->Branch("clct_ID_ring", clct_ID_ring,"clct_ID_ring[clct_nClcts]/I");
  tree->Branch("clct_ID_layer", clct_ID_layer,"clct_ID_layer[clct_nClcts]/I");
  tree->Branch("clct_ID_chamberID", clct_ID_chamberID,"clct_ID_chamberID[clct_nClcts]/I");
  tree->Branch("clct_ID_chamber", clct_ID_chamber,"clct_ID_chamber[clct_nClcts]/I");
  tree->Branch("clct_ID_endcap", clct_ID_endcap,"clct_ID_endcap[clct_nClcts]/I");
  tree->Branch("clct_ID_station", clct_ID_station,"clct_ID_station[clct_nClcts]/I");
  tree->Branch("clct_ID_chamberSerial", clct_ID_chamberSerial,"clct_ID_chamberSerial[clct_nClcts]/I");
  tree->Branch("clct_ID_ringSerial", clct_ID_ringSerial,"clct_ID_ringSerial[clct_nClcts]/I");

  // Correlated LCTs
  tree->Branch("correlatedLct_nLcts",&correlatedLct_nLcts,"correlatedLct_nLcts/I");
  tree->Branch("correlatedLct_BX", correlatedLct_BX,"correlatedLct_BX[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_trkNumber", correlatedLct_trkNumber,"correlatedLct_trkNumber[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_quality", correlatedLct_quality,"correlatedLct_quality[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_keyWG", correlatedLct_keyWG,"correlatedLct_keyWG[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_strip", correlatedLct_strip,"correlatedLct_strip[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_pattern", correlatedLct_pattern,"correlatedLct_pattern[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_bend", correlatedLct_bend,"correlatedLct_bend[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_CLCTPattern", correlatedLct_CLCTPattern,"correlatedLct_CLCTPattern[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_cscID", correlatedLct_cscID,"correlatedLct_cscID[correlatedLct_nLcts]/s");
  tree->Branch("correlatedLct_BX0", correlatedLct_BX0,"correlatedLct_BX0[correlatedLct_nLcts]/s");
  tree->Branch("correlatedLct_syncErr", correlatedLct_syncErr,"correlatedLct_syncErr[correlatedLct_nLcts]/s");
  tree->Branch("correlatedLct_ID_chamber", correlatedLct_ID_chamber,"correlatedLct_ID_chamber[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_ring", correlatedLct_ID_ring,"correlatedLct_ID_ring[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_station", correlatedLct_ID_station,"correlatedLct_ID_station[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_endcap", correlatedLct_ID_endcap,"correlatedLct_ID_endcap[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_layer", correlatedLct_ID_layer,"correlatedLct_ID_layer[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_chamberSerial", correlatedLct_ID_chamberSerial,"correlatedLct_ID_chamberSerial[correlatedLct_nLcts]/I");
  tree->Branch("correlatedLct_ID_ringSerial", correlatedLct_ID_ringSerial,"correlatedLct_ID_ringSerial[correlatedLct_nLcts]/I");

  // TMB
  tree->Branch("tmb_nTmb",&tmb_nTmb,"tmb_nTmb/I");
  tree->Branch("tmb_BXNCount", tmb_BXNCount,"tmb_BXNCount[tmb_nTmb]/I");
  tree->Branch("tmb_ALCTMatchTime", tmb_ALCTMatchTime,"tmb_ALCTMatchTime[tmb_nTmb]/I");
  tree->Branch("tmb_ID_chamberID", tmb_ID_chamberID,"tmb_ID_chamberID[tmb_nTmb]/I");
  tree->Branch("tmb_ID_chamber", tmb_ID_chamber,"tmb_ID_chamber[tmb_nTmb]/I");
  tree->Branch("tmb_ID_endcap", tmb_ID_endcap,"tmb_ID_endcap[tmb_nTmb]/I");
  tree->Branch("tmb_ID_station", tmb_ID_station,"tmb_ID_station[tmb_nTmb]/I");
  tree->Branch("tmb_ID_ring", tmb_ID_ring,"tmb_ID_ring[tmb_nTmb]/I");
  tree->Branch("tmb_ID_layer", tmb_ID_layer,"tmb_ID_layer[tmb_nTmb]/I");
  tree->Branch("tmb_ID_chamberSerial", tmb_ID_chamberSerial,"tmb_ID_chamberSerial[tmb_nTmb]/I");
  tree->Branch("tmb_ID_ringSerial", tmb_ID_ringSerial,"tmb_ID_ringSerial[tmb_nTmb]/I");
  tree->Branch("tmb_alct0key", tmb_alct0key,"tmb_alct0key[tmb_nTmb]/I");
  tree->Branch("tmb_alctRelL1A", tmb_alctRelL1A,"tmb_alctRelL1A[tmb_nTmb]/I");

  // Calibrations
  tree->Branch("calibrations_nCalib",&calibrations_nCalib,"calibrations_nCalib/I");
  tree->Branch("calibrations_Gain_slope", calibrations_Gain_slope,"calibrations_Gain_slope[calibrations_nCalib]/D");
  tree->Branch("calibrations_XT_slope_left", calibrations_XT_slope_left,"calibrations_XT_slope_left[calibrations_nCalib]/D");
  tree->Branch("calibrations_XT_slope_right", calibrations_XT_slope_right,"calibrations_XT_slope_right[calibrations_nCalib]/D");
  tree->Branch("calibrations_XT_intercept_left", calibrations_XT_intercept_left,"calibrations_XT_intercept_left[calibrations_nCalib]/D");
  tree->Branch("calibrations_XT_intercept_right", calibrations_XT_intercept_right,"calibrations_XT_intercept_right[calibrations_nCalib]/D");
  tree->Branch("calibrations_Pedestals_ped", calibrations_Pedestals_ped,"calibrations_Pedestals_ped[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_33", calibrations_NoiseMatrix_33,"calibrations_NoiseMatrix_33[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_34", calibrations_NoiseMatrix_34,"calibrations_NoiseMatrix_34[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_35", calibrations_NoiseMatrix_35,"calibrations_NoiseMatrix_35[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_44", calibrations_NoiseMatrix_44,"calibrations_NoiseMatrix_44[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_45", calibrations_NoiseMatrix_45,"calibrations_NoiseMatrix_45[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_46", calibrations_NoiseMatrix_46,"calibrations_NoiseMatrix_46[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_55", calibrations_NoiseMatrix_55,"calibrations_NoiseMatrix_55[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_56", calibrations_NoiseMatrix_56,"calibrations_NoiseMatrix_56[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_57", calibrations_NoiseMatrix_57,"calibrations_NoiseMatrix_57[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_66", calibrations_NoiseMatrix_66,"calibrations_NoiseMatrix_66[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_67", calibrations_NoiseMatrix_67,"calibrations_NoiseMatrix_67[calibrations_nCalib]/D");
  tree->Branch("calibrations_NoiseMatrix_77", calibrations_NoiseMatrix_77,"calibrations_NoiseMatrix_77[calibrations_nCalib]/D");
  // Associated RecHits
  tree->Branch("assocRecHits_nAssocRH",&assocRecHits_nAssocRH,"assocRecHits_nAssocRH/I");
  tree->Branch("assocRecHits_codeBroad", assocRecHits_codeBroad,"assocRecHits_codeBroad[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_codeNarrow", assocRecHits_codeNarrow,"assocRecHits_codeNarrow[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_layer", assocRecHits_ID_layer,"assocRecHits_ID_layer[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_station", assocRecHits_ID_station,"assocRecHits_ID_station[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_ring", assocRecHits_ID_ring,"assocRecHits_ID_ring[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_chamber", assocRecHits_ID_chamber,"assocRecHits_ID_chamber[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_endcap", assocRecHits_ID_endcap,"assocRecHits_ID_endcap[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_ID_centerStrip", assocRecHits_ID_centerStrip,"assocRecHits_ID_centerStrip[assocRecHits_nAssocRH]/I");
  tree->Branch("assocRecHits_globalX", assocRecHits_globalX,"assocRecHits_globalX[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_globalY", assocRecHits_globalY,"assocRecHits_globalY[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_localX", assocRecHits_localX,"assocRecHits_localX[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_localY", assocRecHits_localY,"assocRecHits_localY[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_sumQ", assocRecHits_sumQ,"assocRecHits_sumQ[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_ratioSumQ", assocRecHits_ratioSumQ,"assocRecHits_ratioSumQ[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_Time", assocRecHits_Time,"assocRecHits_Time[assocRecHits_nAssocRH]/D");
  tree->Branch("assocRecHits_width", assocRecHits_width,"assocRecHits_width[assocRecHits_nAssocRH]/I");

  // Non Associated RecHits
  tree->Branch("nonAssocRecHits_nNonAssocRH",&nonAssocRecHits_nNonAssocRH,"nonAssocRecHits_nNonAssocRH/I");
  tree->Branch("nonAssocRecHits_codeBroad", nonAssocRecHits_codeBroad,"nonAssocRecHits_codeBroad[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_codeNarrow", nonAssocRecHits_codeNarrow,"nonAssocRecHits_codeNarrow[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_layer", nonAssocRecHits_ID_layer,"nonAssocRecHits_ID_layer[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_station", nonAssocRecHits_ID_station,"nonAssocRecHits_ID_station[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_ring", nonAssocRecHits_ID_ring,"nonAssocRecHits_ID_ring[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_chamber", nonAssocRecHits_ID_chamber,"nonAssocRecHits_ID_chamber[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_endcap", nonAssocRecHits_ID_endcap,"nonAssocRecHits_ID_endcap[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_ID_centerStrip", nonAssocRecHits_ID_centerStrip,"nonAssocRecHits_ID_centerStrip[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_globalX", nonAssocRecHits_globalX,"nonAssocRecHits_globalX[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_globalY", nonAssocRecHits_globalY,"nonAssocRecHits_globalY[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_localX", nonAssocRecHits_localX,"nonAssocRecHits_localX[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_localY", nonAssocRecHits_localY,"nonAssocRecHits_localY[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_sumQ", nonAssocRecHits_sumQ,"nonAssocRecHits_sumQ[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_ratioSumQ", nonAssocRecHits_ratioSumQ,"nonAssocRecHits_ratioSumQ[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_Time", nonAssocRecHits_Time,"nonAssocRecHits_Time[nonAssocRecHits_nNonAssocRH]/D");
  tree->Branch("nonAssocRecHits_width", nonAssocRecHits_width,"nonAssocRecHits_width[nonAssocRecHits_nNonAssocRH]/I");
  tree->Branch("nonAssocRecHits_distToGoodRH", nonAssocRecHits_distToGoodRH,"nonAssocRecHits_distToGoodRH[nonAssocRecHits_nNonAssocRH]/D");

  // Gas Gain
  tree->Branch("gasGain_nGasGain",&gasGain_nGasGain,"gasGain_nGasGain/I");
  tree->Branch("gasGain_chamberType", gasGain_chamberType,"gasGain_chamberType[gasGain_nGasGain]/I");
  tree->Branch("gasGain_HVSegNumber", gasGain_HVSegNumber,"gasGain_HVSegNumber[gasGain_nGasGain]/I");
  tree->Branch("gasGain_NmbHVSegments", gasGain_NmbHVSegments,"gasGain_NmbHVSegments[gasGain_nGasGain]/I");
  tree->Branch("gasGain_location", gasGain_location,"gasGain_location[gasGain_nGasGain]/I");
  tree->Branch("gasGain_chamber", gasGain_chamber,"gasGain_chamber[gasGain_nGasGain]/I");
  tree->Branch("gasGain_ring", gasGain_ring,"gasGain_ring[gasGain_nGasGain]/I");
  tree->Branch("gasGain_station", gasGain_station,"gasGain_station[gasGain_nGasGain]/I");
  tree->Branch("gasGain_endcap", gasGain_endcap,"gasGain_endcap[gasGain_nGasGain]/I");
  tree->Branch("gasGain_layer", gasGain_layer,"gasGain_layer[gasGain_nGasGain]/I");
  tree->Branch("gasGain_ADC3x3Sum", gasGain_ADC3x3Sum,"gasGain_ADC3x3Sum[gasGain_nGasGain]/D");

}








//define this as a plug-in
DEFINE_FWK_MODULE(CSCRootMaker);
