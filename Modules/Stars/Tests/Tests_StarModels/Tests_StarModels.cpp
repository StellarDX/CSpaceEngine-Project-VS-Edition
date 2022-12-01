#include <iostream>
#include <fstream>
#include "CSE/Stars.h"

using namespace std;
using namespace cse;

int main()
{
    Object MS = MainSequenceStarModel("G2V")(random);
    MS.Name[0] = "MainSequenceStar";
    Object HMS = HPMainSequenceStarModel("A1V")(random);
    HMS.Name[0] = "HPMainSequenceStar";
    Object RGBr = RedGiantBranch()(random);
    RGBr.Name[0] = "RedGiantBranch";
    Object HB = HorizontalBranch()(random);
    HB.Name[0] = "HorizontalBranch";
    Object RC = RedClumpGiantModel()(random);
    RC.Name[0] = "RedClumpGiant";
    Object AGB = AsymptoticGiantBranch()(random);
    AGB.Name[0] = "AsymptoticGiantBranch";
    Object SAGB = SuperAsymptoticGiantBranch()(random);
    SAGB.Name[0] = "SuperAsymptoticGiantBranch";
    Object RMS = RandomMainSequenceStar();
    RMS.Name[0] = "RandomMainSequenceStar";
    Object Csd = CoolSubdwarfModel("sdM1")(random);
    Csd.Name[0] = "CoolSubdwarf";
    Object sdO = SubdwarfOStarModel()(random);
    sdO.Name[0] = "SubdwarfOStar";
    Object sdB = SubdwarfBStarModel()(random);
    sdB.Name[0] = "SubdwarfBStar";
    Object WR = WolfRayetStarModel("WN5h")(random);
    WR.Name[0] = "WolfRayetStar";
    Object HWR = HPWolfRayetStarModel("WO2")(random);
    HWR.Name[0] = "HPWolfRayetStar";
    Object OB = RandomOBStar();
    OB.Name[0] = "RandomOBStar";
    Object LSG = LowMassSubgiantModel()(random);
    LSG.Name[0] = "LowMassSubgiant";
    Object MSG = MidSizedSubgiantModel()(random);
    MSG.Name[0] = "MidSizedSubgiant";
    Object HSG = MassiveSubgiantModel()(random);
    HSG.Name[0] = "MassiveSubgiant";
    Object BG = BlueGiantModel()(random);
    BG.Name[0] = "BlueGiant";
    Object SG = SupergiantModel("A2Ia")(random);
    SG.Name[0] = "Supergiant";
    Object BSG = RandomBlueSupergiant();
    BSG.Name[0] = "RandomBlueSupergiant";
    Object RSG = RandomRedSupergiant();
    RSG.Name[0] = "RandomRedSupergiant";
    Object YSG = RandomYellowSupergiant();
    YSG.Name[0] = "RandomYellowSupergiant";
    Object HG = HypergiantModel("O2 0")(random);
    HG.Name[0] = "Hypergiant";
    Object CS = CarbonStarModel("C")(random);
    CS.Name[0] = "CarbonStar";
    Object SS = CarbonStarModel("S")(random);
    SS.Name[0] = "STypeStar";
    Object WD = WhiteDwarfModel("DZ8")(random);
    WD.Name[0] = "WhiteDwarf";
    Object BD = BrownDwarfModel("L2.5V")(random);
    BD.Name[0] = "BrownDwarf";
    Object HBD = HPBrownDwarfModel("T1V")(random);
    HBD.Name[0] = "HPBrownDwarf";

    ofstream Report("TestReport_StarGen.sc");
    OSCStream fout(Report);
    fout << NoBooleans << FlatObjectDimAsRadius
        << MS << HMS << RGBr << HB << RC
        << AGB << SAGB << RMS << Csd << sdO
        << sdB << WR << HWR << OB << LSG
        << MSG << HSG << BG << SG << BSG
        << RSG << YSG << HG << CS << SS
        << WD << BD << HBD;
    fout.write();
}
