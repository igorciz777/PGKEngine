#ifndef PGK_MATH_H
#define PGK_MATH_H


#include <algorithm>
#include <cmath>
#include <immintrin.h>
#include <cstdint>
#include <QColor>

#define EPS  1e-5f
#define MOD  1000000007
#define INV_TWO_PI 0.1591549f
#define TWO_PI 6.283185f

template <class T>
class T_Quat;
template <class T>
class T_Vec4;

template <class T>
class T_Vec2
{
public:
    T x, y;

    T_Vec2() : x(0), y(0) {}
    T_Vec2(T x, T y) : x(x), y(y) {}

    static const T_Vec2<T> ZERO;

    inline float length() const { return sqrt(x*x + y*y); }
    inline float lengthSq() const { return x*x + y*y; }
    inline float dot(const T_Vec2& v) const { return x*v.x + y*v.y; }
    inline float distance(const T_Vec2& v) const { return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y)); }
    inline float distanceSq(const T_Vec2& v) const { return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y); }
    inline T_Vec2& normalize() { float len = length(); x /= len; y /= len; return *this; }

    inline T_Vec2 operator+(const T_Vec2& v) const { return T_Vec2(x + v.x, y + v.y); }
    inline T_Vec2 operator-(const T_Vec2& v) const { return T_Vec2(x - v.x, y - v.y); }
    inline T_Vec2 operator*(const T_Vec2& v) const { return T_Vec2(x * v.x, y * v.y); }
    inline T_Vec2 operator/(const T_Vec2& v) const { return T_Vec2(x / v.x, y / v.y); }
    inline T_Vec2 operator*(T s) const { return T_Vec2(x * s, y * s); }
    inline T_Vec2 operator/(T s) const { return T_Vec2(x / s, y / s); }
    inline T_Vec2& operator+=(const T_Vec2& v) { x += v.x; y += v.y; return *this; }
    inline T_Vec2& operator-=(const T_Vec2& v) { x -= v.x; y -= v.y; return *this; }
    inline T_Vec2& operator*=(const T_Vec2& v) { x *= v.x; y *= v.y; return *this; }
    inline T_Vec2& operator/=(const T_Vec2& v) { x /= v.x; y /= v.y; return *this; }
    inline T_Vec2& operator*=(T s) { x *= s; y *= s; return *this; }
    inline T_Vec2& operator/=(T s) { x /= s; y /= s; return *this; }
    inline T_Vec2 operator-() const { return T_Vec2(-x, -y); }
    inline bool operator==(const T_Vec2& v) const { return x == v.x && y == v.y; }
    inline bool operator!=(const T_Vec2& v) const { return x != v.x || y != v.y; }
    inline bool operator<(const T_Vec2& v) const { return x < v.x && y < v.y; }
    inline bool operator<=(const T_Vec2& v) const { return x <= v.x && y <= v.y; }
    inline bool operator>(const T_Vec2& v) const { return x > v.x && y > v.y; }
    inline bool operator>=(const T_Vec2& v) const { return x >= v.x && y >= v.y; }
};

typedef T_Vec2<int> iVec2;
typedef T_Vec2<float> Vec2;
typedef T_Vec2<double> dVec2;

template <class T>
const T_Vec2<T> T_Vec2<T>::ZERO(0, 0);

template <class T>
class T_Vec3
{
public:
    T x, y, z;

    T_Vec3() : x(0), y(0), z(0) {}
    T_Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    T_Vec3(T_Vec4<T> v) : x(v.x), y(v.y), z(v.z) {}
    static const T_Vec3<T> ZERO;

    inline float length() const { return sqrt(x*x + y*y + z*z); }
    inline float lengthSq() const { return x*x + y*y + z*z; }
    inline float distance(const T_Vec3& v) const { return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z)); }
    inline float distanceSq(const T_Vec3& v) const { return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z); }
    inline float dot(const T_Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
    inline T_Vec3 cross(const T_Vec3& v) const { return T_Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
    inline T_Vec3& normalize() { float len = length(); x /= len; y /= len; z /= len; return *this; }
    inline T_Vec3 reflect(const T_Vec3& normal) const { return *this - normal * 2 * dot(normal); }


    inline T_Vec3 operator+(const T_Vec3& v) const { return T_Vec3(x + v.x, y + v.y, z + v.z); }
    inline T_Vec3 operator-(const T_Vec3& v) const { return T_Vec3(x - v.x, y - v.y, z - v.z); }
    inline T_Vec3 operator*(const T_Vec3& v) const { return T_Vec3(x * v.x, y * v.y, z * v.z); }
    inline T_Vec3 operator/(const T_Vec3& v) const { return T_Vec3(x / v.x, y / v.y, z / v.z); }
    inline T_Vec3 operator*(T s) const { return T_Vec3(x * s, y * s, z * s); }
    inline T_Vec3 operator/(T s) const { return T_Vec3(x / s, y / s, z / s); }
    inline T_Vec3& operator+=(const T_Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    inline T_Vec3& operator-=(const T_Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    inline T_Vec3& operator*=(const T_Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    inline T_Vec3& operator/=(const T_Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    inline T_Vec3& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
    inline T_Vec3& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }
    inline T_Vec3 operator-() const { return T_Vec3(-x, -y, -z); }
    inline bool operator==(const T_Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
    inline bool operator!=(const T_Vec3& v) const { return x != v.x || y != v.y || z != v.z; }
    inline bool operator<(const T_Vec3& v) const { return x < v.x && y < v.y && z < v.z; }
    inline bool operator<=(const T_Vec3& v) const { return x <= v.x && y <= v.y && z <= v.z; }
    inline bool operator>(const T_Vec3& v) const { return x > v.x && y > v.y && z > v.z; }
    inline bool operator>=(const T_Vec3& v) const { return x >= v.x && y >= v.y && z >= v.z; }
    inline T_Vec3 operator>>(int s) const { return T_Vec3(x >> s, y >> s, z >> s); }
    inline T_Vec3 operator<<(int s) const { return T_Vec3(x << s, y << s, z << s); }
};

typedef T_Vec3<uint16_t> cVec3; // color vec3
typedef T_Vec3<int> iVec3;
typedef T_Vec3<float> Vec3;
typedef T_Vec3<double> dVec3;

//OBJ_Loader compatibility
typedef Vec3 Vector3;

template <class T>
const T_Vec3<T> T_Vec3<T>::ZERO(0, 0, 0);

template <class T>
class T_Vec4
{
public:
    T x, y, z, w;

    T_Vec4() : x(0), y(0), z(0), w(0) {}
    T_Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    T_Vec4(const T_Vec3<T>& v) : x(v.x), y(v.y), z(v.z), w(1) {}

    static const T_Vec4<T> ZERO;

    inline float length() const { return sqrt(x*x + y*y + z*z + w*w); }
    inline float lengthSq() const { return x*x + y*y + z*z + w*w; }
    inline float dot(const T_Vec4& v) { return x*v.x + y*v.y + z*v.z + w*v.w; }
    inline T_Vec4& normalize() { float len = length(); x /= len; y /= len; z /= len; w /= len; return *this; }

    inline T_Vec4 operator+(const T_Vec4& v) const { return T_Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
    inline T_Vec4 operator-(const T_Vec4& v) const { return T_Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
    inline T_Vec4 operator*(const T_Vec4& v) const { return T_Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
    inline T_Vec4 operator/(const T_Vec4& v) const { return T_Vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
    inline T_Vec4 operator*(T s) const { return T_Vec4(x * s, y * s, z * s, w * s); }
    inline T_Vec4 operator/(T s) const { return T_Vec4(x / s, y / s, z / s, w / s); }
    inline T_Vec4& operator+=(const T_Vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    inline T_Vec4& operator-=(const T_Vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    inline T_Vec4& operator*=(const T_Vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    inline T_Vec4& operator/=(const T_Vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
    inline T_Vec4& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    inline T_Vec4& operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    inline T_Vec4 operator-() const { return T_Vec4(-x, -y, -z, -w); }
    inline bool operator==(const T_Vec4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
    inline bool operator!=(const T_Vec4& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
    inline bool operator<(const T_Vec4& v) const { return x < v.x && y < v.y && z < v.z && w < v.w; }
    inline bool operator<=(const T_Vec4& v) const { return x <= v.x && y <= v.y && z <= v.z && w <= v.w; }
    inline bool operator>(const T_Vec4& v) const { return x > v.x && y > v.y && z > v.z && w > v.w; }
    inline bool operator>=(const T_Vec4& v) const { return x >= v.x && y >= v.y && z >= v.z && w >= v.w; }
};

typedef T_Vec4<int> iVec4;
typedef T_Vec4<float> Vec4;
typedef T_Vec4<double> dVec4;

template <class T>
const T_Vec4<T> T_Vec4<T>::ZERO(0, 0, 0, 0);

template <class T>
class T_Mat2
{
public:
    T m00, m01, m10, m11;

    T_Mat2() : m00(0), m01(0), m10(0), m11(0) {}
    T_Mat2(T m00, T m01, T m10, T m11) : m00(m00), m01(m01), m10(m10), m11(m11) {}

    static const T_Mat2<T> IDENTITY;

    inline float determinant() const { return m00*m11 - m01*m10; }

    inline T_Mat2 operator+(const T_Mat2& m) const { return T_Mat2(m00 + m.m00, m01 + m.m01, m10 + m.m10, m11 + m.m11); }
    inline T_Mat2 operator-(const T_Mat2& m) const { return T_Mat2(m00 - m.m00, m01 - m.m01, m10 - m.m10, m11 - m.m11); }
    inline T_Mat2 operator*(const T_Mat2& m) const { return T_Mat2(m00*m.m00 + m01*m.m10, m00*m.m01 + m01*m.m11,
                                                                   m10*m.m00 + m11*m.m10, m10*m.m01 + m11*m.m11); }
    inline T_Mat2 operator*(T s) const { return T_Mat2(m00 * s, m01 * s, m10 * s, m11 * s); }
    inline T_Mat2 operator/(T s) const { return T_Mat2(m00 / s, m01 / s, m10 / s, m11 / s); }
    inline T_Mat2& operator+=(const T_Mat2& m) { m00 += m.m00; m01 += m.m01; m10 += m.m10; m11 += m.m11; return *this; }
    inline T_Mat2& operator-=(const T_Mat2& m) { m00 -= m.m00; m01 -= m.m01; m10 -= m.m10; m11 -= m.m11; return *this; }
    inline T_Mat2& operator*=(const T_Mat2& m) { *this = *this * m; return *this; }
    inline T_Mat2& operator*=(T s) { m00 *= s; m01 *= s; m10 *= s; m11 *= s; return *this; }
    inline T_Mat2& operator/=(T s) { m00 /= s; m01 /= s; m10 /= s; m11 /= s; return *this; }
    inline T_Mat2 operator-() const { return T_Mat2(-m00, -m01, -m10, -m11); }
    inline bool operator==(const T_Mat2& m) const { return m00 == m.m00 && m01 == m.m01 && m10 == m.m10 && m11 == m.m11; }
    inline bool operator!=(const T_Mat2& m) const { return m00 != m.m00 || m01 != m.m01 || m10 != m.m10 || m11 != m.m11; }
};

typedef T_Mat2<int> iMat2;
typedef T_Mat2<float> Mat2;
typedef T_Mat2<double> dMat2;

template <class T>
const T_Mat2<T> T_Mat2<T>::IDENTITY(1, 0, 0, 1);

template <class T>
class T_Mat3
{
public:
    T m00, m01, m02, m10, m11, m12, m20, m21, m22;

    T_Mat3() : m00(0), m01(0), m02(0), m10(0), m11(0), m12(0), m20(0), m21(0), m22(0) {}
    T_Mat3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22) {}

    static const T_Mat3<T> IDENTITY;

    inline float determinant() const { return m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m02*m11*m20 - m01*m10*m22 - m00*m12*m21; }

    inline T_Mat3 operator+(const T_Mat3& m) const { return T_Mat3(m00 + m.m00, m01 + m.m01, m02 + m.m02, m10 + m.m10, m11 + m.m11, m12 + m.m12, m20 + m.m20, m21 + m.m21, m22 + m.m22); }
    inline T_Mat3 operator-(const T_Mat3& m) const { return T_Mat3(m00 - m.m00, m01 - m.m01, m02 - m.m02, m10 - m.m10, m11 - m.m11, m12 - m.m12, m20 - m.m20, m21 - m.m21, m22 - m.m22); }
    inline T_Mat3 operator*(const T_Mat3& m) const { return T_Mat3(m00*m.m00 + m01*m.m10 + m02*m.m20, m00*m.m01 + m01*m.m11 + m02*m.m21, m00*m.m02 + m01*m.m12 + m02*m.m22,
                                                                   m10*m.m00 + m11*m.m10 + m12*m.m20, m10*m.m01 + m11*m.m11 + m12*m.m21, m10*m.m02 + m11*m.m12 + m12*m.m22,
                                                                   m20*m.m00 + m21*m.m10 + m22*m.m20, m20*m.m01 + m21*m.m11 + m22*m.m21, m20*m.m02 + m21*m.m12 + m22*m.m22); }
    inline T_Mat3 operator*(T s) const { return T_Mat3(m00 * s, m01 * s, m02 * s, m10 * s, m11 * s, m12 * s, m20 * s, m21 * s, m22 * s); }
    inline T_Mat3 operator/(T s) const { return T_Mat3(m00 / s, m01 / s, m02 / s, m10 / s, m11 / s, m12 / s, m20 / s, m21 / s, m22 / s); }
    inline T_Mat3& operator+=(const T_Mat3& m) { m00 += m.m00; m01 += m.m01; m02 += m.m02; m10 += m.m10; m11 += m.m11; m12 += m.m12; m20 += m.m20; m21 += m.m21; m22 += m.m22; return *this; }
    inline T_Mat3& operator-=(const T_Mat3& m) { m00 -= m.m00; m01 -= m.m01; m02 -= m.m02; m10 -= m.m10; m11 -= m.m11; m12 -= m.m12; m20 -= m.m20; m21 -= m.m21; m22 -= m.m22; return *this; }
    inline T_Mat3& operator*=(const T_Mat3& m) { *this = *this * m; return *this; }
    inline T_Mat3& operator*=(T s) { m00 *= s; m01 *= s; m02 *= s; m10 *= s; m11 *= s; m12 *= s; m20 *= s; m21 *= s; m22 *= s; return *this; }
    inline T_Mat3& operator/=(T s) { m00 /= s; m01 /= s; m02 /= s; m10 /= s; m11 /= s; m12 /= s; m20 /= s; m21 /= s; m22 /= s; return *this; }
    inline T_Mat3 operator-() const { return T_Mat3(-m00, -m01, -m02, -m10, -m11, -m12, -m20, -m21, -m22); }
    inline bool operator==(const T_Mat3& m) const { return m00 == m.m00 && m01 == m.m01 && m02 == m.m02 && m10 == m.m10 && m11 == m.m11 && m12 == m.m12 && m20 == m.m20 && m21 == m.m21 && m22 == m.m22; }
    inline bool operator!=(const T_Mat3& m) const { return m00 != m.m00 || m01 != m.m01 || m02 != m.m02 || m10 != m.m10 || m11 != m.m11 || m12 != m.m12 || m20 != m.m20 || m21 != m.m21 || m22 != m.m22; }
};

typedef T_Mat3<int> iMat3;
typedef T_Mat3<float> Mat3;
typedef T_Mat3<double> dMat3;

template <class T>
const T_Mat3<T> T_Mat3<T>::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);

template <class T>
class T_Mat4
{
public:
    T m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;

    T_Mat4() : m00(0), m01(0), m02(0), m03(0), m10(0), m11(0), m12(0), m13(0), m20(0), m21(0), m22(0), m23(0), m30(0), m31(0), m32(0), m33(0) {}
    T_Mat4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33) {}

    static const T_Mat4<T> IDENTITY;
    static const T_Mat4<T> Translate(Vec3 v){
        return T_Mat4(1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0, 1);
    }
    static const T_Mat4<T> Scale(Vec3 v){
        return T_Mat4(v.x, 0, 0, 0,
                    0, v.y, 0, 0,
                    0, 0, v.z, 0,
                    0, 0, 0, 1);
    }
    static const T_Mat4<T> RotateX(float angle){
        float c = cosf(angle);
        float s = sinf(angle);
        return T_Mat4(1, 0, 0, 0,
                    0, c, -s, 0,
                    0, s, c, 0,
                    0, 0, 0, 1);
    }
    static const T_Mat4<T> RotateY(float angle){
        float c = cosf(angle);
        float s = sinf(angle);
        return T_Mat4(c, 0, s, 0,
                    0, 1, 0, 0,
                    -s, 0, c, 0,
                    0, 0, 0, 1);
    }
    static const T_Mat4<T> RotateZ(float angle){
        float c = cosf(angle);
        float s = sinf(angle);
        return T_Mat4(c, -s, 0, 0,
                    s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    }
    static const T_Mat4<T> Rotate(const T_Quat<T> &rot){
        T_Mat4<T> m = rot.toMatrix();
        return m;
    }
    static const T_Mat4<T> Transform(const T_Vec3<T> &v, const T_Vec3<T> &r, const T_Vec3<T> &s){
        return Translate(v) * RotateX(r.x) * RotateY(r.y) * RotateZ(r.z) * Scale(s);
    }

    static const T_Mat4<T> Transform(const T_Vec3<T> &v, const T_Quat<T> &r, const T_Vec3<T> &s){
        if(s == Vec3(1, 1, 1))
            return Translate(v) * r.toMatrix();
        return Translate(v) * r.toMatrix() * Scale(s);
    }

    static const T_Mat4<T> OrthographicProjection(T left, T right, T bottom, T top, T near, T far){
        return T_Mat4<T>(2 / (right - left), 0, 0, -(right + left) / (right - left),
                         0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                         0, 0, -2 / (far - near), -(far + near) / (far - near),
                         0, 0, 0, 1);
    }
    static const T_Mat4<T> PerspectiveProjection(T fovX, T aspect, T near, T far){
        float tangent = tan(fovX/2 * acosf(-1) / 180);
        return T_Mat4<T>
            (1/(aspect*tangent),0,0,0,
                0,1/tangent,0,0,
                         0,0,(far+near)/(far-near),(2*far*near)/(far-near),
                0,0,-1,0);
    }

    inline T_Mat4 inverse() const {
        T s0 = m00 * m11 - m10 * m01;
        T s1 = m00 * m12 - m10 * m02;
        T s2 = m00 * m13 - m10 * m03;
        T s3 = m01 * m12 - m11 * m02;
        T s4 = m01 * m13 - m11 * m03;
        T s5 = m02 * m13 - m12 * m03;
        T c5 = m22 * m33 - m32 * m23;
        T c4 = m21 * m33 - m31 * m23;
        T c3 = m21 * m32 - m31 * m22;
        T c2 = m20 * m33 - m30 * m23;
        T c1 = m20 * m32 - m30 * m22;
        T c0 = m20 * m31 - m30 * m21;
        T invdet = 1 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
        return T_Mat4(
            (m11 * c5 - m12 * c4 + m13 * c3) * invdet,
            (-m01 * c5 + m02 * c4 - m03 * c3) * invdet,
            (m31 * s5 - m32 * s4 + m33 * s3) * invdet,
            (-m21 * s5 + m22 * s4 - m23 * s3) * invdet,
            (-m10 * c5 + m12 * c2 - m13 * c1) * invdet,
            (m00 * c5 - m02 * c2 + m03 * c1) * invdet,
            (-m30 * s5 + m32 * s2 - m33 * s1) * invdet,
            (m20 * s5 - m22 * s2 + m23 * s1) * invdet,
            (m10 * c4 - m11 * c2 + m13 * c0) * invdet,
            (-m00 * c4 + m01 * c2 - m03 * c0) * invdet,
            (m30 * s4 - m31 * s2 + m33 * s0) * invdet,
            (-m20 * s4 + m21 * s2 - m23 * s0) * invdet,
            (-m10 * c3 + m11 * c1 - m12 * c0) * invdet,
            (m00 * c3 - m01 * c1 + m02 * c0) * invdet,
            (-m30 * s3 + m31 * s1 - m32 * s0) * invdet,
            (m20 * s3 - m21 * s1 + m22 * s0) * invdet
        );
    }

    inline float determinant() const { return m00*m11*m22*m33 + m01*m12*m23*m30 + m02*m13*m20*m31 + m03*m10*m21*m32 - m03*m12*m21*m30 - m02*m10*m23*m31 - m01*m13*m20*m32 - m00*m11*m22*m33; }
    inline T_Mat4 transpose() const { return T_Mat4(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33); }

    inline T_Mat4 operator+(const T_Mat4& m) const { return T_Mat4(m00 + m.m00, m01 + m.m01, m02 + m.m02, m03 + m.m03, m10 + m.m10, m11 + m.m11, m12 + m.m12, m13 + m.m13, m20 + m.m20, m21 + m.m21, m22 + m.m22, m23 + m.m23, m30 + m.m30, m31 + m.m31, m32 + m.m32, m33 + m.m33); }
    inline T_Mat4 operator-(const T_Mat4& m) const { return T_Mat4(m00 - m.m00, m01 - m.m01, m02 - m.m02, m03 - m.m03, m10 - m.m10, m11 - m.m11, m12 - m.m12, m13 - m.m13, m20 - m.m20, m21 - m.m21, m22 - m.m22, m23 - m.m23, m30 - m.m30, m31 - m.m31, m32 - m.m32, m33 - m.m33); }
    inline T_Mat4 operator*(const T_Mat4& m) const { return T_Mat4(m00*m.m00 + m01*m.m10 + m02*m.m20 + m03*m.m30, m00*m.m01 + m01*m.m11 + m02*m.m21 + m03*m.m31, m00*m.m02 + m01*m.m12 + m02*m.m22 + m03*m.m32, m00*m.m03 + m01*m.m13 + m02*m.m23 + m03*m.m33,
                                                                   m10*m.m00 + m11*m.m10 + m12*m.m20 + m13*m.m30, m10*m.m01 + m11*m.m11 + m12*m.m21 + m13*m.m31, m10*m.m02 + m11*m.m12 + m12*m.m22 + m13*m.m32, m10*m.m03 + m11*m.m13 + m12*m.m23 + m13*m.m33,
                                                                   m20*m.m00 + m21*m.m10 + m22*m.m20 + m23*m.m30, m20*m.m01 + m21*m.m11 + m22*m.m21 + m23*m.m31, m20*m.m02 + m21*m.m12 + m22*m.m22 + m23*m.m32, m20*m.m03 + m21*m.m13 + m22*m.m23 + m23*m.m33,
                                                                   m30*m.m00 + m31*m.m10 + m32*m.m20 + m33*m.m30, m30*m.m01 + m31*m.m11 + m32*m.m21 + m33*m.m31, m30*m.m02 + m31*m.m12 + m32*m.m22 + m33*m.m32, m30*m.m03 + m31*m.m13 + m32*m.m23 + m33*m.m33); }
    inline T_Vec4<T> operator*(const T_Vec4<T>& v) const { return T_Vec4<T>(m00*v.x + m01*v.y + m02*v.z + m03*v.w, m10*v.x + m11*v.y + m12*v.z + m13*v.w, m20*v.x + m21*v.y + m22*v.z + m23*v.w, m30*v.x + m31*v.y + m32*v.z + m33*v.w); }
    inline T_Vec3<T> operator*(const T_Vec3<T>& v) const { return T_Vec3<T>(m00*v.x + m01*v.y + m02*v.z + m03, m10*v.x + m11*v.y + m12*v.z + m13, m20*v.x + m21*v.y + m22*v.z + m23); }
    inline T_Vec2<T> operator*(const T_Vec2<T>& v) const { return T_Vec2<T>(m00*v.x + m01*v.y + m03, m10*v.x + m11*v.y + m13); }
    inline T_Mat4 operator*(T s) const { return T_Mat4(m00 * s, m01 * s, m02 * s, m03 * s, m10 * s, m11 * s, m12 * s, m13 * s, m20 * s, m21 * s, m22 * s, m23 * s, m30 * s, m31 * s, m32 * s, m33 * s); }
    inline T_Mat4 operator/(T s) const { return T_Mat4(m00 / s, m01 / s, m02 / s, m03 / s, m10 / s, m11 / s, m12 / s, m13 / s, m20 / s, m21 / s, m22 / s, m23 / s, m30 / s, m31 / s, m32 / s, m33 / s); }
    inline T_Mat4& operator+=(const T_Mat4& m) const { m00 += m.m00; m01 += m.m01; m02 += m.m02; m03 += m.m03; m10 += m.m10; m11 += m.m11; m12 += m.m12; m13 += m.m13; m20 += m.m20; m21 += m.m21; m22 += m.m22; m23 += m.m23; m30 += m.m30; m31 += m.m31; m32 += m.m32; m33 += m.m33; return *this; }
    inline T_Mat4& operator-=(const T_Mat4& m) const { m00 -= m.m00; m01 -= m.m01; m02 -= m.m02; m03 -= m.m03; m10 -= m.m10; m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m20 -= m.m20; m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m30 -= m.m30; m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; return *this; }
    inline T_Mat4& operator*=(const T_Mat4& m) const { *this = *this * m; return *this; }
    inline T_Mat4& operator*=(T s) const { m00 *= s; m01 *= s; m02 *= s; m03 *= s; m10 *= s; m11 *= s; m12 *= s; m13 *= s; m20 *= s; m21 *= s; m22 *= s; m23 *= s; m30 *= s; m31 *= s; m32 *= s; m33 *= s; return *this; }
    inline T_Mat4& operator/=(T s) const { m00 /= s; m01 /= s; m02 /= s; m03 /= s; m10 /= s; m11 /= s; m12 /= s; m13 /= s; m20 /= s; m21 /= s; m22 /= s; m23 /= s; m30 /= s; m31 /= s; m32 /= s; m33 /= s; return *this; }
    inline T_Mat4 operator-() const { return T_Mat4(-m00, -m01, -m02, -m03, -m10, -m11, -m12, -m13, -m20, -m21, -m22, -m23, -m30, -m31, -m32, -m33); }
    inline bool operator==(const T_Mat4& m) const { return m00 == m.m00 && m01 == m.m01 && m02 == m.m02 && m03 == m.m03 && m10 == m.m10 && m11 == m.m11 && m12 == m.m12 && m13 == m.m13 && m20 == m.m20 && m21 == m.m21 && m22 == m.m22 && m23 == m.m23 && m30 == m.m30 && m31 == m.m31 && m32 == m.m32 && m33 == m.m33; }
    inline bool operator!=(const T_Mat4& m) const { return m00 != m.m00 || m01 != m.m01 || m02 != m.m02 || m03 != m.m03 || m10 != m.m10 || m11 != m.m11 || m12 != m.m12 || m13 != m.m13 || m20 != m.m20 || m21 != m.m21 || m22 != m.m22 || m23 != m.m23 || m30 != m.m30 || m31 != m.m31 || m32 != m.m32 || m33 != m.m33; }
};

typedef T_Mat4<int> iMat4;
typedef T_Mat4<float> Mat4;
typedef T_Mat4<double> dMat4;

template <class T>
const T_Mat4<T> T_Mat4<T>::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

template <class T>
inline T_Vec2<T> operator*(T s, const T_Vec2<T>& v) { return v * s; }

template <class T>
inline T_Vec3<T> operator*(T s, const T_Vec3<T>& v) { return v * s; }

template <class T>
inline T_Vec4<T> operator*(T s, const T_Vec4<T>& v) { return v * s; }

template <class T>
inline T_Mat2<T> operator*(T s, const T_Mat2<T>& m) { return m * s; }

template <class T>
inline T_Mat3<T> operator*(T s, const T_Mat3<T>& m) { return m * s; }

template <class T>
inline T_Mat4<T> operator*(T s, const T_Mat4<T>& m) { return m * s; }

template <class T>
class T_Quat
{
public:
    T x, y, z, w;

    enum class RotationOrder { XYZ, XZY, YXZ, YZX, ZXY, ZYX };
    inline T_Quat& normalize() {
        T len = length();
        len = 1.f / len;
        x *= len;
        y *= len;
        z *= len;
        w *= len;
        return *this; 
    }

    T_Quat() : x(0), y(0), z(0), w(1) {}
    T_Quat(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    T_Quat(const T_Vec3<T>& axis, T radians){
        T halfAngle = radians * 0.5f;

        T sin_2 = sinf(halfAngle);
        T cos_2 = cosf(halfAngle);

        T sin_norm = sin_2 / sqrtf(axis.x * axis.x + axis.y*axis.y + axis.z*axis.z);

        w = cos_2;
        x = axis.x * sin_norm;
        y = axis.y * sin_norm;
        z = axis.z * sin_norm;
    }
    T_Quat(const T_Vec3<T>& eulerAngles, RotationOrder order){
        T_Vec3<T> xyz = T_Vec3<T>(eulerAngles.x * 0.5f,eulerAngles.y * 0.5f,eulerAngles.z * 0.5f);
        T cX = cosf(xyz.x);
        T cY = cosf(xyz.y);
        T cZ = cosf(xyz.z);

        T sX = sinf(xyz.x);
        T sY = sinf(xyz.y);
        T sZ = sinf(xyz.z);
        switch(order){
            case RotationOrder::YXZ:{
                x = sX * sY * sZ + cX * cY * cZ;
                y = sX * sZ * cY + sY * cX * cZ;
                z = sX * cY * cZ - sY * sZ * cX;
                w = sZ * cX * cY - sX * sY * cZ;
            } break;
            case RotationOrder::XYZ:{
                x = sX * cY * cZ - cX * sY * sZ;
                y = cX * sY * cZ + sX * cY * sZ;
                z = cX * cY * sZ + sX * sY * cZ;
                w = cX * cY * cZ - sX * sY * sZ;
            } break;
            case RotationOrder::YZX:{
                x = sX * cY * sZ + cX * sY * cZ;
                y = cX * sY * sZ - sX * cY * cZ;
                z = cX * cY * sZ + sX * sY * cZ;
                w = cX * cY * cZ - sX * sY * sZ;
            } break;
            case RotationOrder::ZXY:{
                x = sX * cY * cZ + cX * sY * sZ;
                y = cX * sY * cZ - sX * cY * sZ;
                z = cX * cY * sZ - sX * sY * cZ;
                w = cX * cY * cZ + sX * sY * sZ;
            } break;
            case RotationOrder::XZY:{
                x = sX * cY * cZ - cX * sY * sZ;
                y = cX * sY * cZ + sX * cY * sZ;
                z = cX * cY * sZ - sX * sY * cZ;
                w = cX * cY * cZ + sX * sY * sZ;
            } break;
            case RotationOrder::ZYX:{
                x = sX * cY * sZ - cX * sY * cZ;
                y = cX * sY * sZ + sX * cY * cZ;
                z = cX * cY * sZ + sX * sY * cZ;
                w = cX * cY * cZ - sX * sY * sZ;
            } break;
        }
    }

    T_Quat(const T_Mat4<T> &m){
        T trace = m.m00 + m.m11 + m.m22;
        if (trace > 0) {
            T s = 0.5 / sqrt(trace + 1);
            w = 0.25 / s;
            x = (m.m21 - m.m12) * s;
            y = (m.m02 - m.m20) * s;
            z = (m.m10 - m.m01) * s;
        } else {
            if (m.m00 > m.m11 && m.m00 > m.m22) {
                T s = 2 * sqrt(1 + m.m00 - m.m11 - m.m22);
                w = (m.m21 - m.m12) / s;
                x = 0.25 * s;
                y = (m.m01 + m.m10) / s;
                z = (m.m02 + m.m20) / s;
            } else if (m.m11 > m.m22) {
                T s = 2 * sqrt(1 + m.m11 - m.m00 - m.m22);
                w = (m.m02 - m.m20) / s;
                x = (m.m01 + m.m10) / s;
                y = 0.25 * s;
                z = (m.m12 + m.m21) / s;
            } else {
                T s = 2 * sqrt(1 + m.m22 - m.m00 - m.m11);
                w = (m.m10 - m.m01) / s;
                x = (m.m02 + m.m20) / s;
                y = (m.m12 + m.m21) / s;
                z = 0.25 * s;
            }
        }
    }

    T_Mat4<T> toMatrix() const {
        T xx = x*x, yy = y*y, zz = z*z;
        T xy = x*y, xz = x*z, xw = x*w;
        T yz = y*z, yw = y*w, zw = z*w;

        return T_Mat4<T>(
            1 - 2*(yy + zz),   2*(xy - zw),   2*(xz + yw), 0,
            2*(xy + zw),   1 - 2*(xx + zz),   2*(yz - xw), 0,
            2*(xz - yw),   2*(yz + xw),   1 - 2*(xx + yy), 0,
            0, 0, 0, 1
            );
    }

    static const T_Quat<T> IDENTITY;

    inline T length() const { return sqrt(x*x + y*y + z*z + w*w); }
    inline T lengthSq() const { return x*x + y*y + z*z + w*w; }
    inline T_Quat conjugate() const { return T_Quat(-x, -y, -z, w); }
    inline T_Quat inverse() const { return conjugate() / lengthSq(); }

    inline T_Quat operator+(const T_Quat& q) const { return T_Quat(x + q.x, y + q.y, z + q.z, w + q.w); }
    inline T_Quat operator-(const T_Quat& q) const { return T_Quat(x - q.x, y - q.y, z - q.z, w - q.w); }
    inline T_Quat operator*(const T_Quat& q) const {
        return T_Quat(
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w,
            w*q.w - x*q.x - y*q.y - z*q.z
            );
    }
    inline T_Vec3<T> operator*(const T_Vec3<T>& v) const {
        T qx = x, qy = y, qz = z, qw = w;
        T ix = qw*v.x + qy*v.z - qz*v.y;
        T iy = qw*v.y + qz*v.x - qx*v.z;
        T iz = qw*v.z + qx*v.y - qy*v.x;
        T iw = -qx*v.x - qy*v.y - qz*v.z;
        return T_Vec3<T>(ix*qw + iw*-qx + iy*-qz - iz*-qy,
                         iy*qw + iw*-qy + iz*-qx - ix*-qz,
                         iz*qw + iw*-qz + ix*-qy - iy*-qx);
    }
    inline T_Quat operator*(T s) const { return T_Quat(x * s, y * s, z * s, w * s); }
    inline T_Quat operator-() const { return T_Quat(-x, -y, -z, -w); }

    inline T_Vec3<T> toEuler(RotationOrder order) const {
        T sqw = w*w;
        T sqx = x*x;
        T sqy = y*y;
        T sqz = z*z;
        T unit = sqx + sqy + sqz + sqw;
        T test = x*y + z*w;
        T heading, attitude, bank;
        if (test > 0.499 * unit) {
            heading = 2 * atan2(x, w);
            attitude = acosf(w);
            bank = 0;
        } else if (test < -0.499 * unit) {
            heading = -2 * atan2(x, w);
            attitude = acosf(w);
            bank = 0;
        } else {
            heading = atan2(2*y*w-2*x*z, sqx - sqy - sqz + sqw);
            attitude = asinf(2*test/unit);
            bank = atan2(2*x*w-2*y*z, -sqx + sqy - sqz + sqw);
        }
        heading = fmod(heading, TWO_PI);
        attitude = fmod(attitude, TWO_PI);
        bank = fmod(bank, TWO_PI);
        if (order == RotationOrder::XYZ) return T_Vec3<T>(heading, attitude, bank);
        if (order == RotationOrder::YZX) return T_Vec3<T>(bank, heading, attitude);
        if (order == RotationOrder::ZXY) return T_Vec3<T>(attitude, bank, heading);
        if (order == RotationOrder::XZY) return T_Vec3<T>(bank, attitude, heading);
        if (order == RotationOrder::YXZ) return T_Vec3<T>(attitude, heading, bank);
        if (order == RotationOrder::ZYX) return T_Vec3<T>(heading, bank, attitude);
        return T_Vec3<T>(0, 0, 0);
    }

    inline void rotateVector(T_Vec3<T> &vec){
        T tx = 2.f * (y * vec.z - z * vec.y);
        T ty = 2.f * (z * vec.x - x * vec.z);
        T tz = 2.f * (x * vec.y - y * vec.x);

        vec.x = vec.x + w * tx + y * tz - z * ty;
        vec.y = vec.y + w * ty + z * tx - x * tz;
        vec.z = vec.z + w * tz + x * ty - y * tx;
    }

    static inline T_Quat slerp(T_Quat &qa, T_Quat &qb, const T &t) {

        T cosHalfTheta = qa.x*qb.x + qa.y*qb.y + qa.z*qb.z + qa.w*qb.w;

        cosHalfTheta = std::clamp(cosHalfTheta, T(-1), T(1));

        if (cosHalfTheta < 0) {
            qb = -qb;
            cosHalfTheta = -cosHalfTheta;
        }

        if (cosHalfTheta > T(0.9995)) {
            T_Quat result = qa + (qb - qa) * t;
            return result.normalize();
        }

        T halfTheta = acosf(cosHalfTheta);
        T sinHalfTheta = sqrt(T(1) - cosHalfTheta*cosHalfTheta);

        T ratioA = sinf((T(1) - t) * halfTheta) / sinHalfTheta;
        T ratioB = sinf(t * halfTheta) / sinHalfTheta;

        T_Quat result = T_Quat(
            qa.x * ratioA + qb.x * ratioB,
            qa.y * ratioA + qb.y * ratioB,
            qa.z * ratioA + qb.z * ratioB,
            qa.w * ratioA + qb.w * ratioB
            );

        return result.normalize();
    }
};

typedef T_Quat<float> Quat;
typedef T_Quat<double> dQuat;

template <class T>
const T_Quat<T> T_Quat<T>::IDENTITY(0, 0, 0, 1);

namespace PGK_Math
{
    template <class T>
    inline const T_Mat4<T> worldMatrix(const T_Vec3<T> &pos, const T_Quat<T> &rot, const T_Vec3<T> &scale){
        return T_Mat4<T>(scale.x * (1 - 2 * rot.y * rot.y - 2 * rot.z * rot.z), scale.x * (2 * rot.x * rot.y - 2 * rot.z * rot.w), scale.x * (2 * rot.x * rot.z + 2 * rot.y * rot.w), pos.x,
                         scale.y * (2 * rot.x * rot.y + 2 * rot.z * rot.w), scale.y * (1 - 2 * rot.x * rot.x - 2 * rot.z * rot.z), scale.y * (2 * rot.y * rot.z - 2 * rot.x * rot.w), pos.y,
                         scale.z * (2 * rot.x * rot.z - 2 * rot.y * rot.w), scale.z * (2 * rot.y * rot.z + 2 * rot.x * rot.w), scale.z * (1 - 2 * rot.x * rot.x - 2 * rot.y * rot.y), pos.z,
                         0, 0, 0, 1);
    }

    template <class T>
    inline const T_Mat4<T> viewportMatrix(const int &x, const int &y, const int &w, const int &h, const T &near, const T &far){
        return T_Mat4<T>(w/2, 0, 0, x + w/2,
                         0, h/2, 0, y + h/2,
                         0, 0, (far - near)/2, (far + near)/2,
                         0, 0, 0, 1);
    }

    template <class T>
    inline const T_Mat4<T> normalMatrix(const T_Mat4<T> &worldTransform){
        T_Mat4<T> rotMat = worldTransform;
        rotMat.m30 = 0;
        rotMat.m31 = 0;
        rotMat.m32 = 0;
        return rotMat.inverse().transpose();
    }

    template <class T>
    inline const T_Mat4<T> lookAtMatrix(const T_Vec3<T> &pos, const T_Vec3<T> &target, const T_Vec3<T> &up){
        T_Vec3<T> z = (pos - target).normalize();
        T_Vec3<T> x = up.cross(z).normalize();
        T_Vec3<T> y = z.cross(x);
        return T_Mat4<T>(x.x, x.y, x.z, -x.dot(pos),
                         y.x, y.y, y.z, -y.dot(pos),
                         z.x, z.y, z.z, -z.dot(pos),
                         0, 0, 0, 1);
    }

    cVec3 interpolatecVec3(const cVec3 &p00, const cVec3 &p10, const cVec3 &p01, const cVec3 &p11, const float &a, const float &b);
    inline Vec3 clipToNDC(const Vec4 &clip){ return Vec3(clip.x/clip.w,clip.y/clip.w,clip.z/clip.w); }
    inline Vec2 projectionToScreen(const Vec3 &ndc, const int &scW, const int &scH){ return Vec2( ((ndc.x + 1) * scW) / 2, ((-ndc.y + 1) * scH) / 2); }
    inline Vec3 projectionToScreen(const Vec3 &ndc, const int &scW, const int &scH, const float &near, const float &far){ return Vec3( ((ndc.x + 1) * scW) / 2, ((-ndc.y + 1) * scH) / 2, (ndc.z - near) / (far - near)); }
    inline float degToRad(const float &deg){ return deg * acosf(-1.f) / 180; }
    inline float edgeFunction(const Vec2 &a, const Vec2 &b, const Vec2 &c) { return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }
    inline float edgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c) { return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }
    Vec3 getBarycentric(const Vec2 &A, const Vec2 &B, const Vec2 &C, const Vec2 &P);
    Vec3 getBarycentric(const Vec3 &A, const Vec3 &B, const Vec3 &C, const Vec3 &P);
    inline float lerp(const float &a, const float &b, const float &t){ return a * (1 - t) + b * t; }
    inline Vec3 biLerp(const Vec3 &p00, const Vec3 &p10, const Vec3 &p01, const Vec3 &p11, const float &a, const float &b){ return p00 * (1 - a) * (1 - b) + p10 * a * (1 - b) + p01 * (1 - a) * b + p11 * a * b; }
    uint64_t fastpow(uint64_t base, uint64_t power);
    inline bool intersectTriangle(const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t)
    {
        const float EPSILON = 0.000001f;
        const Vec3 edge1 = v1 - v0;
        const Vec3 edge2 = v2 - v0;
        const Vec3 h = rayDir.cross(edge2);
        const float a = edge1.dot(h);

        if (a > -EPSILON && a < EPSILON) return false;

        const float f = 1.0f / a;
        const Vec3 s = rayOrigin - v0;
        const float u = f * s.dot(h);

        if (u < 0.0f || u > 1.0f) return false;

        const Vec3 q = s.cross(edge1);
        const float v = f * rayDir.dot(q);

        if (v < 0.0f || u + v > 1.0f) return false;

        t = f * edge2.dot(q);
        return t > EPSILON;
    }
    inline cVec3 cVec3FromQColor(const QColor &color){ return cVec3(color.red(),color.green(),color.blue()); }
    inline QColor QColorFromcVec3(const cVec3 &color){ return QColor(color.x,color.y,color.z); }
};

#endif // PGK_MATH_H
