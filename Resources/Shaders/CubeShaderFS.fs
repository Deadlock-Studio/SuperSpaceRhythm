precision mediump float;
uniform samplerCube u_cubeTexture;

varying vec4 v_pos;

void main()
{
	gl_FragColor = textureCube(u_cubeTexture, v_pos.xyz);
}
