//
// Created by Luis on 7/16/2021.
//

#include <G4RunManagerFactory.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisExecutive.hh>
#include <globals.hh>
#include <string>
#include <vector>

#include "IaxoGeometry.h"

using namespace std;

class DetectorConstruction : public G4VUserDetectorConstruction {
   public:
    inline DetectorConstruction() : G4VUserDetectorConstruction() {}

    virtual G4VPhysicalVolume* Construct() {
        IaxoGeometry geometry;
        auto world = geometry.GetWorld();
        return world;
    }
};

int main() {
    IaxoGeometry geometry;
    auto world = geometry.GetWorld();

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    auto visManager = new G4VisExecutive();
    visManager->Initialize();
    auto UImanager = G4UImanager::GetUIpointer();

    // runManager->SetUserInitialization(new DetectorConstruction());

    return 0;
    vector<string> commands = {
        "/run/initialize",           //
        "/vis/open OGLSX",           //
        "/vis/scene/create",         //
        "/vis/scene/add/volume",     //
        "/vis/sceneHandler/attach",  //
        "/vis/viewer/flush",         //
    };

    for (const auto& command : commands) {
        UImanager->ApplyCommand(command);
    }

    return 0;
}