
class twoPhaseLiquid : public Scene
{
public:

	twoPhaseLiquid(const char* name) : Scene(name) {}

	void Initialize()
	{
		float radius = 0.05f;
		float restDistance = radius*0.5f;

		int dx = 200;
		int dy = 40;
		int dz = 1; 

		float radius2 = restDistance*0.5f;//2ŽŸŒ³

		//make sludge
		CreateParticleGrid(Vec3(-dx/2*radius2-0.5f*restDistance, 0.5f*restDistance, 0.f), dx/2, dy, dz, radius2, Vec3(0.0f), 1.f/4.f, false, 0.f, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
		//make water
		CreateParticleGrid(Vec3(0.5f*restDistance, 0.5f*restDistance, 0.f), dx/2, dy, dz, radius2, Vec3(0.0f), 1.f/1.f, false, 0.f, NvFlexMakePhase(1, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));

		//param
		g_numSubsteps = 2;
		g_params.numIterations = 3;
		g_windStrength = 0.f;

		//fluidParams
		g_params.radius = radius;
		g_params.dynamicFriction = 0.00f;
		g_params.viscosity = 0.001f;
		g_params.vorticityConfinement = 0.0f;
		g_params.fluidRestDistance = restDistance;
		g_params.relaxationFactor = 1.0f;
		g_params.smoothing = 0.5f;
		g_params.diffuseThreshold *= 0.25f;
		g_params.cohesion = 0.05f;
		
		g_maxDiffuseParticles = 64*1024;
		g_diffuseScale = 0.5f;
		g_params.diffuseBallistic = 16;
		g_params.diffuseBuoyancy = 1.0f;
		g_params.diffuseDrag = 1.0f;

		g_emitters[0].mEnabled = false;

		g_params.numPlanes = 6;

		g_waveFloorTilt = 0.0f;
		g_waveFrequency = 1.5f;
		g_waveAmplitude = 2.0f;

		// draw options		
		g_drawPoints = true;
		g_drawEllipsoids = false;
		g_drawDiffuse = false;
		g_drawMesh = true;
		g_drawDensity = false;
		g_lightDistance = 10.8f;
		g_fluidColor = Vec4(1.f, 1.f, 1.f, 1.0f);
	}
	virtual void CenterCamera()
	{
		float radius = 0.1f;
		float restDistance = radius*0.55f;

		g_camPos.x = 0.f;
		g_camPos.y = 0.5f*restDistance*50.f;
		g_camPos.z = restDistance*65.f;
		g_camAngle = Vec3(0.f, 0.f, 0.f);
	}
};
		