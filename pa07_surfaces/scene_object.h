#ifndef INCLUDED_SCENE_OBJECT

//
// The "scene_object" module provides the SceneObject class (see
// below).
//

#include "hedgehog.h"
#include "mesh.h"
#include "transform.h"

class SceneObject
//
// This abstract class is the parent of all "real" (as opposed to
// purely geometrical) objects in the scene. In later implementations,
// they may have Hedgehogs (see the "hedgehog" module) associated and
// drawn with them.
//
{
protected:
    vector<Hedgehog *> faceHedgehogs;
    vector<Hedgehog *> vertexHedgehogs;

public:
    // At least one older g++ compiler complains if this is missing.
    virtual ~SceneObject() { };

    //
    // Force all child classes to implement their own display()
    // methods.
    //
    virtual void display(const Transform &viewTransform) = 0;

public:
    const void addHedgehogs(Mesh *mesh);
    const void displayHedgehogs(
        const Transform &viewTransform,
        const double quillLength) const;
};

#define INCLUDED_SCENE_OBJECT
#endif
