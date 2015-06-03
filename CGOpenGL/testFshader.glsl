#version 430 core

const vec3 lightDir = {0.7, 1.0, 0.0};

in VertexData {
    vec3 normal;
} FragmentIn;

out vec3 color;

void main()
{
	vec3 nLightDir = normalize(lightDir);
	float light = dot(normalize(FragmentIn.normal), nLightDir);
    color = vec3(1,0,0) * light;
}