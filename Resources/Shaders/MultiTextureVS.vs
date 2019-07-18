attribute vec3 a_posL;
attribute vec2 a_uv;
uniform mat4 u_mvp;
uniform mat4 m_model;

varying vec4 v_posL;
varying vec2 v_uv;

void main()
{
	gl_Position = u_mvp * vec4(a_posL, 1.0);
	v_uv = a_uv;
	v_posL = m_model * vec4(a_posL, 1.0);
}
   