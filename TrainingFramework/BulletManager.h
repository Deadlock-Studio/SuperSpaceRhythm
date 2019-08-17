#pragma once
#define RADIUS 100
#define PI  3.14159265358979323846
class BulletManager
{
protected:
	BulletManager() {}
	~BulletManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	BulletManager(const BulletManager &) {}
	BulletManager& operator =(const BulletManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new BulletManager;
	}
	static BulletManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			GetInstance()->DeleteAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	//Functions
	void Circle(Vector3 pos);
	void Plus(Vector3 pos);
	void Cross(Vector3 pos);
	void Flower(Vector3 pos);
	void DoubleSpiral(Vector3 pos);
	void ClockwiseSpriral(Vector3 pos);
	void CounterClockwiseSpiral(Vector3 pos);
	void Twist(Vector3 pos);
	void Triangle(Vector3 pos);
	void X(Vector3 pos);
	void Square(Vector3 pos);
	void Arc(Vector3 pos);
	void Arrow(Vector3 pos);
	void Trace(Vector3 pos);
	void DeleteAll();


protected:
	static BulletManager * ms_pInstance;

public:
	
};

