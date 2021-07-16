//
// Created by lobis on 16/07/2021.
//

#include "BabyIaxoGeometry.h"

#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4Orb.hh>
#include <G4PVPlacement.hh>
#include <G4RunManager.hh>
#include <G4Sphere.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>
#include <filesystem>

namespace materials {
auto Vacuum = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Galactic");
auto Copper = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Cu");
auto PTFE = G4NistManager::Instance() -> FindOrBuildMaterial("G4_TEFLON");
auto Kapton = G4NistManager::Instance() -> FindOrBuildMaterial("G4_KAPTON");
auto Lead = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Pb");
auto Mylar = G4NistManager::Instance() -> FindOrBuildMaterial("G4_MYLAR");
// auto Scintillator = G4NistManager::Instance() -> FindOrBuildMaterial("BC408"); // TODO: build
auto LightGuide = G4NistManager::Instance() -> FindOrBuildMaterial("G4_LUCITE");
auto Cadmium = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Cd");
auto Neoprene = G4NistManager::Instance() -> FindOrBuildMaterial("G4_NEOPRENE");
auto Steel = G4NistManager::Instance() -> FindOrBuildMaterial("G4_STAINLESS_STEEL");

}  // namespace materials

namespace dimensions::chamber {
// Body
double Height(30.0 * mm), Diameter(102.0 * mm);
double BackplateThickness(15.0 * mm), SquareSide(134.0 * mm);
double TeflonWallThickness(1.0 * mm);
// Readout
double ReadoutKaptonThickness(0.5 * mm), ReadoutCopperThickness(0.2 * mm), ReadoutPlaneSide(60.0 * mm);
// Cathode
double CathodeTeflonDiskHoleRadius(15.0), CathodeTeflonDiskThickness(5.0), CathodeCopperSupportOuterRadius(45.0);
double CathodeCopperSupportInnerRadius(8.5), CathodeCopperSupportThickness(1.0), CathodeWindowThickness(0.004);
double CathodePatternDiskRadius(4.25), CathodePatternLineWidth(0.3);
}  // namespace dimensions::chamber

void BabyIAXOGeometry::Initialize() {
    G4bool checkOverlaps = true;

    double worldSize = 1000 * mm;

    auto solidWorld = new G4Box("World", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    auto logicWorld = new G4LogicalVolume(solidWorld, materials::Vacuum, solidWorld->GetName());
    logicWorld->SetVisAttributes(G4VisAttributes(false));  // invisible

    G4VPhysicalVolume* physicalWorld = new G4PVPlacement(nullptr,                // rotation
                                                         G4ThreeVector(),        // position
                                                         logicWorld,             // its logical volume
                                                         logicWorld->GetName(),  // its name
                                                         nullptr,                // its mother  volume
                                                         false,                  // no boolean operation
                                                         0,                      // copy number
                                                         checkOverlaps);         // overlaps checking
    fWorld = physicalWorld;

    // // Chamber
    // Chamber body
    auto solidChamberBody = new G4SubtractionSolid(
        "Chamber",  //
        new G4Box("solidChamberBase", dimensions::chamber::SquareSide / 2, dimensions::chamber::SquareSide / 2, dimensions::chamber::Height / 2),
        new G4Tubs("chamberBodyHole", 0, dimensions::chamber::Diameter / 2, dimensions::chamber::Height / 2 * expansionFactor, 0, 2 * M_PI));
    auto logicChamberBody = new G4LogicalVolume(solidChamberBody, materials::Copper, solidChamberBody->GetName());
    // Chamber backplate
    auto solidChamberBackplate = new G4Box("solidChamberBackplate", dimensions::chamber::SquareSide / 2, dimensions::chamber::SquareSide / 2,
                                           dimensions::chamber::BackplateThickness / 2);
    auto logicChamberBackplate = new G4LogicalVolume(solidChamberBackplate, materials::Copper, solidChamberBackplate->GetName());
    // Chamber teflon inner wall
    auto solidChamberTeflonWall = new G4Tubs("solidChamberTeflonWall", dimensions::chamber::Diameter / 2 - dimensions::chamber::TeflonWallThickness,
                                             dimensions::chamber::Diameter / 2, dimensions::chamber::Height / 2, 0, 2 * M_PI);
    auto logicChamberTeflonWall = new G4LogicalVolume(solidChamberTeflonWall, materials::PTFE, solidChamberTeflonWall->GetName());
    // Chamber kapton readout
    auto solidKaptonReadout = new G4Box("solidKaptonReadout", dimensions::chamber::SquareSide / 2, dimensions::chamber::SquareSide / 2,
                                        dimensions::chamber::ReadoutKaptonThickness / 2);
    auto logicKaptonReadout = new G4LogicalVolume(solidKaptonReadout, materials::Kapton, solidKaptonReadout->GetName());
    // Chamber copper readout
    auto solidCopperReadout = new G4Box("solidCopperReadout", dimensions::chamber::ReadoutPlaneSide / 2, dimensions::chamber::ReadoutPlaneSide / 2,
                                        dimensions::chamber::ReadoutCopperThickness / 2);
    auto logicCopperReadout = new G4LogicalVolume(solidCopperReadout, materials::Copper, solidCopperReadout->GetName());

    // Chamber placement
    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberBody->GetName(), logicChamberBody, physicalWorld, false, 0);

    new G4PVPlacement(
        nullptr,
        G4ThreeVector(0, 0,
                      -dimensions::chamber::Height / 2 - dimensions::chamber::ReadoutKaptonThickness - dimensions::chamber::BackplateThickness / 2),
        logicChamberBackplate->GetName(), logicChamberBackplate, physicalWorld, false, 0);

    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberTeflonWall->GetName(), logicChamberTeflonWall, physicalWorld, false, 0);

    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -dimensions::chamber::Height / 2 - dimensions::chamber::ReadoutKaptonThickness / 2),
                      logicKaptonReadout->GetName(), logicKaptonReadout, physicalWorld, false, 0);
    auto rotation = new G4RotationMatrix();
    rotation->rotateZ(M_PI_4);  // 45 degrees
    new G4PVPlacement(rotation, G4ThreeVector(0, 0, -dimensions::chamber::Height / 2 + dimensions::chamber::ReadoutCopperThickness / 2),
                      logicCopperReadout->GetName(), logicCopperReadout, physicalWorld, false, 0);
}
