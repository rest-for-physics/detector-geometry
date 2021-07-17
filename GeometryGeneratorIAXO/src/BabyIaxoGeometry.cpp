
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
#include <G4UnionSolid.hh>
#include <G4VisAttributes.hh>
#include <filesystem>

// TODO: place this in the base class
namespace materials {
auto Gas = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Ar");
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

namespace chamber {
// Body
double Height(30.0 * mm), Diameter(102.0 * mm);
double BackplateThickness(15.0 * mm), SquareSide(134.0 * mm);
double TeflonWallThickness(1.0 * mm);
// Readout
double ReadoutKaptonThickness(0.5 * mm), ReadoutCopperThickness(0.2 * mm), ReadoutPlaneSide(60.0 * mm);
// Cathode
double CathodeTeflonDiskHoleRadius(15.0 * mm), CathodeTeflonDiskThickness(5.0 * mm), CathodeCopperSupportOuterRadius(45.0 * mm);
double CathodeCopperSupportInnerRadius(8.5 * mm), CathodeCopperSupportThickness(1.0 * mm), CathodeWindowThickness(0.004 * mm);
double CathodePatternDiskRadius(4.25 * mm), CathodePatternLineWidth(0.3 * mm);
}  // namespace chamber

namespace shielding {
double SizeXY(590.0 * mm), SizeZ(540.0 * mm), ShaftShortSideX(194.0 * mm), ShaftShortSideY(170.0 * mm), ShaftLongSide(340.0 * mm);
double DetectorToShieldingSeparation(-60.0 * mm), EnvelopeThickness(10.0 * mm);
double OffsetZ(DetectorToShieldingSeparation + chamber::Height / 2 + chamber::ReadoutKaptonThickness + chamber::BackplateThickness);
}  // namespace shielding

void BabyIAXOGeometry::Initialize() {
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
                                                         fCheckOverlaps);        // overlaps checking
    fWorld = physicalWorld;

    PlaceChamber();
    PlaceShielding();
}

void BabyIAXOGeometry::PlaceChamber() {
    // // Chamber
    auto rotation45degZ = new G4RotationMatrix(0, 0, M_PI_4);
    auto rotation90degZ = new G4RotationMatrix(0, 0, 2 * M_PI_4);
    auto rotation135degZ = new G4RotationMatrix(0, 0, 3 * M_PI_4);
    // Chamber body
    auto solidChamberBody =
        new G4SubtractionSolid("ChamberBody", new G4Box("ChamberBodyBase", chamber::SquareSide / 2, chamber::SquareSide / 2, chamber::Height / 2),
                               new G4Tubs("ChamberBodyHole", 0, chamber::Diameter / 2, chamber::Height / 2 * fExpansionFactor, 0, 2 * M_PI));
    auto logicChamberBody = new G4LogicalVolume(solidChamberBody, materials::Copper, solidChamberBody->GetName());
    // Chamber backplate
    auto solidChamberBackplate = new G4Box("ChamberBackplate", chamber::SquareSide / 2, chamber::SquareSide / 2, chamber::BackplateThickness / 2);
    auto logicChamberBackplate = new G4LogicalVolume(solidChamberBackplate, materials::Copper, solidChamberBackplate->GetName());
    // Chamber teflon inner wall
    auto solidChamberTeflonWall = new G4Tubs("ChamberTeflonWall", chamber::Diameter / 2 - chamber::TeflonWallThickness, chamber::Diameter / 2,
                                             chamber::Height / 2, 0, 2 * M_PI);
    auto logicChamberTeflonWall = new G4LogicalVolume(solidChamberTeflonWall, materials::PTFE, solidChamberTeflonWall->GetName());
    // Chamber kapton readout
    auto solidKaptonReadout = new G4Box("KaptonReadout", chamber::SquareSide / 2, chamber::SquareSide / 2, chamber::ReadoutKaptonThickness / 2);
    auto logicKaptonReadout = new G4LogicalVolume(solidKaptonReadout, materials::Kapton, solidKaptonReadout->GetName());
    // Chamber copper readout
    auto solidCopperReadout =
        new G4Box("CopperReadout", chamber::ReadoutPlaneSide / 2, chamber::ReadoutPlaneSide / 2, chamber::ReadoutCopperThickness / 2);
    auto logicCopperReadout = new G4LogicalVolume(solidCopperReadout, materials::Copper, solidCopperReadout->GetName());
    // Chamber cathode support
    auto solidCathodeTeflonDiskBase = new G4Tubs("CathodeTeflonDiskBase", chamber::CathodeTeflonDiskHoleRadius, chamber::SquareSide / 2,
                                                 chamber::CathodeTeflonDiskThickness / 2, 0, 2 * M_PI);
    auto solidCathodeCopperDisk = new G4Tubs("CathodeCopperDisk", chamber::CathodeCopperSupportInnerRadius, chamber::CathodeCopperSupportOuterRadius,
                                             chamber::CathodeCopperSupportThickness / 2, 0, 2 * M_PI);
    auto solidCathodeTeflonDisk =
        new G4SubtractionSolid("CathodeTeflonDisk", solidCathodeTeflonDiskBase, solidCathodeCopperDisk, nullptr,
                               G4ThreeVector(0, 0, -chamber::CathodeTeflonDiskThickness / 2 + chamber::CathodeCopperSupportThickness / 2));
    auto logicCathodeTeflonDisk = new G4LogicalVolume(solidCathodeTeflonDisk, materials::PTFE, solidCathodeTeflonDisk->GetName());
    // cathode pattern
    auto solidCathodeWindow = new G4Tubs("CathodeWindow", 0, chamber::CathodeTeflonDiskHoleRadius, chamber::CathodeWindowThickness / 2, 0, 2 * M_PI);
    auto logicCathodeWindow = new G4LogicalVolume(solidCathodeWindow, materials::Mylar, solidCathodeWindow->GetName());
    auto solidCathodePatternLineAux = new G4Box("CathodePatternLineAux", chamber::CathodePatternLineWidth / 2,
                                                chamber::CathodeCopperSupportInnerRadius, chamber::CathodeCopperSupportThickness / 2);
    auto solidCathodePatternCentralHole =
        new G4Tubs("CathodePatternCentralHole", 0, chamber::CathodePatternDiskRadius, chamber::CathodeCopperSupportThickness * 1.10 / 2, 0, 2 * M_PI);
    auto solidCathodePatternLine = new G4SubtractionSolid("CathodePatternLine", solidCathodePatternLineAux, solidCathodePatternCentralHole);
    auto solidCathodePatternDisk = new G4Tubs("CathodeCopperDiskAux", chamber::CathodePatternDiskRadius - chamber::CathodePatternLineWidth,
                                              chamber::CathodePatternDiskRadius, chamber::CathodeCopperSupportThickness / 2, 0, 2 * M_PI);

    auto solidCathodeCopperDiskAux0 = new G4UnionSolid("aux", solidCathodeCopperDisk, solidCathodePatternLine);
    auto solidCathodeCopperDiskAux1 = new G4UnionSolid("aux", solidCathodeCopperDiskAux0, solidCathodePatternLine, rotation45degZ, G4ThreeVector());
    auto solidCathodeCopperDiskAux2 = new G4UnionSolid("aux", solidCathodeCopperDiskAux1, solidCathodePatternLine, rotation90degZ, G4ThreeVector());
    auto solidCathodeCopperDiskAux = new G4UnionSolid("aux", solidCathodeCopperDiskAux2, solidCathodePatternLine, rotation135degZ, G4ThreeVector());

    auto solidCathodeCopperDiskFinal = new G4UnionSolid("CathodeCopperDiskFinal", solidCathodeCopperDiskAux, solidCathodePatternDisk);
    auto logicCathodeCopperDiskFinal = new G4LogicalVolume(solidCathodeCopperDiskFinal, materials::Copper, solidCathodeCopperDiskFinal->GetName());

    auto solidCathodeFillingBase =
        new G4Tubs("CathodeFillingBase", 0, chamber::CathodeTeflonDiskHoleRadius, chamber::CathodeTeflonDiskThickness / 2, 0, 2 * M_PI);
    auto solidCathodeFilling =
        new G4SubtractionSolid("CathodeFilling", solidCathodeFillingBase, solidCathodeCopperDiskFinal, nullptr,
                               G4ThreeVector(0, 0, -chamber::CathodeTeflonDiskThickness / 2 + chamber::CathodeCopperSupportThickness / 2));
    auto logicCathodeFilling = new G4LogicalVolume(solidCathodeFilling, materials::Vacuum, solidCathodeFilling->GetName());

    // Chamber gas
    auto solidGasBase = new G4Tubs("GasBase", 0, chamber::Diameter / 2 - chamber::TeflonWallThickness, chamber::Height / 2, 0, 2 * M_PI);

    auto solidGasSubtraction = new G4SubtractionSolid("GasSubtraction", solidGasBase, solidCopperReadout, rotation45degZ,
                                                      G4ThreeVector(0, 0, -chamber::Height / 2 + chamber::ReadoutCopperThickness / 2));
    auto solidGas = new G4SubtractionSolid("Gas", solidGasSubtraction, solidCathodeWindow, nullptr,
                                           G4ThreeVector(0, 0, chamber::Height / 2 - chamber::CathodeWindowThickness / 2));
    auto logicGas = new G4LogicalVolume(solidGas, materials::Gas, solidGas->GetName());
    // Chamber placement
    new G4PVPlacement(nullptr, G4ThreeVector(), logicGas->GetName(), logicGas, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberBody->GetName(), logicChamberBody, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -chamber::Height / 2 - chamber::ReadoutKaptonThickness - chamber::BackplateThickness / 2),
                      logicChamberBackplate->GetName(), logicChamberBackplate, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberTeflonWall->GetName(), logicChamberTeflonWall, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -chamber::Height / 2 - chamber::ReadoutKaptonThickness / 2), logicKaptonReadout->GetName(),
                      logicKaptonReadout, fWorld, false, 0);
    new G4PVPlacement(rotation45degZ, G4ThreeVector(0, 0, -chamber::Height / 2 + chamber::ReadoutCopperThickness / 2), logicCopperReadout->GetName(),
                      logicCopperReadout, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, chamber::Height / 2 + chamber::CathodeTeflonDiskThickness / 2), logicCathodeTeflonDisk->GetName(),
                      logicCathodeTeflonDisk, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, chamber::Height / 2 - chamber::CathodeWindowThickness / 2), logicCathodeWindow->GetName(),
                      logicCathodeWindow, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, chamber::Height / 2 + chamber::CathodeCopperSupportThickness / 2),
                      logicCathodeCopperDiskFinal->GetName(), logicCathodeCopperDiskFinal, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, chamber::Height / 2 + chamber::CathodeTeflonDiskThickness / 2), logicCathodeFilling->GetName(),
                      logicCathodeFilling, fWorld, false, 0);
}

void BabyIAXOGeometry::PlaceShielding() {
    auto solidShielding = new G4SubtractionSolid(
        "Shielding",  //
        new G4Box("ShieldingBlock", shielding::SizeXY / 2, shielding::SizeXY / 2, shielding::SizeZ / 2),
        new G4Box("ShieldingShaft", shielding::ShaftShortSideX / 2, shielding::ShaftShortSideY / 2, shielding::ShaftLongSide / 2 * fExpansionFactor),
        nullptr, G4ThreeVector(0, 0, shielding::SizeZ / 2 - shielding::ShaftLongSide / 2 - (1 - fExpansionFactor) * shielding::ShaftLongSide / 2));
    auto logicShielding = new G4LogicalVolume(solidShielding, materials::Lead, solidShielding->GetName());

    // Placement
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -shielding::OffsetZ), logicShielding->GetName(), logicShielding, fWorld, false, 0);
}
