#pragma once

namespace r {

//Random float between fmin and fmax, fmin is included, fmax not.
float random(float fmin, float fmax);
//Random integer from imin to imax, including the values imin and imax.
int irandom(int imin, int imax);
//Random chance, in percentage from 0-100
bool chance(float percentage);

}
