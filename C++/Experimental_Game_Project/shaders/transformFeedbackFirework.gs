#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in float Type0[];
in vec3 Position0[];
in vec3 Velocity0[];
in float Age0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;

uniform float gLauncherPower;
uniform float gDeltaTimeMillis;
uniform float gTime;
uniform sampler1D gRandomTexture;
uniform float gLauncherLifetime;
uniform float gShellLifetime;
uniform float gSecondaryShellLifetime;
uniform vec3 gLauncherOrientation;
uniform int gExplosionPairs;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

vec3 GetRandomDir(float TexCoord) {
    vec3 Dir = texture(gRandomTexture, TexCoord).xyz;
    Dir -= vec3(0.5, 0.5, 0.5);
    return normalize(Dir);
}

void main() {
	vec3 gravity = vec3(0.0, 0.0, -0.001);

	float Age = Age0[0] + gDeltaTimeMillis;

	if (Type0[0] == PARTICLE_TYPE_LAUNCHER)
	{
	    if (Age > gLauncherLifetime) {
		    Type1 = PARTICLE_TYPE_SHELL;
		    Position1 = Position0[0];
		    Velocity1 = gLauncherPower * normalize(gLauncherOrientation);
		    Age1 = 0.0;
		    Age = 0.0;
	    	
		    EmitVertex();
	    	EndPrimitive();
	    }
	
	    Type1 = Type0[0];
	    Position1 = Position0[0];
	    Velocity1 = Velocity0[0];
	    Age1 = Age;
	    
	    EmitVertex();
	    EndPrimitive();
	    
	} else if (Type0[0] == PARTICLE_TYPE_SHELL) {
		if (Age < gShellLifetime) {	
		    Type1 = Type0[0];
		    Velocity1 = Velocity0[0] + gravity;
		    Position1 = Position0[0] + Velocity0[0] * gDeltaTimeMillis;
	
		    Age1 = Age;
			
		    EmitVertex();
		    EndPrimitive();
	    } else {
	    	Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
	    	Position1 = Position0[0];
	    	Age1 = 0.0;
	    	
	    	float totPairs = gExplosionPairs;
			for (float idx = 0.0; idx < totPairs; idx += 1)
			{
			    Velocity1 = GetRandomDir(gTime + idx / (totPairs + 1)) / 20.0;
		    	EmitVertex();
		    	EndPrimitive();
		    	Velocity1 = -1 * Velocity1;
		    	EmitVertex();
		    	EndPrimitive();
			}
	    }
	} else if (Type0[0] == PARTICLE_TYPE_SECONDARY_SHELL) {
		if (Age < gSecondaryShellLifetime) {
		    Type1 = Type0[0];
		    Velocity1 = Velocity0[0] + gravity;
		    Position1 = Position0[0] + Velocity0[0] * gDeltaTimeMillis;
	
		    Age1 = Age;
			
		    EmitVertex();
		    EndPrimitive();
		}
	}
}
	