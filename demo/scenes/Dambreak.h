

class Dambreak : public Scene
{
public:

	Dambreak(const char* name) : Scene(name) {}

	void Initialize()
	{
		float radius = 0.05f;
		float restDistance = radius*0.5f;

		int dx = 80;
		int dy = 160;
		int dz = 1;

		float radius2 = restDistance*0.5f;

		//make water
		CreateParticleGrid(Vec3(0.5f*restDistance, 0.5f*restDistance, 0.5f*restDistance), dx, dy, dz, radius2, Vec3(0.0f), 1.f/1.f, false, 0.f, NvFlexMakePhase(1, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));

		//right wall
		float thickness = restDistance / 2.f;
		AddBox(Vec3(0.0001f, 4.f/2.f, thickness), Vec3(4.f+0.1f, 4.f / 2.f, thickness));
		//barrier
		AddBox(Vec3(0.16438f/2.f, 0.32876f/2.f, thickness), Vec3(2.f+0.16438f/2.f, 0.32876f / 2.f, thickness));

		//param
		g_numSubsteps = 3;
		g_params.numIterations = 4;
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


		g_maxDiffuseParticles = 64 * 1024;
		g_diffuseScale = 0.5f;
		g_params.diffuseBallistic = 16;
		g_params.diffuseBuoyancy = 1.0f;
		g_params.diffuseDrag = 1.0f;
		
		g_emitters[0].mEnabled = false;

		g_params.numPlanes = 5;

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
		//g_warmup = true;
	}
	virtual void CenterCamera()
	{
		float radius = 0.1f;
		float restDistance = radius*0.55f;

		g_camPos.x = 1.75f;
		g_camPos.y = 0.5f*restDistance*50.f;
		g_camPos.z = restDistance*65.f;
		g_camAngle = Vec3(0.f, 0.f, 0.f);
	}
};