#version 450 core
out vec4 outCol;


in struct PS
{
    vec3 normal;
}ps;

void main()
{
    outCol = vec4(ps.normal.xy,0.2,1);
};