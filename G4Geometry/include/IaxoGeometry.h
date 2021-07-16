//
// Created by Luis on 7/16/2021.
//

#ifndef IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H
#define IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H

class G4VPhysicalVolume;
class G4LogicalVolume;

class IaxoGeometry {
   private:
    G4VPhysicalVolume* fWorld;

   public:
    IaxoGeometry();
    inline G4VPhysicalVolume* GetWorld() const { return fWorld; }
};

#endif  // IAXO_DETECTOR_GEOMETRY_IAXOGEOMETRY_H
