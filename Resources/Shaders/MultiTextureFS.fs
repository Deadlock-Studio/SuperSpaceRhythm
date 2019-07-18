precision mediump float;
uniform sampler2D u_blendmap;
uniform sampler2D u_rock;
uniform sampler2D u_dirt;
uniform sampler2D u_grass;
uniform vec3 u_cam;
uniform float u_fogStart;
uniform float u_fogRange;
uniform vec3 u_fogColor;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	vec4 blendmap = texture2D(u_blendmap, v_uv);
	vec4 rock = texture2D(u_rock, v_uv);
	vec4 dirt = texture2D(u_dirt, v_uv);
	vec4 grass = texture2D(u_grass, v_uv);
	vec4 texel = blendmap.x * rock + blendmap.y * dirt + blendmap.z * grass;

	float Dist = distance(u_cam, v_posL.xyz);
	float FogFact = clamp((Dist - u_fogStart)/u_fogRange,0.0,1.0);

	gl_FragColor = vec4(mix(texel.rgb,u_fogColor,FogFact),texel.a);
}
