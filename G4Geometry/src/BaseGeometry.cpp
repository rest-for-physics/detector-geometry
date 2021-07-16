//
// Created by Luis on 7/16/2021.
//

#include "BaseGeometry.h"

#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4GDMLParser.hh>
#include <G4NistManager.hh>
#include <G4RunManager.hh>
#include <G4SubtractionSolid.hh>
#include <G4VisAttributes.hh>
#include <filesystem>

using namespace std;

void BaseGeometry::WriteGDML(const std::string& filename) {
    G4GDMLParser parser;
    if (std::filesystem::is_regular_file(filename)) std::filesystem::remove(filename);
    parser.Write(filename, fWorld);
}

// TODO: make it accessible from derived class
/*
map<string, G4Material*> materials = {
    {"copper", G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu")},
};
*/

bool BaseGeometry::CheckOverlaps() { return fWorld->CheckOverlaps(); }
