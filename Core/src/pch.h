#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <random>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <windows.h>
inline HANDLE hcc = GetStdHandle(STD_OUTPUT_HANDLE);
#include "Log.h"
#endif
