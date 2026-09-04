#pragma once

struct Position
{
	float X;
	float Y;

	Position operator+(const Position& right) const
	{
		return Position{ X + right.X, Y + right.Y };
	}
	Position operator+(float right) const
	{
		return Position{ X + right, Y + right };
	}
	Position& operator+=(const Position& right)
	{
		X += right.X;
		Y += right.Y;
		return *this;
	}
	Position& operator+=(const float& right)
	{
		X += right;
		Y += right;
		return *this;
	}
	Position operator-(const Position& right) const
	{
		return Position{ X - right.X, Y - right.Y };
	}
	Position operator-(float right) const
	{
		return Position{ X - right, Y - right };
	}
	Position& operator-=(const Position& right)
	{
		X -= right.X;
		Y -= right.Y;
		return *this;
	}
	Position& operator-=(const float& right)
	{
		X -= right;
		Y -= right;
		return *this;
	}
};