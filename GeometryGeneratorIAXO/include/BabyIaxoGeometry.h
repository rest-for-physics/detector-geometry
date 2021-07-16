#ifndef GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H
#define GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H

#include "BaseGeometry.h"

class BabyIAXOGeometry : public BaseGeometry {
   public:
    static void Initialize();

   private:
    static void PlaceChamber();
    static void PlaceShielding();
    static void PlacePipe();
    static void PlaceVetoes();
};

#endif  // GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H
