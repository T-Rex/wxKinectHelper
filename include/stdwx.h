#ifndef HMI_STDWX_H_
#define HMI_STDWX_H_

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/config.h> 
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/list.h> 
#include <wx/log.h>
#include <wx/mstream.h>
#include <wx/stdpaths.h> 
#include <wx/string.h>
#include <wx/uri.h>
#include <wx/wfstream.h>
#include <wx/busyinfo.h>
#include <wx/filesys.h>

#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif

#endif
