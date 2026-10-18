#pragma once

#include <string>
#include "Color.h"

struct WindowStyle
{
	// Background & Border
	std::wstring			backgroundTexture					{ L"" };
	Color					backgroundColor						{ 0.0f, 0.0f, 0.0f, 0.0f };
	Color					borderColor							{ 0.0f, 0.0f, 0.0f, 0.0f };
	float					borderSize							{ 0.0f };

	// Title bar
	float					titleBarHeight						{ 0.0f };
	Color					titleBarColor						{ 0.0f, 0.0f, 0.0f, 0.0f };
	std::wstring			titleBarTexture						{ L"" };
	std::wstring			titleBarFont						{ L"" };

	// Close Button
	float					closeButtonSize						{ 0.0f };
	std::wstring			closeButtonTexture					{ L"" };
};
