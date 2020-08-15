#version 150 core

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D shadowTexture;

// light camera uniforms
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// view camera uniforms
uniform mat4 cameraView;
uniform mat4 cameraProj;

in vec2 texCoord;

out vec4 fragColor;

vec3 WorldPosFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    mat4 projMatrixInv = inverse(cameraProj);
    mat4 viewMatrixInv = inverse(cameraView);

    vec4 clipSpacePosition = vec4(texCoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = projMatrixInv * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition =  viewMatrixInv * viewSpacePosition;
//    worldSpacePosition = inverseModelMatrix * worldSpacePosition;
    worldSpacePosition /= worldSpacePosition.w;

    return worldSpacePosition.xyz;
}

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}


float CalcShadowFactor(vec4 LightSpacePos)
{
  float bias = 0.00005f;
  vec2 texelSize = 1.0 / textureSize(shadowTexture, 0);
  vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
  vec2 UVCoords;
  UVCoords.x = 0.5 * ProjCoords.x + 0.5;
  UVCoords.y = 0.5 * ProjCoords.y + 0.5;
  float z = 0.5 * ProjCoords.z + 0.5;

  // percentage close filtering of the shadow map
  float shadow = 0.0;
  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
    {
      float pcfDepth = texture(shadowTexture, UVCoords + vec2(x, y) * texelSize).r;
      shadow += z - bias > pcfDepth ? 0.5 : 1.0;
    }
  }
  return shadow / 9.0f;
}

vec3 to_color(vec3 v)
{
  return (normalize(v) + 1.0f) / 2.0f;
}

void main()
{
  vec3 worldPosition = WorldPosFromDepth(texture(depthTexture, texCoord).r);
  vec4 positionInLightSpace = projectionMatrix * viewMatrix * vec4(worldPosition, 1.0f);
  float shadowFactor = CalcShadowFactor(positionInLightSpace);
  fragColor = vec4(shadowFactor * texture(colorTexture, texCoord).rgb, 1.0f);
}
