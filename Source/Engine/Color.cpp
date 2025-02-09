#include "Color.h"

TColorF TColorF::White = TColorF{1.0f, 1.0f, 1.0f, 1.0f};
TColorF TColorF::Black = TColorF{0u, 0u, 0u, 1.0f};

TColorF TColorF::Red = TColorF{1.0f, 0.0f, 0.0f, 1.0f};
TColorF TColorF::Green = TColorF{0.0f, 1.0f, 0.0f, 1.0f};
TColorF TColorF::Blue = TColorF{0.0f, 0.0f, 1.0f, 1.0f};

TColorF TColorF::Magenta = TColorF{1.0f, 0.0f, 1.0f, 1.0f};
TColorF TColorF::Yellow = TColorF{1.0f, 1.0f, 0.0f, 1.0f};
TColorF TColorF::Cyan = TColorF{0.0f, 1.0f, 1.0f, 1.0f};

TColorI TColorI::White = TColorI{255u, 255u, 255u, 255u};
TColorI TColorI::Black = TColorI{0u, 0u, 0u, 255u};

TColorI TColorI::Red = TColorI{255u, 0u, 0u, 255u};
TColorI TColorI::Green = TColorI{0u, 255u, 0u, 255u};
TColorI TColorI::Blue = TColorI{0u, 0u, 255u, 255u};

TColorI TColorI::Magenta = TColorI{255u, 0u, 255u, 255u};
TColorI TColorI::Yellow = TColorI{255u, 255u, 0u, 255u};
TColorI TColorI::Cyan = TColorI{0u, 255u, 255u, 255u};