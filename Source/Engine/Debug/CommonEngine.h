#pragma once

#define SINGLETON_DECLARE(CLASS_NAME)                                                    \
public: static CLASS_NAME& Instance() { static CLASS_NAME instance; return instance; } \
private: CLASS_NAME(){}                                                                       \
