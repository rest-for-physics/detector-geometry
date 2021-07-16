#ifndef GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H
#define GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H

#include "BaseGeometry.h"

class BabyIAXOGeometry : public BaseGeometry {
   public:
    static void Initialize();
    static void PlaceChamber();
};

#endif  // GEOMETRY_GENERATOR_IAXO_BABYIAXOGEOMETRY_H
