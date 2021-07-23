
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
double Height(30.0 * mm), Diameter(102.0 * mm), BackplateThickness(15.0 * mm), SquareSide(134.0 * mm), TeflonWallThickness(1.0 * mm);
// Readout
double ReadoutKaptonThickness(0.5 * mm), ReadoutCopperThickness(0.2 * mm), ReadoutPlaneSide(60.0 * mm);
// Drift Field
double DriftFieldRingsHeight(10.0 * mm), DriftFieldRingsThickness(0.2 * mm), DriftFieldRimThickness(ReadoutCopperThickness),
    DriftFieldRingsGap(5 * mm), DriftFieldRingsThicknessPositionZ(-DriftFieldRingsGap / 2 - DriftFieldRingsHeight / 2);
// Cathode
double CathodeTeflonDiskHoleRadius(15.0 * mm), CathodeTeflonDiskThickness(5.0 * mm), CathodeCopperSupportOuterRadius(45.0 * mm),
    CathodeCopperSupportInnerRadius(8.5 * mm), CathodeCopperSupportThickness(1.0 * mm), CathodeWindowThickness(0.004 * mm),
    CathodePatternDiskRadius(4.25 * mm), CathodePatternLineWidth(0.3 * mm);
}  // namespace chamber

namespace shielding {
double SizeXY(590.0 * mm), SizeZ(540.0 * mm), ShaftShortSideX(194.0 * mm), ShaftShortSideY(170.0 * mm), ShaftLongSide(340.0 * mm),
    DetectorToShieldingSeparation(-60.0 * mm), EnvelopeThickness(10.0 * mm),
    OffsetZ(DetectorToShieldingSeparation + chamber::Height / 2 + chamber::ReadoutKaptonThickness + chamber::BackplateThickness);
}  // namespace shielding

namespace pipe {
double TotalLength(491.0 * mm);
// Outside
double ChamberFlangeThickness(14.0 * mm), ChamberFlangeRadius(chamber::SquareSide / 2), TelescopeFlangeThickness(18.0 * mm),
    TelescopeFlangeRadius(150.0 / 2 * mm);
double Section2of2Length(150.0 * mm - TelescopeFlangeThickness),
    Section1of2Length(TotalLength - TelescopeFlangeThickness - ChamberFlangeThickness - Section2of2Length), OuterRadius1(92.0 / 2 * mm),
    OuterRadius2(108.0 / 2 * mm), Union1Z(ChamberFlangeThickness / 2 + Section1of2Length / 2),
    Union2Z(Union1Z + Section1of2Length / 2 + Section2of2Length / 2), Union3Z(Union2Z + Section2of2Length / 2 + TelescopeFlangeThickness / 2);

// Inside
double InsideSection1of3Radius(43.0 / 2 * mm), InsideSection2of3Radius(68.0 / 2 * mm), InsideSection3of3Radius(85.0 / 2 * mm),
    InsideSectionTelescopeRadius(108.0 / 2 * mm), InsideCone1of3Length(21.65 * mm), InsideCone2of3Length(14.72 * mm), InsideCone3of3Length(9.0 * mm),
    InsideSection3of3Length(115.0 * mm - InsideCone3of3Length),
    InsideSection2of3Length(290.0 * mm - InsideSection3of3Length - InsideCone3of3Length - InsideCone2of3Length),
    InsideSection1of3Length(201.0 * mm - InsideCone1of3Length), InsideUnion1Z(InsideSection1of3Length / 2 + InsideCone1of3Length / 2),
    InsideUnion2Z(InsideUnion1Z + InsideCone1of3Length / 2 + InsideSection2of3Length / 2),
    InsideUnion3Z(InsideUnion2Z + InsideSection2of3Length / 2 + InsideCone2of3Length / 2),
    InsideUnion4Z(InsideUnion3Z + InsideCone2of3Length / 2 + InsideSection3of3Length / 2),
    InsideUnion5Z(InsideUnion4Z + InsideSection3of3Length / 2 + InsideCone3of3Length / 2),
    FillingOffsetWithPipe(InsideSection1of3Length / 2 - ChamberFlangeThickness / 2);
// World
double ZinWorld(ChamberFlangeThickness / 2 + chamber::Height / 2 + chamber::CathodeTeflonDiskThickness);
}  // namespace pipe

void BabyIAXOGeometry::Initialize() {
    double worldSize = 1500 * mm;

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
    // PlaceShielding();
    PlacePipe();
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
    // Chamber copper readout
    auto solidCopperReadout =
        new G4Box("CopperReadout", chamber::ReadoutPlaneSide / 2, chamber::ReadoutPlaneSide / 2, chamber::ReadoutCopperThickness / 2);
    auto logicCopperReadout = new G4LogicalVolume(solidCopperReadout, materials::Copper, solidCopperReadout->GetName());
    // Drift Field components
    auto solidDriftFieldRings1 =
        new G4Tubs("solidDriftFieldRings1", chamber::Diameter / 2 - chamber::TeflonWallThickness / 2 - chamber::DriftFieldRingsThickness / 2,
                   chamber::Diameter / 2 - chamber::TeflonWallThickness / 2 + chamber::DriftFieldRingsThickness / 2,
                   chamber::DriftFieldRingsHeight / 2, 0, 2 * M_PI);
    auto solidDriftFieldRings = new G4UnionSolid("solidDriftFieldRings", solidDriftFieldRings1, solidDriftFieldRings1, nullptr,
                                                 G4ThreeVector(0, 0, chamber::DriftFieldRingsHeight + chamber::DriftFieldRingsGap));
    auto logicDriftFieldRings = new G4LogicalVolume(solidDriftFieldRings, materials::Copper, solidDriftFieldRings->GetName());

    auto solidDriftFieldRimAux = new G4Tubs("solidDriftFieldRimAux", 0, chamber::Diameter / 2 - chamber::TeflonWallThickness,
                                            chamber::DriftFieldRimThickness / 2, 0, 2 * M_PI);
    auto solidDriftFieldRim =
        new G4SubtractionSolid("solidDriftFieldRim", solidDriftFieldRimAux, solidCopperReadout, rotation45degZ, G4ThreeVector());
    auto logicDriftFieldRim = new G4LogicalVolume(solidDriftFieldRim, materials::Copper, solidDriftFieldRim->GetName());
    // Chamber teflon inner wall
    auto solidChamberTeflonWallAux = new G4Tubs("ChamberTeflonWallAux", chamber::Diameter / 2 - chamber::TeflonWallThickness, chamber::Diameter / 2,
                                                chamber::Height / 2, 0, 2 * M_PI);
    auto solidChamberTeflonWall = new G4SubtractionSolid("solidChamberTeflonWall", solidChamberTeflonWallAux, solidDriftFieldRings, nullptr,
                                                         G4ThreeVector(0, 0, chamber::DriftFieldRingsThicknessPositionZ));
    auto logicChamberTeflonWall = new G4LogicalVolume(solidChamberTeflonWall, materials::PTFE, solidChamberTeflonWall->GetName());
    // Chamber kapton readout
    auto solidKaptonReadout = new G4Box("KaptonReadout", chamber::SquareSide / 2, chamber::SquareSide / 2, chamber::ReadoutKaptonThickness / 2);
    auto logicKaptonReadout = new G4LogicalVolume(solidKaptonReadout, materials::Kapton, solidKaptonReadout->GetName());
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

    auto solidGasSubtraction1 = new G4SubtractionSolid("GasSubtraction1", solidGasBase, solidCopperReadout, rotation45degZ,
                                                       G4ThreeVector(0, 0, -chamber::Height / 2 + chamber::ReadoutCopperThickness / 2));
    auto solidGasSubtraction2 = new G4SubtractionSolid("GasSubtraction2", solidGasSubtraction1, solidCopperReadout, rotation45degZ,
                                                       G4ThreeVector(0, 0, -chamber::Height / 2 + chamber::DriftFieldRimThickness / 2));
    auto solidGas = new G4SubtractionSolid("Gas", solidGasSubtraction2, solidCathodeWindow, nullptr,
                                           G4ThreeVector(0, 0, chamber::Height / 2 - chamber::CathodeWindowThickness / 2));
    auto logicGas = new G4LogicalVolume(solidGas, materials::Gas, solidGas->GetName());
    // Chamber placement
    new G4PVPlacement(nullptr, G4ThreeVector(), logicGas->GetName(), logicGas, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberBody->GetName(), logicChamberBody, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -chamber::Height / 2 - chamber::ReadoutKaptonThickness - chamber::BackplateThickness / 2),
                      logicChamberBackplate->GetName(), logicChamberBackplate, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(), logicChamberTeflonWall->GetName(), logicChamberTeflonWall, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, chamber::DriftFieldRingsThicknessPositionZ), logicDriftFieldRings->GetName(), logicDriftFieldRings,
                      fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -chamber::Height / 2 + chamber::DriftFieldRimThickness / 2), logicDriftFieldRim->GetName(),
                      logicDriftFieldRim, fWorld, false, 0);
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

void BabyIAXOGeometry::PlacePipe() {
    // Body
    auto solidDetectorPipeChamberFlange =
        new G4Tubs("solidDetectorPipeChamberFlange", 0, pipe::ChamberFlangeRadius, pipe::ChamberFlangeThickness / 2, 0, 2 * M_PI);
    auto solidDetectorPipeTelescopeFlange =
        new G4Tubs("solidDetectorPipeTelescopeFlange", 0, pipe::TelescopeFlangeRadius, pipe::TelescopeFlangeThickness / 2, 0, 2 * M_PI);
    auto solidDetectorPipeSection1of2 = new G4Tubs("solidDetectorPipeSection1of2", 0, pipe::OuterRadius1, pipe::Section1of2Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeSection2of2 = new G4Tubs("solidDetectorPipeSection2of2", 0, pipe::OuterRadius2, pipe::Section2of2Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeAux1 = new G4UnionSolid("solidDetectorPipeAux1", solidDetectorPipeChamberFlange, solidDetectorPipeSection1of2, nullptr,
                                                  G4ThreeVector(0, 0, pipe::Union1Z));
    auto solidDetectorPipeAux2 =
        new G4UnionSolid("solidDetectorPipeAux2", solidDetectorPipeAux1, solidDetectorPipeSection2of2, nullptr, G4ThreeVector(0, 0, pipe::Union2Z));
    auto solidDetectorPipeNotEmpty = new G4UnionSolid("solidDetectorPipeNotEmpty", solidDetectorPipeAux2, solidDetectorPipeTelescopeFlange, nullptr,
                                                      G4ThreeVector(0, 0, pipe::Union3Z));
    // Inside
    auto solidDetectorPipeInside1of3 =
        new G4Tubs("solidDetectorPipeInside1of3", 0, pipe::InsideSection1of3Radius, pipe::InsideSection1of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInside2of3 =
        new G4Tubs("solidDetectorPipeInside2of3", 0, pipe::InsideSection2of3Radius, pipe::InsideSection2of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInside3of3 =
        new G4Tubs("solidDetectorPipeInside3of3", 0, pipe::InsideSection3of3Radius, pipe::InsideSection3of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInsideCone1of3 = new G4Cons("solidDetectorPipeInsideCone1of3", 0, pipe::InsideSection1of3Radius, 0,
                                                      pipe::InsideSection2of3Radius, pipe::InsideCone1of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInsideCone2of3 = new G4Cons("solidDetectorPipeInsideCone2of3", 0, pipe::InsideSection2of3Radius, 0,
                                                      pipe::InsideSection3of3Radius, pipe::InsideCone2of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInsideCone3of3 = new G4Cons("solidDetectorPipeInsideCone3of3", 0, pipe::InsideSection3of3Radius, 0,
                                                      pipe::InsideSectionTelescopeRadius, pipe::InsideCone3of3Length / 2, 0, 2 * M_PI);
    auto solidDetectorPipeInsideAux1 = new G4UnionSolid("solidDetectorPipeInsideAux1", solidDetectorPipeInside1of3, solidDetectorPipeInsideCone1of3,
                                                        nullptr, G4ThreeVector(0, 0, pipe::InsideUnion1Z));
    auto solidDetectorPipeInsideAux2 = new G4UnionSolid("solidDetectorPipeInsideAux2", solidDetectorPipeInsideAux1, solidDetectorPipeInside2of3,
                                                        nullptr, G4ThreeVector(0, 0, pipe::InsideUnion2Z));
    auto solidDetectorPipeInsideAux3 = new G4UnionSolid("solidDetectorPipeInsideAux3", solidDetectorPipeInsideAux2, solidDetectorPipeInsideCone2of3,
                                                        nullptr, G4ThreeVector(0, 0, pipe::InsideUnion3Z));
    auto solidDetectorPipeInsideAux4 = new G4UnionSolid("solidDetectorPipeInsideAux4", solidDetectorPipeInsideAux3, solidDetectorPipeInside3of3,
                                                        nullptr, G4ThreeVector(0, 0, pipe::InsideUnion4Z));
    auto solidDetectorPipeFilling = new G4UnionSolid("DetectorPipeFilling", solidDetectorPipeInsideAux4, solidDetectorPipeInsideCone3of3, nullptr,
                                                     G4ThreeVector(0, 0, pipe::InsideUnion5Z));
    auto solidDetectorPipe = new G4SubtractionSolid("DetectorPipe", solidDetectorPipeNotEmpty, solidDetectorPipeFilling, nullptr,
                                                    G4ThreeVector(0, 0, pipe::InsideSection1of3Length / 2 - pipe::ChamberFlangeThickness / 2));

    // Placements
    auto logicDetectorPipe = new G4LogicalVolume(solidDetectorPipe, materials::Copper, solidDetectorPipe->GetName());
    auto logicDetectorPipeFilling = new G4LogicalVolume(solidDetectorPipeFilling, materials::Vacuum, solidDetectorPipeFilling->GetName());

    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, pipe::ZinWorld), logicDetectorPipe->GetName(), logicDetectorPipe, fWorld, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, pipe::ZinWorld + pipe::FillingOffsetWithPipe), logicDetectorPipeFilling->GetName(),
                      logicDetectorPipeFilling, fWorld, false, 0);
}
