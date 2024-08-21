#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <map>
#include <fstream>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <windows.h>
inline HANDLE hcc = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
#include "Log.h"
