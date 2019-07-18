attribute vec3 a_posL;
uniform mat4 u_mvp;

varying vec4 v_pos;

void main()
{
	gl_Position = u_mvp * vec4(a_posL, 1.0);
	v_pos = vec4(a_posL, 1.0);
}
   