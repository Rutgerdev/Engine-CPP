#version 330 // for glsl version (12 is for older versions , say opengl 2.1
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float quadratic;
    float linear;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float innerCircle;
    float outerCircle;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float quadratic;
    float linear;
};

vec3 getDirectionalLight(DirLight light, vec3 n, vec3 view, vec3 diffSample);
vec3 getPointLight(PointLight light, vec3 n, vec3 view, vec3 diffSample);
vec3 getSpotLight(SpotLight light, vec3 n, vec3 view, vec3 diffSample);


uniform int pointLightCount;


uniform PointLight pointLight[50];
uniform SpotLight spotLight;
uniform DirLight dirLight;


uniform bool mat_useSpecMap;
uniform float mat_shininess;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;




uniform vec3 cameraPosition;

in vec3 vertices;
in mat3 TBN;
in vec2 uvs;


out vec4 fragment_color;

void main( void )
{

    vec3 normal = texture(normalMap, uvs).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	//vec3 normN = normalize(normals);

    vec3 viewDirection = normalize(cameraPosition - vertices);
	vec4 sampledDiffuse = texture(diffuseMap, uvs).rgba;


    vec3 finalColor = vec3(0);


	finalColor += getDirectionalLight(dirLight,normal,viewDirection,sampledDiffuse.rgb);
 



    for(int i = 0; i < pointLightCount; i++)
    {
       finalColor += getPointLight(pointLight[i],normal,viewDirection, sampledDiffuse.rgb);
    }
	 
    finalColor += getSpotLight(spotLight, normal, viewDirection,sampledDiffuse.rgb);
	fragment_color = vec4(finalColor,sampledDiffuse.a);

}

vec3 getDirectionalLight(DirLight light, vec3 n, vec3 view, vec3 diffSample)
{
    vec3 lightDirection = normalize(-light.direction);

    vec3 ambientTerm = light.ambient * diffSample;

	float diffuse = max(dot(n,lightDirection),0.f);
    vec3 diffuseTerm = light.diffuse * diffuse * diffSample;

		
	
	vec3 specularTerm = vec3(0,0,0);

 // vec3 reflectDirection = reflect(-lightDirection, n);
 // float specular = dot(view, reflectDirection);
	
	vec3 halfwayDir = normalize(lightDirection + view);
	float specular = dot(n,halfwayDir);
	if(specular > 0)
	{
		specular = pow(specular, 32);
		if(mat_useSpecMap)
		  {
		    specularTerm = light.specular * specular * vec3(texture(specularMap, uvs)) * mat_shininess;
		  }else{
		    specularTerm = light.specular * specular * mat_shininess;
			}
	}
    return (ambientTerm + diffuseTerm + specularTerm);
}


vec3 getPointLight(PointLight light, vec3 n, vec3 view, vec3 diffSample)
{
	vec3 lightDirection = normalize(light.position - vertices);

    vec3 ambientTerm = light.ambient * diffSample;

	float diffuse = max(dot(n,lightDirection),0.f);
    vec3 diffuseTerm = light.diffuse * diffuse * diffSample;


	vec3 specularTerm = vec3(0,0,0);


  //  vec3 reflectDirection = reflect(-lightDirection, n);
  //  float specular = dot(view, reflectDirection);
	
	vec3 halfwayDir = normalize(lightDirection + view);
	float specular = dot(n,halfwayDir);
	if(specular > 0)
	{
		specular = pow(specular, mat_shininess);
		if(mat_useSpecMap)
		  {
		    specularTerm = light.specular * specular * vec3(texture(specularMap, uvs)) * mat_shininess;
		  }else{
		    specularTerm = light.specular * specular * mat_shininess;
			}
	
	}
   
    //Attenuation
    float distance = length(light.position - vertices);
    float attenuation = 1.0f / (1 + 0.22f * distance + 0.20 * pow(distance,2));

    ambientTerm *= attenuation;
    diffuseTerm *= attenuation;
    specularTerm *= attenuation;

    return (ambientTerm + diffuseTerm + specularTerm);
}

vec3 getSpotLight(SpotLight light, vec3 n, vec3 view,vec3 diffSample)
{


    vec3 lightDirection = normalize(light.position - vertices);

    vec3 ambientTerm = light.ambient * diffSample;

	float diffuse = max(dot(n,lightDirection),0.f);
    vec3 diffuseTerm = light.diffuse * diffuse * diffSample;


	vec3 specularTerm = vec3(0,0,0);


  //  vec3 reflectDirection = reflect(-lightDirection, n);
  //  float specular = dot(view, reflectDirection);
	
	vec3 halfwayDir = normalize(lightDirection + view);
	float specular = dot(n,halfwayDir);
	if(specular > 0)
	{
		specular = pow(specular, mat_shininess);
		if(mat_useSpecMap)
		  {
		    specularTerm = light.specular * specular * vec3(texture(specularMap, uvs)) * mat_shininess;
		  }else{
		    specularTerm = light.specular * specular * mat_shininess;
			}
	
	}

    //attenuation
    float distance = length(light.position - vertices);
    float attenuation = 1.f / (1 + light.linear * distance + light.quadratic * pow(distance,2));


    //spot area
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = (light.innerCircle - light.outerCircle);
    float intensity = clamp((theta - light.outerCircle) / epsilon, 0.0f, 1.0f);

    ambientTerm *= intensity * attenuation;
    diffuseTerm *= intensity * attenuation;
    specularTerm *= intensity * attenuation;
    return (ambientTerm + diffuseTerm + specularTerm);
}
