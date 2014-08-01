/*
 * sw/lalg_math.hpp
 *
 * belongs to me(Arvydas Bataitis)
 *
 * hands of!
 *
 * 2014-07
 *
 */
#ifndef _SW_LALG_MATH_HPP_
#define _SW_LALG_MATH_HPP_

#include <cmath>
#include <cstdint>
#include <climits>

/* 
 *		Library usses these definitions instead of actual functions
 *		in case one would like to use its own functions.
 */
#define _MATH_SIN(X) std::sin(X)
#define _MATH_COS(X) std::cos(X)
#define _MATH_SQRT(X) std::sqrt(X)
#define _MATH_ATAN2(Y,X) std::atan2((Y),(X))
#define _MATH_ABS(X) std::abs(X)

#define _SW_LALG_MATH_INLINE_


template < typename type1, typename type2 = type1> struct _sw_if_integral { };

template < typename type2 > struct _sw_if_integral < int8_t,   type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < int16_t,  type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < int32_t,  type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < int64_t,  type2 > { typedef type2 enable; };

template < typename type2 > struct _sw_if_integral < uint8_t,  type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < uint16_t, type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < uint32_t, type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_integral < uint64_t, type2 > { typedef type2 enable; };

template < typename type1, typename type2 = type1 > struct _sw_if_floating { };

template < typename type2 > struct _sw_if_floating < float,  type2 > { typedef type2 enable; };
template < typename type2 > struct _sw_if_floating < double, type2 > { typedef type2 enable; };

/*	class Vector :
 *
 *		Default class template, empty because every reasonable combination of type and size will be specialized.
 *		Specialized versions of Vector may act as other similar structures.	For example Vector<float,4> also has
 *		a functionality of Quaternion, while Vector<2,float> has member variables [x,y], [u,v] and [w,h] which makes
 *		code more readable when vector is used as a vector, uv coordinate or a width and height variable.
 */
template < uint32_t size, typename type > class Vector { };

/*	class Vector<2,type> :
 *
 *		Specialized class template for all types having 2 elements per vector.
 */
template < typename type > class Vector < 2, type > {

	public :

/* constructors */

	_SW_LALG_MATH_INLINE_ Vector ( ) { };

	_SW_LALG_MATH_INLINE_ explicit Vector ( type const & ab ) : x(ab), y(ab) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<2,type> const & ab ) : x(ab.x), y(ab.y) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<3,type> const & ab ) : x(ab.x), y(ab.y) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<4,type> const & ab ) : x(ab.x), y(ab.y) { };

	_SW_LALG_MATH_INLINE_ Vector ( type const & a, type const & b ) : x(a), y(b) { };

/* assignment operators */

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator = ( Vector<2,type> const other ) { (this)->x = other.x; (this)->y = other.y; return (*this); };

/* arithmetics operators */

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator + ( type const other ) const { return Vector<2,type>((this)->x + other, (this)->y + other); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator - ( type const other ) const { return Vector<2,type>((this)->x - other, (this)->y - other); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator * ( type const other ) const { return Vector<2,type>((this)->x * other, (this)->y * other); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator / ( type const other ) const { return Vector<2,type>((this)->x / other, (this)->y / other); };


	_SW_LALG_MATH_INLINE_ Vector<2,type> operator + ( Vector<2,type> const other ) const { return Vector<2,type>((this)->x + other.x, (this)->y + other.y); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator - ( Vector<2,type> const other ) const { return Vector<2,type>((this)->x - other.x, (this)->y - other.y); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator * ( Vector<2,type> const other ) const { return Vector<2,type>((this)->x * other.x, (this)->y * other.y); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> operator / ( Vector<2,type> const other ) const { return Vector<2,type>((this)->x / other.x, (this)->y / other.y); };


	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator += ( type const other ) { (this)->x += other; (this)->y += other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator -= ( type const other ) { (this)->x -= other; (this)->y -= other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator *= ( type const other ) { (this)->x *= other; (this)->y *= other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator /= ( type const other ) { (this)->x /= other; (this)->y /= other; return (*this); };


	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator += ( Vector<2,type> const other ) { (this)->x += other.x; (this)->y += other.y; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator -= ( Vector<2,type> const other ) { (this)->x -= other.x; (this)->y -= other.y; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator *= ( Vector<2,type> const other ) { (this)->x *= other.x; (this)->y *= other.y; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<2,type> & operator /= ( Vector<2,type> const other ) { (this)->x /= other.x; (this)->y /= other.y; return (*this); };

/* comparison operators */

	_SW_LALG_MATH_INLINE_ bool operator >  ( type const other ) const { return ((this)->x >  other) && ((this)->y >  other); };

	_SW_LALG_MATH_INLINE_ bool operator <  ( type const other ) const { return ((this)->x <  other) && ((this)->y <  other); };

	_SW_LALG_MATH_INLINE_ bool operator >= ( type const other ) const { return ((this)->x >= other) && ((this)->y >= other); };

	_SW_LALG_MATH_INLINE_ bool operator <= ( type const other ) const { return ((this)->x <= other) && ((this)->y <= other); };

	_SW_LALG_MATH_INLINE_ bool operator == ( type const other ) const { return ((this)->x == other) && ((this)->y == other); };

	_SW_LALG_MATH_INLINE_ bool operator != ( type const other ) const { return ((this)->x != other) && ((this)->y != other); };


	_SW_LALG_MATH_INLINE_ bool operator >  ( Vector<2,type> const other ) const { return ((this)->x >  other.x) && ((this)->y >  other.y); };

	_SW_LALG_MATH_INLINE_ bool operator <  ( Vector<2,type> const other ) const { return ((this)->x <  other.x) && ((this)->y <  other.y); };

	_SW_LALG_MATH_INLINE_ bool operator >= ( Vector<2,type> const other ) const { return ((this)->x >= other.x) && ((this)->y >= other.y); };

	_SW_LALG_MATH_INLINE_ bool operator <= ( Vector<2,type> const other ) const { return ((this)->x <= other.x) && ((this)->y <= other.y); };

	_SW_LALG_MATH_INLINE_ bool operator == ( Vector<2,type> const other ) const { return ((this)->x == other.x) && ((this)->y == other.y); };

	_SW_LALG_MATH_INLINE_ bool operator != ( Vector<2,type> const other ) const { return ((this)->x != other.x) && ((this)->y != other.y); };

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,bool>::enable equals ( Vector<2,type> const other, type const & error ) const {

		return (_MATH_ABS((this)->x - other.x) < error) && (_MATH_ABS((this)->y - other.y) < error);
	};

	/* get length of this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type>::enable length ( ) const { return _MATH_SQRT((this)->length2()); };

	/* get length^2 of this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type>::enable length2 ( ) const { return (this)->x * (this)->x + (this)->y * (this)->y; };

	/* normalize this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable normalize ( ) {

		type const length = (this)->length();

		(this)->x = (this)->x / length;

		(this)->y = (this)->y / length;
	};

	/* get normalized vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable normalized ( ) const {

		type const length = (this)->length();

		return Vector<2,type>((this)->x / length, (this)->y / length);
	};

	/* get dot product */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable dot_product ( Vector<2,type> const other ) const { 

		return (this)->x * other.x + (this)->y * other.y; 
	};

	/* get perpendicular dot product */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable perp_dot_product ( Vector<2,type> const other ) const	{ 

		return -(this)->y * other.x + (this)->x * other.y; 
	};

	/* translate this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_x ( type const other ) { 

		(this)->x = (this)->x + other; 
	};

	/* translate this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_x ( Vector<2,type> const other ) { 

		(this)->x = (this)->x + other.x; 
	};

	/* get translated vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable translated_x ( type const other ) const { 

		return Vector<2,type>((this)->x + other, (this)->y); 
	};

	/* get translated vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable translated_x ( Vector<2,type> const other ) const { 

		return Vector<2,type>((this)->x + other.x, (this)->y); 
	};

	/* translate this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_y ( type const other ) { 

		(this)->y = (this)->y + other; 
	};

	/* translate this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_y ( Vector<2,type> const other ) { 

		(this)->y = (this)->y + other.y; 
	};

	/* get translated vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable translated_y ( type const other ) const { 

		return Vector<2,type>((this)->x, (this)->y + other); 
	};

	/* get translated vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable translated_y ( Vector<2,type> const other ) const { 

		return Vector<2,type>((this)->x, (this)->y + other.y); 
	};

	/* scale this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_x ( type const other ) { 

		(this)->x = (this)->x * other; 
	};

	/* scale this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_x ( Vector<2,type> const other ) { 

		(this)->x = (this)->x * other.x; 
	};

	/* get scaled vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable scaled_x ( type const other ) const { 

		return Vector<2,type>((this)->x * other, (this)->y); 
	};

	/* get scaled vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable scaled_x ( Vector<2,type> const other ) const { 

		return Vector<2,type>((this)->x * other.x, (this)->y); 
	};

	/* scale this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_y ( type const other ) { 

		(this)->y = (this)->y * other; 
	};

	/* scale this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_y ( Vector<2,type> const other ) { 

		(this)->y = (this)->y * other.y; 
	};

	/* get scaled vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable scaled_y ( type const other ) const { 

		return Vector<2,type>((this)->x, (this)->y * other); 
	};

	/* get scaled vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable scaled_y ( Vector<2,type> const other ) const { 

		return Vector<2,type>((this)->x, (this)->y * other.y); 
	};

	/* rotate this vector on z axis around [0x,0y] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate ( type const angle ) {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		type const rotx = (this)->x * _cos - (this)->y * _sin;

		(this)->y		 = (this)->x * _sin + (this)->y * _cos;

		(this)->x = rotx;
	};

	/* get rotated vector on z axis around [0x,0y] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable rotated ( type const angle ) const {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		return Vector<2,type>((this)->x * _cos - (this)->y * _sin, (this)->x * _sin + (this)->y * _cos);
	};

	/* rotate vector on z axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate ( Vector<2,type> const other, type const angle ) {

		Vector<2,type> const dif = (this)->operator-(other);

		(this)->operator=(other + dif.rotated(angle));
	};

	/* get rotated vector on z axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable rotated ( Vector<2,type> const other, type const angle ) const {

		Vector<2,type> const dif = (this)->operator-(other);

		return other + dif.rotated(angle);
	};

	/* get angle between two vectors in the range from -PI to PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable angle_pi ( Vector<2,type> const other ) const {

		type const angle = (this)->angle(other);

		return (angle < 0) ? angle + 6.28318530718f : angle;
	};

	/* get angle between two vectors in the range from 0 to 2PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable angle ( Vector<2,type> const other ) const {

		return std::atan2((this)->perp_dot_product(other), (this)->dot_product(other));
	};

	/* project this vector on to another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable project ( Vector<2,type> const other ) {

		(*this) = other * (this)->dot_product(other) / other.length2();
	};

	/* get this vector projected on to another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<2,type>>::enable projected ( Vector<2,type> const other ) const {

		return other * ((this)->dot_product(other) / other.length2());
	};

	/* vector axis */

	union {

		struct {

			type x, y;
		};

		struct {

			type u, v;
		};

		struct {

			type w, h;
		};
	};
};

/*	class Vector<3,type> :
 *
 *		Specialized class template for all types having 3 elements per vector
 */
template < typename type > class Vector < 3, type > {

	public :

/* contructors */

	_SW_LALG_MATH_INLINE_ Vector ( ) { };

	_SW_LALG_MATH_INLINE_ explicit Vector ( type const & abc ) : x(abc), y(abc), z(abc) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<2,type> const & ab, type const & c ) : x(ab.x), y(ab.y), z(c) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<3,type> const & abc ) : x(abc.x), y(abc.y), z(abc.z) { };

	_SW_LALG_MATH_INLINE_ Vector ( Vector<4,type> const & abc ) : x(abc.x), y(abc.y), z(abc.z) { };

	_SW_LALG_MATH_INLINE_ Vector ( type const & a, type const & b, type const & c ) : x(a), y(b), z(c) { };

/* assignment operators */

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator = ( Vector<3,type> const other ) { (this)->x = other.x; (this)->y = other.y; (this)->z - other.z return (*this); };

/* arithmetics operators */

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator + (type const other ) const { return Vector<3,type>((this)->x + other, (this)->y + other, (this)->z + other); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator - (type const other ) const { return Vector<3,type>((this)->x - other, (this)->y - other, (this)->z - other); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator * (type const other ) const { return Vector<3,type>((this)->x * other, (this)->y * other, (this)->z * other); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator / (type const other ) const { return Vector<3,type>((this)->x / other, (this)->y / other, (this)->z / other); };


	_SW_LALG_MATH_INLINE_ Vector<3,type> operator + (Vector<3,type> const other ) const { return Vector<3,type>((this)->x + other.x, (this)->y + other.y, (this)->z + other.z); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator - (Vector<3,type> const other ) const { return Vector<3,type>((this)->x - other.x, (this)->y - other.y, (this)->z - other.z); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator * (Vector<3,type> const other ) const { return Vector<3,type>((this)->x * other.x, (this)->y * other.y, (this)->z * other.z); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> operator / (Vector<3,type> const other ) const { return Vector<3,type>((this)->x / other.x, (this)->y / other.y, (this)->z / other.z); };


	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator += (type const other ) { (this)->x += other; (this)->y += other; (this)->z += other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator -= (type const other ) { (this)->x -= other; (this)->y -= other; (this)->z -= other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator *= (type const other ) { (this)->x *= other; (this)->y *= other; (this)->z *= other; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator /= (type const other ) { (this)->x /= other; (this)->y /= other; (this)->z /= other; return (*this); };


	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator += (Vector<3,type> const other ) { (this)->x += other.x; (this)->y += other.y; (this)->z += other.z; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator -= (Vector<3,type> const other ) { (this)->x -= other.x; (this)->y -= other.y; (this)->z -= other.z; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator *= (Vector<3,type> const other ) { (this)->x *= other.x; (this)->y *= other.y; (this)->z *= other.z; return (*this); };

	_SW_LALG_MATH_INLINE_ Vector<3,type> & operator /= (Vector<3,type> const other ) { (this)->x /= other.x; (this)->y /= other.y; (this)->z /= other.z; return (*this); };

/* comparison operators */

	_SW_LALG_MATH_INLINE_ bool operator >  (type const other ) const { return ((this)->x >  other) && ((this)->y >  other) && ((this)->z >  other); };

	_SW_LALG_MATH_INLINE_ bool operator <  (type const other ) const { return ((this)->x <  other) && ((this)->y <  other) && ((this)->z <  other); };

	_SW_LALG_MATH_INLINE_ bool operator >= (type const other ) const { return ((this)->x >= other) && ((this)->y >= other) && ((this)->z >= other); };

	_SW_LALG_MATH_INLINE_ bool operator <= (type const other ) const { return ((this)->x <= other) && ((this)->y <= other) && ((this)->z <= other); };

	_SW_LALG_MATH_INLINE_ bool operator == (type const other ) const { return ((this)->x == other) && ((this)->y == other) && ((this)->z == other); };

	_SW_LALG_MATH_INLINE_ bool operator != (type const other ) const { return ((this)->x != other) && ((this)->y != other) && ((this)->z != other); };


	_SW_LALG_MATH_INLINE_ bool operator >  (Vector<3,type> const other ) const { return ((this)->x >  other.x) && ((this)->y >  other.y) && ((this)->z >  other.z); };

	_SW_LALG_MATH_INLINE_ bool operator <  (Vector<3,type> const other ) const { return ((this)->x <  other.x) && ((this)->y <  other.y) && ((this)->z <  other.z); };

	_SW_LALG_MATH_INLINE_ bool operator >= (Vector<3,type> const other ) const { return ((this)->x >= other.x) && ((this)->y >= other.y) && ((this)->z >= other.z); };

	_SW_LALG_MATH_INLINE_ bool operator <= (Vector<3,type> const other ) const { return ((this)->x <= other.x) && ((this)->y <= other.y) && ((this)->z <= other.z); };

	_SW_LALG_MATH_INLINE_ bool operator == (Vector<3,type> const other ) const { return ((this)->x == other.x) && ((this)->y == other.y) && ((this)->z == other.z); };

	_SW_LALG_MATH_INLINE_ bool operator != (Vector<3,type> const other ) const { return ((this)->x != other.x) && ((this)->y != other.y) && ((this)->z != other.z); };

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,bool>::enable equals ( Vector<3,type> const other, type const & error ) const {

		return (_MATH_ABS((this)->x - other.x) < error) && (_MATH_ABS((this)->y - other.y) < error) && (_MATH_ABS((this)->z - other.z) < error); 
	};

	/* get length of this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable length ( ) const { return _MATH_SQRT((this)->length2()); };

	/* get length^2 of this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable length2 ( ) const { return (this)->x * (this)->x + (this)->y * (this)->y + (this)->z * (this)->z; };

	/* normalize this vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable normalize ( ) {

		type const length = (this)->length();

		(this)->x = (this)->x / length;

		(this)->y = (this)->y / length;

		(this)->z = (this)->z / length;
	};

	/* get normalized vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable normalized ( ) const {

		type const length = (this)->length();

		return Vector<3,type>((this)->x / length, (this)->y / length, (this)->z / length);
	};

	/* get dot product */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable dot_product ( Vector<3,type> const other ) const { 

		return (this)->x * other.x + (this)->y * other.y + (this)->z * other.z;
	};

	/* get cross product */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable cross_product ( Vector<3,type> const other ) const {

		type cross_x = ((this)->y * other.z) - ((this)->z * other.y);

		type cross_y = ((this)->z * other.x) - ((this)->x * other.z);

		type cross_z = ((this)->x * other.y) - ((this)->y * other.x);

		return Vector<3,type>(cross_x,cross_y,cross_z);
	};

	/* translate this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_x ( type const other ) { 

		(this)->x = (this)->x + other; 
	};

	/* translate this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_x ( Vector<2,type> const other ) { 

		(this)->x = (this)->x + other.x; 
	};

	/* get translated vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_x ( type const other ) const { 

		return Vector<3,type>((this)->x + other, (this)->y, (this)->z); 
	};

	/* get translated vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_x ( Vector<2,type> const other ) const { 

		return Vector<3,type>((this)->x + other.x, (this)->y, (this)->z);  
	};

	/* translate this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_y ( type const other ) { 

		(this)->y = (this)->y + other; 
	};

	/* translate this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_y ( Vector<3,type> const other ) { 

		(this)->y = (this)->y + other.y; 
	};

	/* get translated vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_y ( type const other ) const { 

		return Vector<3,type>((this)->x, (this)->y + other, (this)->z); 
	};

	/* get translated vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_y ( Vector<3,type> const other ) const { 

		return Vector<3,type>((this)->x, (this)->y + other.y, (this)->z); 
	};

	/* translate this vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_z ( type const other ) { 

		(this)->z = (this)->z + other;
	};

	/* translate this vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable translate_z ( Vector<3,type> const other ) { 

		(this)->z = (this)->z + other.z;
	};

	/* get translated vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_z ( type const other ) const { 

		return Vector<3,type>((this)->x, (this)->y, (this)->z + other); 
	};

	/* get translated vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable translated_z ( Vector<3,type> const other ) const { 

		return Vector<3,type>((this)->x, (this)->y, (this)->z + other.z); 
	};

	/* scale this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_x ( type const other ) { 

		(this)->x = (this)->x * other; 
	};

	/* scale this vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_x ( Vector<3,type> const other ) { 

		(this)->x = (this)->x * other.x; 
	};

	/* get scaled vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_x ( type const other ) const { 

		return Vector<3,type>((this)->x * other, (this)->y, (this)->z); 
	};

	/* get scaled vector on x axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_x ( Vector<3,type> const other ) const { 

		return Vector<3,type>((this)->x * other.x, (this)->y, (this)->z); 
	};

	/* scale this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_y ( type const other ) { 

		(this)->y = (this)->y * other; 
	};

	/* scale this vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_y ( Vector<3,type> const other ) { 

		(this)->y = (this)->y * other.y; 
	};

	/* get scaled vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_y ( type const other ) const { 

		return Vector<3,type>((this)->x, (this)->y * other, (this)->z); 
	};

	/* get scaled vector on y axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_y ( Vector<3,type> const other ) const { 

		return Vector<3,type>((this)->x, (this)->y * other.y, (this)->z); 
	};

	/* scale this vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_z ( type const other ) { 

		(this)->z = (this)->z * other; 
	};

	/* scale this vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable scale_z ( Vector<3,type> const other ) { 

		(this)->z = (this)->z * other.z; 
	};

	/* get scaled vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_z ( type const other ) const { 

		return Vector<3,type>((this)->x, (this)->y, (this)->z * other); 
	};

	/* get scaled vector on z axis */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable scaled_z ( Vector<3,type> const other ) const { 

		return Vector<3,type>((this)->x, (this)->y, (this)->z * other.z); 
	};

	/* rotate this vector on x axis around [0y,0z] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_x ( type const angle ) {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		type const roty = (this)->y * _cos - (this)->z * _sin;

		(this)->z		 = (this)->y * _sin + (this)->z * _cos;

		(this)->y = roty;
	};

	/* get rotated vector on x axis around [0y,0z] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_x ( type const angle ) const {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		return Vector<3,type>((this)->x,(this)->y * _cos - (this)->z * _sin, (this)->y * _sin + (this)->z * _cos);
	};

	/* rotate this vector on y axis around [0x,0z] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_y ( type const angle ) {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		type const rotx = (this)->x *    _cos  + (this)->z * _sin;

		(this)->z		 = (this)->x * (- _sin) + (this)->z * _cos;

		(this)->x = rotx;
	};

	/* get rotated vector on y axis around [0x,0z] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_y ( type const angle ) const {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		return Vector<3,type>((this)->x * _cos + (this)->z * _sin, (this)->z, (this)->x * (- _sin) + (this)->z * _cos);
	};

	/* rotate this vector on z axis around [0x,0y] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_z ( type const angle ) {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		type const rotx = (this)->x * _cos - (this)->y * _sin;

		(this)->y		 = (this)->x * _sin + (this)->y * _cos;

		(this)->x = rotx;
	};

	/* get rotated vector on z axis around [0x,0y] */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_z ( type const angle ) const {

		type const _sin = _MATH_SIN(angle);

		type const _cos = _MATH_COS(angle);

		return Vector<3,type>((this)->x * _cos - (this)->y * _sin, (this)->x * _sin + (this)->y * _cos, (this)->z);
	};

	/* rotate vector on x axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_x ( Vector<3,type> const other, type const angle ) {

		Vector<3,type> const dif = (this)->operator-(other);

		(this)->operator=(other + dif.rotated_x(angle));
	};

	/* get rotated vector on x axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_x ( Vector<3,type> const other, type const angle ) const {

		Vector<3,type> const dif = (this)->operator-(other);

		return other + dif.rotated_x(angle);
	};

	/* rotate vector on y axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_y ( Vector<3,type> const other, type const angle ) {

		Vector<3,type> const dif = (this)->operator-(other);

		(this)->operator=(other + dif.rotated_y(angle));
	};

	/* get rotated vector on y axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_y ( Vector<3,type> const other, type const angle ) const {

		Vector<3,type> const dif = (this)->operator-(other);

		return other + dif.rotated_y(angle);
	};

	/* rotate vector on z axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_z ( Vector<3,type> const other, type const angle ) {

		Vector<3,type> const dif = (this)->operator-(other);

		(this)->operator=(other + dif.rotated_z(angle));
	};

	/* get rotated vector on z axis around another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable rotated_z ( Vector<3,type> const other, type const angle ) const {

		Vector<3,type> const dif = (this)->operator-(other);

		return other + dif.rotated_z(angle);
	};

	/* get angle on x axis between two vectors in the range from -PI to PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_x_pi ( Vector<2,type> const other ) const {

		type const angle = (this)->angle_x(other);

		return (angle < 0) ? angle + 6.28318530718f : angle;
	};

	/* get angle on x axis between two vectors in the range from 0 to 2PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_x ( Vector<2,type> const other ) const {

		return _MATH_ATAN2((Vector<2,type>((this)->y,(this)->z).perp_dot_product(other)),(Vector<2,type>((this)->y,(this)->z).dot_product(other)));
	};

	/* get angle on y axis between two vectors in the range from -PI to PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_y_pi ( Vector<2,type> const other ) const {

		type const angle = (this)->angle_x(other);

		return (angle < 0) ? angle + 6.28318530718f : angle;
	};

	/* get angle on y axis between two vectors in the range from 0 to 2PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_y ( Vector<2,type> const other ) const {

		return _MATH_ATAN2((Vector<2,type>((this)->x,(this)->z).perp_dot_product(other)),(Vector<2,type>((this)->x,(this)->z).dot_product(other)));
	};

	/* get angle on z axis between two vectors in the range from -PI to PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_z_pi ( Vector<2,type> const other ) const {

		type const angle = (this)->angle_z(other);

		return (angle < 0) ? angle + 6.28318530718f : angle;
	};

	/* get angle on z axis between two vectors in the range from 0 to 2PI */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,type>::enable angle_z ( Vector<2,type> const other ) const {

		return _MATH_ATAN2((Vector<2,type>((this)->x,(this)->y).perp_dot_product(other)),(Vector<2,type>((this)->x,(this)->y).dot_product(other)));
	};

	/* project this vector on to another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable project ( Vector<3,type> const other ) {

		(*this) = other * (this)->dot_product(other) / other.length2();
	};

	/* get this vector projected on to another vector */
	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Vector<3,type>>::enable projected ( Vector<3,type> const other ) const {

		return other * ((this)->dot_product(other) / other.length2());
	};

/* vector axis */

	type x, y, z;
};

/* Matrix :
 *
 *		Matrix class template.
 *
 */
template < uint32_t n_columns, uint32_t n_rows, typename type, bool column_major > class Matrix {

	public :

	type elm[n_columns][n_rows];
};

/* Matrix<4,4,type,true> :
 *
 *		Specialized matrix class template.
 *
 */
template < typename type > class Matrix < 4, 4, type, false > {

	public :

	typedef Matrix <4,4,type,false> this_t;

	/* add one matrix to another matrix */
	_SW_LALG_MATH_INLINE_ this_t operator + ( this_t const & other ) const {

		this_t result;

		for(unsigned int k = 0; k < 4; k++)

			for(unsigned int j = 0; j < 4; j++)

				result.elm[k][j] = (this)->elm[4][4] + other.elm[4][4];

		return result;
	};
	
	/* subtract matrix from another matrix */
	_SW_LALG_MATH_INLINE_ this_t operator - ( this_t const & other ) const {

		this_t result;

		for(unsigned int k = 0; k < 4; k++)

			for(unsigned int j = 0; j < 4; j++)

				result.elm[k][j] = (this)->elm[4][4] - other.elm[4][4];

		return result;
	};
	
	/* multiply matrix with another matrix */
	_SW_LALG_MATH_INLINE_ this_t operator * ( this_t const & other ) const {

		this_t result;

		result.elm[0][0] = ((this)->elm[0][0] * other.elm[0][0]) + ((this)->elm[0][1] * other.elm[1][0]) + ((this)->elm[0][2] * other.elm[2][0]) + ((this)->elm[0][3] * other.elm[3][0]);
		result.elm[0][1] = ((this)->elm[0][0] * other.elm[0][1]) + ((this)->elm[0][1] * other.elm[1][1]) + ((this)->elm[0][2] * other.elm[2][1]) + ((this)->elm[0][3] * other.elm[3][1]);
		result.elm[0][2] = ((this)->elm[0][0] * other.elm[0][2]) + ((this)->elm[0][1] * other.elm[1][2]) + ((this)->elm[0][2] * other.elm[2][2]) + ((this)->elm[0][3] * other.elm[3][2]);
		result.elm[0][3] = ((this)->elm[0][0] * other.elm[0][3]) + ((this)->elm[0][1] * other.elm[1][3]) + ((this)->elm[0][2] * other.elm[2][3]) + ((this)->elm[0][3] * other.elm[3][3]);

		result.elm[1][0] = ((this)->elm[1][0] * other.elm[0][0]) + ((this)->elm[1][1] * other.elm[1][0]) + ((this)->elm[1][2] * other.elm[2][0]) + ((this)->elm[1][3] * other.elm[3][0]);
		result.elm[1][1] = ((this)->elm[1][0] * other.elm[0][1]) + ((this)->elm[1][1] * other.elm[1][1]) + ((this)->elm[1][2] * other.elm[2][1]) + ((this)->elm[1][3] * other.elm[3][1]);
		result.elm[1][2] = ((this)->elm[1][0] * other.elm[0][2]) + ((this)->elm[1][1] * other.elm[1][2]) + ((this)->elm[1][2] * other.elm[2][2]) + ((this)->elm[1][3] * other.elm[3][2]);
		result.elm[1][3] = ((this)->elm[1][0] * other.elm[0][3]) + ((this)->elm[1][1] * other.elm[1][3]) + ((this)->elm[1][2] * other.elm[2][3]) + ((this)->elm[1][3] * other.elm[3][3]);

		result.elm[2][0] = ((this)->elm[2][0] * other.elm[0][0]) + ((this)->elm[2][1] * other.elm[1][0]) + ((this)->elm[2][2] * other.elm[2][0]) + ((this)->elm[2][3] * other.elm[3][0]);
		result.elm[2][1] = ((this)->elm[2][0] * other.elm[0][1]) + ((this)->elm[2][1] * other.elm[1][1]) + ((this)->elm[2][2] * other.elm[2][1]) + ((this)->elm[2][3] * other.elm[3][1]);
		result.elm[2][2] = ((this)->elm[2][0] * other.elm[0][2]) + ((this)->elm[2][1] * other.elm[1][2]) + ((this)->elm[2][2] * other.elm[2][2]) + ((this)->elm[2][3] * other.elm[3][2]);
		result.elm[2][3] = ((this)->elm[2][0] * other.elm[0][3]) + ((this)->elm[2][1] * other.elm[1][3]) + ((this)->elm[2][2] * other.elm[2][3]) + ((this)->elm[2][3] * other.elm[3][3]);

		result.elm[3][0] = ((this)->elm[3][0] * other.elm[0][0]) + ((this)->elm[3][1] * other.elm[1][0]) + ((this)->elm[3][2] * other.elm[2][0]) + ((this)->elm[3][3] * other.elm[3][0]);
		result.elm[3][1] = ((this)->elm[3][0] * other.elm[0][1]) + ((this)->elm[3][1] * other.elm[1][1]) + ((this)->elm[3][2] * other.elm[2][1]) + ((this)->elm[3][3] * other.elm[3][1]);
		result.elm[3][2] = ((this)->elm[3][0] * other.elm[0][2]) + ((this)->elm[3][1] * other.elm[1][2]) + ((this)->elm[3][2] * other.elm[2][2]) + ((this)->elm[3][3] * other.elm[3][2]);
		result.elm[3][3] = ((this)->elm[3][0] * other.elm[0][3]) + ((this)->elm[3][1] * other.elm[1][3]) + ((this)->elm[3][2] * other.elm[2][3]) + ((this)->elm[3][3] * other.elm[3][3]);

		return result;
	};
	
	/* multiply matrix with 2D vector */
	_SW_LALG_MATH_INLINE_ Vector<2,type> operator * ( Vector<2,type> const & other ) const {

		Vector<2,type> result;

		result.x = ((this)->elm[0][0] * other.x) + ((this)->elm[0][1] * other.y) + (this)->elm[0][3];
		result.y = ((this)->elm[0][0] * other.x) + ((this)->elm[0][1] * other.y) + (this)->elm[0][3];

		return result;
	};
	
	/* multiply matrix with 3D vector */
	_SW_LALG_MATH_INLINE_ Vector<3,type> operator * ( Vector<3,type> const & other ) const {

		Vector<3,type> result;

		result.x = ((this)->elm[0][0] * other.x) + ((this)->elm[0][1] * other.y) + ((this)->elm[0][2] * other.z) + (this)->elm[0][3];
		result.y = ((this)->elm[0][0] * other.x) + ((this)->elm[0][1] * other.y) + ((this)->elm[0][2] * other.z) + (this)->elm[1][3];

		return result;
	};
	
	/* set matrix to identity matrix */
	_SW_LALG_MATH_INLINE_ void identity ( ) {

		(this)->elm[0][0] = 1; (this)->elm[0][1] = 0; (this)->elm[0][2] = 0; (this)->elm[0][3] = 0;
		(this)->elm[1][0] = 0; (this)->elm[1][1] = 1; (this)->elm[1][2] = 0; (this)->elm[1][3] = 0;
		(this)->elm[2][0] = 0; (this)->elm[2][1] = 0; (this)->elm[2][2] = 1; (this)->elm[2][3] = 0;
		(this)->elm[3][0] = 0; (this)->elm[3][1] = 0; (this)->elm[3][2] = 0; (this)->elm[3][3] = 1;
	};

	type elm[4][4];
};

template < typename type > class Quad {

	public :

	_SW_LALG_MATH_INLINE_ Quad<type> () { };

	_SW_LALG_MATH_INLINE_ Quad<type> (Quad<type> const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];
	};

	_SW_LALG_MATH_INLINE_ Quad<type> (Vector<2,type> const a, Vector<2,type> const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector<2,type>(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector<2,type>(c.x,a.y);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> (Vector<2,type> const a, Vector<2,type> const b, Vector<2,type> const c, Vector<2,type> const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> & operator = (Quad<type> const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];
	};

	_SW_LALG_MATH_INLINE_ Quad<type> operator + (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0] + other,
			(this)->points[1] + other,
			(this)->points[2] + other,
			(this)->points[3] + other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> operator - (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0] - other,
			(this)->points[1] - other,
			(this)->points[2] - other,
			(this)->points[3] - other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> operator * (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> operator / (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0] / other,
			(this)->points[1] / other,
			(this)->points[2] / other,
			(this)->points[3] / other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> & operator += (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] + other;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> & operator -= (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] - other;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> & operator *= (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> & operator /= (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] / other;
	};

	_SW_LALG_MATH_INLINE_ void build (Vector<2,type> const a, Vector<2,type> const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector<2,type>(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector<2,type>(c.x,a.y);
	};
	
	_SW_LALG_MATH_INLINE_ void build (Vector<2,type> const a, Vector<2,type> const b, Vector<2,type> const c, Vector<2,type> const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_SW_LALG_MATH_INLINE_ void build_wh (Vector<2,type> const luc, Vector<2,type> const wh) {

		(this)->points[0] = luc;
		(this)->points[1] = luc.translated_y(wh.h);
		(this)->points[2] = luc + wh;
		(this)->points[3] = luc.translated_x(wh.w);
	};

	_SW_LALG_MATH_INLINE_ void build_uv (Vector<2,type> const & tex_wh, Vector<2,type> const & luc, Vector<2,type> const & wh) {

		Vector<2,type> const pixel = Vector<2,type>(type(1.0) / type(tex_wh.w), type(1.0) / type(tex_wh.h));

		Vector<2,type> const lucf = Vector<2,type>(type(luc.x) * pixel.x, type(luc.y) * pixel.y);

		Vector<2,type> const whf = Vector<2,type>(type(whf.w) * pixel.w, type(whf.h) * pixel.h);

		(this)->build_wh(lucf,whf);
	};
	
	_SW_LALG_MATH_INLINE_ void build (Quad<type> const & other) {
	
		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = other.points[i];
	};

	_SW_LALG_MATH_INLINE_ Vector<2,type> center () const {

		Vector2F center(0);

		for(unsigned int i = 0; i < 4; i++)

			center += (this)->points[i];

		return center / 4;
	};

	_SW_LALG_MATH_INLINE_ void translate (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] + other;
	};

	_SW_LALG_MATH_INLINE_ void translate (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] + other;
	};

	_SW_LALG_MATH_INLINE_ void translate_x (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other;
	};

	_SW_LALG_MATH_INLINE_ void translate_x (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other.x;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> translated_x (type const other) const {

		return Quad<type>(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> translated_x (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_SW_LALG_MATH_INLINE_ void translate_y (type const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other;
	};

	_SW_LALG_MATH_INLINE_ void translate_y (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other.y;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> translated_y (type const other) const {

		return Quad<type>(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> translated_y (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_SW_LALG_MATH_INLINE_ void scale (type const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_SW_LALG_MATH_INLINE_ void scale (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_SW_LALG_MATH_INLINE_ void scale_centered (type const other) {

		Vector<2,type> const center = (this)->center();

		Vector<2,type> const scal_center = center * other;

		Vector<2,type> const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_SW_LALG_MATH_INLINE_ void scale_centered (Vector<2,type> const other) {

		Vector<2,type> const center = (this)->center();

		Vector<2,type> const scal_center = center * other;

		Vector<2,type> const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled (type const other) const {

		return Quad<type>(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_centered (type const other) const {

		Vector<2,type> const center = (this)->center();

		Vector<2,type> const scal_center = center * other;

		Vector<2,type> const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_centered (Vector<2,type> const other) const {

		Vector<2,type> const center = (this)->center();

		Vector<2,type> const scal_center = center * other;

		Vector<2,type> const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_SW_LALG_MATH_INLINE_ void scale_x (type const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other;
	};

	_SW_LALG_MATH_INLINE_ void scale_x (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x;
	};

	_SW_LALG_MATH_INLINE_ void scale_x_centered (type const other) {

		type const center = (this)->center().x;

		type const scal_center = center * other;

		type const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other + center_dif;
	};

	_SW_LALG_MATH_INLINE_ void scale_x_centered (Vector<2,type> const other) {

		type const center = (this)->center().x;

		type const scal_center = center * other.x;

		type const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x + center_dif;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_x (type const other) const {

		return Quad<type>(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_x (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_x_centered (type const other) const {

		type const center = (this)->center().x;

		type const scal_center = center * other;

		type const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_x_centered (Vector<2,type> const other) const {

		type const center = (this)->center().x;

		type const scal_center = center * other.x;

		type const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_SW_LALG_MATH_INLINE_ void scale_y (type const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other;
	};

	_SW_LALG_MATH_INLINE_ void scale_y (Vector<2,type> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y;
	};

	_SW_LALG_MATH_INLINE_ void scale_y_centered (type const other) {

		type const center = (this)->center().y;

		type const scal_center = center * other;

		type const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other + center_dif;
	};

	_SW_LALG_MATH_INLINE_ void scale_y_centered (Vector<2,type> const other) {

		type const center = (this)->center().y;

		type const scal_center = center * other.y;

		type const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y + center_dif;
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_y (type const other) const {

		return Quad<type>(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_y (Vector<2,type> const other) const {

		return Quad<type>(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_y_centered (type const & other) const {

		type const center = (this)->center().y;

		type const scal_center = center * other;

		type const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	_SW_LALG_MATH_INLINE_ Quad<type> scaled_y_centered (Vector<2,type> const & other) const {

		type const center = (this)->center().y;

		type const scal_center = center * other.y;

		type const center_dif = center - scal_center;

		return Quad<type>(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	_SW_LALG_MATH_INLINE_ Vector<2,type> center () const {

		Vector<2,type> center(0.0f);

		for(unsigned int i = 0; i < 4; i++)

			center = center + (this)->points[i];

		return center / 4;
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate (type const angle) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(angle);
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate (Vector<2,type> const & other, type const & angle) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(other,angle);
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,void>::enable rotate_centered (type const angle) {

		Vector<2,type> const center = (this)->center();

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(center,angle);
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Quad<type>>::enable rotated (type const & angle) const {

		return Quad<type>(

			(this)->points[0].rotated(angle),
			(this)->points[1].rotated(angle),
			(this)->points[2].rotated(angle),
			(this)->points[3].rotated(angle)
		);
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Quad<type>>::enable rotated (Vector<2,type> const & other, type const & angle) const {

		return Quad<type>(

			(this)->points[0].rotated(other,angle),
			(this)->points[1].rotated(other,angle),
			(this)->points[2].rotated(other,angle),
			(this)->points[3].rotated(other,angle)
		);
	};

	_SW_LALG_MATH_INLINE_ typename _sw_if_floating<type,Quad<type>>::enable rotated_centered (type const & angle) const {

		Vector<2,type> const center = (this)->center();

		return Quad<type>(

			(this)->points[0].rotated(center,angle),
			(this)->points[1].rotated(center,angle),
			(this)->points[2].rotated(center,angle),
			(this)->points[3].rotated(center,angle)
		);
	};

	/* points */
	Vector<2,type> points[4];
};

#endif // ifndef _SW_LALG_MATH_HPP_