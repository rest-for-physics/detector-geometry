
#ifndef GEOMETRY_GENERATOR_IAXO_OPTICALTEST_H
#define GEOMETRY_GENERATOR_IAXO_OPTICALTEST_H

#include "BaseGeometry.h"

class OpticalTest : public BaseGeometry {
   public:
    static void Initialize();

   private:
    static void OpNovice();
};

#endif  // GEOMETRY_GENERATOR_IAXO_OPTICALTEST_H
