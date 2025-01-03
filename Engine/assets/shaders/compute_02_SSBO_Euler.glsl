#version 430 core

// https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
//
// Using a Shader Storage Buffer Object, which is a general 
//	buffer of whatever type you'd like
//
layout (local_size_x = 250, local_size_y = 1, local_size_z = 1) in;

struct sPysicsState
{
	// We are only interested in vec3s BUT
	//	the compute buffer is happier with vec4s
	// (and out life will be WAY easier)
	vec4 position;	
	vec4 velocity;
	vec4 acceleration;
};

const uint NUMBER_OF_ENTITIES = 250;

layout(std430, binding = 5) buffer physicInfo
{
	// Simulate 250 entities
    sPysicsState physicData_SSBO[NUMBER_OF_ENTITIES];
};

uniform float deltaTime;

void main()
{
	// Get the 1D index into this array
	// gl_WorkGroupID.x will be between 0 and 249
	// gl_NumWorkGroups will be 240 (based on glDispatch(250,1,1)
	// Or you could use gl_LocalInvocationIndex, which will also be 0 to 249
	
//	int index = int( gl_WorkGroupID.x / gl_NumWorkGroups );
	
	uint index = gl_LocalInvocationIndex;
	
//	struct sPysicsState
//	{
//		vec4 position;	
//		vec4 velocity;
//		vec4 acceleration;
//};	
	vec3 pos = physicData_SSBO[index].position.xyz;
	vec3 vel = physicData_SSBO[index].velocity.xyz;
	vec3 acc = physicData_SSBO[index].acceleration.xyz;
	
//	const float deltaTime = 0.1f;
	
	// Do the Euler integration step
	vec3 deltaVel = acc * deltaTime;
	vel += deltaVel;
	
	vec3 deltaPos = vel * deltaTime;
	pos += deltaPos;
	
	// The ground plane is at y = -50.0
	// Bounce the balls when they hit the ground
	// const float GROUND_PLANE_Y = -50.0f;
	// if ( pos.y <= GROUND_PLANE_Y )
	// {
		// vel.y = abs(vel.y);
	// }
	
	physicData_SSBO[index].position.xyz = pos;
	physicData_SSBO[index].velocity.xyz = vel;
	physicData_SSBO[index].acceleration.xyz = acc;

	return;
}
