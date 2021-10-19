#include "light.h"
#include "view.h"
#include "work_arounds.h"

const Vector3 Light::towards(void) const
{
    //
    // It is convenient for the user to specify the direction the
    // light is travelling, but in shading calculations we are more
    // interested in the direction pointing towards the light, hence
    // the "-" sign.
    //
    // Be sure the result is unit length.
    //
    return -direction.normalized();
}
