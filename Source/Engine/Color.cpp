#include "Color.h"

TColorf TColorf::White = TColorf{1.0f, 1.0f, 1.0f, 1.0f};
TColorf TColorf::Black = TColorf{0u, 0u, 0u, 1.0f};

TColorf TColorf::Red = TColorf{1.0f, 0.0f, 0.0f, 1.0f};
TColorf TColorf::Green = TColorf{0.0f, 1.0f, 0.0f, 1.0f};
TColorf TColorf::Blue = TColorf{0.0f, 0.0f, 1.0f, 1.0f};

TColorf TColorf::Magenta = TColorf{1.0f, 0.0f, 1.0f, 1.0f};
TColorf TColorf::Yellow = TColorf{1.0f, 1.0f, 0.0f, 1.0f};
TColorf TColorf::Cyan = TColorf{0.0f, 1.0f, 1.0f, 1.0f};

TColorI TColorI::White = TColorI{255u, 255u, 255u, 255u};
TColorI TColorI::Black = TColorI{0u, 0u, 0u, 255u};

TColorI TColorI::Red = TColorI{255u, 0u, 0u, 255u};
TColorI TColorI::Green = TColorI{0u, 255u, 0u, 255u};
TColorI TColorI::Blue = TColorI{0u, 0u, 255u, 255u};

TColorI TColorI::Magenta = TColorI{255u, 0u, 255u, 255u};
TColorI TColorI::Yellow = TColorI{255u, 255u, 0u, 255u};
TColorI TColorI::Cyan = TColorI{0u, 255u, 255u, 255u};