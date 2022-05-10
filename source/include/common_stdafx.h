#pragma once

// Change these values to use different versions
#define WINVER		0x0600
#define _WIN32_WINNT	0x0600
#define _WIN32_IE	0x0700
#define _RICHEDIT_VER	0x0200

#pragma warning(disable:4996)

// Basic
#include <intsafe.h>

// STL
#include <vector>
#include <list>
#include <map>

// ATL
#include <atlstr.h>
#include <atlbase.h>
#define _WTL_NO_CSTRING
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>

// Mirror related
#include "mirror_version.h"
#include "base/assert_return.h"
#include "base/log/log.h"

#include "modules/framework.h"
#include "modules/module_id.h"
#include "modules/component_id.h"

#include "mirror/resource/resource.h"