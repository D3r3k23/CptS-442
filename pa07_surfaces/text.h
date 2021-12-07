#ifndef INCLUDED_TEXT
#include <string>

using namespace std;

class Text {

public:
    const void renderAt(double xFrac, double yFrac,
                        string lines[], int nLines) const;
};

extern Text text;

#define INCLUDED_TEXT
#endif // INCLUDED_TEXT
