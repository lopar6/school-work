in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing  color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 fillLightColor;
uniform vec3 fillLightPos;
uniform vec3 fillLightScale;
uniform vec3 keyLightColor;
uniform vec3 keyLightPos;
uniform vec3 keyLightScale;
uniform vec3 viewPosition;
uniform vec2 uvScale;
//uniform sampler2D sceneTexture;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int textureIndex;

void main()
{
//Calculate Ambient lighting
float ambientStrength = 0.4f; // Set ambient or global lighting strength
vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); // Generate ambient light color

//Calculate Diffuse lighting*/
vec3 norm = normalize(vertexNormal);
vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
float fillImpact = max(dot(norm, fillLightDirection), 0.0);
vec3 keyLightDirection = normalize(keyLightPos - vertexFragmentPos);
float keyImpact = max(dot(norm, keyLightDirection), 0.0);
vec3 diffuse = (fillLightColor * fillImpact * fillLightScale) + (keyLightColor * keyImpact * keyLightScale);
vec4 textureColor = texture(objectColor, vertexTextureCoordinate * uvScale);

// Texture holds the color to be used for all three components
//vec4 textureColor;
//switch (textureIndex) {
//case 0:
//	textureColor = texture(texture0, vertexTextureCoordinate * uvScale);
//	break;
//case 1:
//	textureColor = texture(texture1, vertexTextureCoordinate * uvScale);
//	break;
//case 2:
//	textureColor = texture(texture2, vertexTextureCoordinate * uvScale);
//	break;
//}

// Calculate phong result
vec3 phong = (ambient + diffuse) * textureColor.xyz;

fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}