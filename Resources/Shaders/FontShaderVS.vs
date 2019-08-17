attribute vec3 a_posL;
attribute vec2 a_uv;
uniform mat4 u_mvp;

varying vec2 v_uv;

void main()
{
	gl_Position = u_mvp * vec4(a_posL, 1.0);
	v_uv = a_uv;
}
   