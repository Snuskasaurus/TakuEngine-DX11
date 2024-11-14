#include "Color.h"

const TFloatColor TFloatColor::White(1.0f, 1.0f, 1.0f, 1.0f);
const TFloatColor TFloatColor::Black(0.0f, 0.0f, 0.0f, 1.0f);
const TFloatColor TFloatColor::Red(1.0f, 0.0f, 0.0f, 1.0f);
const TFloatColor TFloatColor::Green(0.0f, 1.0f, 0.0f, 1.0f);
const TFloatColor TFloatColor::Blue(0.0f, 0.0f, 1.0f, 1.0f);
const TFloatColor TFloatColor::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
const TFloatColor TFloatColor::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
const TFloatColor TFloatColor::Cyan(0.0f, 1.0f, 1.0f, 1.0f);

const TInt8Color TInt8Color::White(255u, 255u, 255u, 255u);
const TInt8Color TInt8Color::Black = TInt8Color{0u, 0u, 0u, 255u};
const TInt8Color TInt8Color::Red = TInt8Color{255u, 0u, 0u, 255u};
const TInt8Color TInt8Color::Green = TInt8Color{0u, 255u, 0u, 255u};
const TInt8Color TInt8Color::Blue = TInt8Color{0u, 0u, 255u, 255u};
const TInt8Color TInt8Color::Magenta = TInt8Color{255u, 0u, 255u, 255u};
const TInt8Color TInt8Color::Yellow = TInt8Color{255u, 255u, 0u, 255u};
const TInt8Color TInt8Color::Cyan = TInt8Color{0u, 255u, 255u, 255u};