#version 450 core

layout(binding = 0) uniform sampler2D diff2d;

in struct PS
{
	vec3 normal;
	vec2 st;
}ps;

out vec4 outColor;

void main()
{
	vec4 diffColor = texture(diff2d, ps.st);  
	vec3 gamma = vec3(1.0 /2.2);
	vec3 color = pow(diffColor.rgb, gamma);
	//diffColor = vec4(color, diffColor.a); 
	if(diffColor.a > 0.1){
		outColor = diffColor;
		}else{
		discard;
		}
};