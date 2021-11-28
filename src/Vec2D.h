#pragma once

class Vec2D
{
public:
	float m_x;
	float m_y;

	Vec2D() : m_x(0.0f), m_y(0.0f)
	{
	}

	Vec2D(float x, float y) : m_x(x), m_y(y)
	{
	}

	Vec2D operator+(const Vec2D other) const
	{
		Vec2D result;

		// Write the vector addition here, results should return 
		result.m_x = m_x + other.m_x;
		result.m_y = m_y + other.m_y;
		

		return result;
	}

	Vec2D operator-(const Vec2D other) const
	{
		Vec2D result;

		// Write the vector addition here, results should return 
		result.m_x = m_x - other.m_x;
		result.m_y = m_y - other.m_y;


		return result;
	}
};


class Vec3D
{
public:
	float m_x;
	float m_y;
	float m_z;

	Vec3D() : m_x(0.0f), m_y(0.0f), m_z(0.0f)
	{
	}

	Vec3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
	{
	}

	Vec3D operator+(const Vec3D other) const
	{
		Vec3D result;

		// Write the vector addition here, results should return 
		result.m_x = m_x + other.m_x;

		result.m_y = m_y + other.m_y;

		result.m_z = m_z + other.m_z;



		return result;
	}

};


