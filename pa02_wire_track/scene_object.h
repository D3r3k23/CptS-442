#ifndef INCLUDED_SCENE_OBJECT

//
// The "scene_object" module provides the SceneObject class (see
// below).
//

#include "transform.h"

class SceneObject
//
// This abstract class is the parent of all "real" (as opposed to
// purely geometrical) objects in the scene. In later implementations,
// they may have Hedgehogs (see the "hedgehog" module) associated and
// drawn with them.
//
{
public:
    // At least one older g++ compiler complains if this is missing.
    virtual ~SceneObject() { };

    //
    // Force all child classes to implement their own display()
    // methods.
    //
    virtual void display(const Transform &viewTransform) = 0;
};

#define INCLUDED_SCENE_OBJECT
#endif
