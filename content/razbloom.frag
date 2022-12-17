#version 120
uniform sampler2D currentTexture;
uniform float time;
uniform vec2 resolution;
uniform float intensity;
uniform float blurSizeAmount;
//const float blurSize=1./640.;
//const float intensity=2.95;

void main(){
	vec2 iResolution=resolution;
	vec4 sum=vec4(0);
	vec2 texcoord=gl_FragCoord.xy/iResolution.xy;
	float blurSize= 1. / blurSizeAmount;

	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y))*.20;

	sum+=texture2D(currentTexture,vec2(texcoord.x+blurSize,texcoord.y))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x-blurSize,texcoord.y))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y+blurSize))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y-blurSize))*.12;

	sum+=texture2D(currentTexture,vec2(texcoord.x+blurSize,texcoord.y+blurSize))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x+blurSize,texcoord.y-blurSize))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x-blurSize,texcoord.y+blurSize))*.12;
	sum+=texture2D(currentTexture,vec2(texcoord.x-blurSize,texcoord.y-blurSize))*.12;

	sum+=texture2D(currentTexture,vec2(texcoord.x+2.*blurSize,texcoord.y))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x-2.*blurSize,texcoord.y))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y+2.*blurSize))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y-2.*blurSize))*.09;

	sum+=texture2D(currentTexture,vec2(texcoord.x+2.*blurSize,texcoord.y+2*blurSize))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x+2.*blurSize,texcoord.y-2*blurSize))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x-2*blurSize,texcoord.y+2.*blurSize))*.09;
	sum+=texture2D(currentTexture,vec2(texcoord.x-2*blurSize,texcoord.y-2.*blurSize))*.09;

	sum+=texture2D(currentTexture,vec2(texcoord.x+3.*blurSize,texcoord.y))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x-3.*blurSize,texcoord.y))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y+3.*blurSize))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y-3.*blurSize))*.07;

	sum+=texture2D(currentTexture,vec2(texcoord.x+3.*blurSize,texcoord.y+3*blurSize))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x+3.*blurSize,texcoord.y-3*blurSize))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x-3*blurSize,texcoord.y+3.*blurSize))*.07;
	sum+=texture2D(currentTexture,vec2(texcoord.x-3*blurSize,texcoord.y-3.*blurSize))*.07;

	sum+=texture2D(currentTexture,vec2(texcoord.x+4.*blurSize,texcoord.y))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x-4.*blurSize,texcoord.y))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y+4.*blurSize))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y-4.*blurSize))*.05;

	sum+=texture2D(currentTexture,vec2(texcoord.x+4.*blurSize,texcoord.y+4*blurSize))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x+4.*blurSize,texcoord.y-4*blurSize))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x-4*blurSize,texcoord.y+4.*blurSize))*.05;
	sum+=texture2D(currentTexture,vec2(texcoord.x-4*blurSize,texcoord.y-4.*blurSize))*.05;

	sum+=texture2D(currentTexture,vec2(texcoord.x+5.*blurSize,texcoord.y))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x-5.*blurSize,texcoord.y))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y+5.*blurSize))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x,texcoord.y-5.*blurSize))*.04;

	sum+=texture2D(currentTexture,vec2(texcoord.x+5.*blurSize,texcoord.y+5*blurSize))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x+5.*blurSize,texcoord.y-5*blurSize))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x-5*blurSize,texcoord.y+5.*blurSize))*.04;
	sum+=texture2D(currentTexture,vec2(texcoord.x-5*blurSize,texcoord.y-5.*blurSize))*.04;


	//increase blur with intensity!
	gl_FragColor=sum*intensity+texture2D(currentTexture,texcoord);

}
