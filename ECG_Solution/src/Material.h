#pragma once
class Material
{
public:
	Material(float k_ambient, float k_diffuse, float k_specular, float alpha) :
		ka(k_ambient),
		kd(k_diffuse),
		ks(k_specular),
		alpha(alpha)
	{

	}

	~Material()
	{

	}

	float ka, kd, ks, alpha;
};

