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
auto copper = G4NistManager::Instance() -> FindOrBuildMaterial("G4_Cu");
};

namespace dimensions::chamber {
double Height(30.0 * mm), Diameter(102.0 * mm);
double BackplateThickness(15.0 * mm), SquareSide(134.0 * mm);
double TeflonWallThickness(1.0 * mm);
}  // namespace dimensions::chamber

void BabyIAXOGeometry::Initialize() {
    G4NistManager* nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    double worldSize = 1000 * mm;

    G4Material* materialWorld = nist->FindOrBuildMaterial("G4_Galactic");

    auto solidWorld = new G4Box("World", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    auto logicWorld = new G4LogicalVolume(solidWorld, materialWorld, solidWorld->GetName());
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

    auto solidChamber = new G4SubtractionSolid(
        "Chamber",  //
        new G4Box("solidChamberBase", dimensions::chamber::SquareSide / 2, dimensions::chamber::SquareSide / 2, dimensions::chamber::Height / 2),
        new G4Tubs("chamberBodyHole", 0, dimensions::chamber::Diameter / 2, dimensions::chamber::Height / 2 * expansionFactor, 0, 2 * M_PI));
    auto logicChamber = new G4LogicalVolume(solidChamber, materials::copper, solidChamber->GetName());
    auto physicalChamber = new G4PVPlacement(nullptr, G4ThreeVector(), logicChamber->GetName(), logicChamber, physicalWorld, false, 0);
    /*
                val chamberBodySolid = solids.subtraction(
                solids.box(
                    Chamber.SquareSide.mm,
                    Chamber.SquareSide.mm,
                    Chamber.Height.mm,
                    "chamberBodyBaseSolid"
                ), solids.tube(Chamber.Diameter.mm / 2, Chamber.Height.mm, "chamberBodyHoleSolid"),
                "chamberBodySolid"
            )
            val chamberBodyVolume = volume(iaxoMaterials.copper, chamberBodySolid, "chamberBodyVolume")

            val chamberBackplateSolid = solids.box(
                Chamber.SquareSide.mm,
                Chamber.SquareSide.mm,
                Chamber.BackplateThickness.mm,
                "chamberBackplateSolid"
            )
            val chamberBackplateVolume =
                volume(iaxoMaterials.copper, chamberBackplateSolid, "chamberBackplateVolume")

     */
    // chamber
}
