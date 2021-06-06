#version 330 core
out vec4 fragColor;

uniform vec2 iResolution;
uniform float iTime;

#define fragCoord gl_FragCoord

#define DRAG_MULT 0.048
#define ITERATIONS_RAYMARCH 13
#define ITERATIONS_NORMAL 48

#define Resolution (iResolution.xy)
#define Time (iTime)

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .01

float sdBox( vec3 p, vec3 b ) {
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdCapsule(vec3 p, vec3 a ,vec3 b, float r){
 	vec3 ab = b-a;
 	vec3 ap = p-a;
    
    float t = dot(ab,ap) / dot(ab,ab);
    t = clamp(t,0.,1.);
    vec3 c = a + t*ab;
    
    return (length(p-c) - r);
}

float sdTorus(vec3 p, vec2 r){
    float x = length(p.xz)-r.x;
    return length(vec2(x,p.y))-r.y;
    
}

float sdPlane(vec3 p, float h){
    return p.y-h;
}

float gyroid(vec3 p,float scale){
    p*= scale;
 	return (abs(dot(sin(p),cos(p.zyx)))-.2)/scale;
}

mat2 rot(float angle) { 
 float c = cos(angle);
 float s = sin(angle);
 return mat2(c,-s,s,c);
}

float getDist(vec3 p){

    float planeDist = sdPlane(p,sin((p.x+iTime)+p.y)/10. - cos(p.z)/10.);

    return planeDist;
    
}

float RayMarch(vec3 ro, vec3 rd){
    float d0=0.;
    
    for(int i=0;i<MAX_STEPS;i++){
        vec3 p = ro + rd*d0;
        float ds = getDist(p);
        d0 += ds;
        if (d0 > MAX_DIST||ds < SURF_DIST) break;
        
    }
    
    return d0;
}

vec3 getNormal (vec3 p){
 float d = getDist(p);
 vec2 e = vec2(.01,0);
 vec3 n = d - vec3(
     getDist(p-e.xyy),
     getDist(p-e.yxy),
     getDist(p-e.yyx)
     );
    
 return normalize(n);
}

float GetLight(vec3 p){
    vec3 lightPos = vec3(0,5,3);

    vec3 l = normalize(lightPos-p);
    vec3 n = getNormal(p);
    float dif = clamp(dot(n,l),0.,1.);
    
    
    float d = RayMarch(p+n*SURF_DIST*2.,l);
    
    if (d<length(lightPos-p)) dif *=.3;
    
    int x = int(p.x+501.+iTime);
    int z = int(p.z+iTime);
    
    
    //Checkerboard pattern on plane
    if (sdPlane(p,sin((p.x+iTime)+p.y)/10. - cos(p.z)/10.)<SURF_DIST)
    if ((x+z)%2==0) dif *= .1;
        
   	dif -= smoothstep(30.,100.,p.z);
    
    return dif; 
}


void main(){

    vec2 uv = (fragCoord.xy-.5*iResolution.xy)/iResolution.y;

    
  
    vec3 col = vec3(0);
    
    vec3 ro = vec3(0,2,0);
   
    
  
    vec3 rd = normalize(vec3(uv.x,uv.y-.2,1));
    
    
    
    
    float d = RayMarch(ro,rd);
    vec3 p = ro + rd * d;
    float dif = GetLight(p);
    
    d /= 12.;
    col = vec3(dif);
    
    fragColor = vec4(col,1.0);
}