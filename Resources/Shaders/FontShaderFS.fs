precision mediump float;
uniform sampler2D u_texture;
uniform float u_alpha;
uniform vec4 textColor;
varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	vec4 texel = vec4(1.0, 1.0, 1.0, texture2D(u_texture, v_uv).w);
	gl_FragColor = textColor * texel;
}
