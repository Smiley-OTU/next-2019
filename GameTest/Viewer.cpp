#include "stdafx.h"
#include "Viewer.h"

CViewer::CViewer()
{
}

CViewer::~CViewer()
{
}

float CViewer::GetFov()
{
	return m_fov;
}

void CViewer::SetFov(float fov)
{
	m_fov = fov;
}
