
#include <TApplication.h>
#include <TEveGeoNode.h>
#include <TEveManager.h>
#include <TEveViewer.h>
#include <TGLViewer.h>
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

/*
 * // May be useful to do simple test in simulation or to use the Geant4 viewer
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
*/

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
    geoManager->CheckOverlaps(0.0001, "d");

    const int transparencyLevel = 35;
    for (int i = 0; i < gGeoManager->GetListOfVolumes()->GetEntries(); i++) {
        auto volume = gGeoManager->GetVolume(i);
        auto material = volume->GetMaterial();
        if (material->GetDensity() <= 0.01) {
            volume->SetTransparency(95);
            if (material->GetDensity() <= 0.01) {
                volume->SetVisibility(kFALSE);
            }
        } else {
            volume->SetTransparency(transparencyLevel);
        }
    }

    cout << "Opening TEveManager" << endl;

    TApplication app("Geometry Viewer", &argc, argv);
    auto eveManager = TEveManager::Create();

    TGeoNode* node = geoManager->GetTopNode();
    auto topNode = new TEveGeoTopNode(geoManager, node);
    eveManager->AddGlobalElement(topNode);

    auto glViewer = eveManager->GetDefaultGLViewer();
    glViewer->UseLightColorSet();

    eveManager->FullRedraw3D(kTRUE);

    glViewer->SavePicture("picture.png");

    cout << "Geometry Viewer Loaded!" << endl;
    app.Run();

    return 0;
}