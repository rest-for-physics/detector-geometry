#include "OpticalTest.h"

#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4Orb.hh>
#include <G4PVPlacement.hh>
#include <G4RunManager.hh>
#include <G4Sphere.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <G4VisAttributes.hh>
#include <filesystem>

void OpticalTest::Initialize() {
    double worldSize = 1 * m;

    auto solidWorld = new G4Box("World", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    auto logicWorld = new G4LogicalVolume(solidWorld, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), solidWorld->GetName());
    logicWorld->SetVisAttributes(G4VisAttributes(false));  // invisible

    fWorld = new G4PVPlacement(nullptr,                // rotation
                               G4ThreeVector(),        // position
                               logicWorld,             // its logical volume
                               logicWorld->GetName(),  // its name
                               nullptr,                // its mother  volume
                               false,                  // no boolean operation
                               0,                      // copy number
                               fCheckOverlaps);        // overlaps checking

    auto hydrogen = G4NistManager::Instance()->FindOrBuildElement(1);  // Z = 1 = Hydrogen
    auto carbon = G4NistManager::Instance()->FindOrBuildElement(6);    // Z = 6 = Carbon

    auto scintillatorMaterial = new G4Material("BC408 plastic scintillator", 1.032 * g / cm3, 2);  // BC408
    // H to C ratio = 1.104
    scintillatorMaterial->AddElement(hydrogen, 0.085000);
    scintillatorMaterial->AddElement(carbon, 0.915000);

    auto solidScintillator = new G4Box("Scintillator", 200 * mm / 2, 50 * mm / 2, 800 * mm / 2);
    auto LogicScintillator = new G4LogicalVolume(solidScintillator, scintillatorMaterial, solidScintillator->GetName());
    new G4PVPlacement(nullptr, G4ThreeVector(), LogicScintillator->GetName(), LogicScintillator, fWorld, false, 0);

    std::vector<G4double> photonEnergy = {2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV,
                                          2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV, 2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
                                          2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
                                          3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV};

    std::vector<G4double> refractiveIndex1 = {1.3435, 1.344, 1.3445, 1.345,  1.3455, 1.346,  1.3465, 1.347,  1.3475, 1.348,  1.3485,
                                              1.3492, 1.35,  1.3505, 1.351,  1.3518, 1.3522, 1.3530, 1.3535, 1.354,  1.3545, 1.355,
                                              1.3555, 1.356, 1.3568, 1.3572, 1.358,  1.3585, 1.359,  1.3595, 1.36,   1.3608};
    std::vector<G4double> absorption = {3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m,  12.346 * m, 13.889 * m, 15.152 * m, 17.241 * m,
                                        18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m, 45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m,
                                        55.556 * m, 52.632 * m, 52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
                                        30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m, 17.500 * m, 14.500 * m};
    std::vector<G4double> scintilFast = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                                         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
    std::vector<G4double> scintilSlow = {0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00, 3.00, 2.00,
                                         1.00, 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00};
    auto myMPT1 = new G4MaterialPropertiesTable();
    // Values can be added to the material property table individually.
    // Check that group velocity is calculated from RINDEX
    myMPT1->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0], 1)->SetSpline(true);
    for (size_t i = 1; i < photonEnergy.size(); ++i) {
        myMPT1->AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
    }
    if (myMPT1->GetProperty("RINDEX")->GetVectorLength() != myMPT1->GetProperty("GROUPVEL")->GetVectorLength()) {
        G4ExceptionDescription ed;
        ed << "Error calculating group velocities. Incorrect number of entries "
              "in group velocity material property vector.";
        G4Exception("OpNovice::OpNoviceDetectorConstruction", "OpNovice001", FatalException, ed);
    }
    myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption)->SetSpline(true);
    myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFast)->SetSpline(true);
    myMPT1->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintilSlow)->SetSpline(true);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD", 50. / MeV);
    myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
    myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1. * ns);
    myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD2", 0.2);

    std::vector<G4double> energy_scintillatorMaterial = {
        1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV, 1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
        1.7971 * eV,  1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV, 1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
        2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV, 2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
        2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV, 2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
        3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV, 3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
        4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV, 5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV};

    // Rayleigh scattering length calculated by G4OpRayleigh
    // assume 100 times larger than the rayleigh scattering for now.
    std::vector<G4double> mie_scintillatorMaterial = {
        167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m, 128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
        97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m, 71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
        51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m, 36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
        24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m, 16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
        9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m, 5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
        3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m, 1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m};

    // gforward, gbackward, forward backward ratio
    G4double mie_scintillatorMaterial_const[3] = {0.99, 0.99, 0.8};

    myMPT1->AddProperty("MIEHG", energy_scintillatorMaterial, mie_scintillatorMaterial)->SetSpline(true);
    myMPT1->AddConstProperty("MIEHG_FORWARD", mie_scintillatorMaterial_const[0]);
    myMPT1->AddConstProperty("MIEHG_BACKWARD", mie_scintillatorMaterial_const[1]);
    myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO", mie_scintillatorMaterial_const[2]);

    G4cout << "scintillatorMaterial G4MaterialPropertiesTable:" << G4endl;
    myMPT1->DumpTable();

    scintillatorMaterial->SetMaterialPropertiesTable(myMPT1);
}

void OpticalTest::OpNovice() {
    // Adapted from https://github.com/Geant4/geant4/blob/master/examples/extended/optical/OpNovice/src/OpNoviceDetectorConstruction.cc
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_Air");
    G4Material* scintillatorMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_scintillatorMaterial");

    std::vector<G4double> photonEnergy = {2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV,
                                          2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV, 2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
                                          2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
                                          3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV};

    std::vector<G4double> refractiveIndex1 = {1.3435, 1.344, 1.3445, 1.345,  1.3455, 1.346,  1.3465, 1.347,  1.3475, 1.348,  1.3485,
                                              1.3492, 1.35,  1.3505, 1.351,  1.3518, 1.3522, 1.3530, 1.3535, 1.354,  1.3545, 1.355,
                                              1.3555, 1.356, 1.3568, 1.3572, 1.358,  1.3585, 1.359,  1.3595, 1.36,   1.3608};
    std::vector<G4double> absorption = {3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m,  12.346 * m, 13.889 * m, 15.152 * m, 17.241 * m,
                                        18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m, 45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m,
                                        55.556 * m, 52.632 * m, 52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
                                        30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m, 17.500 * m, 14.500 * m};
    std::vector<G4double> scintilFast = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                                         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
    std::vector<G4double> scintilSlow = {0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00, 3.00, 2.00,
                                         1.00, 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00};
    auto myMPT1 = new G4MaterialPropertiesTable();
    // Values can be added to the material property table individually.
    // Check that group velocity is calculated from RINDEX
    myMPT1->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0], 1)->SetSpline(true);
    for (size_t i = 1; i < photonEnergy.size(); ++i) {
        myMPT1->AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
    }
    if (myMPT1->GetProperty("RINDEX")->GetVectorLength() != myMPT1->GetProperty("GROUPVEL")->GetVectorLength()) {
        G4ExceptionDescription ed;
        ed << "Error calculating group velocities. Incorrect number of entries "
              "in group velocity material property vector.";
        G4Exception("OpNovice::OpNoviceDetectorConstruction", "OpNovice001", FatalException, ed);
    }
    myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption)->SetSpline(true);
    myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFast)->SetSpline(true);
    myMPT1->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintilSlow)->SetSpline(true);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD", 50. / MeV);
    myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
    myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1. * ns);
    myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
    myMPT1->AddConstProperty("SCINTILLATIONYIELD2", 0.2);

    std::vector<G4double> energy_scintillatorMaterial = {
        1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV, 1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
        1.7971 * eV,  1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV, 1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
        2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV, 2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
        2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV, 2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
        3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV, 3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
        4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV, 5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV};

    // Rayleigh scattering length calculated by G4OpRayleigh
    // assume 100 times larger than the rayleigh scattering for now.
    std::vector<G4double> mie_scintillatorMaterial = {
        167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m, 128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
        97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m, 71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
        51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m, 36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
        24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m, 16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
        9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m, 5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
        3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m, 1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m};

    // gforward, gbackward, forward backward ratio
    G4double mie_scintillatorMaterial_const[3] = {0.99, 0.99, 0.8};

    myMPT1->AddProperty("MIEHG", energy_scintillatorMaterial, mie_scintillatorMaterial)->SetSpline(true);
    myMPT1->AddConstProperty("MIEHG_FORWARD", mie_scintillatorMaterial_const[0]);
    myMPT1->AddConstProperty("MIEHG_BACKWARD", mie_scintillatorMaterial_const[1]);
    myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO", mie_scintillatorMaterial_const[2]);

    G4cout << "scintillatorMaterial G4MaterialPropertiesTable:" << G4endl;
    myMPT1->DumpTable();

    scintillatorMaterial->SetMaterialPropertiesTable(myMPT1);

    // Set the Birks Constant for the scintillatorMaterial scintillator
    scintillatorMaterial->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

    // Air
    std::vector<G4double> refractiveIndex2 = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                                              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2);

    G4cout << "Air G4MaterialPropertiesTable:" << G4endl;
    myMPT2->DumpTable();

    air->SetMaterialPropertiesTable(myMPT2);

    // ------------- Volumes --------------
    auto hall_xyz = 10.0 * m;
    auto tank_xyz = 10.0 * m;
    auto bubble_xyz = 10.0 * m;

    G4Box* expHall_box = new G4Box("expHall", hall_xyz / 2, hall_xyz / 2, hall_xyz / 2);
    G4LogicalVolume* expHall_log = new G4LogicalVolume(expHall_box, air, "expHall", 0, 0, 0);
    G4VPhysicalVolume* expHall_phys = new G4PVPlacement(0, G4ThreeVector(), expHall_log, "expHall", fWorld->GetLogicalVolume(), false, 0);
    // The scintillatorMaterial Tank
    G4Box* scintillatorMaterialTank_box = new G4Box("Tank", tank_xyz / 2, tank_xyz / 2, tank_xyz / 2);
    G4LogicalVolume* scintillatorMaterialTank_log = new G4LogicalVolume(scintillatorMaterialTank_box, scintillatorMaterial, "Tank", 0, 0, 0);
    G4VPhysicalVolume* scintillatorMaterialTank_phys =
        new G4PVPlacement(0, G4ThreeVector(), scintillatorMaterialTank_log, "Tank", expHall_log, false, 0);
    // The Air Bubble
    G4Box* bubbleAir_box = new G4Box("Bubble", bubble_xyz / 2, bubble_xyz / 2, bubble_xyz / 2);
    G4LogicalVolume* bubbleAir_log = new G4LogicalVolume(bubbleAir_box, air, "Bubble", 0, 0, 0);
    new G4PVPlacement(0, G4ThreeVector(0, 2.5 * m, 0), bubbleAir_log, "Bubble", scintillatorMaterialTank_log, false, 0);
    // ------------- Surfaces --------------
    // scintillatorMaterial Tank
    G4OpticalSurface* opscintillatorMaterialSurface = new G4OpticalSurface("scintillatorMaterialSurface");
    opscintillatorMaterialSurface->SetType(dielectric_LUTDAVIS);
    opscintillatorMaterialSurface->SetFinish(Rough_LUT);
    opscintillatorMaterialSurface->SetModel(DAVIS);

    G4LogicalBorderSurface* scintillatorMaterialSurface =
        new G4LogicalBorderSurface("scintillatorMaterialSurface", scintillatorMaterialTank_phys, expHall_phys, opscintillatorMaterialSurface);

    G4OpticalSurface* opticalSurface =
        dynamic_cast<G4OpticalSurface*>(scintillatorMaterialSurface->GetSurface(scintillatorMaterialTank_phys, expHall_phys)->GetSurfaceProperty());
    if (opticalSurface) opticalSurface->DumpInfo();

    // Air Bubble
    G4OpticalSurface* opAirSurface = new G4OpticalSurface("AirSurface");
    opAirSurface->SetType(dielectric_dielectric);
    opAirSurface->SetFinish(polished);
    opAirSurface->SetModel(glisur);

    G4LogicalSkinSurface* airSurface = new G4LogicalSkinSurface("AirSurface", bubbleAir_log, opAirSurface);

    opticalSurface = dynamic_cast<G4OpticalSurface*>(airSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());
    if (opticalSurface) opticalSurface->DumpInfo();

    // Generate & Add Material Properties Table attached to the optical surfaces
    std::vector<G4double> ephoton = {2.034 * eV, 4.136 * eV};

    // OpticalAirSurface
    std::vector<G4double> reflectivity = {0.3, 0.5};
    std::vector<G4double> efficiency = {0.8, 1.0};

    G4MaterialPropertiesTable* myST2 = new G4MaterialPropertiesTable();

    myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity);
    myST2->AddProperty("EFFICIENCY", ephoton, efficiency);
    G4cout << "Air Surface G4MaterialPropertiesTable:" << G4endl;
    myST2->DumpTable();

    opAirSurface->SetMaterialPropertiesTable(myST2);
}
