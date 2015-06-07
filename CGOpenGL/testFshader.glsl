#version 430 core

const vec3 lightDir = {0.7, 1.0, 0.0};

in VertexData {
    vec3 normal;
} FragmentIn;

out vec4 color;

void main()
{
	vec3 nLightDir = normalize(lightDir);
	float light = clamp(dot(normalize(FragmentIn.normal), nLightDir), 0, 1);
    color = vec4(vec3(1,0,0) * light + vec3(0.1,0.1,0.1), 1);
}