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
  uv.y-=16.6;// To adjust y position
  
  uv.x*=iResolution.x/iResolution.y;// x: * aspect ratio, y: >
  
  float d=length(uv)-.1;// signed distance function
  
  vec3 col=vec3(step(0.,-d),0,0);// create red circle with black background
  
  float glow=.01/d;// create glow and diminish it with distance
  glow=clamp(glow,0.,1.);// remove artifacts
  
  col+=vec3(glow*35.,0,0);// add glow
  
  gl_FragColor=vec4(col,1.);// output color
}