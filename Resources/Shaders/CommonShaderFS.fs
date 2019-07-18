precision mediump float;
uniform sampler2D u_texture;
uniform vec3 u_cam;
uniform float u_fogStart;
uniform float u_fogRange;
uniform vec3 u_fogColor;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	float Dist = distance(u_cam, v_posL.xyz);
	float FogFact = clamp((Dist - u_fogStart)/u_fogRange,0.0,1.0);

	vec4 texel = texture2D(u_texture, v_uv);
	gl_FragColor = vec4(mix(texel.rgb,u_fogColor,FogFact),texel.a);
}
