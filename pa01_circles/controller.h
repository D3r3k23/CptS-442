#ifndef INCLUDED_CONTROLLER

//
// The "controller" module provides the Controller class (see
// below).
//

class Controller
//
// includes all settings affecting the display that the user can
// control while *coaster* is running
//
{
public:
    bool antialiasEnabled;

    // unused, but it keeps C++ initialization syntax clear (see below)
    bool _endInits;

Controller()
    :
    antialiasEnabled(false),
    // keep this at the end so previous initializations can end with commas
    _endInits(false)
    { };
    void init();
};

extern Controller controller;

#define INCLUDED_CONTROLLER
#endif // INCLUDED_CONTROLLER
