#version 430

layout(rgba16f, binding = 0) uniform image2D img_output;

layout(std430,  binding = 0) buffer id_materials { int id_material[];};
layout(std430,  binding = 1) buffer verticez     { vec3 buf_vertex[];};
layout(std430,  binding = 2) buffer vertex_ids   { ivec3 buf_vertex_id[];};
layout(std430,  binding = 3) buffer bvh_nodes    { ivec3 buf_bvh_node[]; };
layout(std430,  binding = 4) buffer bvh_aabbs    { mat2x3 buf_bvh_aabb[]; };
layout(std430,  binding = 5) buffer bvh_ranges   { ivec2 buf_bvh_range[]; };

const float PI    = 3.14159265f;
const float T_MAX = 999999.0f;
// https://stackoverflow.com/questions/16069959/glsl-how-to-ensure-largest-possible-float-value-without-overflow
const float FLT_MAX=3.402823466e+38f;

uniform int tri_num;

const int mtl_num = 2;
const int mtl_jmp = 10;

const float materials[mtl_jmp * mtl_num] = float[](
	1.0f,  1.0f,  1.0f, 0.1f, 0.9f, 0.3f, 20.0f, 0.4f, 0.0f, 1.0f,
	0.25f, 0.87f, 0.5f, 0.1f, 0.9f, 0.3f, 20.0f, 0.1f, 0.4f, 1.2f
);

uniform vec3  eye;   // vec3( -0.75f,  1.40f,  2.25f );
uniform vec3  view;  // horizontalAngle = 2.5f, verticalAngle = -0.5f;
uniform vec3  up;
uniform vec3  light; // vec3( -2.0f, 10.0f, -2.0f );
uniform float fov;   // 90.0f;

struct Ray {
	int   pre_obj, obj;
	float t, i;
	vec3  pos, dir, n;
};

// Returns a random number based on a vec3.
float random(vec3 seed) {
	float dt = dot(seed, vec3(12.9898f, 78.233f, 45.164f)) * 43758.5453f;
	return dt - floor(dt);
}

void TriangleIntersect(inout Ray r,in int obj_id){
	if(obj_id==r.pre_obj)return;
	ivec3 tri_ids=buf_vertex_id[obj_id];
	
	vec3  tri_v1 = buf_vertex[tri_ids.x],
		  tri_v2 = buf_vertex[tri_ids.y],
		  tri_v3 = buf_vertex[tri_ids.z];
	
	vec3  v  = r.pos - tri_v1,
		  t1 = tri_v2 - tri_v1,
		  t2 = tri_v3 - tri_v1;
	
	/*
	// Face Culling
	if(dot(cross(t1, t2), -r.dir) < 0.0f)
	{
		continue;
	}
	*/
	
	float temp = dot(cross(r.dir, t2), t1);
	
	if(temp != 0.0f)
	{
		float s1 = dot(cross(r.dir, t2), v) / temp,
			  s2 = dot(cross(v, t1), r.dir) / temp,
			  t  = dot(cross(v, t1), t2) / temp;
		
		if(t > 0.0f && t < r.t && s1 >= 0.0f && s2 >= 0.0f && s1+s2 <= 1.0f)
		{
			r.obj = obj_id;
			r.t   = t;
		}
	}
}

float AABBIntersect(in Ray r, in int aabb_id){
	// r.pos, r.dir
	const mat2x3 aabb=buf_bvh_aabb[aabb_id];
	const vec3 v1=(aabb[0]-r.pos)/r.dir;
	const vec3 v2=(aabb[1]-r.pos)/r.dir;
	const vec3 mn=min(v1,v2),mx=max(v1,v2);
	const float mn_v=max(max(mn.x,mn.y),mn.z),mx_v=min(min(mx.x,mx.y),min(mx.z,r.t));
	if(mn_v>=mx_v)return FLT_MAX;
	return mn_v;
}

void BVHIntersect(inout Ray r){
#define BVH
#ifdef BVH
	/*if(abs(buf_bvh_aabb[1][0].x-(-1.36719f))<1e-3&&abs(buf_bvh_aabb[1][1].y-(0.984375f))<1e-3){
		for(int i=0;i<tri_num;i++)TriangleIntersect(r,i);
		return;
	}*/
	int id=0;
	uint goto_sibling=0;
	while(true){
		// trace down
		while(true){
			const ivec3 node=buf_bvh_node[id];
			if(node.x==-1){
				for(int i=buf_bvh_range[id].x;i<=buf_bvh_range[id].y;i++)TriangleIntersect(r,i);
				break;
			}
			const float tx=AABBIntersect(r,node.x);
			const float ty=AABBIntersect(r,node.y);
			if(tx==FLT_MAX&&ty==FLT_MAX)break;
			id=tx<=ty?node.x:node.y;
			goto_sibling<<=1;
			if(max(tx,ty)!=FLT_MAX)goto_sibling|=1;
		}
		// trace back
		while(goto_sibling!=0&&(goto_sibling&1)==0){
			goto_sibling>>=1;
			id=buf_bvh_node[id].z;
		}
		if(goto_sibling==0)return;
		const ivec3 node=buf_bvh_node[buf_bvh_node[id].z];
		id=id==node.x?node.y:node.x;
		goto_sibling^=1;
	}
#else
	for(int n = 0; n < tri_num; ++n)
	{
		TriangleIntersect(r,n);
	}
#endif
}

void Intersect(inout Ray r)
{
	BVHIntersect(r);
	if(r.obj != -1)
	{
		int  id = r.obj;
		ivec3 tri_ids=buf_vertex_id[id];
		vec3  tri_v1 = buf_vertex[tri_ids.x],
			  tri_v2 = buf_vertex[tri_ids.y],
			  tri_v3 = buf_vertex[tri_ids.z];
		
		vec3 t1 = tri_v2 - tri_v1,
			 t2 = tri_v3 - tri_v1;
		r.n = normalize(cross(t1, t2));
	}
}

vec3 PhongLighting(Ray ray_hit, float rate)
{
	int   id  = id_material[ray_hit.obj] * mtl_jmp;
	float r   = materials[id+0], g  = materials[id+1], b  = materials[id+2], ia = 1.0f,
		  ka  = materials[id+3], kd = materials[id+4], ks = materials[id+5], exp = materials[id+6];
	vec3  p   = ray_hit.pos + ray_hit.dir * ray_hit.t;
	vec3  l   = normalize(light - p);
	vec3  h   = normalize(-ray_hit.dir + l);
	vec3  rgb = vec3(0.0f);
	
	Ray ray_light = Ray(ray_hit.obj, -1, T_MAX, ray_hit.i, p, l, vec3(0.0f));
	Intersect(ray_light);
	if(ray_light.obj != -1)
	{
		rgb = vec3(r,g,b) * 0.1f;
	}
	else
	{
		float cosDiffuse  = clamp(dot(ray_hit.n, l), 0.0f, 1.0f);
		float cosSpecular = pow(clamp(dot(ray_hit.n, h), 0.0f, 1.0f), exp);
		//float intensity = (ka*ia + kd*cosDiffuse + ks*cosSpecular);
		//rgb =  vec3(clamp(r*intensity, 0.0f, 1.0f), clamp(g*intensity, 0.0f, 1.0f), clamp(b*intensity, 0.0f, 1.0f));
		
		
		vec3  Diffuse  = vec3(r,g,b);
		vec3  Ambient  = Diffuse * 0.1f;
		vec3  Specular = vec3(0.3f);
		float distance = length(light - p), LightPower = 100.0f;
		rgb =
			Ambient  +
			Diffuse  * LightPower * cosDiffuse  / (distance * distance) +
			Specular * LightPower * cosSpecular / (distance * distance);
		
	}
	
	return rgb * (ray_hit.i * rate);
}

vec3 RayTracing(Ray ray_trace)
{
	vec3 rgb = vec3(0.0f);
	
	while(ray_trace.i > 0.1f)
	{
		// Intersect
		Intersect(ray_trace);
		
		if(ray_trace.obj != -1)
		{
			int   id = id_material[ray_trace.obj] * mtl_jmp;
			float reflect = materials[id+7], refract = materials[id+8], nr = materials[id+9];
			
			float random_num = random(ray_trace.dir + vec3( reflect, refract, nr));
			
			if((reflect > 0.0f && refract <= 0.0f) || (dot(-ray_trace.dir, ray_trace.n) > 0.0f && random_num > 0.5f))
			{
				rgb += PhongLighting(ray_trace, 1.0f - reflect);
				
				ray_trace.pre_obj = ray_trace.obj;
				ray_trace.obj     = -1;
				ray_trace.pos     = ray_trace.pos + ray_trace.dir * ray_trace.t;
				ray_trace.dir     = ray_trace.dir + 2.0f * dot(-ray_trace.dir, ray_trace.n) * ray_trace.n;
				ray_trace.t       = T_MAX;
				ray_trace.n       = vec3(0.0f);
				ray_trace.i       = reflect * ray_trace.i;
			}
			else if(refract > 0.0f)
			{
				rgb += PhongLighting(ray_trace, 1.0f - refract);
				
				vec3 new_pos = ray_trace.pos + ray_trace.dir * ray_trace.t;
				Ray ray_test = Ray(ray_trace.obj, -1, T_MAX, refract * ray_trace.i, new_pos, ray_trace.dir, vec3(0.0f));
				Intersect(ray_test);
				nr = dot(-ray_test.dir, ray_test.n) > 0.0f ? 1.0f/nr : nr;
				vec3 temp = nr * (ray_trace.dir - ray_trace.n * dot(ray_trace.dir, ray_trace.n));
				if(length(temp) < 1.0f)
				{
					ray_trace.pre_obj = ray_trace.obj;
					ray_trace.obj     = -1;
					ray_trace.pos     = new_pos;
					ray_trace.dir     = temp - ray_trace.n * sqrt(1 - length(temp) * length(temp));
					ray_trace.t       = T_MAX;
					ray_trace.n       = vec3(0.0f);
					ray_trace.i       = refract * ray_trace.i;
				}
				else
				{
					break;
				}
			}
			else
			{
				rgb += PhongLighting(ray_trace, 1.0f);
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	return rgb;
}

void main()
{
	// Get Index in Global Work Group
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	
	ivec2 dims   = imageSize(img_output);
	vec3  dx     = normalize(cross(view, up));
    vec3  dy     = normalize(cross(view, dx));
	vec3  center = eye + view * (dims.x / 2) / tan((fov / 2) / 360 * 2 * PI);
	float x =  float(pixel_coords.x * 2 - dims.x) / 2.0f;
	float y = -float(pixel_coords.y * 2 - dims.y) / 2.0f;
	
	// Generate Ray
	vec3 pos = eye;
	vec3 dir = normalize(center + x * dx + y * dy - eye);
	Ray  ray_start = Ray(-1, -1, T_MAX, 1, pos, dir, vec3(0.0f));
	
	// Ray Tracing
	vec4 pixel = vec4(RayTracing(ray_start), 1.0f);
	
	// Output Color
	imageStore(img_output, pixel_coords, pixel);
}
