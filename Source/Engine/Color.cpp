#include "Color.h"

SColor SColor::White = SColor{255u, 255u, 255u, 255u};
SColor SColor::Black = SColor{0u, 0u, 0u, 255u};

SColor SColor::Red = SColor{255u, 0u, 0u, 255u};
SColor SColor::Green = SColor{0u, 255u, 0u, 255u};
SColor SColor::Blue = SColor{0u, 0u, 255u, 255u};

SColor SColor::Magenta = SColor{255u, 0u, 255u, 255u};
SColor SColor::Yellow = SColor{255u, 255u, 0u, 255u};
SColor SColor::Cyan = SColor{0u, 255u, 255u, 255u};