//
// Created by Luis on 7/16/2021.
//

#ifndef IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H
#define IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H

#include <string>

class G4VPhysicalVolume;
class G4LogicalVolume;

class IaxoGeometry {
   private:
    static inline G4VPhysicalVolume* fWorld = nullptr;

   public:
    inline IaxoGeometry() = default;
    static void Initialize();
    static G4VPhysicalVolume* GetWorld() { return fWorld; }

    static void WriteGDML(const std::string&);
    static bool CheckOverlaps();
};

#endif  // IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H
