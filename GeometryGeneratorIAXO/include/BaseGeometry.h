#ifndef GEOMETRY_GENERATOR_IAXO_BASEGEOMETRY_H
#define GEOMETRY_GENERATOR_IAXO_BASEGEOMETRY_H

#include <map>
#include <string>

class G4VPhysicalVolume;
class G4LogicalVolume;
// class G4Material;

class BaseGeometry {
   protected:
    static inline G4VPhysicalVolume* fWorld = nullptr;
    // static std::map<std::string, G4Material*> materials;
    constexpr static const double fExpansionFactor = 1.01;  // for subtractions, to improve visibility (should never affect simulation outcome!)
    static inline bool fCheckOverlaps = true;

   public:
    inline BaseGeometry() = default;
    // virtual void Initialize() = 0;
    static G4VPhysicalVolume* GetWorld() { return fWorld; }

    static void WriteGDML(const std::string&);
    static bool CheckOverlaps();
};

#endif  // GEOMETRY_GENERATOR_IAXO_BASEGEOMETRY_H
