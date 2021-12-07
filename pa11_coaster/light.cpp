#include "controller.h"
#include "light.h"
#include "transform.h"
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
# ifdef WORK_AROUND_CXX_RETURN_OVERLOADED_EXPRESSION_BUG
    Vector3 result = direction;
    result = -result.normalized();
    return result;
# else // this is the buggy expression
    return (-direction).normalized();
# endif
}
