precision mediump float;
uniform sampler2D u_texture;
uniform sampler2D u_dismap;
uniform sampler2D u_mask;
uniform float u_time;

uniform vec3 u_cam;
uniform float u_fogStart;
uniform float u_fogRange;
uniform vec3 u_fogColor;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	vec2 disp = texture2D(u_dismap,vec2(v_uv.x,v_uv.y+u_time)).xy;
	vec2 newuv = v_uv + ( 2.0 * disp - 1.0 ) * 0.2;
	vec4 color = texture2D(u_texture,newuv);
	vec4 alphamask = texture2D(u_mask,v_uv);
	vec4 texel = color * vec4(1.0,1.0,1.0,alphamask.a);

	float Dist = distance(u_cam, v_posL.xyz);
	float FogFact = clamp((Dist - u_fogStart)/u_fogRange,0.0,1.0);
	if (texel.a < 0.1)
		discard;
	gl_FragColor = vec4(mix(texel.rgb,u_fogColor,FogFact),texel.a);
}
