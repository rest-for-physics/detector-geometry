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

using namespace std;

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

    //
    Scintillator();
}

void OpticalTest::Scintillator() {
    // Adapted from https://github.com/Geant4/geant4/blob/master/examples/extended/optical/OpNovice/src/OpNoviceDetectorConstruction.cc
    auto hydrogen = G4NistManager::Instance()->FindOrBuildElement(1);  // Z = 1 = Hydrogen
    auto carbon = G4NistManager::Instance()->FindOrBuildElement(6);    // Z = 6 = Carbon

    auto scintillatorMaterial = new G4Material("BC408 plastic scintillator", 1.032 * g / cm3, 2);  // BC408
    // H to C ratio = 1.104
    scintillatorMaterial->AddElement(hydrogen, 0.085000);
    scintillatorMaterial->AddElement(carbon, 0.915000);

    double scintillatorLength = 800 * mm;
    double scintillatorWidth = 200 * mm;
    double scintillatorThickness = 50 * mm;
    double lightGuide1Length = 130.0 * mm;
    double lightGuide2Length = 80.0 * mm;

    auto scintillatorMaterialPropertiesTable = new G4MaterialPropertiesTable();

    vector<double> photonEnergy = {2.95200 * eV};

    auto buildVector = [photonEnergy](double value) {
        vector<double> result;
        for (size_t i = 0; i < photonEnergy.size(); i++) result.push_back(value);
        return result;
    };

    vector<double> scintillation = buildVector(1);
    vector<double> refractiveIndexScintillator = buildVector(1.54);
    vector<double> absorptionLength = buildVector(210.0 * cm);

    scintillatorMaterialPropertiesTable->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintillation);
    scintillatorMaterialPropertiesTable->AddProperty("RINDEX", photonEnergy, refractiveIndexScintillator);
    scintillatorMaterialPropertiesTable->AddProperty("ABSLENGTH", photonEnergy, absorptionLength);
    scintillatorMaterialPropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
    scintillatorMaterialPropertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);
    scintillatorMaterialPropertiesTable->AddConstProperty("SCINTILLATIONRISETIME1", 0.9 * ns);
    scintillatorMaterialPropertiesTable->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.9 * ns);
    scintillatorMaterialPropertiesTable->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    scintillatorMaterial->SetMaterialPropertiesTable(scintillatorMaterialPropertiesTable);

    // scintillatorMaterial->GetIonisation()->SetBirksConstant(0.110 * mm / MeV); // TODO: WHAT DOES THIS DO?

    auto solidScintillator = new G4Box("Scintillator", scintillatorWidth / 2, scintillatorThickness / 2, scintillatorLength / 2);
    auto LogicScintillator = new G4LogicalVolume(solidScintillator, scintillatorMaterial, solidScintillator->GetName());
    new G4PVPlacement(nullptr, G4ThreeVector(), LogicScintillator->GetName(), LogicScintillator, fWorld, false, 0);

    G4Material* lightGuideMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_LUCITE");

    auto solidLightGuide1 = new G4Trd("LightGuide1", scintillatorWidth / 2, scintillatorWidth / 2, scintillatorThickness / 2,
                                      scintillatorThickness / 2 + 5, lightGuide1Length / 2);
    auto solidLightGuide2 = new G4Trd("LightGuide2", scintillatorWidth / 2, 70.0 / 2, scintillatorThickness / 2 + 5, 70.0 / 2, lightGuide2Length / 2);

    auto solidLightGuide = new G4UnionSolid("LightGuide", solidLightGuide1, solidLightGuide2, nullptr,
                                            G4ThreeVector(0, 0, lightGuide1Length / 2 + lightGuide2Length / 2));
    auto logicLightGuide = new G4LogicalVolume(solidLightGuide, lightGuideMaterial, solidLightGuide->GetName());
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, scintillatorLength / 2 + lightGuide1Length / 2), logicLightGuide->GetName(), logicLightGuide,
                      fWorld, false, 0);

    auto airPropertiesTable = new G4MaterialPropertiesTable();
    airPropertiesTable->AddProperty("RINDEX", photonEnergy, buildVector(1.00))->SetSpline(true);
    auto worldMaterial = fWorld->GetLogicalVolume()->GetMaterial();
    worldMaterial->SetMaterialPropertiesTable(airPropertiesTable);

    auto lightGuidePropertiesTable = new G4MaterialPropertiesTable();
    lightGuidePropertiesTable->AddProperty("RINDEX", photonEnergy, buildVector(1.49))->SetSpline(true);
    lightGuideMaterial->SetMaterialPropertiesTable(lightGuidePropertiesTable);
}
