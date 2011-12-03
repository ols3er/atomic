#version 330 compatibility
#pragma include("semantics.h")

#ifdef GLSL_VS
ia_out(GLSL_POSITION)           vec4 ia_VertexPosition;
ia_out(GLSL_NORMAL)             vec3 ia_VertexNormal;
ia_out(GLSL_INSTANCE_POSITION)  vec4 ia_InstancePosition;
#endif
vs_out vec3 vs_VertexPosition;
vs_out vec3 vs_VertexNormal;
vs_out vec4 vs_VertexColor;

#if defined(GLSL_VS)

void main()
{
    vec4 fractionPos = ia_InstancePosition;
    vec4 vert = ia_VertexPosition+fractionPos;
    vert.w = 1.0;

    vs_VertexPosition = vert.xyz;
    vs_VertexNormal = ia_VertexNormal;
    vs_VertexColor = vec4(0.6, 0.6, 0.6, 1.0);
    //gl_Position = ModelViewProjectionMatrix * vert;

    gl_Position = gl_ModelViewProjectionMatrix * vert;
}

#elif defined(GLSL_PS)

ps_out(0) vec4 ps_VertexColor;
ps_out(1) vec4 ps_FragNormal;
ps_out(2) vec4 ps_FragPosition;

void main()
{
    ps_VertexColor = vs_VertexColor;
    ps_FragNormal = vec4(vs_VertexNormal, 1.0);
    ps_FragPosition = vec4(vs_VertexPosition, 1.0);
}

#endif
