#include "imgl.hpp"

#ifdef IMGL_NO_IMGUI // if not included in header include here
// TODO: no dllexport here
#include "imgui.h"
#endif

// abstracts window opening, opengl context init, window position saving, vsync, etc.  (window buffer clearing currently also happens here)
// changing OS and or graphics library (DirectX, Vulkan, etc.) would require a different header here
#include "platform_window.hpp"

namespace imgl {
	Window window; // if this represents an open window then the destructor will always close the window at the end of the program, so close_window is optional
	
	Region get_current_region () {
		return {{ 0, (fv2)window.get_size() }};
	}

	bool begin_window (::std::string const& title, rgb8 clear_col, e_input_mode input_mode, s32v2 default_size) {
		if (!window.is_open()) {
			window.open_window(title, default_size);
		}

		bool keep_open = window.begin_window(input_mode);

		glEnable(GL_FRAMEBUFFER_SRGB);

		auto sz = window.get_size();
		glViewport(0,0, sz.x,sz.y);
		glScissor(0,0, sz.x,sz.y);

		rgbf col = (rgbf)clear_col / 255;
		glClearColor(col.x, col.y, col.z, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		return keep_open;
	}

	void end_window (e_display_mode display_mode) {
		window.end_window(display_mode);
	}

	void close_window () {
		window.close_window();
	}

	void enable_drawing_on_resize (draw_func draw_callback) {
		window.draw_callback = draw_callback;
		glfwSetWindowRefreshCallback(window.window, window.glfw_refresh_callback);
	}

	void window_loop (std::string const& title, draw_func draw, rgb8 clear_col, e_input_mode input_mode, e_display_mode display_mode, s32v2 default_size) {
		for (;;) {
			if (!imgl::begin_window(title, 0, input_mode, default_size))
				break; // Window was closed via a standart close command (Close button or Alt-F4)

			static bool once = true;
			if (once)
				imgl::enable_drawing_on_resize(draw);
			once = false;

			window.glfw_refresh_callback_called_inside_frame_call = true;

			draw();

			window.glfw_refresh_callback_called_inside_frame_call = false;

			imgl::end_window(display_mode);
		}
		close_window(); // optional: explicitly close window, but window will be closed anyway at program exit
	}

}

#include "dear_imgui.hpp"
#include "rendering.hpp"

#include <memory>
using std::unique_ptr;
using std::make_unique;

namespace imgl {
	//

	void text (::std::string const& str, ::std::string font) {
		//int lines = count_lines(str);
		//
		//auto reg = get_current_region();
		//
		//f32 avail_height_px = reg.full_avail_size_px().y;
		//f32 desired_line_h_px = avail_height_px / (float)lines;
		//
		//auto* sized_font = cached_font(font, desired_line_h_px);
		//
		//render_text(window, reg, str, sized_font, desired_line_h_px);
	}

	void image_rect (u8* pixels, s32v2 size_px, Region reg, bool filter_nearest, bool top_down) {
		
		unique_ptr<u8[]> copy = nullptr;
		if (top_down) {
			copy = make_unique<u8[]>(size_px.x * size_px.y);
			
			u8* src = pixels;
			u8* dst = copy.get();

			flip_vertical_copy(dst, src, size_px.x * sizeof(u8), size_px.y);

			pixels = dst;
		}

		auto tex = single_mip_texture(PF_LR8, pixels, size_px, filter_nearest ? FILTER_NEAREST : FILTER_BILINEAR);

		std::vector<Textured_Vertex> vbo_data;
		push_rect(&vbo_data, reg.rect_window_px.low, reg.rect_window_px.high, fv2(0,1),fv2(1,0));
		draw_textured(&vbo_data[0], (int)vbo_data.size(), tex);
	}
	void image_rect (rgba8* pixels, s32v2 size_px, Region reg, bool filter_nearest, bool top_down) {

		unique_ptr<rgba8[]> copy = nullptr;
		if (top_down) {
			copy = make_unique<rgba8[]>(size_px.x * size_px.y);

			rgba8* src = pixels;
			rgba8* dst = copy.get();

			flip_vertical_copy(dst, src, size_px.x * sizeof(rgba8), size_px.y);

			pixels = dst;
		}

		auto tex = single_mip_texture(PF_SRGBA8, pixels, size_px, filter_nearest ? FILTER_NEAREST : FILTER_BILINEAR);

		std::vector<Textured_Vertex> vbo_data;
		push_rect(&vbo_data, reg.rect_window_px.low, reg.rect_window_px.high, fv2(0,1),fv2(1,0));
		draw_textured(&vbo_data[0], (int)vbo_data.size(), tex);
	}

}
