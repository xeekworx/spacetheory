#include "graphics2d.h"
#include <glad\glad.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>
#include <exception>

using namespace spacetheory;

static NVGcontext * nvg_context = NULL; // Created on the first constructor
static size_t nvg_instance_count = 0;

const glm::vec4 spacetheory::graphics2d::transparent(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 spacetheory::graphics2d::black(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 spacetheory::graphics2d::white(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 spacetheory::graphics2d::red(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 spacetheory::graphics2d::green(0.0f, 1.0f, 0.4f, 1.0f);
const glm::vec4 spacetheory::graphics2d::blue(0.0f, 0.2f, 1.0f, 1.0f);

graphics2d::graphics2d(const bool antialias)
	: m_fbo(nullptr), m_width(0.0f), m_height(0.0f), m_ready(false), m_antialias(antialias)
{
	graphics2d::create_nvg_context();

	GLfloat v[4];
	glGetFloatv(GL_VIEWPORT, &v[0]);
	m_width = v[2];
	m_height = v[3];
}

graphics2d::graphics2d(const uint32_t width, const uint32_t height, const bool antialias)
	: m_fbo(nullptr), m_width(0.0f), m_height(0.0f), m_ready(false), m_antialias(antialias)
{
	graphics2d::create_nvg_context();

	if(NULL == (m_fbo = nvgluCreateFramebuffer(nvg_context, width, height, 0/*NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY*/))) {
		throw std::exception("Failed to create NVG Frame Buffer");
	}

	m_width = (float) width;
	m_height = (float) height;
}

graphics2d::~graphics2d()
{
	if(is_ready()) end();
	if(m_fbo) nvgluDeleteFramebuffer((NVGLUframebuffer *) m_fbo);

	graphics2d::delete_nvg_context();
}

void graphics2d::create_nvg_context()
{
	int flags = NVG_STENCIL_STROKES | NVG_ANTIALIAS;
#ifdef _DEBUG
	flags |= NVG_DEBUG;
#endif
	if(NULL == (nvg_context = nvgCreateGL3(flags))) {
		throw std::exception("Failed to create NVG Context");
	}

	nvg_instance_count++;
}

void graphics2d::delete_nvg_context()
{
	nvg_instance_count--;

	if(nvg_context && nvg_instance_count == 0) {
		nvgDeleteGL3(nvg_context);
		nvg_context = NULL;
	}
}

void graphics2d::begin()
{
	if(!is_ready()) {
		// SAVE VIEWPORT:
		glGetFloatv(GL_VIEWPORT, m_glstate.viewport);
		
		// FBO:
		if(m_fbo) {
			nvgluBindFramebuffer((NVGLUframebuffer *) m_fbo);
			glViewport(0, 0, (GLsizei) m_width, (GLsizei) m_height);
		}
		else {
			m_width = m_glstate.viewport[2];
			m_height = m_glstate.viewport[3];
		}

		// ENABLE BLENDING:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// BEGIN NANOVG DRAWING:
		nvgBeginFrame(nvg_context, (int) m_width, (int) m_height, 1.f);

		m_ready = true;
	}
}

void graphics2d::end()
{
	if(is_ready()) {
		m_ready = false;

		// End nanovg drawing:
		nvgEndFrame(nvg_context);

		// FBO:
		if(m_fbo) {
			nvgluBindFramebuffer(NULL);
		}

		// RESTORE SAVED VIEWPORT:
		glViewport(
			(GLint) m_glstate.viewport[0], 
			(GLint) m_glstate.viewport[1], 
			(GLsizei) m_glstate.viewport[2], 
			(GLsizei) m_glstate.viewport[3]
		);
	}
}

void graphics2d::cancel()
{
	nvgCancelFrame(nvg_context);
}

void graphics2d::test()
{
	if(is_ready()) {
		//nvgReset(nvg_context);
		nvgBeginPath(nvg_context);
		nvgRect(nvg_context, 0, 0, 150, 150);
		nvgFillColor(nvg_context, nvgRGBA(0, 255, 100, 255));
		nvgFill(nvg_context);

		draw_roundrect(rectangle(200, 200, 150, 150), corner_radius(20.0f), 1.f, green, green);
	}
}

void graphics2d::scale_percent(const float percent)
{
	scale_percent(percent, percent);
}

void graphics2d::scale_percent(const float x_percent, const float y_percent)
{
	nvgScale(nvg_context, x_percent / 100.0f, y_percent / 100.0f);
}

void graphics2d::scale_factor(const float factor)
{
	scale_factor(factor, factor);
}

void graphics2d::scale_factor(const float x_factor, const float y_factor)
{
	nvgScale(nvg_context, x_factor, y_factor);
}

void graphics2d::reset_transform()
{
	nvgResetTransform(nvg_context);
}

void graphics2d::clear(const glm::vec4& c)
{
	if(is_ready()) {
		// This will always clear as opaque since premultiplied alpha is being used
		// with nanovg's fbo.
		//glClearColor(c.r, c.g, c.b, c.a);
		//glClear(GL_COLOR_BUFFER_BIT);

		NVGcontext * vg = nvg_context;
		nvgSave(vg);
		nvgReset(vg);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, m_width, m_height);
		nvgFillColor(vg, nvgRGBAf(c.r, c.g, c.b, c.a));
		nvgFill(vg);
		nvgRestore(vg);
	}
}

void graphics2d::draw_rect(rectangle& rect, const float border_width, const glm::vec4& border_color, const glm::vec4& fill_color)
{
	NVGcontext * vg = nvg_context;
	NVGcolor nvg_stroke_color = nvgRGBAf(border_color.r, border_color.g, border_color.b, border_color.a);
	NVGcolor nvg_fill_color = nvgRGBAf(fill_color.r, fill_color.g, fill_color.b, fill_color.a);

	//NVGpaint gradient = nvgLinearGradient(vg, rect.x, 0, rect.x + rect.w, 0, nvgRGB(255, 255, 255), nvgRGB(0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, (float) rect.x, (float) rect.y, (float) rect.w, (float) rect.h);
	nvgStrokeWidth(vg, border_width);
	nvgStrokeColor(vg, nvg_stroke_color);
	nvgStroke(vg);
	if (fill_color != transparent) {
		nvgFillColor(vg, nvg_fill_color);
		//nvgFillPaint(vg, gradient);
		nvgFill(vg);
	}
}

void graphics2d::draw_rect(rectangle& rect, const glm::vec4& border_color, const glm::vec4& fill_color)
{
	draw_rect(rect, 1.0f, border_color, fill_color);
}

void graphics2d::fill_rect(rectangle& rect, const glm::vec4& fill_color)
{
	draw_rect(rect, 0.0f, graphics2d::transparent, fill_color);
}

void graphics2d::draw_roundrect(rectangle& rect, const corner_radius& radius, const float border_width, const glm::vec4& border_color, const glm::vec4& fill_color)
{
	NVGcontext * vg = nvg_context;
	NVGcolor nvg_stroke_color = nvgRGBAf(border_color.r, border_color.g, border_color.b, border_color.a);
	NVGcolor nvg_fill_color = nvgRGBAf(fill_color.r, fill_color.g, fill_color.b, fill_color.a);

	nvgBeginPath(vg);
	nvgRoundedRectVarying(
		vg,
		(float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h,
		radius.topleft(), radius.topright(), radius.bottomright(), radius.bottomleft()
	);
	if (fill_color != transparent) {
		nvgFillColor(vg, nvg_fill_color);
		nvgFill(vg);
	}
	nvgStrokeWidth(vg, border_width);
	nvgStrokeColor(vg, nvg_stroke_color);
	nvgStroke(vg);
}

void graphics2d::draw_roundrect(rectangle& rect, const corner_radius& radius, const glm::vec4& border_color, const glm::vec4& fill_color)
{
	draw_roundrect(rect, radius, 1.0f, border_color, fill_color);
}

void graphics2d::fill_roundrect(rectangle& rect, const corner_radius& radius, const glm::vec4& fill_color)
{
	draw_roundrect(rect, radius, 0.0f, graphics2d::transparent, fill_color);
}

void graphics2d::draw(const graphics2d& source, const float x, const float y)
{
	// This destination need to be ready (begin called), 
	// the source needs to have an FBO, and 
	// the source should not be ready (end called):
	if(!is_ready() || source.m_fbo == 0 || source.is_ready()) return;
	else {
		NVGLUframebuffer * source_fbo = (NVGLUframebuffer *) source.m_fbo;
		NVGcontext * vg = nvg_context;
		const float& source_width = source.m_width;
		const float& source_height = source.m_height;

		NVGpaint paint_img = nvgImagePattern(vg, x, y, source_width, source_height, 0.0f, source_fbo->image, 1.0f);
		nvgBeginPath(vg);
		nvgRect(vg, x, y, source_width, source_height);
		nvgFillPaint(vg, paint_img);
		nvgFill(vg);
	}
}