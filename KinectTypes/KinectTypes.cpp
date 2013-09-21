#include "KinectTypes.h"
#include <stdlib.h>
#include <memory.h>
namespace kinect_data{
VideoFrame::VideoFrame(const unsigned char* buffer, uint32_t bufsize)
	: m_data(NULL)
	, m_nchannels(0)
	, m_width(0)
	, m_height(0)
	, m_rawData(NULL)
{
	//check data size
	if(bufsize > sizeof(VideoFrame))
	{
		const uint32_t* pdata = (const uint32_t*)buffer;
		m_width = pdata[0];
		m_height = pdata[1];
		m_nchannels = pdata[2];
		if(bufsize == GetSize())
		{
			m_rawData = new unsigned char[bufsize]();
			memcpy(m_rawData, buffer, bufsize);
			m_data = m_rawData + GetHeaderSize();
		}
	}
}

VideoFrame::VideoFrame(uint32_t width, uint32_t height, uint32_t nchannels, const unsigned char* data)
	: m_width(width)
	, m_height(height)
	, m_nchannels(nchannels)
	, m_data(NULL)
	, m_rawData(NULL)
{
	uint32_t sz = GetFrameSize();
	m_rawData = new unsigned char[sz + GetHeaderSize()]();
	uint32_t* pdata = (uint32_t*)m_rawData;
	pdata[0] = m_width;
	pdata[1] = m_height;
	pdata[2] = m_nchannels;
	m_data = m_rawData + GetHeaderSize();
	memcpy(m_data, data, sz);
}

VideoFrame::~VideoFrame()
{
	if(m_rawData != NULL)
	{
		delete m_rawData;
	}
}

uint32_t VideoFrame::GetFrameSize()
{
	return m_width * m_height * m_nchannels;
}

uint32_t VideoFrame::GetSize()
{
	return GetFrameSize() + GetHeaderSize();
}

bool VideoFrame::IsOK()
{
	return m_rawData != NULL && m_data != NULL;
}
};