#version 430 core

layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

// Can be std430 as well (can only use 430 on buffers, not named uniform blocks)
layout ( std140, binding = 4 ) buffer Pos
{
	vec4 Positions[ ];
};

void main()
{
	// Get the 1D index into this array
	// gl_WorkGroupID.x will be between 0 and 999
	// gl_NumWorkGroups will be 1000 (based on glDispatch(1000,1,1)
	// Or you could use gl_LocalInvocationIndex, which will also be 0 to 999
	
	uint index = gl_GlobalInvocationID.x;
	
	vec4 theValue = Positions[index];
	
	theValue.x *= 2.0f;
	theValue.y = cos(theValue.x);
	theValue.z = sin(theValue.x);
	theValue.w = theValue.x + theValue.y + theValue.x;
	
	Positions[index] = theValue;
	
}
