#include "CScreen.h"

CScreen::CScreen()
	: m_sOptions({960, 540})
{
}

CScreen::~CScreen()
{

}

const vector2Int CScreen::getScreenVector()
{
	vector2Int screen = vector2Int(m_sOptions.width, m_sOptions.height);

	return screen;
}
