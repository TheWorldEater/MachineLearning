#pragma once
#include "basic_typedefs.hpp"
#include "math.hpp"
#include "preprocessor_stuff.hpp" // for BOOL_XOR

namespace imgl {
namespace vector {
//

#if 0 // debug
	#define constexpr 
	#define ALWAYSINLINE inline
	#define INL inline
#else
	#define ALWAYSINLINE FORCEINLINE // to prevent tons of small functions in debug mode
	#define INL inline
#endif

union u8v2;
union u8v3;
union u8v4;
union s32v2;
union s32v3;
union s32v4;
union s64v2;
union s64v3;
union s64v4;

//
#define T	bool
#define V2	bv2
#define V3	bv3
#define V4	bv4
#define BOOLVEC	1
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4
#undef BOOLVEC

#define BOOLVEC	0
#define BV2	bv2
#define BV3	bv3
#define BV4	bv4

//
#define FLTVEC 1

#define T	f32
#define V2	fv2
#define V3	fv3
#define V4	fv4
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4

//
#define T	f64
#define V2	dv2
#define V3	dv3
#define V4	dv4
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4

#undef FLTVEC

//
#define INTVEC	1

#define T	s32
#define V2	s32v2
#define V3	s32v3
#define V4	s32v4
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4

#define T	s64
#define V2	s64v2
#define V3	s64v3
#define V4	s64v4
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4

#define T	u8
#define V2	u8v2
#define V3	u8v3
#define V4	u8v4
	
	#include "vector_tv2.hpp"
	#include "vector_tv3.hpp"
	#include "vector_tv4.hpp"
	
#undef T
#undef V2
#undef V3
#undef V4

#undef INTVEC

//
#define T	f32
#define V2	fv2
#define V3	fv3
#define V4	fv4
#define M2	fm2
#define M3	fm3
#define M4	fm4
#define HM	fhm

#undef BOOLVEC
#undef BV2
#undef BV3
#undef BV4

// length and normalization for "1 dimensional vectors" ie. scalars
INL f32 length (f32 x) {	return imgl::abs(x); }
INL s32 length (s32 x) {	return imgl::abs(x); }

INL f32 normalize (f32 x) {	return x / length(x); }
INL s32 normalize (s32 x) {	return x / length(x); }


INL fv2::operator u8v2() const {	return u8v2((u8)x, (u8)y); }
INL fv3::operator u8v3() const {	return u8v3((u8)x, (u8)y, (u8)z); }
INL fv4::operator u8v4() const {	return u8v4((u8)x, (u8)y, (u8)z, (u8)w); }

INL fv2::operator s32v2() const {	return s32v2((s32)x, (s32)y); }
INL fv3::operator s32v3() const {	return s32v3((s32)x, (s32)y, (s32)z); }
INL fv4::operator s32v4() const {	return s32v4((s32)x, (s32)y, (s32)z, (s32)w); }

INL fv2::operator s64v2() const {	return s64v2((s64)x, (s64)y); }
INL fv3::operator s64v3() const {	return s64v3((s64)x, (s64)y, (s64)z); }
INL fv4::operator s64v4() const {	return s64v4((s64)x, (s64)y, (s64)z, (s64)w); }

struct M2 {
	V2 arr[2];
	
	ALWAYSINLINE explicit M2 () {}
private: // don't allow a contructor with column mayor order because it could be confusing, use static functions row and column instead, still need a contructor though, to implement the functions below
	ALWAYSINLINE explicit constexpr M2 (V2 a, V2 b): arr{a,b} {}
public:
	
	static INL constexpr M2 row (		V2 a, V2 b ) {				return M2{V2(a.x,b.x),V2(b.y,b.y)}; }
	static INL constexpr M2 column (	V2 a, V2 b ) {				return M2{a,b}; }
	static INL constexpr M2 row (		T a, T b,
									T e, T f ) {				return M2{V2(a,e),V2(b,f)}; }
	static INL constexpr M2 ident () {								return row(1,0, 0,1); }
	static INL constexpr M2 nan () {								return row(QNAN,QNAN, QNAN,QNAN); }
	
	INL M2& operator*= (M2 r);
};
struct M3 {
	V3 arr[3];
	
	ALWAYSINLINE explicit M3 () {}
private: //
	ALWAYSINLINE explicit constexpr M3 (V3 a, V3 b, V3 c): arr{a,b,c} {}
public:
	
	static INL constexpr M3 row (		V3 a, V3 b, V3 c ) {		return M3{V3(a.x,b.x,c.x),V3(a.y,b.y,c.y),V3(a.z,b.z,c.z)}; }
	static INL constexpr M3 column (	V3 a, V3 b, V3 c ) {		return M3{a,b,c}; }
	static INL constexpr M3 row (		T a, T b, T c,
									T e, T f, T g,
									T i, T j, T k ) {			return M3{V3(a,e,i),V3(b,f,j),V3(c,g,k)}; }
	static INL constexpr M3 ident () {								return row(1,0,0, 0,1,0, 0,0,1); }
	static INL constexpr M3 nan () {								return row(QNAN,QNAN,QNAN, QNAN,QNAN,QNAN, QNAN,QNAN,QNAN); }
	INL constexpr M3 (M2 m): arr{V3(m.arr[0], 0), V3(m.arr[1], 0), V3(0,0,1)} {}
	
	INL M2 m2 () const {											return M2::column( arr[0].xy(), arr[1].xy() ); }
	
	INL M3& operator*= (M3 r);
};
struct M4 {
	V4 arr[4];
	
	ALWAYSINLINE explicit M4 () {}
private: //
	ALWAYSINLINE explicit constexpr M4 (V4 a, V4 b, V4 c, V4 d): arr{a,b,c,d} {}
public:

	static INL constexpr M4 row (		V4 a, V4 b, V4 c, V4 d ) {	return M4{V4(a.x,b.x,c.x,d.x),V4(a.y,b.y,c.y,d.y),V4(a.z,b.z,c.z,d.z),V4(a.w,b.w,c.w,d.w)}; }
	static INL constexpr M4 column (	V4 a, V4 b, V4 c, V4 d ) {	return M4{a,b,c,d}; }
	static INL constexpr M4 row (		T a, T b, T c, T d,
									T e, T f, T g, T h,
									T i, T j, T k, T l,
									T m, T n, T o, T p ) {		return M4{V4(a,e,i,m),V4(b,f,j,n),V4(c,g,k,o),V4(d,h,l,p)}; }
	static INL constexpr M4 ident () {								return row(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1); }
	static INL constexpr M4 nan () {								return row(QNAN,QNAN,QNAN,QNAN, QNAN,QNAN,QNAN,QNAN, QNAN,QNAN,QNAN,QNAN, QNAN,QNAN,QNAN,QNAN); }
	INL constexpr M4 (M2 m): arr{V4(m.arr[0], 0,0), V4(m.arr[1], 0,0), V4(0,0,1,0), V4(0,0,0,1)} {}
	INL constexpr M4 (M3 m): arr{V4(m.arr[0], 0), V4(m.arr[1], 0), V4(m.arr[2], 0), V4(0,0,0,1)} {}
	
	INL M2 m2 () const {											return M2::column( arr[0].xy(), arr[1].xy() ); }
	INL M3 m3 () const {											return M3::column( arr[0].xyz(), arr[1].xyz(), arr[2].xyz() ); }
	
	INL M4& operator*= (M4 r);
};
struct HM { // last row implicit 0,0,0,1
	V3 arr[4];
	
	ALWAYSINLINE explicit HM () {}
private: //
	ALWAYSINLINE explicit constexpr HM (V3 a, V3 b, V3 c, V3 d): arr{a,b,c,d} {}
public:

	static INL constexpr HM row (		V4 a, V4 b, V4 c ) {		return HM{V3(a.x,b.x,c.x),V3(a.y,b.y,c.y),V3(a.z,b.z,c.z),V3(a.w,b.w,c.w)}; }
	static INL constexpr HM column (	V3 a, V3 b, V3 c, V3 d ) {	return HM{a,b,c,d}; }
	static INL constexpr HM row (		T a, T b, T c, T d,
									T e, T f, T g, T h,
									T i, T j, T k, T l ) {		return HM{V3(a,e,i),V3(b,f,j),V3(c,g,k),V3(d,h,l)}; }
	static INL constexpr HM ident () {								return row(1,0,0,0, 0,1,0,0, 0,0,1,0); }
	static INL constexpr HM nan () {								return row(QNAN,QNAN,QNAN,QNAN, QNAN,QNAN,QNAN,QNAN, QNAN,QNAN,QNAN,QNAN); }
	INL constexpr HM (M2 m): arr{V3(m.arr[0], 0), V3(m.arr[1], 0), V3(0,0,1), V3(0,0,0)} {}
	INL constexpr HM (M3 m): arr{m.arr[0], m.arr[1], m.arr[2], V3(0,0,0)} {}
	
	INL M2 m2 () const {											return M2::column( arr[0].xy(), arr[1].xy() ); }
	INL M3 m3 () const {											return M3::column( arr[0], arr[1], arr[2] ); }
	INL M4 m4 () const {											return M4::column( V4(arr[0],0), V4(arr[1],0), V4(arr[2],0), V4(arr[3],1) ); }
	
	INL HM& operator*= (HM r);
};

INL V2 operator* (M2 m, V2 v) {
	V2 ret;
	ret.x = m.arr[0].x * v.x  +m.arr[1].x * v.y;
	ret.y = m.arr[0].y * v.x  +m.arr[1].y * v.y;
	return ret;
}
INL M2 operator* (M2 l, M2 r) {
	M2 ret;
	ret.arr[0] = l * r.arr[0];
	ret.arr[1] = l * r.arr[1];
	return ret;
}

INL V3 operator* (M3 m, V3 v) {
	V3 ret;
	ret.x = m.arr[0].x * v.x  +m.arr[1].x * v.y  +m.arr[2].x * v.z;
	ret.y = m.arr[0].y * v.x  +m.arr[1].y * v.y  +m.arr[2].y * v.z;
	ret.z = m.arr[0].z * v.x  +m.arr[1].z * v.y  +m.arr[2].z * v.z;
	return ret;
}
INL M3 operator* (M3 l, M3 r) {
	M3 ret;
	ret.arr[0] = l * r.arr[0];
	ret.arr[1] = l * r.arr[1];
	ret.arr[2] = l * r.arr[2];
	return ret;
}

INL V4 operator* (M4 m, V4 v) {
	V4 ret;
	ret.x = m.arr[0].x * v.x  +m.arr[1].x * v.y  +m.arr[2].x * v.z  +m.arr[3].x * v.w;
	ret.y = m.arr[0].y * v.x  +m.arr[1].y * v.y  +m.arr[2].y * v.z  +m.arr[3].y * v.w;
	ret.z = m.arr[0].z * v.x  +m.arr[1].z * v.y  +m.arr[2].z * v.z  +m.arr[3].z * v.w;
	ret.w = m.arr[0].w * v.x  +m.arr[1].w * v.y  +m.arr[2].w * v.z  +m.arr[3].w * v.w;
	return ret;
}
INL M4 operator* (M4 l, M4 r) {
	M4 ret;
	ret.arr[0] = l * r.arr[0];
	ret.arr[1] = l * r.arr[1];
	ret.arr[2] = l * r.arr[2];
	ret.arr[3] = l * r.arr[3];
	return ret;
}

INL V3 operator* (HM m, V3 v) { // the common case of wanting to translate/rotate/scale some v3 -> if you just want to rotate/scale instead of doing this "M4*v4(v3,0)" -> just do "M4.m3() * V3"
	// implicit v.w = 1
	V3 ret;
	ret.x = m.arr[0].x * v.x  +m.arr[1].x * v.y  +m.arr[2].x * v.z  +m.arr[3].x;
	ret.y = m.arr[0].y * v.x  +m.arr[1].y * v.y  +m.arr[2].y * v.z  +m.arr[3].y;
	ret.z = m.arr[0].z * v.x  +m.arr[1].z * v.y  +m.arr[2].z * v.z  +m.arr[3].z;
	return ret;
}
INL HM operator* (HM l, HM r) {
	HM ret;
	#if 0
	ret.arr[0] = l.m3() * r.arr[0];	// implicit r.arr[0].w = 0
	ret.arr[1] = l.m3() * r.arr[1];	// implicit r.arr[1].w = 0
	ret.arr[2] = l.m3() * r.arr[2];	// implicit r.arr[2].w = 0
	ret.arr[3] = l * r.arr[3];		// implicit r.arr[3].w = 1
	#else
	ret.arr[0].x = l.arr[0].x * r.arr[0].x  +l.arr[1].x * r.arr[0].y  +l.arr[2].x * r.arr[0].z;
	ret.arr[0].y = l.arr[0].y * r.arr[0].x  +l.arr[1].y * r.arr[0].y  +l.arr[2].y * r.arr[0].z;
	ret.arr[0].z = l.arr[0].z * r.arr[0].x  +l.arr[1].z * r.arr[0].y  +l.arr[2].z * r.arr[0].z;
	
	ret.arr[1].x = l.arr[0].x * r.arr[1].x  +l.arr[1].x * r.arr[1].y  +l.arr[2].x * r.arr[1].z;
	ret.arr[1].y = l.arr[0].y * r.arr[1].x  +l.arr[1].y * r.arr[1].y  +l.arr[2].y * r.arr[1].z;
	ret.arr[1].z = l.arr[0].z * r.arr[1].x  +l.arr[1].z * r.arr[1].y  +l.arr[2].z * r.arr[1].z;
	
	ret.arr[2].x = l.arr[0].x * r.arr[2].x  +l.arr[1].x * r.arr[2].y  +l.arr[2].x * r.arr[2].z;
	ret.arr[2].y = l.arr[0].y * r.arr[2].x  +l.arr[1].y * r.arr[2].y  +l.arr[2].y * r.arr[2].z;
	ret.arr[2].z = l.arr[0].z * r.arr[2].x  +l.arr[1].z * r.arr[2].y  +l.arr[2].z * r.arr[2].z;
	
	ret.arr[3].x = l.arr[0].x * r.arr[3].x  +l.arr[1].x * r.arr[3].y  +l.arr[2].x * r.arr[3].z  +l.arr[3].x;
	ret.arr[3].y = l.arr[0].y * r.arr[3].x  +l.arr[1].y * r.arr[3].y  +l.arr[2].y * r.arr[3].z  +l.arr[3].y;
	ret.arr[3].z = l.arr[0].z * r.arr[3].x  +l.arr[1].z * r.arr[3].y  +l.arr[2].z * r.arr[3].z  +l.arr[3].z;
	#endif
	return ret;
}

INL M2& M2::operator*= (M2 r) {
	return *this = *this * r;
}
INL M3& M3::operator*= (M3 r) {
	return *this = *this * r;
}
INL M4& M4::operator*= (M4 r) {
	return *this = *this * r;
}
INL HM& HM::operator*= (HM r) {
	return *this = *this * r;
}

INL M2 inverse (M2 m) {
	T inv_det = T(1) / ( (m.arr[0].x * m.arr[1].y) -(m.arr[1].x * m.arr[0].y) );
	
	M2 ret;
	ret.arr[0].x = m.arr[1].y * +inv_det;
	ret.arr[0].y = m.arr[0].y * -inv_det;
	ret.arr[1].x = m.arr[1].x * -inv_det;
	ret.arr[1].y = m.arr[0].x * +inv_det;
	return ret;
}

#if 0
template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat2x2<T, P> compute_inverse(tmat2x2<T, P> const & m)
{
	T OneOverDeterminant = static_cast<T>(1) / (
		+ m[0][0] * m[1][1]
		- m[1][0] * m[0][1]);

	tmat2x2<T, P> Inverse(
		+ m[1][1] * OneOverDeterminant,
		- m[0][1] * OneOverDeterminant,
		- m[1][0] * OneOverDeterminant,
		+ m[0][0] * OneOverDeterminant);

	return Inverse;
}
template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x3<T, P> compute_inverse(tmat3x3<T, P> const & m)
{
	T OneOverDeterminant = static_cast<T>(1) / (
		+ m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	tmat3x3<T, P> Inverse(uninitialize);
	Inverse[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
	Inverse[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
	Inverse[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
	Inverse[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
	Inverse[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

	return Inverse;
}
template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T, P> compute_inverse(tmat4x4<T, P> const & m)
{
	T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	tvec4<T, P> Fac0(Coef00, Coef00, Coef02, Coef03);
	tvec4<T, P> Fac1(Coef04, Coef04, Coef06, Coef07);
	tvec4<T, P> Fac2(Coef08, Coef08, Coef10, Coef11);
	tvec4<T, P> Fac3(Coef12, Coef12, Coef14, Coef15);
	tvec4<T, P> Fac4(Coef16, Coef16, Coef18, Coef19);
	tvec4<T, P> Fac5(Coef20, Coef20, Coef22, Coef23);

	tvec4<T, P> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
	tvec4<T, P> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
	tvec4<T, P> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
	tvec4<T, P> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

	tvec4<T, P> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	tvec4<T, P> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	tvec4<T, P> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	tvec4<T, P> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	tvec4<T, P> SignA(+1, -1, +1, -1);
	tvec4<T, P> SignB(-1, +1, -1, +1);
	tmat4x4<T, P> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	tvec4<T, P> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	tvec4<T, P> Dot0(m[0] * Row0);
	T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	T OneOverDeterminant = static_cast<T>(1) / Dot1;

	return Inverse * OneOverDeterminant;
}
#endif

INL M2 scale2 (V2 v) {
	return M2::column(	V2(v.x,0),
						V2(0,v.y) );
}
INL M2 rotate2 (T ang) {
	auto sc = sin_cos(ang);
	return M2::row(	+sc.c,	-sc.s,
					+sc.s,	+sc.c );
}

INL fv2 rotate2_90 (fv2 v) {
	return V2(-v.y, v.x);
}
INL fv2 rotate2_180 (fv2 v) {
	return -v;
}
INL fv2 rotate2_270 (fv2 v) {
	return V2(v.y, -v.x);
}

INL M3 scale3 (V3 v) {
	return M3::column(	V3(v.x,0,0),
						V3(0,v.y,0),
						V3(0,0,v.z) );
}
INL M3 rotate3_X (T ang) {
	auto sc = sin_cos(ang);
	return M3::row(	1,		0,		0,
					0,		+sc.c,	-sc.s,
					0,		+sc.s,	+sc.c);
}
INL M3 rotate3_Y (T ang) {
	auto sc = sin_cos(ang);
	return M3::row(	+sc.c,	0,		+sc.s,
					0,		1,		0,
					-sc.s,	0,		+sc.c);
}
INL M3 rotate3_Z (T ang) {
	auto sc = sin_cos(ang);
	return M3::row(	+sc.c,	-sc.s,	0,
					+sc.s,	+sc.c,	0,
					0,		0,		1);
}

INL M4 translate4 (V3 v) {
	return M4::column(	V4(1,0,0,0),
						V4(0,1,0,0),
						V4(0,0,1,0),
						V4(v,1) );
}
INL M4 scale4 (V3 v) {
	return M4::column(	V4(v.x,0,0,0),
						V4(0,v.y,0,0),
						V4(0,0,v.z,0),
						V4(0,0,0,1) );
}
INL M4 rotate4_X (T ang) {
	auto sc = sin_cos(ang);
	return M4::row(	1,		0,		0,		0,
					0,		+sc.c,	-sc.s,	0,
					0,		+sc.s,	+sc.c,	0,
					0,		0,		0,		1 );
}
INL M4 rotate4_Y (T ang) {
	auto sc = sin_cos(ang);
	return M4::row(	+sc.c,	0,		+sc.s,	0,
					0,		1,		0,		0,
					-sc.s,	0,		+sc.c,	0,
					0,		0,		0,		1 );
}
INL M4 rotate4_Z (T ang) {
	auto sc = sin_cos(ang);
	return M4::row(	+sc.c,	-sc.s,	0,		0,
					+sc.s,	+sc.c,	0,		0,
					0,		0,		1,		0,
					0,		0,		0,		1 );
}

INL HM translateH (V3 v) {
	return HM::column(	V3(1,0,0),
						V3(0,1,0),
						V3(0,0,1),
						v );
}
INL HM scaleH (V3 v) {
	return HM::column(	V3(v.x,0,0),
						V3(0,v.y,0),
						V3(0,0,v.z),
						V3(0,0,0) );
}
INL HM rotateH_X (T ang) {
	auto sc = sin_cos(ang);
	return HM::row(	1,		0,		0,		0,
					0,		+sc.c,	-sc.s,	0,
					0,		+sc.s,	+sc.c,	0 );
}
INL HM rotateH_Y (T ang) {
	auto sc = sin_cos(ang);
	return HM::row(	+sc.c,	0,		+sc.s,	0,
					0,		1,		0,		0,
					-sc.s,	0,		+sc.c,	0 );
}
INL HM rotateH_Z (T ang) {
	auto sc = sin_cos(ang);
	return HM::row(	+sc.c,	-sc.s,	0,		0,
					+sc.s,	+sc.c,	0,		0,
					0,		0,		1,		0 );
}

INL HM transl_rot_scale (V3 t, M3 r, V3 s) {
	return translateH(t) * HM(r);// * scaleH(s);
}

#undef T
#undef V2
#undef V3
#undef V4
#undef M2
#undef M3
#undef M4

#undef constexpr 
#undef ALWAYSINLINE
#undef INL

//
}
using namespace vector;
}