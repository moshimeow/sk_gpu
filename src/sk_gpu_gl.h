#pragma once

///////////////////////////////////////////

typedef struct skr_buffer_t {
	skr_use_         use;
	skr_buffer_type_ type;
	uint32_t         _target;
	uint32_t         _buffer;
} skr_buffer_t;

typedef struct skr_mesh_t {
	uint32_t _ind_buffer;
	uint32_t _vert_buffer;
	uint32_t _layout;
} skr_mesh_t;

typedef struct skr_shader_stage_t {
	skr_stage_ type;
	uint32_t   _shader;
} skr_shader_stage_t;

typedef struct skr_shader_t {
	skr_shader_meta_t *meta;
	uint32_t           _vertex;
	uint32_t           _pixel;
	uint32_t           _program;
} skr_shader_t;

typedef struct skr_pipeline_t {
	skr_transparency_ transparency;
	skr_cull_         cull;
	bool              wireframe;
	skr_shader_t      _shader;
} skr_pipeline_t;

typedef struct skr_tex_t {
	int32_t       width;
	int32_t       height;
	int32_t       array_count;
	skr_use_      use;
	skr_tex_type_ type;
	skr_tex_fmt_  format;
	skr_mip_      mips;
	uint32_t      _texture;
	uint32_t      _framebuffer;
	uint32_t      _target;
} skr_tex_t;

typedef struct skr_swapchain_t {
	int32_t  width;
	int32_t  height;
	uint32_t _framebuffer;
} skr_swapchain_t;

typedef struct skr_platform_data_t {
#if __ANDROID__
	void *_egl_display;
	void *_egl_config;
	void *_egl_context;
#elif _WIN32
	void *_gl_hdc;
	void *_gl_hrc;
#endif
} skr_platform_data_t;