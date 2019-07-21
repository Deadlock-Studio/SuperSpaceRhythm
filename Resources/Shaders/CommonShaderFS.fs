precision mediump float;
uniform sampler2D u_texture;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	vec4 texel = texture2D(u_texture, v_uv);
	if (texel.a < 0.1)
		discard;
	gl_FragColor = texel;
}
