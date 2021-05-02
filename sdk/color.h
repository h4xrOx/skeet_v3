#pragma once

class Color {
private:
	// easy reinterpret.
	union {
		struct { 
			uint8_t m_r; 
			uint8_t m_g; 
			uint8_t m_b; 
			uint8_t m_a; 
		};

		uint32_t m_rgba;
	};

public:
	struct Hsv_t {
		float Hue, Saturation, Value;
	};

	// ctors.
	__forceinline Color( ) : m_r{ 0 }, m_g{ 0 }, m_b{ 0 }, m_a{ 0 }, m_rgba{} {}
	__forceinline Color( int r, int g, int b, int a = 255 ) : m_r{ ( uint8_t )r }, m_g{ ( uint8_t )g }, m_b{ ( uint8_t )b }, m_a{ ( uint8_t )a } {}
	__forceinline Color( uint32_t rgba ) : m_rgba{ rgba } {}

	static Color hsl_to_rgb( float h, float s, float l ) {
		float q;

		if( l < 0.5f )
			q = l * ( s + 1.f );

		else
			q = l + s - ( l * s );

		float p = 2 * l - q;

		float rgb[ 3 ];
		rgb[ 0 ] = h + ( 1.f / 3.f );
		rgb[ 1 ] = h;
		rgb[ 2 ] = h - ( 1.f / 3.f );

		for( int i = 0; i < 3; ++i ) {
			if( rgb[ i ] < 0 )
				rgb[ i ] += 1.f;

			if( rgb[ i ] > 1 )
				rgb[ i ] -= 1.f;

			if( rgb[ i ] < ( 1.f / 6.f ) )
				rgb[ i ] = p + ( ( q - p ) * 6 * rgb[ i ] );
			else if( rgb[ i ] < 0.5f )
				rgb[ i ] = q;
			else if( rgb[ i ] < ( 2.f / 3.f ) )
				rgb[ i ] = p + ( ( q - p ) * 6 * ( ( 2.f / 3.f ) - rgb[ i ] ) );
			else
				rgb[ i ] = p;
		}

		return { 
			int( rgb[ 0 ] * 255.f ), 
			int( rgb[ 1 ] * 255.f ), 
			int( rgb[ 2 ] * 255.f ) 
		};
	}

	struct Palette_t {
		static Color Black(int alpha = 255) { return Color(13, 13, 13, alpha); }
		static Color LightBlack(int alpha = 255) { return Color(32, 32, 32, alpha); }
		static Color DarkGrey(int alpha = 255) { return Color(44, 44, 44, alpha); }
		static Color Grey(int alpha = 255) { return Color(60, 60, 60, alpha); }
		static Color LightGrey(int alpha = 255) { return Color(77, 77, 77, alpha); }
		static Color Red(int alpha = 255) { return Color(236, 98, 95, alpha); }
		static Color White(int alpha = 255) { return Color(255, 255, 255, alpha); }

		struct MenuColors_t {
			static Color Form() { return Color( 41, 46, 57 ); }
			static Color Container() { return Color( 49, 55, 66 ); }
			static Color Outlines( ) { return Color( 77, 84, 96 ); }
			static Color ContainerHovered() { return Color( 59, 65, 76 ); }
		};
	};

	__forceinline Color OverrideAlpha(int alpha, bool adaptive = false) {
		return Color(r(), g(), b(), adaptive ? static_cast<int>(static_cast<float>(alpha) * (m_a / 255.f)) : alpha);
	}

	inline static Color Blend(Color a, Color b, float multiplier) {
		return Color(
			a.r() + static_cast<int>(multiplier * (b.r() - a.r())),
			a.g() + static_cast<int>(multiplier * (b.g() - a.g())),
			a.b() + static_cast<int>(multiplier * (b.b() - a.b())),
			a.a() + static_cast<int>(multiplier * (b.a() - a.a()))
		);
	}	

	float saturation() const {
		float r = m_r / 255.0f;
		float g = m_g / 255.0f;
		float b = m_b / 255.0f;

		float mx = std::max(r, std::max(g, b));
		float mn = std::min(r, std::min(g, b));

		float delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	float brightness() const {
		float r = m_r / 255.0f;
		float g = m_g / 255.0f;
		float b = m_b / 255.0f;

		return std::max(r, std::max(g, b));
	}

	static Color FromHSB(float hue, float saturation, float brightness) {
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1) {
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		if (h < 2) {
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		if (h < 3) {
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		if (h < 4) {
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		if (h < 5) {
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255)
		);
	}

	static const char* ToHex(Color in) {
		std::stringstream Final;

		Final << "#";
		Final << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << 255;
		Final << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << 255;
		Final << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << 255;
		Final << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << 255;

		return Final.str().c_str();
	}

	static Color HSVtoRGB(float hue, float sat, float val) {
		float red, grn, blu;
		float i, f, p, q, t;
		Color result;

		if (val == 0) {
			red = 0;
			grn = 0;
			blu = 0;
		}
		else {
			hue /= 60;
			i = floor(hue);
			f = hue - i;
			p = val * (1 - sat);
			q = val * (1 - (sat * f));
			t = val * (1 - (sat * (1 - f)));
			if (i == 0) {
				red = val;
				grn = t;
				blu = p;
			}
			else if (i == 1) {
				red = q;
				grn = val;
				blu = p;
			}
			else if (i == 2) {
				red = p;
				grn = val;
				blu = t;
			}
			else if (i == 3) {
				red = p;
				grn = q;
				blu = val;
			}
			else if (i == 4) {
				red = t;
				grn = p;
				blu = val;
			}
			else if (i == 5) {
				red = val;
				grn = p;
				blu = q;
			}
		}

		result = Color(int(red * 255), int(grn * 255), int(blu * 255));
		return result;
	}

	static Hsv_t RGBtoHSV(Color a) {
		float red, grn, blu;
		red = (float)a.r() / 255.f;
		grn = (float)a.g() / 255.f;
		blu = (float)a.b() / 255.f;
		float hue, sat, val;
		float x, f, i;
		Hsv_t result;

		x = std::min(std::min(red, grn), blu);
		val = std::max(std::max(red, grn), blu);
		if (x == val) {
			hue = 0;
			sat = 0;
		}
		else {
			f = (red == x) ? grn - blu : ((grn == x) ? blu - red : red - grn);
			i = (red == x) ? 3 : ((grn == x) ? 5 : 1);
			hue = fmod((i - f / (val - x)) * 60, 360);
			sat = ((val - x) / val);
		}
		result.Hue = hue;
		result.Saturation = sat;
		result.Value = val;

		return result;
	}

	// member accessors.
	__forceinline uint8_t& r( ) { return m_r; }
	__forceinline uint8_t& g( ) { return m_g; }
	__forceinline uint8_t& b( ) { return m_b; }
	__forceinline uint8_t& a( ) { return m_a; }
	__forceinline uint32_t& rgba( ) { return m_rgba; }

    // operators.
    __forceinline operator uint32_t() { return m_rgba; }
};

namespace colors {
	static Color white{ 255, 255, 255, 255 };
    static Color black{ 0, 0, 0, 255 };
	static Color red{ 255, 0, 0, 255 };
	static Color green{ 0, 255, 0, 255 };
	static Color yellow{ 255, 255, 0, 255 };
	static Color burgundy{ 0xff2d00b3 };
    static Color light_blue{ 95, 174, 227, 255 };
    static Color orange{ 243, 156, 18, 255 };
	static Color transparent_white{ 255, 255, 255, 210 };
    static Color transparent_green{ 0, 255, 0, 210 };
    static Color transparent_yellow{ 255, 255, 0, 210 };
}