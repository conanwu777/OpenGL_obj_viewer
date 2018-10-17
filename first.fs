#version 400 core
flat in vec3 fColor;
in vec2 fTex;

out vec4 Color;

uniform sampler2D tex;
uniform int sw;

void main()
{
	float grey;
	grey = (fColor.x + fColor.y + fColor.z) / 3.0;
	vec3 g = vec3(grey, grey, grey);
	if (sw == 0)
	{
	    Color = vec4(fColor, 1.0);
	}
	else if (sw <= 100)
	{
	    Color = vec4((sw * g + (100 - sw) * fColor) / 100, 1.0);
	}
	else if (sw <= 200)
	{
		Color = ((sw - 100) * texture(tex, fTex) + (200 - sw) * vec4(g, 1.0)) / 100;
	}
	else if (sw > 200)
	{
		vec4 g = vec4(fColor, 1.0);
		Color = ((sw - 200) * vec4(fColor, 1.0) + (300 - sw) * texture(tex, fTex)) / 100;
	}
}
