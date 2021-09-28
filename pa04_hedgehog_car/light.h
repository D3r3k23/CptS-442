#ifndef INCLUDED_LIGHT

//
// The "light" module provides the Light class (see below).
//

#include <cstdlib> // for NULL

#include "color.h"
#include "geometry.h"

class Light
//
// represents (directional) light sources
//
{
    //
    // Currently, the only light geometry we support is directional.
    // This, then, is the direction of travel of the light specified
    // in *world* coordinates: If our scene takes place at noon on the
    // equator during an equinox, the direction is Vector3(0, 0, -1).
    //
    Vector3 direction;

public:
    // technically, irradiance normal to the light's direction
    Color irradiance;

Light(Color irradiance_, Vector3 direction_)
    : irradiance(irradiance_)
    {
        direction = direction_.normalized();
    };


    // unit vector pointing in the direction of the light
    const Vector3 towards(void) const;
};

#define INCLUDED_LIGHT
#endif // INCLUDED_LIGHT
