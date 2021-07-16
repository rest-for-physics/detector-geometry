
#include <TApplication.h>
#include <TEveGeoNode.h>
#include <TEveManager.h>
#include <TGeoManager.h>

#include <FTFP_BERT.hh>
#include <G4RunManagerFactory.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VUserActionInitialization.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisExecutive.hh>
#include <globals.hh>
#include <string>
#include <vector>

#include "BabyIaxoGeometry.h"

using namespace std;

class DetectorConstruction : public G4VUserDetectorConstruction {
   public:
    inline DetectorConstruction() : G4VUserDetectorConstruction() {}

    virtual G4VPhysicalVolume* Construct() {
        auto world = BaseGeometry::GetWorld();
        return world;
    }
};

class ActionInitialization : public G4VUserActionInitialization {
   public:
    ActionInitialization() = default;

    virtual void BuildForMaster() const {}
    virtual void Build() const {}
};

int main(int argc, char** argv) {
    BabyIAXOGeometry::Initialize();
    const bool overlaps = BabyIAXOGeometry::CheckOverlaps();
    if (overlaps) {
        cout << "OVERLAPS!" << endl;
        return 1;
    }
    const string filename = "geometry.gdml";
    BabyIAXOGeometry::WriteGDML(filename);

    auto geoManager = new TGeoManager();
    TGeoManager::Import(filename.c_str());
    geoManager->CheckOverlaps(0.0001, "d");  // TODO: why not working? (doesn't detect overlaps!)

    cout << "Opening TEveManager" << endl;

    TApplication app("Geometry Viewer", &argc, argv);
    auto eveManager = TEveManager::Create();

    TGeoNode* node = geoManager->GetTopNode();
    auto topNode = new TEveGeoTopNode(geoManager, node);
    eveManager->AddGlobalElement(topNode);

    cout << "Geometry Viewer Loaded!" << endl;
    app.Run();

    return 0;

    G4UIExecutive* ui;
    ui = new G4UIExecutive(argc, argv);

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    auto visManager = new G4VisExecutive();
    visManager->Initialize();

    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new ActionInitialization);

    vector<string> commands = {
        "/run/initialize",            //
        "/vis/open OGL 600x600-0+0",  //
        "/vis/drawVolume",            //
    };

    cout << "RUNNING COMMANDS:" << endl;

    auto UImanager = G4UImanager::GetUIpointer();
    for (const auto& command : commands) {
        UImanager->ApplyCommand(command);
    }

    ui->SessionStart();

    return 0;
}