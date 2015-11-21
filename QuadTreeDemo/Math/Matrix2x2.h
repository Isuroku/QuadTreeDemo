#pragma once
#include "Vector2D.h"
#include <stdexcept>

template <typename TYPE>
class Matrix2x2
{
public:
	union
	{
		struct
		{
			TYPE        m11, m12;
			TYPE        m21, m22;
		};

		TYPE m[2][2];

		struct
		{
			Vector2D<TYPE> forward;
			Vector2D<TYPE> up;
		};
	};

	inline Matrix2x2() {}
	inline Matrix2x2<TYPE>(const Matrix2x2<TYPE>& m2x2)
	{
		m[0][0] = m2x2.m[0][0];
		m[0][1] = m2x2.m[0][1];
		m[1][0] = m2x2.m[1][0];
		m[1][1] = m2x2.m[1][1];
	}

	inline Matrix2x2<TYPE>& operator = (const Matrix2x2<TYPE>& _m)
	{
		m[0][0] = _m.m[0][0];
		m[0][1] = _m.m[0][1];
		m[1][0] = _m.m[1][0];
		m[1][1] = _m.m[1][1];
		return *this;
	}

	inline Vector2D<TYPE> operator [](int id) 
	{ 
		switch (id)
		{
			case 0: return forward;
			case 1: return up;
		}
		throw std::invalid_argument("index error");
	}


	inline int operator == (const Matrix2x2<TYPE> &v)
	{
		return (
			(m[0][0] == v.m[0][0]) && (m[0][1] == v.m[0][1]) &&
			(m[1][0] == v.m[1][0]) && (m[1][1] == v.m[1][1])
			);
	}


	inline void		zero() { memset(m, 0, 2 * 2 * sizeof(TYPE)); }
	inline void		one() { m[0][0] = m[1][1] = 1.f; m[0][1] = m[1][0] = 0.f; }
	inline void		Rotate(TYPE angle)
	{
		TYPE cosine = cos(angle);
		TYPE sine = sin(angle);
		
		m[0][0] = cosine;
		m[1][1] = cosine;
		m[1][0] = -sine;
		m[0][1] = sine;
	}

	Matrix2x2<TYPE> &operator *= (const float f)
	{
		m[0][0] *= f;	m[0][1] *= f;
		m[1][0] *= f;	m[1][1] *= f;
		return *this;
	}

	//установка матрицы по базису
	inline void SetBasis(const Vector2D<TYPE> &inForward, const Vector2D<TYPE> &inUp)
	{
		forward = inForward;
		up = inUp;
		AssertOrthogonal();
	}

	inline void SetBasis(const Vector2D<TYPE> &forward)
	{
		line1 = forward;
		line1.Normalize();

		line2 = Vector2D<TYPE>(line1.y, -line1.x); //против часовой
		AssertOrthogonal();
	}



#ifdef _DEBUG
	void AssertOrthogonal() const
	{
		assert(fabs(forward.Length() - static_cast<TYPE>(1)) < static_cast<TYPE>(0.001));
		assert(fabs(up.Length() - static_cast<TYPE>(1)) < static_cast<TYPE>(0.001));
		assert(fabs(DotProduct(forward, up)) < static_cast<TYPE>(0.001));
	}
#else // !ASSERTS
	inline void AssertOrthogonal() const {}
#endif // ASSERTS
};

typedef Matrix2x2<float> Matrix2x2f;


template <typename TYPE>
inline Vector2D<TYPE> mul(const Vector2D<TYPE> &u, const Matrix2x2<TYPE>& m)
{
	Vector2D<TYPE> res;
	res.x = u.x * m._11 + u.y * m._21;
	res.y = u.x * m._12 + u.y * m._22;

	return res;
}


template <typename TYPE>
Matrix2x2<TYPE> mul(const Matrix2x2<TYPE> &v, const Matrix2x2<TYPE> &u)
{
	Matrix2x2<TYPE> r;
	r.m[0][0] = v.m[0][0] * u.m[0][0] + v.m[0][1] * u.m[1][0];
	r.m[0][1] = v.m[0][0] * u.m[0][1] + v.m[0][1] * u.m[1][1];
	r.m[1][0] = v.m[1][0] * u.m[0][0] + v.m[1][1] * u.m[1][0];
	r.m[1][1] = v.m[1][0] * u.m[0][1] + v.m[1][1] * u.m[1][1];
	return r;
}

template <typename TYPE>
inline Vector2D<TYPE> mul(const Matrix2x2<TYPE>& m, const Vector2D<TYPE> &u)
{
	Vector2D<TYPE> res;
	res.x = u.x * m._11 + u.y * m._12;
	res.y = u.x * m._21 + u.y * m._22;

	return res;
}