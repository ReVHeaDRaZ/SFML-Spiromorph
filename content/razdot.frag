#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main(void)
{
  vec2 iMouse=mouse;
  vec2 iResolution=resolution;
  vec2 uv=gl_FragCoord.xy/iResolution.xy;// x: <0, 1>, y: <0, 1>
  uv.x=uv.x-iMouse.x/iResolution.x;
  uv.y=uv.y+iMouse.y/iResolution.y;
  uv.y-=16;// To adjust y position

  uv.x*=iResolution.x/iResolution.y;// x: * aspect ratio, y: >

  float d=length(uv)-.1;// signed distance function

  vec3 col=vec3(0,0,step(0.,-d));// create blue circle with black background

  float glow=.01/d;// create glow and diminish it with distance
  glow=clamp(glow,0.,1.);// remove artifacts

  col+=vec3(0,0,glow*8.);// add glow

  gl_FragColor=vec4(col,1.);// output color
}