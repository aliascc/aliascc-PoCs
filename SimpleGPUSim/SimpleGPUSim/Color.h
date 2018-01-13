/*
* Copyright (c) 2016 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef _COLOR_H
#define _COLOR_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/********************
*   Game Includes   *
*********************/
#include "MathHelper.h"

/// <summary>
/// Color Structure
/// </summary>
struct Color
{
	/// <summary>
	/// Color Vector
	/// </summary>
	XMFLOAT4 m_Color;

	/// <summary>
	/// Color Constructor
	/// </summary>
	Color()
		: m_Color(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	/// <summary>
	/// Color Constructor
	/// </summary>
	/// <param name="r">Red</param>
	/// <param name="g">Green</param>
	/// <param name="b">Blue</param>
	/// <param name="a">Alfo</param>
	Color(float r, float g, float b, float a)
	{
		m_Color = XMFLOAT4(b, g, r, a);
	}

	/// <summary>
	/// Cast Operator to uint32_t
	/// </summary>
	inline operator uint32_t() const
	{
		union
		{
			struct
			{
				uint8_t b;
				uint8_t g;
				uint8_t r;
				uint8_t a;
			};
			uint32_t color;
		} color;

		color.r = static_cast<uint8_t>(clamp(m_Color.z * 255.0f, 0.0f, 255.0f));
		color.g = static_cast<uint8_t>(clamp(m_Color.y * 255.0f, 0.0f, 255.0f));
		color.b = static_cast<uint8_t>(clamp(m_Color.x * 255.0f, 0.0f, 255.0f));
		color.a = static_cast<uint8_t>(clamp(m_Color.w * 255.0f, 0.0f, 255.0f));

		return color.color;
	}

	/// <summary>
	/// Multiplication with a Float Operator
	/// </summary>
	/// <param name="other">Float to multiply</param>
	/// <returns>Resulting Color</returns>
	inline Color operator*(float other) const
	{
		Color color;

		XMVECTOR xmColor = XMLoadFloat4(&m_Color);

		XMStoreFloat4(&color.m_Color, (xmColor * other));

		return color;
	}

	/// <summary>
	/// Addition with another Color Operator
	/// </summary>
	/// <param name="other">Color to add</param>
	/// <returns>Resulting Color</returns>
	inline Color operator+(const Color& other) const
	{
		Color color;

		XMVECTOR xmColor = XMLoadFloat4(&m_Color);
		XMVECTOR xmOtherColor = XMLoadFloat4(&other.m_Color);

		XMStoreFloat4(&color.m_Color, (xmColor + xmOtherColor));

		return color;
	}

	/// <summary>
	/// Multiplication with a Float Operator
	/// </summary>
	/// <param name="other">Float to multiply</param>
	/// <returns>Resulting Color</returns>
	inline Color& operator*=(float other)
	{
		XMVECTOR xmColor = XMLoadFloat4(&m_Color);

		XMStoreFloat4(&m_Color, (xmColor * other));

		return (*this);
	}

	/// <summary>
	/// Addition with another Color Operator
	/// </summary>
	/// <param name="other">Color to add</param>
	/// <returns>Resulting Color</returns>
	inline Color& operator+=(const Color& other)
	{
		XMVECTOR xmColor = XMLoadFloat4(&m_Color);
		XMVECTOR xmOtherColor = XMLoadFloat4(&other.m_Color);

		XMStoreFloat4(&m_Color, (xmColor + xmOtherColor));

		return (*this);
	}
};

#endif
