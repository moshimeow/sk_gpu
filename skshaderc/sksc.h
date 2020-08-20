// https://simoncoenen.com/blog/programming/graphics/DxcRevised.html
#pragma once

#include <stdint.h>

///////////////////////////////////////////

typedef struct sksc_settings_t {
	bool debug;
	bool row_major;
	int  optimize;
	wchar_t folder[512];
	const wchar_t *vs_entrypoint;
	const wchar_t *ps_entrypoint;
	const wchar_t *shader_model;
	wchar_t shader_model_str[16];
} sksc_settings_t;

typedef enum sksc_shader_lang_ {
	sksc_shader_lang_hlsl,
	sksc_shader_lang_spirv,
	sksc_shader_lang_glsl,
} sksc_shader_lang_;

typedef enum sksc_shader_type_ {
	sksc_shader_type_pixel,
	sksc_shader_type_vertex,
} sksc_shader_type_;

typedef struct sksc_var_t {
	size_t  offset;
	size_t  size;
	wchar_t name[32];
} sksc_var_t;

typedef struct sksc_buffer_t {
	uint8_t type;
	uint8_t slot;
	size_t  size;
	char    name[32];
} sksc_buffer_t;

typedef struct sksc_texture_t {
	uint8_t slot;
	wchar_t name[32];
} sksc_texture_t;

typedef struct sksc_stage_data_t {
	sksc_shader_type_ type;
	sksc_shader_lang_ lang;
	sksc_buffer_t    *buffers;
	int32_t           buffer_count;
	sksc_buffer_t    *textures;
	int32_t           texture_count;
	void             *binary;
	size_t            binary_size;
} sksc_stage_data_t;

typedef struct sksc_shader_t {
	sksc_stage_data_t *stages;
	int32_t            stage_count;
} sksc_shader_t;

///////////////////////////////////////////

void sksc_init    ();
void sksc_shutdown();
void sksc_compile (char *filename, char *hlsl_text, sksc_settings_t *settings);
