// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;			// Later...
uniform mat4 matModel_IT;		// Inverse transpose of the model matrix


//uniform vec3 modelScale;
//uniform vec3 modelOffset;

in vec4 vPos;		
in vec4 vCol;
in vec4 vNormal;	
in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4
// in vec4 vBoneIds;		// Bones
// in vec4 vBoneWeights;
// uniform mat4 BoneMatrices[5];
// out vec4 ex_BoneId;

//out vec4 fColour;
//out vec4 fVertexWorldPos;	
//out vec4 fVertexWorldNormal;
//out vec2 fTextureCoords;		// ADDED

out vec4 gColour;
out vec4 gVertexWorldPos;	
out vec4 gVertexWorldNormal;
out vec2 gTextureCoords;		// ADDED


out vec4 gBonesId;

uniform bool bUseHeightMap;
uniform sampler2D heightMapSampler;		// Texture unit 20
uniform float heightScale;
uniform vec2 UVOffset;

in vec4 vBoneIds;		// Bones
in vec4 vBoneWeights;
uniform mat4 BoneMatrices[150];
out vec4 fBoneId;
uniform bool UseBones;

uniform bool isFire;

uniform vec4 randomPos;

void main()
{
	vec4 position = vPos;
	vec4 normal = vec4(0.0);

	mat4 boneTransform = mat4(0.0);

	if (UseBones)
	{
		for(int i = 0 ; i < 4 ; i++)
		{
		    if( vBoneIds[i] >= 150) 
			{
				//totalPosition = vec4(aPos,1.0f);
				break;
			}
		
			boneTransform += BoneMatrices[int(vBoneIds[i])] * vBoneWeights[i];
			
			// vec4 localNormal = BoneMatrices[int(vBoneIds[i])] * vec4(vNormal.xyz,0.0f);
			// normal += localNormal * vBoneWeights[i];
		}
		
		position = boneTransform * vec4(vPos.xyz, 1.0);
	}
	
	gBonesId = vBoneIds;

	vec4 vertexModelPosition = position;	
	
	vec2 UVFinal = vTextureCoords.st;
	
	if ( bUseHeightMap )
	{
		// Greyscale (black and white) heightmap image
		// NvF5e_height_map.bmp
//		vec3 heightRGB = texture( heightMapSampler, vTextureCoords.st ).rgb;
//		float height = (heightRGB.r + heightRGB.g + heightRGB.b) / 3.0f;
		UVFinal += UVOffset.yx;
		float height = texture( heightMapSampler, UVFinal.st ).r;
		
		// Adjust the height of the y axis from this "colour"
//		const float SCALEADJUST = 25.0f;
//		vertexModelPosition.y += ( height * SCALEADJUST );
		vertexModelPosition.y += ( height * heightScale );
	}
	
	
//	gl_Position = MVP * vec4(finalPos, 1.0);
//	gl_Position = MVP * vertModelPosition;
	mat4 matMVP = matProjection * matView * matModel;
	//gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0);
	//gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0);


	//vec4 position = BoneMatrices[int(vBoneIds[0])];

	// if (UseBones)
	// {
		// if (isFire)
		// {
			// gl_Position = matMVP * 
			// vec4(position.x, position.y + 100.0, position.z, 1.0);
		// }
		// else
		// {
			// gl_Position = matMVP * vec4(position.xyz, 1.0f);
		// }
	// }
	// else
	// {
		// if (isFire)
		// {
			// gl_Position = matMVP * 
			// vec4(vertexModelPosition.x + randomPos.x, vertexModelPosition.y + randomPos.y, vertexModelPosition.z + randomPos.z, 1.0) ;
		// }
		// else
		// {
			// gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0) ;
		// }
	// }
	
	gl_Position = matMVP * position;

		
	// Rotate the normal by the inverse transpose of the model matrix
	// (so that it only is impacted by the rotation, not translation or scale)
	gVertexWorldNormal = matModel_IT * vec4(vNormal.xyz, 1.0f);
	gVertexWorldNormal.xyz = normalize(gVertexWorldNormal.xyz);
	gVertexWorldNormal.w = 1.0f;
	
	if (UseBones)
	{
		gVertexWorldNormal = normalize(normal);
	}
	
	//gVertexWorldPos = matModel * position * vec4( vertexModelPosition.xyz, 1.0f);
	gVertexWorldPos = matModel * vec4( vertexModelPosition.xyz, 1.0f);

	// = matModel * vec4( position.xyz, 1.0f);
	//gVertexWorldPos = matModel * vec4( position.xyz, 1.0f);
	//vec4 position = BoneMatrices[int(vBoneIds[0])] * vPos;
	//gl_Position = matModel * position * vec4( vertexModelPosition.xyz, 1.0f) ;
	//gVertexWorldPos = matMVP * vec4(position.xyz, 1.0);

	gColour = vCol;
	
	// Copy the UV coordinates unchanged (to the fragment shader)
//	textureCoords = vTextureCoords;
	gTextureCoords = UVFinal;
	
	
	//gl_Position = matMVP * position;
}
