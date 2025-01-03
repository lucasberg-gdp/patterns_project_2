#version 420
// Geometry shader
layout(triangles) 			in;
layout(triangle_strip)		out;
layout(max_vertices = 9)	out;

// From the vertex shader
in vec4 gColour[];				// Indexed starting at 0
in vec4 gOriginalPos[];	
in vec4 gVertexWorldPos[];	
in vec4 gVertexWorldNormal[];
in vec2 gTextureCoords[];	

in vec4 gBonesId[];	


// Going to the fragment shader
out vec4 fColour;
out vec4 fOriginalPos;
out vec4 fVertexWorldPos;	
out vec4 fVertexWorldNormal;
out vec2 fTextureCoords;	

out vec4 fBonesId;


uniform bool bIsSkyBox;

uniform bool isFoliage;

uniform float foliageHeight;

uniform float foliageWave;

void main()
{

	// Calculate the normal from this triangle
	vec3 triEdgeA = gVertexWorldPos[2].xyz - gVertexWorldPos[0].xyz;
	vec3 triEdgeB = gVertexWorldPos[1].xyz - gVertexWorldPos[0].xyz;
	
	vec3 newNormal = normalize( cross(triEdgeB, triEdgeA) );	

	fBonesId = gBonesId[0];

	fColour = gColour[0];	

	fOriginalPos = gOriginalPos[0];
	fVertexWorldPos = gVertexWorldPos[0];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[0];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[0];
	}
	fTextureCoords = gTextureCoords[0];		
	gl_Position = gl_in[0].gl_Position;

	EmitVertex();
	
	fBonesId = gBonesId[1];

	fColour = gColour[1];	
	
	fOriginalPos = gOriginalPos[1];
	fVertexWorldPos = gVertexWorldPos[1];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[1];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[1];
	}
	fTextureCoords = gTextureCoords[1];		
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	
	fBonesId = gBonesId[2];

	fColour = gColour[2];	

	fOriginalPos = gOriginalPos[2];
	fVertexWorldPos = gVertexWorldPos[2];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[2];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[2];
	}
	fTextureCoords = gTextureCoords[2];		
	gl_Position = gl_in[2].gl_Position;
	
	EmitVertex();
	
//	if (isFoliage)
//	{
//		float middleXPosition = (gl_in[1].gl_Position.x + gl_in[2].gl_Position.x) / 2.0f;
//		float middleZPosition = (gl_in[1].gl_Position.z + gl_in[2].gl_Position.z) / 2.0f;
//	
//		vec4 leafTop = vec4(middleXPosition + foliageWave,gl_in[0].gl_Position.y + foliageHeight, middleZPosition, gl_in[2].gl_Position.w) ;
//
//		vec3 triEdgeC = leafTop.xyz - gVertexWorldPos[0].xyz;
//		//newNormal = normalize( cross(triEdgeA, triEdgeC) );
//		
//		fColour = vec4(gColour[2].r,gColour[2].g + 0.2f, gColour[2].b, gColour[2].a)  ;				
//
//		fTextureCoords = gTextureCoords[2];		
//	
//
//		gl_Position = leafTop;
//		fVertexWorldPos = leafTop;	
//		
//		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[2];
//		EmitVertex();
//		
//		// vec4 leafTop2 = vec4(gl_in[0].gl_Position.x,leafTop.y,gl_in[0].gl_Position.z, gl_in[0].gl_Position.w) ;
//		// gl_Position = leafTop2;
//		
//		// newNormal = normalize( cross(triEdgeA, triEdgeB) );
//		
//		//EmitVertex();
//	}
	
	EndPrimitive();


}