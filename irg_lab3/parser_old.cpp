#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Vertex3D
{
	double x_;
	double y_;
	double z_;

public:
	Vertex3D(const double t_x, const double t_y, const double t_z) :
		x_(t_x),
		y_(t_y),
		z_(t_z)
	{};


	double x() const
	{
		return x_;
	}

	double y() const
	{
		return y_;
	}

	double z() const
	{
		return z_;
	}
};

class Face3DInd
{
	int ind_v1_;
	int ind_v2_;
	int ind_v3_;

public:
	Face3DInd(const int t_ind_x, const int t_ind_y, const int t_ind_z)
		: ind_v1_(t_ind_x),
		ind_v2_(t_ind_y),
		ind_v3_(t_ind_z)
	{
	}

	int ind_v1() const
	{
		return ind_v1_;
	}

	int ind_v2() const
	{
		return ind_v2_;
	}

	int ind_v3() const
	{
		return ind_v3_;
	}
};

class Object3D
{
	std::vector<Face3DInd> faces_;
	std::vector<Vertex3D> vertices_;

public:
	Object3D(const std::vector<Face3DInd> t_faces, const std::vector<Vertex3D> t_vertices) :
		faces_(t_faces),
		vertices_(t_vertices)
	{
	}
};

Object3D from_stream(std::istream& is)
{
	using namespace std;
	vector<Vertex3D> vertices{};
	vector<Face3DInd> faces{};
	auto defined_face = false;

	for (string line; getline(is, line);)
	{
		stringstream sstream(line);
		auto entity_type = '\0';
		sstream >> entity_type;

		switch (entity_type)
		{
		case 'v': {
			if (defined_face)
			{
				throw std::domain_error("cannot intermix face and vertex definitions");
			}

			double x = 0, y = 0, z = 0;
			if (!(sstream >> x >> y >> z))
			{
				throw std::domain_error("vertex format invalid");
			}

			vertices.emplace_back(x, y, z);
			break;
		}
		case 'f': {
			defined_face = true;
			int v1_ind = 0, v2_ind = 0, v3_ind;

			if (!(sstream >> v1_ind >> v2_ind >> v3_ind))
			{
				throw std::domain_error("face format invalid");
			}

			faces.emplace_back(v1_ind, v2_ind, v3_ind);
			break;
		}
		default: {
			break;
		}
		}
	}

	return Object3D(faces, vertices);
}
/*
int main()
{
	std::istringstream a("v 1.0 2.0 3.0\nv 1.0 1.0 1.0\nf 1 2 3");
	auto object = from_stream(a);
}*/