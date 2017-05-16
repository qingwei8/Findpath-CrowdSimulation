#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

out vec4 color;

uniform vec3 uLightColor;
uniform sampler2D texture_diffuse1;
uniform vec3 lightPos = vec3(0.0, 1.0, 1.5);
uniform vec3 viewPos = vec3(0.0, 6.0, 4.0);

void main()
{
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

	vec3 norm	  = normalize(Normal);
	vec3 lightDir = normalize(lightPos - WorldPos);
	float diff	  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse  = diff * uLightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 objectColor = vec3(1.0, 1.0, 1.0);
	vec3 result = (ambient + diffuse + specular) * objectColor;

    color = vec4(result, 1.0f);
   //color = vec4(uLightColor, 1.0f);
}