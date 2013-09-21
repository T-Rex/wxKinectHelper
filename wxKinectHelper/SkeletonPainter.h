#pragma once

#include <wx/wx.h>

class SkeletonPainterImpl;

class SkeletonPainter
{
public:
	SkeletonPainter();
	~SkeletonPainter();
	void DrawSkeleton(wxDC & dc, void * data);
private:
	SkeletonPainterImpl * m_Impl;
};
