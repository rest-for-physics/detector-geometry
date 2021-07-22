
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
#include "OpticalTest.h"

using namespace std;

int main(int argc, char** argv) {
    typedef OpticalTest Geometry;

    Geometry::Initialize();
    const bool overlaps = Geometry::CheckOverlaps();
    if (overlaps) {
        cout << "OVERLAPS!" << endl;
        return 1;
    }
    const string filename = "geometry.gdml";
    Geometry::WriteGDML(filename);

    // return 0;

    auto geoManager = new TGeoManager();
    TGeoManager::Import(filename.c_str());
    geoManager->CheckOverlaps(0.0001, "d");

    const int transparencyLevel = 35;
    for (int i = 0; i < gGeoManager->GetListOfVolumes()->GetEntries(); i++) {
        auto volume = gGeoManager->GetVolume(i);
        auto material = volume->GetMaterial();
        if (material->GetDensity() <= 0.01) {
            volume->SetTransparency(95);
            if (material->GetDensity() <= 0.001) {
                // We consider this vacuum for display purposes
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

    // glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);

    eveManager->FullRedraw3D(kTRUE);

    // glViewer->SavePicture("picture.png");

    cout << "Geometry Viewer Loaded!" << endl;
    app.Run();

    return 0;
}