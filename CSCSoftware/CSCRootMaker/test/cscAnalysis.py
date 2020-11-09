#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from ROOT import *
import ROOT
from array import array
ROOT.gStyle.SetTitleYOffset(1.5)


def parseOptions():
    
    usage = ('usage: %prog [options] \n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
    parser.add_option('-m','--isMC', dest='isMC', type='int', default=1 ,help='isMC default:0')
    parser.add_option('-f','--file', dest='file', type='string', default='cscRootMaker.root' ,help='file default:blank')
    parser.add_option('-n','--maxEvents', dest='maxEvents', type='int', default=1000 ,help='maxEvents default:100000')
    parser.add_option('-d','--outDir', dest='outDir', type='string',
                      default='/home/msnowball/public_html/CSC/CMSSW6XY/DYToMuMu_14TeV_PU140Bx25_STAR17_61_V1A/' ,help='out directory default:CSC')
    parser.add_option('-j','--jobName',dest='jobName',type='string', default='cscAna',help='name of job and output files')

    parser.add_option('--isDigi', dest='isDigi', type='int', default=1 ,help='isDigi default:1')
    parser.add_option('--isLocalReco', dest='isLocalReco', type='int', default=1 ,help='isLocalReco default:1')
    parser.add_option('--isFullReco', dest='isFullReco', type='int', default=1 ,help='isFullReco default:1')
        
    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()
    


class Analysis():

    def __init__(self):

        self.hists1D = {}
        self.hists2D = {}
        self.totalEvents = 0



        self.simHitsOverallEffDen = 0
        self.simHitsOverallEffNum = 0
        self.simHitsEffDen = [0] * 600
        self.simHitsEffNum = [0] * 600
        self.simHitsLayerEffDen = [[0 for dim1 in range(6)] for dim2 in range(600)]
        self.simHitsLayerEffNum = [[0 for dim1 in range(6)] for dim2 in range(600)]

        self.simHitsChamberEffNum = [[[[[0 for dim1 in range(6)] for dim2 in range(36)] for dim3 in range(4)] for dim4 in range(4)] for dim5 in range(2)]
        self.simHitsChamberEffDen = [[[[[0 for dim1 in range(6)] for dim2 in range(36)] for dim3 in range(4)] for dim4 in range(4)] for dim5 in range(2)]

        self.stationRings = ['ME-1/4','ME-1/3','ME-4/2','ME-3/2','ME-2/2','ME-1/2','ME-4/1','ME-3/1','ME-2/1','ME-1/1',
                             'ME+1/1','ME+2/1','ME+3/1','ME+4/1','ME+1/2','ME+2/2','ME+3/2','ME+4/2','ME+1/3','ME+1/4']
        self.stations = ['ME-1','ME-2','ME-3','ME-4','ME+1','ME+2','ME+3','ME+4']
        self.nChambers = {}
        self.nChambers['ME-1/1'] = 36
        self.nChambers['ME-1/2'] = 36
        self.nChambers['ME-1/3'] = 36
        self.nChambers['ME-1/4'] = 36
        self.nChambers['ME-2/1'] = 18
        self.nChambers['ME-2/2'] = 36
        self.nChambers['ME-3/1'] = 18
        self.nChambers['ME-3/2'] = 36
        self.nChambers['ME-4/1'] = 18
        self.nChambers['ME-4/2'] = 36
        self.nChambers['ME+1/1'] = 36
        self.nChambers['ME+1/2'] = 36
        self.nChambers['ME+1/3'] = 36
        self.nChambers['ME+1/4'] = 36
        self.nChambers['ME+2/1'] = 18
        self.nChambers['ME+2/2'] = 36
        self.nChambers['ME+3/1'] = 18
        self.nChambers['ME+3/2'] = 36
        self.nChambers['ME+4/1'] = 18
        self.nChambers['ME+4/2'] = 36

        self.nRecHitsPerStation = {}
        for key in self.stationRings:
            self.nRecHitsPerStation[key] = 0

        self.defineHistos()




    def doAnalysis(self,file):
        global opt, args
  
        tfile = ROOT.TFile(file,"READ")
        if not tfile:
            raise RunTimeError,"No input file specified or root file could not be found!"

        print "Opened file ", file
        
        if opt.isMC:
            tree = tfile.Get("cscRootMaker/Events")
        else:
            tree = tfile.Get("cscRootMaker/Events")

        if not tree:
            raise RunTimeError,"Tree not found!"
        


        #Analysis Loop
        for i in range( tree.GetEntries() ):
            tree.GetEntry(i)

            if i%1000 == 0:
                print "Event ",i
            if self.totalEvents > opt.maxEvents:
                break
            self.totalEvents+=1

            if opt.isLocalReco or opt.isFullReco:

                #Muons
                #AvgRH/Seg
                for n in range(tree.muons_nMuons):
                    if tree.muons_isStandAloneMuon[n]:
                        segmentCounter = 0
                        recHitsCounter = 0
                        for m in range(len(tree.muons_cscSegmentRecord_nRecHits[n])):
                            self.hists1D['recHitsPerSegment_saMuon_Norm'].Fill(tree.muons_cscSegmentRecord_nRecHits[n][m])
                            segmentCounter += 1
                            recHitsCounter += tree.muons_cscSegmentRecord_nRecHits[n][m]
                        self.hists1D['segmentsPerSaMuon_Norm'].Fill(segmentCounter)
                        self.hists2D['recHitsVSp'].Fill(tree.muons_p[n],tree.muons_nRecHits[n])
                        self.hists2D['recHitsVSpT'].Fill(tree.muons_pt[n],tree.muons_nRecHits[n])
                        avgRHpSeg = -1
                        if segmentCounter > 0:
                            avgRHpSeg = recHitsCounter/float(segmentCounter)
                        self.hists2D['recHitsPerSegVSp'].Fill(tree.muons_p[n],avgRHpSeg)
                        self.hists2D['recHitsPerSegVSpT'].Fill(tree.muons_pt[n],avgRHpSeg)
                        


                #CSCSegments
                CSC_SegmentCounter = [0] * 600
                CSC_SegmentMap = [''] * 600
                for n in range(tree.cscSegments_nSegments):
                    serialRecord = tree.cscSegments_ID_chamberSerial[n]
                    CSC_SegmentCounter[serialRecord] = CSC_SegmentCounter[serialRecord]+1
                    CSC_SegmentMap[serialRecord]+=str(n)+'.'
                    sRing = str(tree.cscSegments_ID_ring[n])
                    sStation = str(tree.cscSegments_ID_station[n])
                    sEndcap = '+'
                    if tree.cscSegments_ID_endcap[n] == 2:
                        sEndcap = '-'
                    string = 'ME'+sEndcap+sStation+'-'+sRing+'_recHitsPerSegment_Norm'
                    self.hists1D[string].Fill(tree.cscSegments_nRecHits[n])
                    self.hists1D['recHitsPerSegment_Norm'].Fill(tree.cscSegments_nRecHits[n])

                for n in range(0,600):
                    if CSC_SegmentCounter[n] == 1:
                        line = CSC_SegmentMap[n].split('.')
                        Identifier=int(line[0])
                        nLayers=0
                        missingLayers = []
                        for m in range(len(tree.cscSegments_recHitRecord_layer[Identifier])):
                            layer = tree.cscSegments_recHitRecord_layer[Identifier][m]
                            missingLayers.append(layer)
                            nLayers+=1
                        if nLayers > 0:
                            self.hists1D['OneSegmentChambers_nRecHitLayers_Norm'].Fill(nLayers)
                            #if not all 6 layers are reconstructed, plot missing layers
                            if nLayers < 6:
                                for j in range(len(missingLayers)):
                                    self.hists1D['OneSegmentChambers_missingLayer_Norm'].Fill(missingLayers[j])

                #2DRecHits
                for n in range(0,tree.recHits2D_nRecHits2D):
                    sEndcap = tree.recHits2D_ID_endcap[n]
                    if sEndcap == 2: sEndcap = '-'
                    else: sEndcap = '+'
                    sStation = str(tree.recHits2D_ID_station[n])
                    sChamber = str(tree.recHits2D_ID_chamber[n])
                    sLayer   = str(tree.recHits2D_ID_layer[n])
                    sRing    = str(tree.recHits2D_ID_ring[n])
                    #Average 2D per station
                    string = 'ME'+sEndcap+sStation+'/'+sRing
                    self.nRecHitsPerStation[string] += 1

                    #locations
                    x = tree.recHits2D_localX[n]
                    y = tree.recHits2D_localY[n]
                    gx = tree.recHits2D_globalX[n]
                    gy = tree.recHits2D_globalY[n]
                    
                    string = 'ME'+sEndcap+sStation+'_l'+sLayer+'_recHits2D'
                    self.hists2D[string].Fill(gx,gy)
                    string = 'ME'+sEndcap+sStation+'_recHits2D'
                    self.hists2D[string].Fill(gx,gy)

                #2DSimHits
                if opt.isMC:
                    for n in range(0,tree.simHits_nSimHits):
                        sEndcap = tree.simHits_ID_endcap[n]
                        if sEndcap == 2: sEndcap = '-'
                        else: sEndcap = '+'
                        sStation = str(tree.simHits_ID_station[n])
                        sChamber = str(tree.simHits_ID_chamber[n])
                        sLayer   = str(tree.simHits_ID_layer[n])
                        sRing    = str(tree.simHits_ID_ring[n])
                        x = tree.simHits_localX[n]
                        y = tree.simHits_localY[n]
                        gx = tree.simHits_globalX[n]
                        gy = tree.simHits_globalY[n]
                        
                        string = 'ME'+sEndcap+sStation+'_l'+sLayer+'_simHits2D'
                        self.hists2D[string].Fill(gx,gy)
                        string = 'ME'+sEndcap+sStation+'_simHits2D'
                        self.hists2D[string].Fill(gx,gy)

                        if abs(tree.simHits_particleType[n]) == 13:
                            shChamberSerial = tree.simHits_ID_chamberSerial[n]
                            shLayer = tree.simHits_ID_layer[n]
                            shChamber = tree.simHits_ID_chamber[n]
                            shRing = tree.simHits_ID_ring[n]
                            shStation = tree.simHits_ID_station[n]
                            shEndcap = tree.simHits_ID_endcap[n]
                            
                            self.simHitsOverallEffDen += 1
                            self.simHitsEffDen[shChamberSerial] += 1
                            self.simHitsLayerEffDen[shChamberSerial][shLayer-1] += 1
                            self.simHitsChamberEffDen[shEndcap-1][shStation-1][shRing-1][shChamber-1][shLayer-1] += 1
                            #SimHit Reco Efficiency
                            for m in range(0,tree.recHits2D_nRecHits2D):
                                if tree.recHits2D_ID_chamberSerial[m] != shChamberSerial: continue
                                if tree.recHits2D_ID_layer[m] != shLayer: continue
                                xLow = tree.recHits2D_localX[m] - sqrt(tree.recHits2D_localXXerr[m])
                                xHigh = tree.recHits2D_localX[m] + sqrt(tree.recHits2D_localXXerr[m])
                                yLow = tree.recHits2D_localY[m] - sqrt(tree.recHits2D_localYYerr[m])
                                yHigh = tree.recHits2D_localY[m] + sqrt(tree.recHits2D_localYYerr[m])
                                if (x < xLow or x > xHigh) and (y < yLow or y > yHigh): continue
                                self.simHitsOverallEffNum += 1
                                self.simHitsEffNum[shChamberSerial] += 1
                                self.simHitsLayerEffNum[shChamberSerial][shLayer-1] += 1
                                self.simHitsChamberEffNum[shEndcap-1][shStation-1][shRing-1][shChamber-1][shLayer-1] += 1
                                break


                                    
            if opt.isDigi and (opt.isLocalReco or opt.isFullReco):
                #ACLT/CLTC
                alctCounter = [0] * 600
                clctCounter = [0] * 600
                lctCounter = [0] * 600
                for i in range(tree.alct_nAlcts):
                    alctCounter[tree.alct_ID_chamberSerial[i]] +=1
                for j in range(tree.clct_nClcts):
                    clctCounter[tree.clct_ID_chamberSerial[j]] +=1
                for jj in range(tree.correlatedLct_nLcts):
                    lctCounter[tree.correlatedLct_ID_chamberSerial[jj]] +=1
                
                for k in range(len(alctCounter)):
                    if alctCounter[k] == 2 and clctCounter[k] == 2:
                        self.hists1D['FourLctChambers_nSegments_Norm'].Fill(CSC_SegmentCounter[k])
                        self.hists1D['FourLctChambers_nCorrelLcts_Norm'].Fill(lctCounter[k])




    def defineHistos(self):

        EC = ['+','-']
        ST = [1,2,3,4]
        RG = [1,2,3,4]
        LR = [1,2,3,4,5,6]


        #CSC Segments
        self.hists1D['OneSegmentChambers_nRecHitLayers_Norm'] = ROOT.TH1F("1SegmentChambers_nLayers","; N Layers; Fraction of Events", 9,-0.5,8.5) 
        self.hists1D['OneSegmentChambers_missingLayer_Norm'] = ROOT.TH1F("1SegmentChambers_missingLayer","; Missing Layer(s); Fraction of Events", 9,-0.5,8.5) 

        #LCTs
        self.hists1D['FourLctChambers_nSegments_Norm'] = ROOT.TH1F("4LctChambers_nSegments","; N Segments; Fractions of Events", 8,-0.5,7.5) 
        self.hists1D['FourLctChambers_nCorrelLcts_Norm'] = ROOT.TH1F("4LctChambers_nCorrelLcts","; N Correlated LCTs; Fractions of Events", 8,-0.5,7.5)

        #SimHits
        self.hists1D['simHitsRecoEfficiency'] = ROOT.TH1F("simHitsRecoEfficiency", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l1'] = ROOT.TH1F("simHitsRecoEfficiency_l1", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l2'] = ROOT.TH1F("simHitsRecoEfficiency_l2", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l3'] = ROOT.TH1F("simHitsRecoEfficiency_l3", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l4'] = ROOT.TH1F("simHitsRecoEfficiency_l4", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l5'] = ROOT.TH1F("simHitsRecoEfficiency_l5", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)
        self.hists1D['simHitsRecoEfficiency_l6'] = ROOT.TH1F("simHitsRecoEfficiency_l6", "; Chamber Serial; RECO Efficiency for Muons", 700, 0, 700)

        #Segments
        self.hists1D['segmentsPerSaMuon_Norm'] = ROOT.TH1F("segmentsPerSaMuon", "; Segments Per Muon; Fraction of SAMuons", 10, -0.5, 9.5)
        
        #Average recHits2D
        self.hists1D['avgRecHits2D'] = ROOT.TH1F("avgRecHits2D", "; Station; Average Rec Hits Per Chamber Per Event", 23,0,23)
        self.hists1D['avgRecHits2D'].SetStats(0)
        self.hists1D['avgRecHits2D'].GetYaxis().SetTitleOffset(1.45)
        for i in range(1,21):
            self.hists1D['avgRecHits2D'].GetXaxis().SetBinLabel(i,self.stationRings[i-1])
        self.hists1D['avgRecHits2D'].GetXaxis().LabelsOption("v")

        self.hists1D['recHitsPerSegment_saMuon_Norm'] = ROOT.TH1F("recHitsPerSegment_saMuon", "; RecHits Per Segment; Fraction of SAMuons", 8,-0.5,7.5)

        self.hists2D['recHitsVSp'] = ROOT.TH2F("recHitsVSp","; P (GeV); N RecHits",1000, 0, 800, 100, 0, 60)
        self.hists2D['recHitsVSpT'] = ROOT.TH2F("recHitsVSpT","; pT (GeV); N RecHits",1000, 0, 800, 100, 0, 60)
        self.hists2D['recHitsPerSegVSp'] = ROOT.TH2F("recHitsPerSegVSp","; P (GeV); N RecHits/Segment", 1000, 0, 800, 8, 0, 8)
        self.hists2D['recHitsPerSegVSpT'] = ROOT.TH2F("recHitsPerSegVSpT","; pT (GeV); N RecHits/Segment", 1000, 0, 800, 8, 0, 8)

        #Segment Layers
        self.hists1D['recHitsPerSegment_Norm'] = ROOT.TH1F("recHitsPerSegment", "; N RecHits; Fraction of Segments", 6, 1.5, 7.5)

        #SimHit Efficiencies
        for i in range(len(EC)):
            for j in range(len(ST)):
                string = 'ME'+str(EC[i])+str(ST[j])
                string1 = string+'_recHits2D'
                self.hists2D[string1] = ROOT.TH2F(string1,"; X; Y", 1600, -800, 800, 1600, -800, 800)
                string2 = string+'_simHits2D'
                self.hists2D[string2] = ROOT.TH2F(string2,"; X; Y", 1600, -800, 800, 1600, -800, 800)
                for k in range(len(RG)):
                    if ST[j] > 1 and RG[k] > 2: continue
                    string3 = string+'-'+str(RG[k])+'_simHitEfficiency'
                    self.hists1D[string3] = ROOT.TH1F(string3,"; Chamber; RECO Efficiency for Muons",40,0,40)
                    string4 = string+'-'+str(RG[k])+'_recHitsPerSegment'
                    self.hists1D[string4+'_Norm'] = ROOT.TH1F(string4+"_recHitsPerSegment", "; N RecHits; Fraction of Segments", 6, 1.5, 7.5)

                for m in range(len(LR)):
                    string = 'ME'+str(EC[i])+str(ST[j])+'_l'+str(LR[m])
                    string1 = string+'_recHits2D'
                    self.hists2D[string1] = ROOT.TH2F(string1,";  X; Y", 1600, -800, 800, 1600, -800, 800)
                    string2 = string+'_simHits2D'
                    self.hists2D[string2] = ROOT.TH2F(string2,";  X; Y", 1600, -800, 800, 1600, -800, 800)
                    for k in range(len(RG)):
                        if ST[j] > 1 and RG[k] > 2: continue
                        string = 'ME'+str(EC[i])+str(ST[j])+'-'+str(RG[k])+'_l'+str(LR[m])
                        string3 = string+'_simHitEfficiency'
                        self.hists1D[string3] = ROOT.TH1F(string3,"; Chamber; RECO Efficiency for Muons",40,0,40)



    def writeHistos(self,Histos1D,Histos2D):
        
        ROOT.gROOT.ProcessLine(".L tdrstyle.cc")
        setTDRStyle(False)
        c = ROOT.TCanvas("c","c",700,700)
        for key in Histos1D:
            c.cd()
            normalized = 'Norm' in key
            if normalized and Histos1D[key].Integral() > 0:
                Histos1D[key].Scale(1/Histos1D[key].Integral())
            Efficiency = 'Efficiency' in key
            if Efficiency:
                Histos1D[key].GetYaxis().SetRangeUser(0.5,1.05)
            Histos1D[key].Draw("HIST")
            c.SaveAs(opt.outDir+'/'+str(Histos1D[key].GetName())+'.eps')
            c.SaveAs(opt.outDir+'/'+str(Histos1D[key].GetName())+'.png')
            c.Clear()

        c1 = ROOT.TCanvas("c1","c1",700,700)
        for key in Histos2D:
            c1.cd()
            Histos2D[key].Draw()
            c1.SaveAs(opt.outDir+'/'+str(Histos2D[key].GetName())+'.eps')
            c1.SaveAs(opt.outDir+'/'+str(Histos2D[key].GetName())+'.png')
            c1.Clear()


    def writeHistosToRoot(self,Histos1D,Histos2D):
        
        ROOT.gROOT.ProcessLine(".L tdrstyle.cc")
        setTDRStyle(False)
        outFile = ROOT.TFile(opt.jobName+'.root',"RECREATE")
        
        for key in Histos1D:
            normalized = 'Norm' in key
            if normalized:
                Histos1D[key].Scale(1/Histos1D[key].Integral())
            outFile.cd()
            Histos1D[key].Write()
        for key in Histos2D:
            Histos2D[key].Write()

        outFile.Write()
        outFile.Close()




    def endjob(self,singleFile):

        #Fill Eff Hists
        for x in range(0,600):
            eff = 0
            if self.simHitsEffDen[x] > 0:
                eff = float(self.simHitsEffNum[x])/self.simHitsEffDen[x]
            if x == 599:
                eff = float(self.simHitsOverallEffNum)/self.simHitsOverallEffDen

            #print x,  self.simHitsEffDen[x], self.simHitsEffNum[x], eff, self.simHitsOverallEffDen, self.simHitsOverallEffDen
            self.hists1D['simHitsRecoEfficiency'].SetBinContent(x+1,eff)
            for y in range(0,6):
                eff = 0
                if self.simHitsLayerEffDen[x][y] > 0:
                    eff = float(self.simHitsLayerEffNum[x][y])/self.simHitsLayerEffDen[x][y]
                self.hists1D['simHitsRecoEfficiency_l'+str(y+1)].SetBinContent(x+1,eff)



        #Average recHits2D
        myFile = open(opt.outDir+'/AverageRecHits2D.txt', 'w')
        counter = 0
        for x in self.stationRings:
            counter+=1
            self.nRecHitsPerStation[x] = self.nRecHitsPerStation[x]/self.nChambers[x]/self.totalEvents
            self.hists1D['avgRecHits2D'].SetBinContent(counter,self.nRecHitsPerStation[x])
            string = x+'   '+str(self.nRecHitsPerStation[x])+'\n'
            myFile.write(string)
        myFile.close()



        myEffFile = open(opt.outDir+'/SimHitEfficiencies.txt', 'w')
        
        for ec in range(0,2):
            EC = '+'
            if ec == 1:
                EC = '-'
            for st in range(0,4):
                for rg in range(0,4):
                    if st+1 > 1 and rg+1 > 2: continue
                    string = 'ME'+EC+str(st+1)+'/'+str(rg+1)+'\n'
                    myEffFile.write(string)
                    myEffFile.write('--------------------\n')
                    for ch in range(0,36):
                        if st+1 > 1 and rg+1 == 1 and ch+1 > 18: continue
                        num = 0
                        den = 0
                        effAr = [0] * 6
                        for lr in range(0,6):
                            eff = 0
                            if self.simHitsChamberEffDen[ec][st][rg][ch][lr] > 0:
                                eff = float(self.simHitsChamberEffNum[ec][st][rg][ch][lr])/self.simHitsChamberEffDen[ec][st][rg][ch][lr]
                                effAr[lr] = eff;
                                num += self.simHitsChamberEffNum[ec][st][rg][ch][lr]
                                den += self.simHitsChamberEffDen[ec][st][rg][ch][lr]
                            string = 'ME'+EC+str(st+1)+'-'+str(rg+1)+'_l'+str(lr+1)+'_simHitEfficiency'
                            self.hists1D[string].SetBinContent(ch+1,eff)
 

                        if den > 0:
                            eff = float(num)/den
                        else: eff = 0
                        string = 'ME'+EC+str(st+1)+'-'+str(rg+1)+'_simHitEfficiency'
                        self.hists1D[string].SetBinContent(ch+1,eff)
                        #string = '  Chamber '+str(ch+1)+': '+str(effAr[0])+'  '+str(effAr[1])+'  '+str(effAr[2])+'  '+str(effAr[3])+'  '+str(effAr[4])+'  '+str(effAr[5])+'     '+str(eff)+'\n'
                        string = '  Chamber {0}: {1:.3f} {2:.3f} {3:.3f} {4:.3f} {5:.3f}   {6:.3f}\n'.format(ch+1,effAr[0],effAr[1],effAr[2],effAr[3],effAr[4],effAr[5],eff)
                        myEffFile.write(string)
                    myEffFile.write('\n')
                myEffFile.write('\n')

                    
        myEffFile.close()

        if self.totalEvents > 0:
            if singleFile:
                self.writeHistos(self.hists1D,self.hists2D)
            else:
                self.writeHistosToRoot(self.hists1D,self.hists2D)
        




#Main  
if __name__ == "__main__":


    global opt, args
    parseOptions()

    myClass = Analysis()
    singleFile = False

    if opt.file.endswith(".root"):
        singleFile = True
    elif opt.file.endswith(".txt"):
        singleFile = False
    else:
        raise RuntimeError, "opt.file: file name does not end with .root or .txt!"

    print "Begin Analysis"

    # Loop for parallel or single file 
    if singleFile:
        myClass.doAnalysis(opt.file)
    else:
        lines = open(opt.file,"r")
        for line in lines:
            f = line.split()
            if not f[0].endswith(".root"): continue
            if len(f) < 1: continue
            print "Opening file",f[0]
            myClass.doAnalysis(f[0])
            

    myClass.endjob(singleFile)
