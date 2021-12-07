#ifndef INCLUDED_CONTROLLER

//
// The "controller" module provides the Controller class (see
// below).
//

#include "hedgehog.h"

enum { LIGHT_HEDGEHOG_DISABLED = -1 };

class Controller
//
// includes all settings affecting the display that the user can
// control while *coaster* is running
//
{
public:
    bool ambientReflectionEnabled;
    bool animationEnabled;
    bool axesEnabled;
    bool backFaceCullingEnabled;
    bool diffuseReflectionEnabled;
    bool fillPolygons;
    bool fullScreenEnabled;
    int lightHedgehogIndex;
    bool normalHedgehogEnabled;
    bool specularReflectionEnabled;
    bool statsEnabled;
    bool useFirstPerson;
    bool useOrthographic;
    // true -> vertex normals, false -> face normals (This only
    // applies to irregular meshes. Regular meshes always use vertex
    // normals.)
bool useVertexNormals;
    bool viewHelpEnabled;

    // unused, but it keeps C++ initialization syntax clear (see below)
    bool _endInits;

Controller()
    :
    ambientReflectionEnabled(true),
    animationEnabled(false),
    axesEnabled(false),
    backFaceCullingEnabled(false),
    diffuseReflectionEnabled(true),
    fillPolygons(true),
    fullScreenEnabled(false),
    lightHedgehogIndex(LIGHT_HEDGEHOG_DISABLED),
    normalHedgehogEnabled(false),
    specularReflectionEnabled(true),
    statsEnabled(false),
    useFirstPerson(false),
    useOrthographic(true),
    useVertexNormals(false),
    viewHelpEnabled(false),
    // keep this at the end so previous initializations can end with commas
    _endInits(false)
    { };
    void init();
};

extern Controller controller;

#define INCLUDED_CONTROLLER
#endif // INCLUDED_CONTROLLER
