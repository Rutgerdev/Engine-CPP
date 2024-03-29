//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
uniform vec3 globalAmbientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPosition;



in vec3 vertexWorldPos;
in vec3 normalWorldPos;

out vec4 fragment_color;


void main( void ) {
    //ambient
    vec3 ambientTerm = globalAmbientColor * diffuseColor;

    //diffuse
    vec3 lightDirection = normalize(lightPos - vertexWorldPos);
    float diffuseIntensity = max(dot(normalize(normalWorldPos), lightDirection),0.0f);
    vec3 diffuseTerm = diffuseIntensity * lightColor * diffuseColor;

    //specular
    vec3 viewDirection = normalize(cameraPosition - vertexWorldPos);
    vec3 reflectDirection = reflect(-lightDirection, normalize(normalWorldPos));
    float specularIntensity = pow(max(dot(reflectDirection, viewDirection), 0.f), 32);
    float specScale = 0.5f;
    vec3 specularTerm = specScale * specularIntensity * lightColor * diffuseColor;

    vec3 finalColor = ambientTerm + diffuseTerm + specularTerm;

    fragment_color = vec4(finalColor, 1.f);
}
