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

		g_numExtraParticles = 6528; //�i�q�_�̐��ɍ��킹��

		g_params.radius = radius;
		g_params.fluidRestDistance = restDistance;
		//�ς����p�����[�^(���Ƃ��Ƃ��̃V�[���ɂ������ϐ�)
		g_numSubsteps = 4;
		g_params.numIterations = 3;
		g_params.dynamicFriction = 0.f;
		g_params.collisionDistance = 0.0025f;//�ǂƂ̋���...���łɏ��������Ă�...�f�t�H���g��0.05f
		g_params.viscosity = 0.001f;//�S�x...0.001f�͐���20���̔S�x�ɑ���
		g_params.cohesion = 0.05f;//�ÏW��...�Ó��Ȑ����Ȃ̂��킩��Ȃ�
		g_params.surfaceTension = 0.0f;//�f�t�H���g�̓[��
		//g_params.maxSpeed = g_numSubsteps*restDistance/g_dt;//���鑬�x�����̃V�[���̃}�b�N�X�X�s�[�h�Ƃ���A��������

		//�ς����p�����[�^(���Ƃ��Ƃ��̃V�[���ɂȂ������ϐ�)
		//g_params.adhesion = 0.0f;//�ǂւ̕t����
		//g_params.solidPressure = 1.0f;//�ő̕\�ʂ���̈���
		//g_params.staticFriction = 0.0f;
		//g_params.particleFriction = 0.0f; // scale friction between particles by default
		//g_params.freeSurfaceDrag = 0.0f;
		//g_params.drag = 0.0f;
		//g_params.lift = 0.0f;

		//�ς��Ă��Ӗ��̂Ȃ��p�����[�^
		g_params.restitution = 0.0f;//�����W��...FleX�ł͊�{�I�ɔ����W����0�Ƃ��Čv�Z��i�߂Ă���(����ŕ����v�Z�����藧���Ă���)
		g_params.shapeCollisionMargin = 0.00001f;//kinetic shapes�Ƃ̋���...���̃V�[���ɂ�kinetic shape�����݂��Ȃ�
		g_params.smoothing = 0.4f;//�����_�����O�̘b

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

		Quat newRotation = QuatFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation);//ratationSpeed[rad/s](g_dt�͏�����Ԃ�1.f/60.f)

		g_buffers->shapePrevRotations[0] = g_buffers->shapeRotations[0];

		g_buffers->shapeRotations[0] = newRotation;

		UpdateShapes();

		//contact == true��pause
		//��]��Ver
		if (g_drawContacts == true && (2.f * kPi - rangeToRad <= fmod(rotation, 2.f * kPi) || rangeToRad >= fmod(rotation, 2.f * kPi))) {
			printf("Frame�F%d\n", g_frame);
			printf("�����W�F%f %f\n", 2.f * kPi - rangeToRad, rangeToRad);
			printf("��]���F%f rad\n", fmod(rotation, 2.f * kPi));
			printf("��]���F%f degree\n", fmod(rotation, 2.f * kPi) * 180.f / kPi);
			printf("----------------\n");
		
			const char *filename = "/.csv"; //�����Ŗ��O�ς��悤�ɂ�����
		
			ofstream ofs(filename, ios::app);//�ǉ���������
											 //ofstream ofs(filename);//�X�V��������
			//frame
			ofs << g_frame << endl;
			//�ݒ肵�������W
			ofs << 2.f * kPi - rangeToRad << "," << rangeToRad << endl;
			//��]��rad
			ofs << fmod(rotation, 2.f * kPi) << endl;
			//��]��degree
			ofs << fmod(rotation, 2.f * kPi) * 180.f / kPi << endl;
		}

		//frame time ���ƂŃt���[�����[�g�ɒu��������̂�Y��Ȃ��悤�ɁA�����ŕς���΍ςޖ��ł͂���
		//if (g_drawContacts == true) {
		//	const char *filename = "/.csv"; //�����Ŗ��O�ς��悤�ɂ�����
		//
		//	ofstream ofs(filename, ios::app);//�ǉ���������
		//									 //ofstream ofs(filename);//�X�V��������
		//									 //frame
		//	ofs << realTime << "," << rotationSpeed << "," << g_realdt*1000.f << endl;
		//	cout << filename << "�ɏ������݂܂���" << endl;
		//}
	}

	float realTime; //�V�~�����[�V�����̎��Ԍo��
	float range = 0.1f; //degree
	float rangeToRad = range * kPi / 180.f; 
	float rotation;
	float rotationSpeed;

	NvFlexExtMovingFrame meshFrame;

	NvFlexTriangleMeshId mesh;
	NvFlexTriangleMeshId mesh1;
};