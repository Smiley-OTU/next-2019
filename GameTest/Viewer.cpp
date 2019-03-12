#include "stdafx.h"
#include "Viewer.h"

CViewer::CViewer()
{
}

CViewer::~CViewer()
{
}

void CViewer::setFov(float fov)
{	//Gotta multiply by 0.5 rather than divide by 2.0 for the optimal setter that gets called like once ;)
	//m_halfFov = fov *0.5f;
	m_fov = fov;
}
