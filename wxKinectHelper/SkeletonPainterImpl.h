#pragma once

#include <wx/wx.h>

class SkeletonPainterImpl
{
public:
	virtual ~SkeletonPainterImpl() {}
	virtual void DrawSkeleton(wxDC & dc, void * data) = 0;
};