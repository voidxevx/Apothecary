#pragma once

#ifdef _WIN32
#ifdef TONIC_BUILD
#define TONIC_API __declspec(dllexport)
#else
#define TONIC_API __declspec(dllimport)
#endif
#endif