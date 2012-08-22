// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//NOTE: I know, we use unsafe string copy functions
#define _CRT_SECURE_NO_WARNINGS

// NOTE: This trick is needed to bind assembly manifest to the current version of the VC CRT
// See also: http://msdn.microsoft.com/ru-ru/library/cc664727.aspx
#define _BIND_TO_CURRENT_CRT_VERSION 1

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
//#include <stdlib.h>
#include <memory.h>

#include "Common.h"
