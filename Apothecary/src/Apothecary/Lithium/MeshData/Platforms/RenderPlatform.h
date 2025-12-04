#pragma once

#include "../ArrayBuffer.h"
#include "../IndexBuffer.h"

#ifndef APOTHEC_API_CUSTOM
#define APOTHEC_API_OPENGL
#endif

#ifdef APOTHEC_API_OPENGL
#include "opengl/OpenGL_ArrayBuffer.h"
#include "opengl/OpenGL_IndexBuffer.h"
#endif