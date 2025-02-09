#pragma once

#include "CommonEngine.h"

// Inspired by https://www.braynzarsoft.net/viewtutorial/q16390-15-high-resolution-timer

class MTime
{
    SINGLETON_DECLARE(MTime)
public:
    static void InitializeTime();
    static float GetDeltaTime();
    static float GetTime();
};