#version 330 core

in vec3 Normal;
in vec3 FragPos;  
out vec4 outColor;

vec3 objectColor;
vec3 lightColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

vec3 lightPos;
vec3 lightPos2;

void main() {
	lightColor = vec3(1.0f,1.0f,1.0f);
	objectColor = vec3(0.4f, 0.4f, 0.4f);
	lightPos = vec3(4.0f, 4.0f, 4.0f);
	lightPos2 = vec3(4.0f, 4.0f, 4.0f);

	float specularStrength = 0.7f;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

	vec3 viewDir = normalize(lightPos2 - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) *  objectColor;
	outColor =  texture(ourTexture, TexCoord) * vec4(result, 0.2);

}
