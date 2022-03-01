class StirredTank2D : public Scene
{
public:

	StirredTank2D(const char* name) : Scene(name) {}

	void Initialize()
	{
		const float radius = 0.0049625f;
		const float restDistance = radius*0.6f;

		Vec3 center = Vec3(0,1.f,0);
		Quat rot = QuatFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), 3.14159265359f / 4.f);

		Mesh* shape = ImportMesh("../../data/puropera9.obj");
		
		Mesh* shape1 = ImportMesh("../../data/container9.obj");

		float scale = 0.1f;

		mesh = CreateTriangleMesh(shape);
		AddTriangleMesh(mesh, Vec3(center), Quat(), Vec3(scale*0.5f, scale*0.25f, scale*0.5f));
		
		mesh1 = CreateTriangleMesh(shape1);
		AddTriangleMesh(mesh1, Vec3(center), rot, Vec3(scale, scale*0.5f, scale));

		Emitter e1;
		e1.mDir = Vec3(0.0f, -1.0f, 0.0f);
		e1.mRight = Vec3(0.0f, 0.0f, -1.0f);
		e1.mPos = Vec3(-0.05f, 1.01f, -0.05f);
		e1.mSpeed = 0.1f; // 2 particle layers per-frame
		e1.mEnabled = true;

		g_emitters.push_back(e1);

		g_numExtraParticles = 6528; //格子点の数に合わせた

		g_params.radius = radius;
		g_params.fluidRestDistance = restDistance;
		//変えれるパラメータ(もともとこのシーンにあった変数)
		g_numSubsteps = 4;
		g_params.numIterations = 3;
		g_params.dynamicFriction = 0.f;
		g_params.collisionDistance = 0.0025f;//壁との距離...すでに小さくしてる...デフォルトは0.05f
		g_params.viscosity = 0.001f;//粘度...0.001fは水の20℃の粘度に相当
		g_params.cohesion = 0.05f;//凝集力...妥当な数字なのかわからない
		g_params.surfaceTension = 0.0f;//デフォルトはゼロ
		//g_params.maxSpeed = g_numSubsteps*restDistance/g_dt;//ある速度をそのシーンのマックススピードとする、解除した

		//変えれるパラメータ(もともとこのシーンになかった変数)
		//g_params.adhesion = 0.0f;//壁への付着力
		//g_params.solidPressure = 1.0f;//固体表面からの圧力
		//g_params.staticFriction = 0.0f;
		//g_params.particleFriction = 0.0f; // scale friction between particles by default
		//g_params.freeSurfaceDrag = 0.0f;
		//g_params.drag = 0.0f;
		//g_params.lift = 0.0f;

		//変えても意味のないパラメータ
		g_params.restitution = 0.0f;//反発係数...FleXでは基本的に反発係数は0として計算を進めている(それで物理計算が成り立っている)
		g_params.shapeCollisionMargin = 0.00001f;//kinetic shapesとの距離...このシーンにはkinetic shapeが存在しない
		g_params.smoothing = 0.4f;//レンダリングの話

		rotation = 0.0f;
		rotationSpeed = 3.75f;

		// draw options		
		g_wireframe = true;
		g_drawPoints = false;
		g_drawEllipsoids = false;
		g_drawDiffuse = false;
	}

	virtual void DoGui()
	{
		imguiSlider("Rotation", &rotationSpeed, 0.0f, 25.0f, 0.01f);
	}

	virtual void CenterCamera()
	{
		g_camPos.x = 0.f;
		g_camPos.y = 1.25f;
		g_camPos.z = 0.f;
		g_camAngle = Vec3(0.f, -kPi / 2.f, 0.f);
	}

	virtual void Update()
	{
		realTime += g_realdt*1000.0f;
		rotation += rotationSpeed*g_dt;	

		Quat newRotation = QuatFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation);//ratationSpeed[rad/s](g_dtは初期状態で1.f/60.f)

		g_buffers->shapePrevRotations[0] = g_buffers->shapeRotations[0];

		g_buffers->shapeRotations[0] = newRotation;

		UpdateShapes();

		//contact == trueでpause
		//回転数Ver
		if (g_drawContacts == true && (2.f * kPi - rangeToRad <= fmod(rotation, 2.f * kPi) || rangeToRad >= fmod(rotation, 2.f * kPi))) {
			printf("Frame：%d\n", g_frame);
			printf("レンジ：%f %f\n", 2.f * kPi - rangeToRad, rangeToRad);
			printf("回転数：%f rad\n", fmod(rotation, 2.f * kPi));
			printf("回転数：%f degree\n", fmod(rotation, 2.f * kPi) * 180.f / kPi);
			printf("----------------\n");
		
			const char *filename = "/.csv"; //自動で名前変わるようにしたい
		
			ofstream ofs(filename, ios::app);//追加書き込み
											 //ofstream ofs(filename);//更新書き込み
			//frame
			ofs << g_frame << endl;
			//設定したレンジ
			ofs << 2.f * kPi - rangeToRad << "," << rangeToRad << endl;
			//回転数rad
			ofs << fmod(rotation, 2.f * kPi) << endl;
			//回転数degree
			ofs << fmod(rotation, 2.f * kPi) * 180.f / kPi << endl;
		}

		//frame time あとでフレームレートに置き換えるのを忘れないように、ここで変えれば済む問題ではある
		//if (g_drawContacts == true) {
		//	const char *filename = "/.csv"; //自動で名前変わるようにしたい
		//
		//	ofstream ofs(filename, ios::app);//追加書き込み
		//									 //ofstream ofs(filename);//更新書き込み
		//									 //frame
		//	ofs << realTime << "," << rotationSpeed << "," << g_realdt*1000.f << endl;
		//	cout << filename << "に書き込みました" << endl;
		//}
	}

	float realTime; //シミュレーションの時間経過
	float range = 0.1f; //degree
	float rangeToRad = range * kPi / 180.f; 
	float rotation;
	float rotationSpeed;

	NvFlexExtMovingFrame meshFrame;

	NvFlexTriangleMeshId mesh;
	NvFlexTriangleMeshId mesh1;
};