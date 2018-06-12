#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cassert>

#ifndef IMGL_NO_OPENGL // library user can use opengl
// TODO: dllexport here
#include "deps/glad/glad.h"
#endif

#ifndef IMGL_NO_IMGUI // library user can use dear imgui
// TODO: dllexport here
#include "deps/dear_imgui/imgui.h"
#endif

#include "basic_typedefs.hpp"
#include "vector.hpp"
#include "colors.hpp"
#include "simple_file_io.hpp"

namespace imgl {
	constexpr bool not_implemented = false; // use like: assert(not_implemented);

	enum e_input_mode {
		POLL_FOR_INPUT,
		WAIT_FOR_INPUT,
	};
	enum e_display_mode {
		VSYNC,
		UNLIMITED,
	};

	struct Rect {
		fv2	low;
		fv2	high;

		inline fv2 get_size () {
			return high -low;
		}
	};
	struct RectI {
		s32v2	low;
		s32v2	high;

		inline s32v2	get_size () {
			return high -low;
		}
	};

	bool begin_window (::std::string const& title, rgb8 clear_col=0, e_input_mode input_mode=WAIT_FOR_INPUT, s32v2 default_size=s32v2(1280,720));

	void end_window (e_display_mode display_mode=VSYNC);
	void close_window ();

	typedef void (*draw_func)(void);
	void window_loop (std::string const& title, draw_func draw, rgb8 clear_col=0, e_input_mode input_mode=WAIT_FOR_INPUT, e_display_mode display_mode=VSYNC, s32v2 default_size=s32v2(1280,720));

#ifndef IMGL_NO_IMGUI
	void begin_imgui ();
	void end_imgui ();
#endif
	
	struct Region {
		Rect	rect_window_px;

		inline fv2 full_avail_size_px () { return rect_window_px.get_size(); }
		inline Region scale_centered (fv2 scale) {
			fv2 old_sz = full_avail_size_px();
			fv2 sz = old_sz * scale;
			Rect r = rect_window_px;
			r.low +=	(old_sz -sz) / 2;
			r.high =	r.low +sz;
			return { r };
		}
		inline Region fit_aspect (fv2 aspect) {
			fv2 sz = full_avail_size_px();
			sz = min(sz.x, sz.y) * min(aspect.x, aspect.y) / aspect;
			return this->scale_centered(sz / full_avail_size_px());
		}
	};

	Region get_current_region ();

	//
	void text (std::string const& str, std::string font="c:/windows/fonts/arial.ttf");

	// linear greyscale
	void image_rect (u8* pixels, s32v2 size_px, Region reg, bool filter_nearest=false, bool top_down=false);
	void image_rect (rgba8* pixels, s32v2 size_px, Region reg, bool filter_nearest=false, bool top_down=false);

	// testing
	void sdf_test ();
}
