// Fragment shader
#version 420

in vec4 fColour;
in vec4 fVertexWorldPos;			// vertex in "world space"
in vec4 fVertexWorldNormal;	
in vec2 fTextureCoords;			// u is x or red, v is y or green

// These are listed in order, starting from zero
out vec4 outputColour;		  // GL_COLOR_ATTACHMENT0// To the frame buffer (aka screen)

in vec4 fBonesId;

uniform bool UseBonesInFragmentShader;

//uniform vec3 directionalLightColour;
// rgb are the rgb of the light colour
//uniform vec4 directionalLight_Direction_power;
// xyz is the normalized direction, w = power (between 0 and 1)

// If true, then passes the colour without calculating lighting
uniform bool bDoNotLight;		// Really a float (0.0 or not zero)

uniform vec4 eyeLocation;

uniform bool bUseDebugColour;	// if this is true, then use debugColourRGBA for the colour
uniform vec4 debugColourRGBA;		

// If FALSE, we use the texture colour as the vertex colour
// (NOT the one from the model file)
uniform bool bUseVertexColours;		// If true, then DOESN'T use texture colours

// Usually, you would pass the alpha transparency as the 4th colour value, 
// 	but for clarity, we'll pass it separately...
uniform float transparencyAlpha;

uniform sampler2D texture_00;			// 2D meaning x,y or s,t or u,v
uniform sampler2D texture_01;
uniform sampler2D texture_02;
uniform sampler2D texture_03;
uniform sampler2D texture_04;			
uniform sampler2D texture_05;
uniform sampler2D texture_06;
uniform sampler2D texture_07;

//
uniform bool bUseHeightMap;
uniform sampler2D heightMapSampler;		// Texture unit 20
uniform sampler2D textureOffScreen;
uniform sampler2D discardSampler;		// Texture unit 21

// Skybox, cubemap, etc.
uniform bool bIsSkyBox;
uniform samplerCube skyBoxTexture;

// For the discard example
uniform bool bUseDiscardMaskTexture;
uniform sampler2D maskSamplerTexture01;

// From the FBO
uniform bool bIsOffScreenTextureQuad;
uniform sampler2D textureOffScreen_0;
uniform sampler2D textureOffScreen_1;
uniform sampler2D textureFBOMixRatio_0_1;
uniform vec2 screenWidthAndHeight;	// x is width

//... and so on
//uniform float textureMixRatio[8];
uniform vec4 textureMixRatio_0_3;		// 1, 0, 0, 0 
uniform vec4 textureMixRatio_4_7;

//uniform samplerCube skyBox;

// flickering
uniform bool isFlickering;
uniform float flickeringAmount;

// static
uniform bool isHavingStatic;
uniform float randomUOffSet;
uniform float randomVOffSet;

uniform bool isFlag;

uniform bool isMap;

uniform bool isRightMap;

uniform bool isFoliage;
uniform float foliageHeight;

uniform bool isFire;

uniform bool isRain;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	// Colour of the light (used for diffuse)
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 50;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 70 uniforms


vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );
							
// Some 2nd pass effects...
vec3 getFBOColour();
vec3 ChromicAberration( float amount, vec4 textureColour );
vec3 BasicBlurScreen();
vec3 BlurScreen(int pixelOffset);
vec3 BlurScreenFaster(int pixelOffset);

vec3 BlurVertexInGame(int pixelOffset, vec4 textureColour);

float DistanceBetweenVec3(vec3 vector1, vec3 vector2);
float DistanceBetweenVec4(vec4 vector1, vec4 vector2);

uniform bool inGame;
uniform vec4 cameraPosition;

void main()
{

//	gl_FragColor = vec4(color, 1.0);

//	if ( bUseHeightMap )
//	{
//		outputColour.rgba = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//		return;
//	}

	// Discard the water
//	if ( bUseHeightMap )
//	{
//		// Range of 0 to 1
//		float height = texture( heightMapSampler, textureCoords.st ).r;
//		
//		if ( height <= 0.005f )
//		{
//			discard;
//		}
//	}



	// offscreen quad
	if ( bIsOffScreenTextureQuad)
	{
		//outputColour.rgb = vec3(1.0f, 0.0f,0.0f);
		
		//vec2 textCoordsScreen = vec2( gl_FragCoord.x / 1920 , gl_FragCoord.y / 1080);
		vec3 theColour = texture( textureOffScreen_0, fTextureCoords.st ).rgb;
		//outputColour.rbg = theColour;
		
		// float brightness = (theColour.r + theColour.g + theColour.b) / 3.0f;
		// outputColour.rgb = vec3( 0.0f, brightness, 0.0f);
		
		theColour.r = texture( textureOffScreen_0, 
							   vec2(fTextureCoords.s + 0.1f, 
							   fTextureCoords.t + 0.1f)).r;
							   
		theColour.g = texture( textureOffScreen_0, 
							   vec2(fTextureCoords.s - 0.1f, 
							   fTextureCoords.t + 0.05f)).g;
							   
		theColour.b = texture( textureOffScreen_0, 
							   vec2(fTextureCoords.s - 0.15f, 
							   fTextureCoords.t - 0.05f)).b;
		
		
		
		theColour = texture ( textureOffScreen_0, 
							  vec2( cos (fTextureCoords.s), sin(fTextureCoords.t))).rgb;
		
		outputColour.rgb = theColour;
		
		outputColour.a = 1.0f;
		//return;
		
		//outputColour.rgb = getFBOColour();
						   
		outputColour.a = 1.0f;
		//return;
	}
	
	// if (isMap)
	// {
		// outputColour.rgb = ChromicAberration(0.5f, );
// //		outputColour.rgb = BasicBlurScreen();

		// // 3 gives this: *** * *** = 7x7= 49
		// // 5 gives this: ***** * ***** = 11x11 = 121 samples
// //		outputColour.rgb = BlurScreen(25);

// //		outputColour.rgb = BlurScreenFaster(5);
		
		// //outputColour.rgb = getFBOColour();
						   
		// outputColour.a = 1.0f;
		// //return;
	// }	
	


	if ( bUseDiscardMaskTexture )
	{
		float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
		// If "black" then discard
		if ( maskValue < 0.01f )
		{
			discard;
//			outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
//			return;
		}	
	}
	
	if ( bIsSkyBox )
	{
		//uniform samplerCube skyBoxTexture;
		vec4 skyBoxSampleColour = texture( skyBoxTexture, fVertexWorldNormal.xyz ).rgba;
		outputColour.rgb = skyBoxSampleColour.rgb;
		outputColour.a = 1.0f;
		return;
	}
	
	// Reflect
	// Reflects based on the eye position
	// genType reflect( genType IncidentAngle, 	genType Nnormal);
//	vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);
//	vec3 reflectAngle = reflect( eyeVector, vertexWorldNormal.xyz);
//	vec3 reflectAngle = refract( eyeVector, vertexWorldNormal.xyz, 	1.333);
//	
//	vec4 skyBoxSampleColour = texture( skyBoxTexture, reflectAngle.xyz ).rgba;
	
//	outputColour.rgb = skyBoxSampleColour.rgb;
//	outputColour.a = 1.0f;
//	return;
	vec4 textureColour;
	
	if (inGame)
	{
		float distanceToCamera = DistanceBetweenVec3(fVertexWorldPos.xyz, cameraPosition.xyz);
		
		//outputColour.rgb = BlurScreen(int(distanceToCamera));
		
		
		
		
		textureColour = texture( texture_00, fTextureCoords.st ).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, fTextureCoords.st ).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;
			
		//textureColour = vec4(BlurVertexInGame(10 , textureColour), 1.0f);
		
		//outputColour.rgb = ChromicAberration( 0.1f, outputColour );
	}
	else
	{
		textureColour = 
			  texture( texture_00, fTextureCoords.st ).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, fTextureCoords.st ).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;
		
		
	}
		


			

	if (isFlickering && isHavingStatic)
	{
		textureColour = 
			  texture( texture_00, vec2(fTextureCoords.s, fTextureCoords.t -  flickeringAmount)).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, vec2(fTextureCoords.s + randomUOffSet, fTextureCoords.t + randomVOffSet)).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;
	
		//outputColour.rgb = theColour.rgb;
		outputColour.a = 1.0f;
		//return;
	}
	else if (isFlickering)
	{
		textureColour = 
			  texture( texture_00, vec2(fTextureCoords.s, fTextureCoords.t -  flickeringAmount)).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, fTextureCoords.st ).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;
	
		//outputColour.rgb = theColour.rgb;
		outputColour.a = 1.0f;
		//return;
	}
	else if (isHavingStatic)
	{
			textureColour = 
			  texture( texture_00, fTextureCoords.st ).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, vec2(fTextureCoords.s + randomUOffSet, fTextureCoords.t + randomVOffSet)).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;
	
		//outputColour.rgb = theColour.rgb;
		outputColour.a = 1.0f;
		//return;
	}

	if (isFire)
	{
		float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
		// If "black" then discard
		if ( maskValue < 0.1f )
		{
			discard;
			//outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
			//return;
		}
		
	}
	
	//textureColour.rgb = BlurScreen(10);
	
	//vec4 vertexRGBA = vec4(BlurScreen(5).rgb, 1.0);
	// Make the 'vertex colour' the texture colour we sampled...
	vec4 vertexRGBA = textureColour;	
	
	if (inGame)
	{
		//vertexRGBA.rgb = ChromicAberration(0.5f, vertexRGBA);
	}
	
	if (isRain)
	{
		//discard;
	
		// float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
		// // if ( maskValue < 0.1f )
		// // {
			// // discard;
			// // //outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
			// // //return;
		// // }

		// if ( maskValue >= 1.0f )
		// {
			// discard;
		// }
		
		// maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).b;
		
		// if (maskValue < 0.6)
		// {
			// discard;
		// }
		vec4 texColor = texture(maskSamplerTexture01, fTextureCoords.st);
    
		// Check if the fragment color is white
		
		if (texColor.r > 0.9 && texColor.g > 0.9 && texColor.b > 0.9)
		{
			discard;
		}
		
		outputColour.b = 1.0f;
		outputColour.r = 0.0f;
		outputColour.g = 0.0f;
		
		// if (texColor.rgb == vec3(1.0, 1.0, 1.0))
		// {
			// // Discard the fragment
			// discard;
		// }
	}


	
	// ...unless we want to use the vertex colours from the model
	if (bUseVertexColours)
	{
		// Use model vertex colour and NOT the texture colour
		vertexRGBA = fColour;
	}
	
	if ( bUseDebugColour )
	{	
		vertexRGBA = debugColourRGBA;
	}
	
	if ( bDoNotLight )
	{
		outputColour = vertexRGBA;
		
		if  (isFlag )
		{
			float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
			// If "white" then discard
			if ( maskValue > 0.9f )
			{
				discard;
			}
			//			outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
			//			return;
		}

		if (isRain)
		{
			outputColour.b = 0.9f;
			outputColour.g -= 0.2f;
			outputColour.r -= 0.2f;
		}
		
		if(isMap)
		{
			outputColour.rgb = ChromicAberration(0.5f, vertexRGBA);
//		outputColour.rgb = BasicBlurScreen();

		// 3 gives this: *** * *** = 7x7= 49
		// 5 gives this: ***** * ***** = 11x11 = 121 samples
//		outputColour.rgb = BlurScreen(25);

//		outputColour.rgb = BlurScreenFaster(5);
		
		//outputColour.rgb = getFBOColour();
						   
			outputColour.a = 1.0f;
			
			return;
		}
		
		if (isRightMap)
		{
			textureColour = 
			  texture( texture_01, fTextureCoords.st ).rgba * 0.5f 	
			+ texture( textureOffScreen_0, fTextureCoords.st ).rgba * 0.5;
			
			outputColour = textureColour;
			
			outputColour.a = 1.0f;
			
			return;
		}
		
		return;
	}
	
	if (inGame)
	{
		//vertexRGBA.rgb = ChromicAberration(0.1f, vertexRGBA);
	}
	

	//vertexRGBA.rgb = BlurScreen(25);
	
	// *************************************
	// Hard code the specular (for now)
	//vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// xyzw or rgba or stuw
	// RGB is the specular highglight colour (usually white or the colour of the light)
	// 4th value is the specular POWER (STARTS at 1, and goes to 1000000s)
	
	vec4 vertexColourLit = calculateLightContrib( vertexRGBA.rgb, fVertexWorldNormal.xyz, 
	                                              fVertexWorldPos.xyz, vertexSpecular );
	// *************************************
			
	outputColour.rgb = vertexColourLit.rgb;
	
	// Real gamma correction is a curve, but we'll Rock-n-Roll it here
	outputColour.rgb *= 1.35f;
	
	// This is where the alpha transparency happens
//	outputColour.a = 1.0f;
	outputColour.a = transparencyAlpha;
	
	if  (isFlag )
	{
		float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
		// If "black" then discard
		if ( maskValue > 0.9f )
		{
			discard;
		}
		//			outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
		//			return;
	}	
	
	// if (isFire)
	// {
		// discard;
	
		// if(outputColour.r == 0.0 && outputColour.g == 0.0 && outputColour.b == 0.0)
		// {
			// discard;
		// }
		// else if (outputColour.r < 0.9)
		// {
			// discard;
		// }
		// else
		// {
			// outputColour.r = 1.0f;
		// }
	// }

	
	if (isFoliage)
	{
		// if (outputColour.g < 0.7f)
		// {
			// outputColour.g = (fVertexWorldPos.y / foliageHeight);
		// }
	}
	
	if (isRain)
	{

	
		if(outputColour.r == 1.0 && outputColour.g == 1.0 && outputColour.b == 1.0)
		{
			discard;
		}
	
		if ( outputColour.a == 0.0 )
		{
			discard;
			//outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
			//return;
		}
	}
	
	if (inGame)
	{
		//outputColour.rgb = ChromicAberration(0.1f, vertexRGBA);
	}

	//outputColour.rgb = BasicBlurScreen();//(10, textureColour);
	
	// DEBUG HACK
//	outputColour.rgb *= 0.0001f;
//	outputColour.rgb += vec3( textureCoords.st, 0.0f);
}

// Does nothing, just samples the FBO texture
vec3 getFBOColour()
{
	vec2 textCoordsScreen = vec2( gl_FragCoord.x / screenWidthAndHeight.x, 
	                              gl_FragCoord.y / screenWidthAndHeight.y );
	vec3 theColour = texture( textureOffScreen_0, textCoordsScreen.st ).rgb;	
	
	return theColour;
}


vec3 ChromicAberration( float amount , vec4 vec)
{
	vec2 textCoordsScreen = vec2( gl_FragCoord.x / screenWidthAndHeight.x, 
	                              gl_FragCoord.y / screenWidthAndHeight.y );
					
	vec3 theColour = vec3(0.0f);
					
	// Red coordinate is off up (-0.025 in y) and to the left (-0.01f in x)
	vec2 redYV = vec2( textCoordsScreen.x - (amount * 0.01f),
					   textCoordsScreen.y - (amount * 0.025f));					   
	theColour.r = texture( textureOffScreen, redYV ).r;	
	
	// Green coordinate is off up (+0.002 in y) and to the right (+0.01f in x)
	vec2 greenYV = vec2( textCoordsScreen.x + (amount * 0.01f),
					     textCoordsScreen.y + (amount * 0.002f));					   
	theColour.g = texture( textureOffScreen, greenYV ).g;	
	
	// Green coordinate is off the left (-0.015f in x)
	vec2 blueYV = vec2( textCoordsScreen.x - (amount * 0.015f),
					     textCoordsScreen.y);					   
	theColour.b = texture( textureOffScreen, blueYV ).b;	
	
	return theColour;
}

vec3 BlurVertexInGame(int pixelOffset, vec4 textureColour)
{
	//   *
	// * O *
	//   *
	// "O" is the pixel we are "on"
	// * are the adjacent pixels
	
	vec2 pixelUV = vec2( gl_FragCoord.x / 800.0, 
	                              gl_FragCoord.y / 600.0 );
					
	vec3 theColourPixel =  texture( texture_00, pixelUV ).rgb;	
				
	// To the left (-ve in X)
	vec2 pixel0 = vec2( (gl_FragCoord.x - 1) / screenWidthAndHeight.x, 
	                     gl_FragCoord.y / screenWidthAndHeight.y );					
	vec3 pixel0Colour =  texture( texture_00, pixel0 ).rgb;	

	// To the right (+ve in X)
	vec2 pixel1 = vec2( (gl_FragCoord.x + 1) / screenWidthAndHeight.x, 
	                     gl_FragCoord.y / screenWidthAndHeight.y );					
	vec3 pixel1Colour =  texture( texture_00, pixel1 ).rgb;	
	
	// Above (-ve in y)
	vec2 pixel2 = vec2(  gl_FragCoord.x / screenWidthAndHeight.x, 
	                     (gl_FragCoord.y - 1) / screenWidthAndHeight.y );					
	vec3 pixel2Colour =  texture( texture_00, pixel2 ).rgb;	
	
	// Below (+ve in y)
	vec2 pixel3 = vec2(  gl_FragCoord.x / screenWidthAndHeight.x, 
	                     (gl_FragCoord.y + 1) / screenWidthAndHeight.y );					
	vec3 pixel3Colour =  texture( texture_00, pixel3 ).rgb;	
	
	// There are five (5) pixels so average by dividing by 5
	vec3 finalColour = vec3(0.0f);
	finalColour += (theColourPixel + pixel0Colour + pixel1Colour + pixel2Colour + pixel3Colour) / 5.0f;

	return finalColour;
}

vec3 BasicBlurScreen()
{
	//   *
	// * O *
	//   *
	// "O" is the pixel we are "on"
	// * are the adjacent pixels
	
	vec2 pixelUV = vec2( gl_FragCoord.x / screenWidthAndHeight.x, 
	                              gl_FragCoord.y / screenWidthAndHeight.y );
					
	vec3 theColourPixel =  texture( textureOffScreen_0, pixelUV ).rgb;	
				
	// To the left (-ve in X)
	vec2 pixel0 = vec2( (gl_FragCoord.x - 1) / screenWidthAndHeight.x, 
	                     gl_FragCoord.y / screenWidthAndHeight.y );					
	vec3 pixel0Colour =  texture( textureOffScreen_0, pixel0 ).rgb;	

	// To the right (+ve in X)
	vec2 pixel1 = vec2( (gl_FragCoord.x + 1) / screenWidthAndHeight.x, 
	                     gl_FragCoord.y / screenWidthAndHeight.y );					
	vec3 pixel1Colour =  texture( textureOffScreen_0, pixel1 ).rgb;	
	
	// Above (-ve in y)
	vec2 pixel2 = vec2(  gl_FragCoord.x / screenWidthAndHeight.x, 
	                     (gl_FragCoord.y - 1) / screenWidthAndHeight.y );					
	vec3 pixel2Colour =  texture( textureOffScreen_0, pixel2 ).rgb;	
	
	// Below (+ve in y)
	vec2 pixel3 = vec2(  gl_FragCoord.x / screenWidthAndHeight.x, 
	                     (gl_FragCoord.y + 1) / screenWidthAndHeight.y );					
	vec3 pixel3Colour =  texture( textureOffScreen_0, pixel3 ).rgb;	
	
	// There are five (5) pixels so average by dividing by 5
	vec3 finalColour = vec3(0.0f);
	finalColour += (theColourPixel + pixel0Colour + pixel1Colour + pixel2Colour + pixel3Colour) / 5.0f;

	return finalColour;
}

vec3 BlurScreen(int pixelOffset)
{
	// pixelOffset = 1   pixelOffset = 2
	// 3x3               5x5
	// * * *             * * * * * 
	// * O *             * * * * * 
	// * * *             * * O * * 
	//                   * * * * * 
	//                   * * * * * 
	// 
	// "O" is the pixel we are "on"
	// * are the adjacent pixels
	
	vec3 outColour = vec3(0.0f);
	int totalSamples = 0;
	
	for ( int xOffset = -pixelOffset; xOffset <= pixelOffset; xOffset++ )
	{
		for ( int yOffset = -pixelOffset; yOffset <= pixelOffset; yOffset++ )
		{
			totalSamples++;
			
			vec2 pixelUV = vec2( (gl_FragCoord.x + xOffset) / screenWidthAndHeight.x, 
	                             (gl_FragCoord.y + yOffset) / screenWidthAndHeight.y );
					
			outColour += texture( textureOffScreen_0, pixelUV ).rgb;	
		}	
	}

	// Average this colour by the number of samples
	outColour.rgb /= float(totalSamples);

	return outColour;
}

vec3 BlurScreenFaster(int pixelOffset)
{
	// pixelOffset = 1   pixelOffset = 2
	// 3x3               5x5
	//   *                   * 
	// * O *                 * 
	//   *               * * O * * 
	//                       *  
	//                       * 
	// 
	// "O" is the pixel we are "on"
	// * are the adjacent pixels
	
	vec3 outColour = vec3(0.0f);
	int totalSamples = 0;
	
	// Add up a horizontal sample
	for ( int xOffset = -pixelOffset; xOffset <= pixelOffset; xOffset++ )
	{
		totalSamples++;
			
		vec2 pixelUV = vec2( (gl_FragCoord.x + xOffset) / screenWidthAndHeight.x, 
	                          gl_FragCoord.y / screenWidthAndHeight.y );
					
		outColour += texture( textureOffScreen_0, pixelUV ).rgb;	
	}
	
	// Add up a vertical sample
	for ( int yOffset = -pixelOffset; yOffset <= pixelOffset; yOffset++ )
	{
		totalSamples++;
			
		vec2 pixelUV = vec2( gl_FragCoord.x / screenWidthAndHeight.x, 
	                         (gl_FragCoord.y + yOffset) / screenWidthAndHeight.y );
					
		outColour += texture( textureOffScreen_0, pixelUV ).rgb;	
	}	


	// Average this colour by the number of samples
	outColour.rgb /= float(totalSamples);

	return outColour;
}

vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)

			// Change made to stop directional light from "overwhelming" other lights
			//return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	//finalObjectColour.a = 1.0f;
	finalObjectColour.a = transparencyAlpha;
	
	return finalObjectColour;
}
float DistanceBetweenVec3(vec3 vector1, vec3 vector2)
{
	float xDifference = (vector2.x - vector1.x) * (vector2.x - vector1.x);
	float yDifference = (vector2.y - vector1.y) * (vector2.y - vector1.y);
	float zDifference = (vector2.z - vector1.z) * (vector2.z - vector1.z);
	return sqrt(xDifference + yDifference + zDifference);
}

float DistanceBetweenVec4(vec4 vector1, vec4 vector2)
{
	float xDifference = (vector2.x - vector1.x) * (vector2.x - vector1.x);
	float yDifference = (vector2.y - vector1.y) * (vector2.y - vector1.y);
	float zDifference = (vector2.z - vector1.z) * (vector2.z - vector1.z);
	float wDifference = (vector2.w - vector1.w) * (vector2.w - vector1.w);
	return sqrt(xDifference + yDifference + zDifference + wDifference);
}


//	// For now, just trust Michael
//	// Very basic directional shader
//	vec3 lightContrib = directionalLightColour * directionalLight_Direction_power.w;
//	// 
//	// Get the dot product of the light and normalize
//	float dotProduct = dot( -directionalLight_Direction_power.xyz,  
//							vertexWorldNormal.xyz );	
//	// Clamp this to a positive number
//	dotProduct = max( 0.0f, dotProduct );		// 0 to 1		
//	
//	lightContrib *= dotProduct;		