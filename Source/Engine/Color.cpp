#include "Color.h"

TColorf TColorf::White = TColorf{1.0f, 1.0f, 1.0f, 1.0f};
TColorf TColorf::Black = TColorf{0u, 0u, 0u, 1.0f};

TColorf TColorf::Red = TColorf{1.0f, 0.0f, 0.0f, 1.0f};
TColorf TColorf::Green = TColorf{0.0f, 1.0f, 0.0f, 1.0f};
TColorf TColorf::Blue = TColorf{0.0f, 0.0f, 1.0f, 1.0f};

TColorf TColorf::Magenta = TColorf{1.0f, 0.0f, 1.0f, 1.0f};
TColorf TColorf::Yellow = TColorf{1.0f, 1.0f, 0.0f, 1.0f};
TColorf TColorf::Cyan = TColorf{0.0f, 1.0f, 1.0f, 1.0f};

TColor TColor::White = TColor{255u, 255u, 255u, 255u};
TColor TColor::Black = TColor{0u, 0u, 0u, 255u};

TColor TColor::Red = TColor{255u, 0u, 0u, 255u};
TColor TColor::Green = TColor{0u, 255u, 0u, 255u};
TColor TColor::Blue = TColor{0u, 0u, 255u, 255u};

TColor TColor::Magenta = TColor{255u, 0u, 255u, 255u};
TColor TColor::Yellow = TColor{255u, 255u, 0u, 255u};
TColor TColor::Cyan = TColor{0u, 255u, 255u, 255u};