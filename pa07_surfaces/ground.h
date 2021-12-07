#ifndef INCLUDED_GROUND

//
// The "ground" module provides the Ground class (see below).
//

#include "height_field.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "transform.h"


class Ground : public SceneObject
//
// a SceneObject representing the ground
//
{
public:
    HeightField *heightField;

    Ground(double extent_);
    void display(const Transform &viewTransform);
    const double height(const double x, const double y) const;
};

#define INCLUDED_GROUND
#endif // INCLUDED_GROUND
