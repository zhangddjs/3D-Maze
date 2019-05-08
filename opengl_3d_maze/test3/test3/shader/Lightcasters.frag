#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
   //环境光照
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

   //漫反射
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

   //镜面高光
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   //衰减
   float distance = length(light.position - FragPos);
   float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*(distance*distance));

   //emission
   vec3 emission = vec3(texture(material.emission, TexCoords));

   vec3 result = (ambient + diffuse + specular) * attenuation;
   color = vec4(result, 1.0f);
}