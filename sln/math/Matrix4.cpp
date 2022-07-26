#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>

//PÊsñðßé
Matrix4 identity()
{
	static const Matrix4 result
	{

			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f

	};
	return result;
}

//gåk¬sñ
Matrix4 scale(const Vector3& s)
{
	Matrix4 result
	{
			s.x,0.0f,0.0f,0.0f,
			0.0f,s.y,0.0f,0.0f,
			0.0f,0.0f,s.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//x²üèÌñ]sñ
Matrix4 rotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos,sin,0.0f,
		0.0f,-sin,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//y²üèÌñ]sñ
Matrix4 rotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		//-----
		cos,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin,0.0f,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//z²üèÌñ]sñ
Matrix4 rotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos,sin,0.0f,0.0f,
		-sin,cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//½sÚ®
Matrix4 transX(float a)
{
	Matrix4 result
	{
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//½sÚ®y
Matrix4 transY(float a)
{
	Matrix4 result
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//½sÚ®z
Matrix4 transZ(float a)
{
	Matrix4 result
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//½sÚ®sñ
Matrix4 translate(const Vector3& t)
{
	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.x,t.y,t.z,1.0f
	};
	return result;
}

//ÀWÏ·ðs¤transformÖðì¬
Vector3 transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};
	return result;
}

//ãüZq*=ÌI[o[[hÖ(sñÆsñÌÏ)
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return m1;
}

//2Zq@ÌI[o[[hÖsñÆsñÌÏ
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

//2Zq@ÌI[o[[hÖ@xNgsñÆsñ
Vector3 operator * (const Vector3& v, const Matrix4& m)
{
	return transform(v, m);
}