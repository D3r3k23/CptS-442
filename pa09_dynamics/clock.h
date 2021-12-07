#ifndef INCLUDED_CLOCK

//
// The "clock" module defines the Clock class (see below).
//

class Clock
//
// abstracts the real-time system clock
//
{

public:
    double read();

};
extern Clock clock_;

#define INCLUDED_CLOCK
#endif // INCLUDED_CLOCK
