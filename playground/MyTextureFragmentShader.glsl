#version 330 core

// Input data (from the vertex shaders)
in vec2 UV;

// Ouput data
layout (location = 0) out vec4 color;

// Values
uniform sampler2D RenderedTexture;
uniform sampler2D BloomTexture;
uniform int   BlurSize; // mask_size = blur_size * 2 + 1

const float offset = 1.0f / 1000.0f;
vec2 offsets[9] = vec2[](
	vec2(-offset, offset),  // top-left
	vec2(0.0f,    offset),  // top-center
	vec2(offset,  offset),  // top-right
	vec2(-offset, 0.0f),    // center-left
	vec2(0.0f,    0.0f),    // center-center
	vec2(offset,  0.0f),    // center-right
	vec2(-offset, -offset), // bottom-left
	vec2(0.0f,    -offset), // bottom-center
	vec2(offset,  -offset)  // bottom-right
);

float hdr_exposure = 0.8f, hdr_rate = 0.8f;

// Returns a random number based on a vec3.
float random(vec3 seed) {
	float dt = dot(seed, vec3(12.9898f, 78.233f, 45.164f)) * 43758.5453f;
	return dt - floor(dt);
}

void main()
{
	color = texture(RenderedTexture, UV);
	
	#if 0
	if(Mode == 1.0f)
	{
		;
	}
	else if(Mode == 2.0f)  // 灰階
	{
		float average = (color.r + color.g + color.b) / 3.0f;
		color.rgb = vec3(average);
	}
	else if(Mode == 3.0f)  // 補色
	{
		color.rgb = vec3(1.0f) - color.rgb;
	}
	else if(Mode == 4.0f)  // 高斯模糊-Force
	{
		int blur_size = 3;
		float b = (2*blur_size+1) * (2*blur_size+1);
		color.rgb = vec3(0.0f);
		for(int i = -blur_size; i <= blur_size; ++i)
		{
			for(int j = -blur_size; j <= blur_size; ++j)
			{
				color.rgb += texture(RenderedTexture, UV + vec2(offset*i, offset*j)).xyz * (1.0f / b);
			}
		}
	}
	else if(Mode == 5.0f)  // 邊緣偵測
	{		
		float kernel[9] = float[](
			-1.0f, -1.0f, -1.0f,
			-1.0f,  9.0f, -1.0f,
			-1.0f, -1.0f, -1.0f
		);
		color.rgb = vec3(0.0f);
		for(int i = 0; i < 9; ++i)
			color.rgb += texture(RenderedTexture, UV + offsets[i]).xyz * kernel[i];
	}
	else if(Mode == 6.0f)  // 高動態範圍-1
	{
		vec3 hdr_color;
		
		hdr_color = color.rgb / (color.rgb + vec3(1.0f));
		hdr_color = pow(hdr_color, vec3(hdr_rate));
		color.rgb = hdr_color;
	}
	else if(Mode == 7.0f)  // 高動態範圍-2
	{
		vec3 hdr_color;
		
		hdr_color = vec3(1.0f) - exp(-color.rgb * hdr_exposure);
		hdr_color = pow(hdr_color, vec3(hdr_rate));
		color.rgb = hdr_color;
	}
	else if(Mode == 100.0f)  // 極亮光通過
	{
		float brightness = dot(color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
		if(brightness < 1.0f)
			color.rgb = vec3(0.0f);
	}
	else if(Mode == 101.0f)  // 高斯模糊-X
	{
		float b = 2 * BlurSize + 1;
		color.rgb = vec3(0.0f);
		for(int i = -BlurSize; i <= BlurSize; ++i)
			color.rgb += texture(RenderedTexture, UV + vec2(offset*i, 0.0f)).xyz * (1.0f / b);
	}
	else if(Mode == 102.0f)  // 高斯模糊-Y
	{
		float b = 2 * BlurSize + 1;
		color.rgb = vec3(0.0f);
		for(int i = -BlurSize; i <= BlurSize; ++i)
			color.rgb += texture(RenderedTexture, UV + vec2(0.0f, offset*i)).xyz * (1.0f / b);
	}
	else if(Mode == 103.0f)  // 泛光
	{
		color.rgb += texture(BloomTexture, UV).rgb * 0.4f;
	}
	else if(Mode == 104.0f)  // 密度樹 -> 向量
	{
		int vec_size = 15;
		float b = 2 * vec_size * vec_size;
		vec3 vec = vec3(0.0f, 0.0f, b);
		for(int i = -vec_size; i <= vec_size; ++i)
		{
			for(int j = -vec_size; j <= vec_size; ++j)
			{
				float k = 1.0f - (i*i+j*j) / b;
				vec += (color.r - texture(RenderedTexture, UV + vec2(offset*i, offset*j)).x) * normalize(vec3(offset*i, offset*j, offset*k));
			}
		}
		color.rgb = 255.0f * normalize(vec);
	}
	else if(Mode == 105.0f)  // 密度樹 -> 向量 -> 陰影
	{
		int vec_size = 15;
		float b = 2 * vec_size * vec_size;
		vec3 vec = vec3(0.0f, 0.0f, b), vec_sun = normalize(vec3(1.0f, -3.0f, 0.3f));
		for(int i = -vec_size; i <= vec_size; ++i)
		{
			for(int j = -vec_size; j <= vec_size; ++j)
			{
				float k = 1.0f - (i*i+j*j) / b;
				vec += (color.r - texture(RenderedTexture, UV + vec2(offset*i, offset*j)).x) * normalize(vec3(offset*i, offset*j, offset*k));
			}
		}
		
		float dx =  vec_sun.x > 0.0f ? 0.0f : 1.0f,
			  dy = -vec_sun.y > 0.0f ? 0.0f : 1.0f;
		float dz = -(vec_sun.x*dx - vec_sun.y*dy);
		float d  = abs(vec_sun.x*UV.x - vec_sun.y*UV.y + dz) / sqrt(vec_sun.x*vec_sun.x+vec_sun.y*vec_sun.y);
		
		if(color.r > 0.0f)
			color.rgb = vec3(1.0f * pow(clamp(dot(normalize(vec), vec_sun), 0.0f, 1.0f), 0.2f) * pow(1.0f - (d / 1.5f), 4.5f));
	}
	#endif
}
