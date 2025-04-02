#pragma once

#include "pch.h"

struct screenOptions
{
	int width, height;
};

class CScreen
{
	SingletonClass(CScreen);

public:
	screenOptions& getOptions() { return m_sOptions; }
	const vector2Int getScreenVector();

private:
	screenOptions m_sOptions;
};

