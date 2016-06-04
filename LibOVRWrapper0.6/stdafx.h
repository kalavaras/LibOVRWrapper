// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#pragma warning(disable : 4503)

// reference additional headers your program requires here
#pragma warning(push)
#pragma warning(disable : 4348)
#pragma warning(disable : 4503)
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#pragma warning(pop)

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

#include "WrapperSettings.h"

#include "../LibOVR/Include/OVR_CAPI.h"
#include "../LibOVR/Include/OVR_Version.h"
#include "../LibOVR/Include/OVR_ErrorCode.h"

#include "../LibOVR/Include/OVR_CAPI_D3D.h"   
#include "../LibOVR/Include/OVR_CAPI_GL.h"  