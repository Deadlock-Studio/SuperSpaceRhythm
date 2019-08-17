precision mediump float;
uniform sampler2D u_texture;
uniform float u_alpha;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	vec4 texel = texture2D(u_texture, v_uv);
	if (texel.a < 0.1)
		discard;
	gl_FragColor = vec4(texel.rgb, u_alpha * texel.a);
}
