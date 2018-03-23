#version 450 core
out vec4 outCol;

in struct PS
{
    vec3 normal;
    vec2 st;
}ps;

void main()
{	
    vec3 lightDir = normalize(vec3(0) - vec3(-0.6, -10, -3));
    float D = dot(ps.normal, lightDir);
    //vec3 color = vec3(ps.st * ps.normal.xy, ps.st.x) * D;
    //vec3 color = vec3(1) * D;
    vec3 color = vec3(ps.st.x, ps.st.y, 0) + vec3(0.3) * D;
    
    outCol = vec4(color,1);
};