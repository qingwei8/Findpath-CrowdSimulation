#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos = vec3(0.0, 1.5, -2.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.8);
uniform vec3 viewPos = vec3(0.0, 6.0, 4.0);

//uniform sampler2D texture_diffuse2;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

void main()
{    
   //color = vec4(texture(texture_diffuse1, TexCoords));
   //color = vec4(0.8, 1.0, 0.8, 1.0);
   //color = vec4(texture(texture_diffuse1, TexCoords)) + vec4(texture(texture_diffuse2, TexCoords)) + vec4(texture(texture_diffuse3, TexCoords)) + vec4(texture(texture_specular1, TexCoords))+ vec4(texture(texture_specular2, TexCoords));
   //vec4 objectcolor = vec4(texture(texture_diffuse1, TexCoords));
   //color = vec4(texture(texture_diffuse1, TexCoords));

	float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * vec3(0.7, 1.0, 0.79);

	vec3 norm	  = normalize(Normal);
	vec3 lightDir = normalize(lightPos - WorldPos);
	float diff	  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse  = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 objectColor = vec3(1.0, 1.0, 1.0);
	vec3 result = (ambient + diffuse + specular) * objectColor;

    color = vec4(result, 1.0f);
}