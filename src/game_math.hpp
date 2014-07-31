#ifndef _ENGINE_MATH_HPP_
#define _ENGINE_MATH_HPP_

#include <cmath>
#include <cstdint>

#ifndef _ENGINE_INLINE_
#define _ENGINE_INLINE_ __attribute__((always_inline))
#endif

template < typename _T > class Vector2 {

	public:

	/* constructors */
	_ENGINE_INLINE_ Vector2() { };

	_ENGINE_INLINE_ explicit Vector2(float const _xy) : x(_xy), y(_xy) { };

	_ENGINE_INLINE_ Vector2(Vector2<_T> const & _xy) : x(_xy.x), y(_xy.y) { };

	_ENGINE_INLINE_ Vector2(float const _x, float const _y) : x(_x), y(_y) { };

	/* assignment operators */
	_ENGINE_INLINE_ Vector2<_T> & operator = (Vector2<_T> const other) { (this)->x = other.x; (this)->y = other.y; return (*this); };


	/* arithmetics operators */
	_ENGINE_INLINE_ Vector2<_T> operator + (float const other) const { return Vector2<_T>((this)->x + other, (this)->y + other); };

	_ENGINE_INLINE_ Vector2<_T> operator - (float const other) const { return Vector2<_T>((this)->x - other, (this)->y - other); };

	_ENGINE_INLINE_ Vector2<_T> operator * (float const other) const { return Vector2<_T>((this)->x * other, (this)->y * other); };

	_ENGINE_INLINE_ Vector2<_T> operator / (float const other) const { return Vector2<_T>((this)->x / other, (this)->y / other); };


	_ENGINE_INLINE_ Vector2<_T> operator + (Vector2<_T> const other) const { return Vector2<_T>((this)->x + other.x, (this)->y + other.y); };

	_ENGINE_INLINE_ Vector2<_T> operator - (Vector2<_T> const other) const { return Vector2<_T>((this)->x - other.x, (this)->y - other.y); };

	_ENGINE_INLINE_ Vector2<_T> operator * (Vector2<_T> const other) const { return Vector2<_T>((this)->x * other.x, (this)->y * other.y); };

	_ENGINE_INLINE_ Vector2<_T> operator / (Vector2<_T> const other) const { return Vector2<_T>((this)->x / other.x, (this)->y / other.y); };


	_ENGINE_INLINE_ Vector2<_T> & operator += (float const other) { (this)->x += other; (this)->y += other; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator -= (float const other) { (this)->x -= other; (this)->y -= other; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator *= (float const other) { (this)->x *= other; (this)->y *= other; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator /= (float const other) { (this)->x /= other; (this)->y /= other; return (*this); };


	_ENGINE_INLINE_ Vector2<_T> & operator += (Vector2<_T> const other) { (this)->x += other.x; (this)->y += other.y; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator -= (Vector2<_T> const other) { (this)->x -= other.x; (this)->y -= other.y; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator *= (Vector2<_T> const other) { (this)->x *= other.x; (this)->y *= other.y; return (*this); };

	_ENGINE_INLINE_ Vector2<_T> & operator /= (Vector2<_T> const other) { (this)->x /= other.x; (this)->y /= other.y; return (*this); };


	/* comparison operators */
	_ENGINE_INLINE_ bool operator >  (float const other) const { return ((this)->x >  other) && ((this)->y >  other); };

	_ENGINE_INLINE_ bool operator <  (float const other) const { return ((this)->x <  other) && ((this)->y <  other); };

	_ENGINE_INLINE_ bool operator >= (float const other) const { return ((this)->x >= other) && ((this)->y >= other); };

	_ENGINE_INLINE_ bool operator <= (float const other) const { return ((this)->x <= other) && ((this)->y <= other); };

	_ENGINE_INLINE_ bool operator == (float const other) const { return ((this)->x == other) && ((this)->y == other); };

	_ENGINE_INLINE_ bool operator != (float const other) const { return ((this)->x != other) && ((this)->y != other); };


	_ENGINE_INLINE_ bool operator >  (Vector2<_T> const other) const { return ((this)->x >  other.x) && ((this)->y >  other.y); };

	_ENGINE_INLINE_ bool operator <  (Vector2<_T> const other) const { return ((this)->x <  other.x) && ((this)->y <  other.y); };

	_ENGINE_INLINE_ bool operator >= (Vector2<_T> const other) const { return ((this)->x >= other.x) && ((this)->y >= other.y); };

	_ENGINE_INLINE_ bool operator <= (Vector2<_T> const other) const { return ((this)->x <= other.x) && ((this)->y <= other.y); };

	_ENGINE_INLINE_ bool operator == (Vector2<_T> const other) const { return ((this)->x == other.x) && ((this)->y == other.y); };

	_ENGINE_INLINE_ bool operator != (Vector2<_T> const other) const { return ((this)->x != other.x) && ((this)->y != other.y); };

	union {

		struct {

			float x, y;
		};

		struct {

			float u, v;
		};

		struct {

			float w, h;
		};
	};
};

typedef Vector2<float>		Vector2F;
typedef Vector2<double>		Vector2D;
typedef Vector2<int32_t>	Vector2I;
typedef Vector2<uint32_t>	Vector2U;

template < > class Vector2 < float > {

	public :

		/* constructors */
	_ENGINE_INLINE_ Vector2() { };

	_ENGINE_INLINE_ explicit Vector2(float const _xy) : x(_xy), y(_xy) { };

	_ENGINE_INLINE_ Vector2(Vector2F const & _xy) : x(_xy.x), y(_xy.y) { };

	_ENGINE_INLINE_ Vector2(float const _x, float const _y) : x(_x), y(_y) { };

	/* assignment operators */
	_ENGINE_INLINE_ Vector2F & operator = (Vector2F const other) { (this)->x = other.x; (this)->y = other.y; return (*this); };


	/* arithmetics operators */
	_ENGINE_INLINE_ Vector2F operator + (float const other) const { return Vector2F((this)->x + other, (this)->y + other); };

	_ENGINE_INLINE_ Vector2F operator - (float const other) const { return Vector2F((this)->x - other, (this)->y - other); };

	_ENGINE_INLINE_ Vector2F operator * (float const other) const { return Vector2F((this)->x * other, (this)->y * other); };

	_ENGINE_INLINE_ Vector2F operator / (float const other) const { return Vector2F((this)->x / other, (this)->y / other); };


	_ENGINE_INLINE_ Vector2F operator + (Vector2F const other) const { return Vector2F((this)->x + other.x, (this)->y + other.y); };

	_ENGINE_INLINE_ Vector2F operator - (Vector2F const other) const { return Vector2F((this)->x - other.x, (this)->y - other.y); };

	_ENGINE_INLINE_ Vector2F operator * (Vector2F const other) const { return Vector2F((this)->x * other.x, (this)->y * other.y); };

	_ENGINE_INLINE_ Vector2F operator / (Vector2F const other) const { return Vector2F((this)->x / other.x, (this)->y / other.y); };


	_ENGINE_INLINE_ Vector2F & operator += (float const other) { (this)->x += other; (this)->y += other; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator -= (float const other) { (this)->x -= other; (this)->y -= other; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator *= (float const other) { (this)->x *= other; (this)->y *= other; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator /= (float const other) { (this)->x /= other; (this)->y /= other; return (*this); };


	_ENGINE_INLINE_ Vector2F & operator += (Vector2F const other) { (this)->x += other.x; (this)->y += other.y; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator -= (Vector2F const other) { (this)->x -= other.x; (this)->y -= other.y; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator *= (Vector2F const other) { (this)->x *= other.x; (this)->y *= other.y; return (*this); };

	_ENGINE_INLINE_ Vector2F & operator /= (Vector2F const other) { (this)->x /= other.x; (this)->y /= other.y; return (*this); };


	/* comparison operators */
	_ENGINE_INLINE_ bool operator >  (float const other) const { return ((this)->x >  other) && ((this)->y >  other); };

	_ENGINE_INLINE_ bool operator <  (float const other) const { return ((this)->x <  other) && ((this)->y <  other); };

	_ENGINE_INLINE_ bool operator >= (float const other) const { return ((this)->x >= other) && ((this)->y >= other); };

	_ENGINE_INLINE_ bool operator <= (float const other) const { return ((this)->x <= other) && ((this)->y <= other); };

	_ENGINE_INLINE_ bool operator == (float const other) const { return ((this)->x == other) && ((this)->y == other); };

	_ENGINE_INLINE_ bool operator != (float const other) const { return ((this)->x != other) && ((this)->y != other); };


	_ENGINE_INLINE_ bool operator >  (Vector2F const other) const { return ((this)->x >  other.x) && ((this)->y >  other.y); };

	_ENGINE_INLINE_ bool operator <  (Vector2F const other) const { return ((this)->x <  other.x) && ((this)->y <  other.y); };

	_ENGINE_INLINE_ bool operator >= (Vector2F const other) const { return ((this)->x >= other.x) && ((this)->y >= other.y); };

	_ENGINE_INLINE_ bool operator <= (Vector2F const other) const { return ((this)->x <= other.x) && ((this)->y <= other.y); };

	_ENGINE_INLINE_ bool operator == (Vector2F const other) const { return ((this)->x == other.x) && ((this)->y == other.y); };

	_ENGINE_INLINE_ bool operator != (Vector2F const other) const { return ((this)->x != other.x) && ((this)->y != other.y); };


	/* get length of this vector */
	_ENGINE_INLINE_ float length () const { return std::sqrt((this)->length2()); };

	/* get length^2 of this vector */
	_ENGINE_INLINE_ float length2 () const { return (this)->x * (this)->x + (this)->y * (this)->y; };


	/* normalize this vector */
	_ENGINE_INLINE_ void normalize () {

		float const length = (this)->length();

		(this)->x = (this)->x / length;

		(this)->y = (this)->y / length;
	};

	/* get normalized vector */
	_ENGINE_INLINE_ Vector2F normalized () const {

		float const length = (this)->length();

		return Vector2F((this)->x / length, (this)->y / length);
	};

	/* get dot product */
	_ENGINE_INLINE_ float dot_product (Vector2F const other) const { 

		return (this)->x * other.x + (this)->y * other.y; 
	};

	/* get perpendicular dot product */
	_ENGINE_INLINE_ float perp_dot_product (Vector2F const other) const	{ 

		return -(this)->y * other.x + (this)->x * other.y; 
	};

	/* translate this vector on x axis */
	_ENGINE_INLINE_ void translate_x (float const other) { 

		(this)->x = (this)->x + other; 
	};

	/* translate this vector on x axis */
	_ENGINE_INLINE_ void translate_x (Vector2F const other) { 

		(this)->x = (this)->x + other.x; 
	};

	/* get translated vector on x axis */
	_ENGINE_INLINE_ Vector2F translated_x (float const other) const { 

		return Vector2F((this)->x + other, (this)->y); 
	};

	/* get translated vector on x axis */
	_ENGINE_INLINE_ Vector2F translated_x (Vector2F const other) const { 

		return Vector2F((this)->x + other.x, (this)->y); 
	};

	/* translate this vector on y axis */
	_ENGINE_INLINE_ void translate_y (float const other) { 

		(this)->y = (this)->y + other; 
	};

	/* translate this vector on y axis */
	_ENGINE_INLINE_ void translate_y (Vector2 const other) { 

		(this)->y = (this)->y + other.y; 
	};

	/* get translated vector on y axis */
	_ENGINE_INLINE_ Vector2F translated_y (float const other) const { 

		return Vector2F((this)->x, (this)->y + other); 
	};

	/* get translated vector on y axis */
	_ENGINE_INLINE_ Vector2F translated_y (Vector2F const other) const { 

		return Vector2F((this)->x, (this)->y + other.y); 
	};

	/* scale this vector on x axis */
	_ENGINE_INLINE_ void scale_x (float const other) { 

		(this)->x = (this)->x * other; 
	};

	/* scale this vector on x axis */
	_ENGINE_INLINE_ void scale_x (Vector2F const other) { 

		(this)->x = (this)->x * other.x; 
	};

	/* get scaled vector on x axis */
	_ENGINE_INLINE_ Vector2F scaled_x (float const other) const { 

		return Vector2F((this)->x * other, (this)->y); 
	};

	/* get scaled vector on x axis */
	_ENGINE_INLINE_ Vector2F scaled_x (Vector2F const other) const { 

		return Vector2F((this)->x * other.x, (this)->y); 
	};

	/* scale this vector on y axis */
	_ENGINE_INLINE_ void scale_y (float const other) { 

		(this)->y = (this)->y * other; 
	};

	/* scale this vector on y axis */
	_ENGINE_INLINE_ void scale_y (Vector2F const other) { 

		(this)->y = (this)->y * other.y; 
	};

	/* get scaled vector on y axis */
	_ENGINE_INLINE_ Vector2F scaled_y (float const other) const { 

		return Vector2F((this)->x, (this)->y * other); 
	};

	/* get scaled vector on y axis */
	_ENGINE_INLINE_ Vector2F scaled_y (Vector2F const other) const { 

		return Vector2F((this)->x, (this)->y * other.y); 
	};

	/* rotate this vector on z axis around [0x,0y] */
	_ENGINE_INLINE_ void rotate (float const angle) {

		float const _sin = std::sin(angle);

		float const _cos = std::cos(angle);

		float const rotx = (this)->x * _cos - (this)->y * _sin;

		(this)->y		 = (this)->x * _sin + (this)->y * _cos;

		(this)->x = rotx;
	};
	
	/* get rotated vector on z axis around [0x,0y] */
	_ENGINE_INLINE_ Vector2F rotated (float const angle) const {

		float const _sin = std::sin(angle);

		float const _cos = std::cos(angle);

		return Vector2F((this)->x * _cos - (this)->y * _sin, (this)->x * _sin + (this)->y * _cos);
	};

	/* rotate vector on z axis around another vector */
	_ENGINE_INLINE_ void rotate (Vector2F const other, float const angle) {

		Vector2F const dif = (this)->operator-(other);

		(this)->operator=(other + dif.rotated(angle));
	};

	/* get rotated vector on z axis around another vector */
	_ENGINE_INLINE_ Vector2F rotated (Vector2F const other, float const angle) const {

		Vector2F const dif = (this)->operator-(other);

		return other + dif.rotated(angle);
	};

	/* get angle between two vectors in the range from -PI to PI */
	_ENGINE_INLINE_ float angle_pi (Vector2F const other) const {

		float const angle = (this)->angle(other);

		return (angle < 0) ? angle + 6.28318530718f : angle;
	};

	/* get angle between two vectors in the range from 0 to 2PI */
	_ENGINE_INLINE_ float angle (Vector2F const other) const {

		return std::atan2((this)->perp_dot_product(other), (this)->dot_product(other));
	};

	/* project this vector on to another vector */
	_ENGINE_INLINE_ void project (Vector2F const other) {

		(*this) = other * (this)->dot_product(other) / other.length2();
	};

	/* get this vector projected on to another vector */
	_ENGINE_INLINE_ Vector2F projected (Vector2F const other) const {

		return other * ((this)->dot_product(other) / other.length2());
	};

	/* vector axis */

	union {

		struct {

			float x, y;
		};

		struct {

			float u, v;
		};

		struct {

			float w, h;
		};
	};
};

template < typename _T > class Quad2 {

	public :

	_ENGINE_INLINE_ Quad2 () { };

	_ENGINE_INLINE_ Quad2 (Quad2<_T> const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];
	};

	_ENGINE_INLINE_ Quad2 (Vector2<_T> const a, Vector2<_T> const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector2<_T>(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector2<_T>(c.x,a.y);
	};

	_ENGINE_INLINE_ Quad2 (Vector2<_T> const a, Vector2<_T> const b, Vector2<_T> const c, Vector2<_T> const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_ENGINE_INLINE_ Quad2<_T> & operator = (Quad2<_T> const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];
	};

	_ENGINE_INLINE_ Quad2<_T> operator + (Vector2<_T> const other) const {

		return Quad2<_T>(

			(this)->points[0] + other,
			(this)->points[1] + other,
			(this)->points[2] + other,
			(this)->points[3] + other
		);
	};

	_ENGINE_INLINE_ Quad2<_T> operator - (Vector2<_T> const other) const {

		return Quad2<_T>(

			(this)->points[0] - other,
			(this)->points[1] - other,
			(this)->points[2] - other,
			(this)->points[3] - other
		);
	};

	_ENGINE_INLINE_ Quad2<_T> operator * (Vector2<_T> const other) const {

		return Quad2<_T>(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2<_T> operator / (Vector2<_T> const other) const {

		return Quad2<_T>(

			(this)->points[0] / other,
			(this)->points[1] / other,
			(this)->points[2] / other,
			(this)->points[3] / other
		);
	};

	_ENGINE_INLINE_ Quad2<_T> & operator += (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] + other;
	};

	_ENGINE_INLINE_ Quad2<_T> & operator -= (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] - other;
	};

	_ENGINE_INLINE_ Quad2<_T> & operator *= (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ Quad2<_T> & operator /= (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] / other;
	};

	_ENGINE_INLINE_ void build (Vector2<_T> const a, Vector2<_T> const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector2<_T>(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector2<_T>(c.x,a.y);
	};
	
	_ENGINE_INLINE_ void build (Vector2<_T> const a, Vector2<_T> const b, Vector2<_T> const c, Vector2<_T> const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_ENGINE_INLINE_ void build_wh (Vector2<_T> const luc, Vector2<_T> const wh) {

		(this)->points[0] = luc;
		(this)->points[1] = luc.translated_y(wh.h);
		(this)->points[2] = luc + wh;
		(this)->points[3] = luc.translated_x(wh.w);
	};
	
	_ENGINE_INLINE_ void build (Quad2<_T> const & other) {
	
		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = other.points[i];
	};

	_ENGINE_INLINE_ Vector2<_T> center () const {

		Vector2F center(0);

		for(unsigned int i = 0; i < 4; i++)

			center = center + (this)->points[i];

		return center / 4;
	};

	_ENGINE_INLINE_ void translate_x (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other;
	};

	_ENGINE_INLINE_ void translate_x (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other.x;
	};

	_ENGINE_INLINE_ Quad2 translated_x (_T const other) const {

		return Quad2(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2 translated_x (Vector2<_T> const other) const {

		return Quad2(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_ENGINE_INLINE_ void translate_y (_T const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other;
	};

	_ENGINE_INLINE_ void translate_y (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other.y;
	};

	_ENGINE_INLINE_ Quad2 translated_y (_T const other) const {

		return Quad2(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2 translated_y (Vector2<_T> const other) const {

		return Quad2(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_ENGINE_INLINE_ void scale (_T const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ void scale (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ void scale_centered (_T const other) {

		Vector2<_T> const center = (this)->center();

		Vector2<_T> const scal_center = center * other;

		Vector2<_T> const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_centered (Vector2<_T> const other) {

		Vector2<_T> const center = (this)->center();

		Vector2<_T> const scal_center = center * other;

		Vector2<_T> const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_ENGINE_INLINE_ Quad2 scaled (_T const other) const {

		return Quad2(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2 scaled (Vector2<_T> const other) const {

		return Quad2(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_centered (_T const other) const {

		Vector2<_T> const center = (this)->center();

		Vector2<_T> const scal_center = center * other;

		Vector2<_T> const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_centered (Vector2<_T> const other) const {

		Vector2<_T> const center = (this)->center();

		Vector2<_T> const scal_center = center * other;

		Vector2<_T> const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_ENGINE_INLINE_ void scale_x (_T const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other;
	};

	_ENGINE_INLINE_ void scale_x (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x;
	};

	_ENGINE_INLINE_ void scale_x_centered (_T const other) {

		float const center = (this)->center().x;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_x_centered (Vector2<_T> const other) {

		float const center = (this)->center().x;

		float const scal_center = center * other.x;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x + center_dif;
	};

	_ENGINE_INLINE_ Quad2 scaled_x (_T const other) const {

		return Quad2(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_x (Vector2<_T> const other) const {

		return Quad2(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_x_centered (_T const other) const {

		float const center = (this)->center().x;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_x_centered (Vector2<_T> const other) const {

		float const center = (this)->center().x;

		float const scal_center = center * other.x;

		float const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_ENGINE_INLINE_ void scale_y (_T const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other;
	};

	_ENGINE_INLINE_ void scale_y (Vector2<_T> const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y;
	};

	_ENGINE_INLINE_ void scale_y_centered (_T const other) {

		float const center = (this)->center().y;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_y_centered (Vector2<_T> const other) {

		float const center = (this)->center().y;

		float const scal_center = center * other.y;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y + center_dif;
	};

	_ENGINE_INLINE_ Quad2 scaled_y (_T const other) const {

		return Quad2(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_y (Vector2<_T> const other) const {

		return Quad2(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_y_centered (_T const other) const {

		float const center = (this)->center().y;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	_ENGINE_INLINE_ Quad2 scaled_y_centered (Vector2<_T> const other) const {

		float const center = (this)->center().y;

		float const scal_center = center * other.y;

		float const center_dif = center - scal_center;

		return Quad2(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	/* points */
	Vector2<_T> points[4];
};

typedef Quad2<float>	Quad2F;
typedef Quad2<double>	Quad2D;
typedef Quad2<int32_t>	Quad2I;
typedef Quad2<uint32_t> Quad2U;

template < > class Quad2 < float > {

	public :

	_ENGINE_INLINE_ Quad2 () { };

	_ENGINE_INLINE_ Quad2 (Quad2F const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];
	};

	_ENGINE_INLINE_ Quad2 (Vector2F const a, Vector2F const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector2F(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector2F(c.x,a.y);
	};

	_ENGINE_INLINE_ Quad2 (Vector2F const a, Vector2F const b, Vector2F const c, Vector2F const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_ENGINE_INLINE_ Quad2F & operator = (Quad2F const & quad) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = quad.points[i];

		return (*this);
	};

	_ENGINE_INLINE_ Quad2F operator + (Vector2F const other) const {

		return Quad2F(

			(this)->points[0] + other,
			(this)->points[1] + other,
			(this)->points[2] + other,
			(this)->points[3] + other
		);
	};

	_ENGINE_INLINE_ Quad2F operator - (Vector2F const other) const {

		return Quad2F(

			(this)->points[0] - other,
			(this)->points[1] - other,
			(this)->points[2] - other,
			(this)->points[3] - other
		);
	};

	_ENGINE_INLINE_ Quad2F operator * (Vector2F const other) const {

		return Quad2F(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2F operator / (Vector2F const other) const {

		return Quad2F(

			(this)->points[0] / other,
			(this)->points[1] / other,
			(this)->points[2] / other,
			(this)->points[3] / other
		);
	};

	_ENGINE_INLINE_ Quad2F & operator += (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] + other;
	};

	_ENGINE_INLINE_ Quad2F & operator -= (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] - other;
	};

	_ENGINE_INLINE_ Quad2F & operator *= (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ Quad2F & operator /= (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] / other;
	};

	_ENGINE_INLINE_ void build (Vector2F const a, Vector2F const c) {

		(this)->points[0] = a;
		(this)->points[1] = Vector2F(a.x,c.y);
		(this)->points[2] = c;
		(this)->points[3] = Vector2F(c.x,a.y);
	};
	
	_ENGINE_INLINE_ void build (Vector2F const a, Vector2F const b, Vector2F const c, Vector2F const d) {

		(this)->points[0] = a;
		(this)->points[1] = b;
		(this)->points[2] = c;
		(this)->points[3] = d;
	};

	_ENGINE_INLINE_ void build_wh (Vector2F const luc, Vector2F const wh) {

		(this)->points[0] = luc;
		(this)->points[1] = luc.translated_y(wh.h);
		(this)->points[2] = luc + wh;
		(this)->points[3] = luc.translated_x(wh.w);
	};
	
	_ENGINE_INLINE_ void build (Quad2F const & other) {
	
		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = other.points[i];
	};
	
	_ENGINE_INLINE_ void build_uv (Vector2U const tex_wh, Vector2U const luc, Vector2U const wh) {
	
		Vector2F const pixel = Vector2F(1.0f / float(tex_wh.w), 1.0f / float(tex_wh.h));

		Vector2F const lucf = Vector2F(float(luc.x) * pixel.x, float(luc.y) * pixel.y);
		
		Vector2F const whf = Vector2F(float(whf.w) * pixel.w, float(whf.h) * pixel.h);

		(this)->build_wh(lucf,whf);
	};

	_ENGINE_INLINE_ Vector2F center () const {

		Vector2F center(0.0f);

		for(unsigned int i = 0; i < 4; i++)

			center = center + (this)->points[i];

		return center / 4.0f;
	};

	_ENGINE_INLINE_ void translate_x (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other;
	};

	_ENGINE_INLINE_ void translate_x (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x + other.x;
	};

	_ENGINE_INLINE_ Quad2F translated_x (float const other) const {

		return Quad2F(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2F translated_x (Vector2F const other) const {

		return Quad2F(

			(this)->points[0].translated_x(other),
			(this)->points[1].translated_x(other),
			(this)->points[2].translated_x(other),
			(this)->points[3].translated_x(other)
		);
	};

	_ENGINE_INLINE_ void translate_y (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other;
	};

	_ENGINE_INLINE_ void translate_y (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y + other.y;
	};

	_ENGINE_INLINE_ Quad2F translated_y (float const other) const {

		return Quad2F(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2F translated_y (Vector2F const other) const {

		return Quad2F(

			(this)->points[0].translated_y(other),
			(this)->points[1].translated_y(other),
			(this)->points[2].translated_y(other),
			(this)->points[3].translated_y(other)
		);
	};

	_ENGINE_INLINE_ void scale (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ void scale (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other;
	};

	_ENGINE_INLINE_ void scale_centered (float const other) {

		Vector2F const center = (this)->center();

		Vector2F const scal_center = center * other;

		Vector2F const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_centered (Vector2F const other) {

		Vector2F const center = (this)->center();

		Vector2F const scal_center = center * other;

		Vector2F const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i] = (this)->points[i] * other + center_dif;
	};

	_ENGINE_INLINE_ Quad2F scaled (float const other) const {

		return Quad2F(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2F scaled (Vector2F const other) const {

		return Quad2F(

			(this)->points[0] * other,
			(this)->points[1] * other,
			(this)->points[2] * other,
			(this)->points[3] * other
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_centered (float const other) const {

		Vector2F const center = (this)->center();

		Vector2F const scal_center = center * other;

		Vector2F const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_centered (Vector2F const other) const {

		Vector2F const center = (this)->center();

		Vector2F const scal_center = center * other;

		Vector2F const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0] * other + center_dif,
			(this)->points[1] * other + center_dif,
			(this)->points[2] * other + center_dif,
			(this)->points[3] * other + center_dif
		);
	};

	_ENGINE_INLINE_ void scale_x (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other;
	};

	_ENGINE_INLINE_ void scale_x (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x;
	};

	_ENGINE_INLINE_ void scale_x_centered (float const other) {

		float const center = (this)->center().x;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_x_centered (Vector2F const other) {

		float const center = (this)->center().x;

		float const scal_center = center * other.x;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].x = (this)->points[i].x * other.x + center_dif;
	};

	_ENGINE_INLINE_ Quad2F scaled_x (float const other) const {

		return Quad2F(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_x (Vector2F const other) const {

		return Quad2F(

			(this)->points[0].scaled_x(other),
			(this)->points[1].scaled_x(other),
			(this)->points[2].scaled_x(other),
			(this)->points[3].scaled_x(other)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_x_centered (float const other) const {

		float const center = (this)->center().x;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_x_centered (Vector2F const other) const {

		float const center = (this)->center().x;

		float const scal_center = center * other.x;

		float const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0].scaled_x(other).translated_x(center_dif),
			(this)->points[1].scaled_x(other).translated_x(center_dif),
			(this)->points[2].scaled_x(other).translated_x(center_dif),
			(this)->points[3].scaled_x(other).translated_x(center_dif)
		);
	};

	_ENGINE_INLINE_ void scale_y (float const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other;
	};

	_ENGINE_INLINE_ void scale_y (Vector2F const other) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y;
	};

	_ENGINE_INLINE_ void scale_y_centered (float const other) {

		float const center = (this)->center().y;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other + center_dif;
	};

	_ENGINE_INLINE_ void scale_y_centered (Vector2F const other) {

		float const center = (this)->center().y;

		float const scal_center = center * other.y;

		float const center_dif = center - scal_center;

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].y = (this)->points[i].y * other.y + center_dif;
	};

	_ENGINE_INLINE_ Quad2F scaled_y (float const other) const {

		return Quad2F(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_y (Vector2F const other) const {

		return Quad2F(

			(this)->points[0].scaled_y(other),
			(this)->points[1].scaled_y(other),
			(this)->points[2].scaled_y(other),
			(this)->points[3].scaled_y(other)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_y_centered (float const other) const {

		float const center = (this)->center().y;

		float const scal_center = center * other;

		float const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	_ENGINE_INLINE_ Quad2F scaled_y_centered (Vector2F const other) const {

		float const center = (this)->center().y;

		float const scal_center = center * other.y;

		float const center_dif = center - scal_center;

		return Quad2F(

			(this)->points[0].scaled_y(other).translated_y(center_dif),
			(this)->points[1].scaled_y(other).translated_y(center_dif),
			(this)->points[2].scaled_y(other).translated_y(center_dif),
			(this)->points[3].scaled_y(other).translated_y(center_dif)
		);
	};

	_ENGINE_INLINE_ void rotate (float const angle) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(angle);
	};

	_ENGINE_INLINE_ void rotate (Vector2F const other, float const angle) {

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(other,angle);
	};

	_ENGINE_INLINE_ void rotate_centered (float const angle) {

		Vector2F const center = (this)->center();

		for(unsigned int i = 0; i < 4; i++)

			(this)->points[i].rotate(center,angle);
	};

	
	_ENGINE_INLINE_ Quad2F rotated (float const angle) const {

		return Quad2F(

			(this)->points[0].rotated(angle),
			(this)->points[1].rotated(angle),
			(this)->points[2].rotated(angle),
			(this)->points[3].rotated(angle)
		);
	};

	_ENGINE_INLINE_ Quad2F rotated (Vector2F const other, float const angle) const {

		return Quad2F(

			(this)->points[0].rotated(other,angle),
			(this)->points[1].rotated(other,angle),
			(this)->points[2].rotated(other,angle),
			(this)->points[3].rotated(other,angle)
		);
	};

	_ENGINE_INLINE_ Quad2F rotated_centered (float const angle) const {

		Vector2F const center = (this)->center();

		return Quad2F(

			(this)->points[0].rotated(center,angle),
			(this)->points[1].rotated(center,angle),
			(this)->points[2].rotated(center,angle),
			(this)->points[3].rotated(center,angle)
		);
	};

	/* points */
	Vector2F points[4];
};

#endif