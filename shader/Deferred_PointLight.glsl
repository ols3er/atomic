#version 330 core
#pragma include("Common.glslh")

#if defined(GLSL_VS)
ia_out(GLSL_POSITION)           vec4 ia_VertexPosition;
ia_out(GLSL_NORMAL)             vec4 ia_VertexNormal;
ia_out(GLSL_INSTANCE_POSITION)  vec4 ia_InstancePosition;
ia_out(GLSL_INSTANCE_COLOR)     vec4 ia_InstanceColor;
#endif
#if defined(GLSL_VS) || defined(GLSL_PS)
vs_out vec4 vs_LightPosition;
vs_out vec4 vs_LightColor;
vs_out vec4 vs_VertexPositionMVP;
#endif
const float u_LightSize     = 1.0;
const float u_LightRange    = u_LightSize*0.98;
const float u_RcpLightRange = 1.0/u_LightRange;

#if defined(GLSL_VS)

void main()
{
    vs_LightPosition = ia_InstancePosition*1.1;
    vs_LightPosition.z *= 3.0;
    vs_LightPosition.z += 0.1;
    vs_LightPosition.w = 0.0;

    vec4 scaled_position = ia_VertexPosition * vec4(u_LightSize, u_LightSize, u_LightSize, 1.0);

    vs_LightColor = vec4(0.1, 0.1, 0.2, 0.1)+normalize(vs_LightPosition)*0.7;
    vs_VertexPositionMVP = u_RS.ModelViewProjectionMatrix * (scaled_position+vs_LightPosition);
    gl_Position = vs_VertexPositionMVP;
}

#elif defined(GLSL_PS)

ps_out(0)   vec4 ps_FragColor;

void main()
{
    vec2 coord;
    coord.x = (1.0 + (vs_VertexPositionMVP.x/vs_VertexPositionMVP.w))*0.5;
    coord.y = (1.0 + (vs_VertexPositionMVP.y/vs_VertexPositionMVP.w))*0.5;
    coord *= u_RS.ScreenTexcoord;

    vec4 NS         = texture(u_NormalBuffer, coord);
    vec3 Normal     = NS.xyz;
    vec3 FragPos    = texture(u_PositionBuffer, coord).xyz;
    vec3 EyePos     = u_RS.CameraPosition.xyz;
    vec3 EyeDir     = normalize(EyePos - FragPos);
    float Shininess = NS.w;

    vec3  LightColor    = vs_LightColor.rgb;
    vec3  LightPos      = vs_LightPosition.xyz;
    vec3  LightDiff     = vs_LightPosition.xyz - FragPos.xyz;
    float LightDist2    = dot(LightDiff, LightDiff);
    float LightDist     = sqrt(LightDist2);
    vec3  LightDir      = LightDiff / LightDist;

    float LightAttenuation  = max(u_LightRange-LightDist, 0.0)*u_RcpLightRange * 1.5;

    vec3 h          = normalize(EyeDir + LightDir);
    float nh        = max(dot(Normal, h), 0.0);
    float Specular  = pow(nh, Shininess);
    float Intensity = max(dot(Normal, LightDir), 0.0);

    vec3 Albedo     = texture(u_ColorBuffer, coord).rgb;
    vec3 Ambient    = normalize(FragPos)*0.05;
    vec4 Result = vec4(0.0, 0.0, 0.0, 1.0);
    Result.rgb += vs_LightColor.rgb * (Ambient + Albedo * Intensity) * LightAttenuation;
    Result.rgb += Result.rgb * Specular * LightAttenuation;
    //Result = vec4(0.1, 0.1, 0.1, 1.0);

    ps_FragColor = Result;
}

#endif