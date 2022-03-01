

class StirredTankSmallRadius : public Scene
{
public:

	StirredTankSmallRadius(const char* name) : Scene(name) {}

	void Initialize()
	{
		const float radius = 0.0049625f * 2.f;
		const float restDistance = radius*0.6f;

		float center_y = 0.23f / 2.f;
		Vec3 center = Vec3(0, center_y, 0);
		float scale = 0.3f;//実実験の深さ(0.15)/blender(0.5)

		Quat rot = QuatFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), M_PI / 2.f); //部品を回転させたいときに使う

		Mesh* paddle = ImportMesh("../../data/paddle2_3D_2.obj");
		Mesh* container = ImportMesh("../../data/container_3D_mixing_2.obj");
		Mesh* buffle = ImportMesh("../../data/buffle_3D.obj");

		mesh = CreateTriangleMesh(paddle);
		AddTriangleMesh(mesh, Vec3(0.f, 0.15f * 0.4f, 0.f), Quat(), Vec3(0.35f));
		mesh1 = CreateTriangleMesh(container);
		AddTriangleMesh(mesh1, Vec3(center), Quat(), Vec3(1.f));
		mesh2 = CreateTriangleMesh(buffle);
		AddTriangleMesh(mesh2, Vec3(0.075f-0.0075f, center_y - 0.04f, 0), Quat(), Vec3(scale));
		mesh3 = CreateTriangleMesh(buffle);
		AddTriangleMesh(mesh2, Vec3(-0.075f + 0.0075f, center_y - 0.04f, 0), Quat(), Vec3(scale));
		mesh4 = CreateTriangleMesh(buffle);
		AddTriangleMesh(mesh2, Vec3(0, center_y - 0.04f, 0.075f - 0.0075f), Quat(rot), Vec3(scale));
		mesh5 = CreateTriangleMesh(buffle);
		AddTriangleMesh(mesh2, Vec3(0, center_y - 0.04f, -0.075f + 0.0075f), Quat(rot), Vec3(scale));

		//トレーサー
		CreateParticleGrid(Vec3(-restDistance * (6.f - 1.f) / 2.f,  restDistance * 2.f, -restDistance * (6.f - 1.f) / 2.f), 6, 5, 6, restDistance, Vec3(0.f), 1.f, false, 0.0f, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));

		//充填粒子
		int width = 18;
		int base = 1;
		CreateParticleGrid(Vec3(-0.075f, 0.0f - restDistance * (base - 1), -0.075f), width, base, width, restDistance, Vec3(0.0f), 0.0f, false, 0.0f, NvFlexMakePhase(0, 0), 0.0f);

		//充填
		float asobi = 0.075f * 0.625f;
		float min_xz = -0.075f + asobi;
		float max_xz = 0.075f - asobi;
		float min_y = 0.f;
		float max_y = 0.15f + asobi * 0.f;
		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr0(min_xz, max_xz);
		uniform_real_distribution<float> distr1(min_y, max_y);
		//CreateParticleGrid(Vec3(0.25f - restDistance*10.f, center_y * 2 + restDistance*2.f, restDistance*2.5f), 5, 10, 5, restDistance, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid), restDistance*0.01f);
		//int numP = 10751;
		int numP = 5162 * 3 - 180;
		//int numP = 0;
		for (int i = 0; i < numP; i++) {
			float random_x = distr0(eng);
			float random_z = distr0(eng);
			if (pow(0.075f, 2) > pow(random_x, 2) + pow(random_z, 2)) {
				CreateParticleGrid(Vec3(random_x, distr1(eng), random_z), 1, 1, 1, restDistance, Vec3(0.0f), 1.f, false, 0.0f, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
			}
			else {
				i--;
			}
		}

		g_params.radius = radius;
		g_params.fluidRestDistance = restDistance;

		//変えれるパラメータ(もともとこのシーンにあった変数)
		g_numSubsteps = 10;
		g_params.numIterations = 30;
		g_params.dynamicFriction = 0.f;
		g_params.collisionDistance = 0.01015625f / 2.f;//壁との距離...すでに小さくしてる...デフォルトは0.05f
		g_params.viscosity = 0.001f;//粘度...0.001fは水の20℃の粘度に相当
		g_params.cohesion = 0.025f;//凝集力...妥当な数字なのかわからない
		g_params.surfaceTension = 0.0f;//デフォルトはゼロ
		g_params.vorticityConfinement = 120.f;
		g_params.damping = 0.f;
		//g_params.maxSpeed = g_numSubsteps*restDistance/g_dt;//ある速度をそのシーンのマックススピードとする、解除した

		//変えれるパラメータ(もともとこのシーンになかった変数)
		//g_params.adhesion = 0.0f;//壁への付着力
		//g_params.solidPressure = 1.0f;//固体表面からの圧力
		//g_params.staticFriction = 0.0f;
		//g_params.particleFriction = 0.0f; // scale friction between particles by default
		//g_params.freeSurfaceDrag = 0.0f;
		//g_params.drag = 0.0f;
		//g_params.lift = 0.0f;
		//g_params.particleCollisionMargin = 0.f
		g_params.shockPropagation = 0.f;
		g_params.relaxationFactor = 0.25f;

		//変えても意味のないパラメータ
		g_params.restitution = 0.0f;//反発係数...FleXでは基本的に反発係数は0として計算を進めている(それで物理計算が成り立っている)
		g_params.shapeCollisionMargin = 0.00001f;//kinetic shapesとの距離...このシーンにはkinetic shapeが存在しない
		g_params.smoothing = 0.4f;//レンダリングの話
		
		rotation = 0.f;
		rotationSpeed = -3.67f;

		// draw options		
		g_drawPoints = true;
		g_drawEllipsoids = true;
		g_drawDiffuse = false;
		g_drawRyumyakusen = false;
		g_drawSprings = false;
		numRenderPar = 180;	//この数が適切に指定されていないと流体のレンダリングが正しくされない
	}

	virtual void DoGui()
	{
		imguiSlider("Rotation", &rotationSpeed, -25.f, 25.0f, 0.01f);
	}

	virtual void CenterCamera()
	{
		g_camPos.x = 0.f;
		g_camPos.y = 0.15f / 2.f;
		g_camPos.z = 0.15f + 0.1f;
		g_camAngle = Vec3(0.f, 0.f, 0.f);
		g_camSpeed = 0.075f * 0.03125f;
	}

	virtual void Update()
	{
		realTime += g_realdt*1000.0f;
		rotation += rotationSpeed*g_dt;

		Quat newRotation = QuatFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation);//ratationSpeed[rad/s](g_dtは初期状態で1.f/60.f)

		g_buffers->shapePrevRotations[0] = g_buffers->shapeRotations[0];

		g_buffers->shapeRotations[0] = newRotation;

		UpdateShapes();

		//拡散の度合
		float sumPar = 180.f;
		float thre_y_b = 0.15f * 0.5f;
		float thre_y_u = 0.15f * 0.5f;
		float thre_xz_i = 0.069f * 0.5f * 0.5f; //翼の半分の長さ
		float thre_xz_i_hub = (0.069f - 0.022 * 2.f) * 0.5f; //翼の半分の長さ

		float cnt_b = 0.f;
		float cnt_u = 0.f;
		float cnt_ue = 0.f;
		float cnt_i = 0.f;
		float cnt_i_hub = 0.f;

		for (int i = 0; i < sumPar; i++) {
			if (g_buffers->positions[i].y < thre_y_b) {
				cnt_b += 1.f;
			}
			else if (g_buffers->positions[i].y > thre_y_u) {
				cnt_u += 1.f;
			}
			if (g_buffers->positions[i].y > 0.15f * 0.4f) {
				cnt_ue += 1.f;
			}
			if (pow(g_buffers->positions[i].x, 2) + pow(g_buffers->positions[i].z, 2) < pow(thre_xz_i, 2) && g_buffers->positions[i].y > 0.15f * 0.4f) {
				cnt_i += 1.f;
			}
			if (pow(g_buffers->positions[i].x, 2) + pow(g_buffers->positions[i].z, 2) < pow(thre_xz_i_hub, 2) && g_buffers->positions[i].y > 0.15f * 0.4f) {
				cnt_i_hub += 1.f;
			}
		}
		float taisekihi = pow(thre_xz_i, 2) / pow(0.075f, 2);
		float taisekihi2 = pow(thre_xz_i_hub, 2) / pow(0.075f, 2);
		float wariai = (cnt_u / cnt_b) / 0.5f;
		float wariai2 = (cnt_i / cnt_ue) / taisekihi;
		float wariai_hub = (cnt_i_hub / cnt_ue) / taisekihi2;
		
		if (g_drawEllipsoids) {
			g_fluidColor = Vec4(1.f - wariai,
								0.f, 
								wariai,
								0.0f);
		} 
		
		//フレームと2つの比の出力
		//if (g_drawContacts == true) {
		//	const char *filename = "/.csv"; //自動で名前変わるようにしたい
		//
		//	ofstream ofs(filename, ios::app);//追加書き込み
		//    //ofstream ofs(filename);//更新書き込み
		//    //frame
		//	//ofs << g_frame << "," << g_frame / 60.f << "," << wariai << "," << wariai2 << "," << wariai_hub << endl;
		//	ofs << g_frame << "," << g_frame / 60.f << "," << cnt_i_hub << "," << cnt_ue << ", " << taisekihi2 <<  wariai2 << "," << wariai_hub << endl;
		//	cout << filename << "に書き込みました" << endl;
		//}

		//frame time あとでフレームレートに置き換えるのを忘れないように、ここで変えれば済む問題ではある
		if (g_drawContacts == true) {
			const char *filename = "/.csv"; //自動で名前変わるようにしたい
		
			ofstream ofs(filename, ios::app);//追加書き込み
			ofs << realTime << "," << g_realdt*1000.f << endl;
			cout << filename << "に書き込みました" << endl;
		}
	}

	float realTime; //シミュレーションの時間経過
	float rotation;
	float rotationSpeed;

	NvFlexExtMovingFrame meshFrame;

	NvFlexTriangleMeshId mesh;
	NvFlexTriangleMeshId mesh1;
	NvFlexTriangleMeshId mesh2;
	NvFlexTriangleMeshId mesh3;
	NvFlexTriangleMeshId mesh4;
	NvFlexTriangleMeshId mesh5;
};