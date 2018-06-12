
union V4 {
	struct {
		T	x, y, z, w;
	};
	T		arr[4];
	
	ALWAYSINLINE T& operator[] (u32 i) {					return arr[i]; }
	constexpr ALWAYSINLINE T operator[] (u32 i) const {	return arr[i]; }
	
	ALWAYSINLINE V4 () {}
	ALWAYSINLINE constexpr V4 (T all):				x{all},	y{all},	z{all},	w{all} {}
	ALWAYSINLINE constexpr V4 (T x, T y, T z, T w):	x{x},	y{y},	z{z},	w{w} {}
	ALWAYSINLINE constexpr V4 (V2 v, T z, T w):		x{v.x},	y{v.y},	z{z},	w{w} {}
	ALWAYSINLINE constexpr V4 (V3 v, T w):			x{v.x},	y{v.y},	z{v.z},	w{w} {}
	
	ALWAYSINLINE constexpr V2 xy () const {			return V2(x,y) ;};
	ALWAYSINLINE constexpr V3 xyz () const {			return V3(x,y,z) ;};
	
#if !BOOLVEC
	INL V4& operator+= (V4 r) {					return *this = V4(x +r.x, y +r.y, z +r.z, w +r.w); }
	INL V4& operator-= (V4 r) {					return *this = V4(x -r.x, y -r.y, z -r.z, w -r.w); }
	INL V4& operator*= (V4 r) {					return *this = V4(x * r.x, y * r.y, z * r.z, w * r.w); }
	INL V4& operator/= (V4 r) {					return *this = V4(x / r.x, y / r.y, z / r.z, w / r.w); }
	
	#if FLTVEC
	INL operator u8v4() const;
	INL operator s64v4() const;
	INL operator s32v4() const;
	#endif
	#if INTVEC
	INL operator fv4() const {					return fv4((f32)x, (f32)y, (f32)z, (f32)w); }
	#endif
#endif
};

#if BOOLVEC
	INL constexpr bool all (V4 b) {				return b.x && b.y && b.z && b.w; }
	INL constexpr bool any (V4 b) {				return b.x || b.y || b.z || b.w; }
	
	INL constexpr V4 operator! (V4 b) {			return V4(!b.x,			!b.y,		!b.z,		!b.w); }
	INL constexpr V4 operator&& (V4 l, V4 r) {	return V4(l.x && r.x,	l.y && r.y,	l.z && r.z,	l.w && r.w); }
	INL constexpr V4 operator|| (V4 l, V4 r) {	return V4(l.x || r.x,	l.y || r.y,	l.z || r.z,	l.w || r.w); }
	INL constexpr V4 XOR (V4 l, V4 r) {			return V4(BOOL_XOR(l.x, r.x),	BOOL_XOR(l.y, r.y),	BOOL_XOR(l.z, r.z),	BOOL_XOR(l.w, r.w)); }
	
#else
	INL constexpr BV4 operator < (V4 l, V4 r) {	return BV4(l.x  < r.x,	l.y  < r.y,	l.z  < r.z,	l.w  < r.w); }
	INL constexpr BV4 operator<= (V4 l, V4 r) {	return BV4(l.x <= r.x,	l.y <= r.y,	l.z <= r.z,	l.w <= r.w); }
	INL constexpr BV4 operator > (V4 l, V4 r) {	return BV4(l.x  > r.x,	l.y  > r.y,	l.z  > r.z,	l.w  > r.w); }
	INL constexpr BV4 operator>= (V4 l, V4 r) {	return BV4(l.x >= r.x,	l.y >= r.y,	l.z >= r.z,	l.w >= r.w); }
	INL constexpr BV4 operator== (V4 l, V4 r) {	return BV4(l.x == r.x,	l.y == r.y,	l.z == r.z,	l.w == r.w); }
	INL constexpr BV4 operator!= (V4 l, V4 r) {	return BV4(l.x != r.x,	l.y != r.y,	l.z != r.z,	l.w != r.w); }
	INL constexpr V4 select (BV4 c, V4 l, V4 r) {
		return V4(	c.x ? l.x : r.x,	c.y ? l.y : r.y,	c.z ? l.z : r.z,	c.w ? l.w : r.w );
	}
	
	INL constexpr bool equal (V4 l, V4 r) {		return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w; }
	
	INL constexpr V4 operator+ (V4 v) {			return v; }
	INL constexpr V4 operator- (V4 v) {			return V4(-v.x, -v.y, -v.z, -v.w); }

	INL constexpr V4 operator+ (V4 l, V4 r) {	return V4(l.x +r.x, l.y +r.y, l.z +r.z, l.w +r.w); }
	INL constexpr V4 operator- (V4 l, V4 r) {	return V4(l.x -r.x, l.y -r.y, l.z -r.z, l.w -r.w); }
	INL constexpr V4 operator* (V4 l, V4 r) {	return V4(l.x * r.x, l.y * r.y, l.z * r.z, l.w	* r.w); }
	INL constexpr V4 operator/ (V4 l, V4 r) {	return V4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w); }
	#if INTVEC
	INL constexpr V4 operator% (V4 l, V4 r) {	return V4(l.x % r.x, l.y % r.y, l.z % r.z, l.w % r.w); }
	#endif

	INL constexpr T dot(V4 l, V4 r) { return l.x*r.x + l.y*r.y + l.z*r.z + l.w*r.w; }

	INL V4 abs(V4 v) { return V4(imgl::abs(v.x), imgl::abs(v.y), imgl::abs(v.z), imgl::abs(v.w)); }
	INL T max_component(V4 v) { return imgl::max(imgl::max(v.x, v.y), imgl::max(v.z, v.w)); }

	INL constexpr V4 min(V4 l, V4 r) { return select(l <= r, l, r); }
	INL constexpr V4 max(V4 l, V4 r) { return select(r >= l, r, l); }
	
	INL constexpr V4 clamp (V4 val, V4 l, V4 h) {return min( max(val,l), h ); }
	
	#if FLTVEC
	INL constexpr V4 lerp (V4 a, V4 b, V4 t) {	return (a * (V4(1) -t)) +(b * t); }
	INL constexpr V4 map (V4 x, V4 in_a, V4 in_b) {	return (x -in_a)/(in_b -in_a); }
	INL constexpr V4 map (V4 x, V4 in_a, V4 in_b, V4 out_a, V4 out_b) {
													return lerp(out_a, out_b, map(x, in_a, in_b)); }
	
	INL T length (V4 v) {						return sqrt(v.x*v.x +v.y*v.y +v.z*v.z +v.w*v.w); }
	INL T length_sqr (V4 v) {					return v.x*v.x +v.y*v.y +v.z*v.z +v.w*v.w; }
	INL T distance (V4 a, V4 b) {				return length(b -a); }
	INL V4 normalize (V4 v) {					return v / V4(length(v)); }
	INL V4 normalize_or_zero (V4 v) { // TODO: epsilon?
		T len = length(v);
		if (len != 0) {
			 v /= len;
		}
		return v;
	}
	
	INL constexpr V4 to_deg (V4 v) {				return v * RAD_TO_DEG; }
	INL constexpr V4 to_rad (V4 v) {				return v * DEG_TO_RAD; }

	INL V4 mymod (V4 val, V4 range) {
		return V4(	imgl::mymod(val.x, range.x),
					imgl::mymod(val.y, range.y),
					imgl::mymod(val.z, range.z),
					imgl::mymod(val.w, range.w) );
	}
	
	INL V4 floor (V4 v) {						return V4(imgl::floor(v.x),	imgl::floor(v.y),	imgl::floor(v.z),	imgl::floor(v.w)); }
	INL V4 ceil (V4 v) {							return V4(imgl::ceil(v.x),	imgl::ceil(v.y),	imgl::ceil(v.z),	imgl::ceil(v.w)); }

	INL V4 pow (V4 v, V4 e) {					return V4(imgl::pow(v.x,e.x),	imgl::pow(v.y,e.y),	imgl::pow(v.z,e.z),	imgl::pow(v.w,e.w)); }
	#endif
#endif
