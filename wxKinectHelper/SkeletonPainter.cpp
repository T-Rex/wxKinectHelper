#include "SkeletonPainter.h"
#if defined(__WXMSW__)
#include "SkeletonPainterImplMSW.h"
#else
#include "SkeletonPainterImpl.h"
#endif

SkeletonPainter::SkeletonPainter()
{
#if defined(__WXMSW__)
	m_Impl = new SkeletonPainterImplMSW;
#else
	m_Impl = NULL;
#endif
}

SkeletonPainter::~SkeletonPainter()
{
	wxDELETE(m_Impl);
}

void SkeletonPainter::DrawSkeleton(wxDC & dc, void * data)
{
	if(m_Impl)
	{
		m_Impl->DrawSkeleton(dc, data);
	}
}
