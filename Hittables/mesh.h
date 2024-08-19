#ifndef RAYTRACINGFROMSCRATCH_MESH_H
#define RAYTRACINGFROMSCRATCH_MESH_H

#include "Hittables/triangle.h"

#include "Hittables/hittable.h"
#include "Hittables/hittable_list.h"

#include <sstream>
#include "MathLib/mathLibrary.h"
#include "fstream"

using std::string;
using std::ifstream;
using std::vector;

class mesh : public hittable
{
public:
    mesh (string path, transform trans, hittable_list& world, shared_ptr<material> mat) : mat(mat), transform(trans) {
        intializeVertexEdge(path);
        intializeTriangle(world);
    }

    void intializeVertexEdge(string path)
    {
        ifstream loadObjFile(path);
        string line;

        //hacky way of grabbing all the vertex data
        while (getline(loadObjFile, line))
        {
            string s;
            std::stringstream ss(line);

            vector<float> floatData;

            bool validate = false;
            string token;

            while (getline(ss, s, ' ')) {
                if (!validate)
                {
                    token = s;

                    if (token != "v" && token != "f")
                        break;
                    else
                        validate = true;
                }
                else {

                    if (token == "v")
                        floatData.push_back(stof(s));
                    else
                    {
                        string vertex;

                        for (auto c : s)
                        {
                            if (c == '/')
                            {
                                break;
                            }

                            vertex += c;
                        }

                        edge_list.push_back(stoi(vertex));
                    }


                }
            }

            if (token == "v" && floatData.size() > 0)
            {
                vertex_list.push_back(floatData);
                mesh_center += point3(floatData[0], floatData[1], floatData[2]);
            }

        }

        loadObjFile.close();
    }

    void intializeTriangle(hittable_list& world)
    {
        mesh_center /= vertex_list.size();

        //as all inputted meshes are triangulated
        for (int i = 0; i < edge_list.size(); i += 3)
        {
            std::vector<point3> v;

            for (int j = 0; j <= 2; j ++)
            {
                std::vector<float> vertex = vertex_list[edge_list[i + j] - 1];

                //apply transformations - order matters
                point3 transformed_vertex(vertex[0], vertex[1], vertex[2]);

                //transform all coordinates so that they are "local"
                transformed_vertex = transformed_vertex - mesh_center;

                //scaling first to avoid shear transformations
                transformed_vertex = transform.scalingMatrix * transformed_vertex;

                //rotations
                transformed_vertex = transform.rotationalMatrix * transformed_vertex;

                //all transformations applied locally finished, we can now shift it to the desired position
                transformed_vertex += transform.position - mesh_center;

                v.push_back(transformed_vertex);
            }

            world.add(make_shared<triangle>( v, mat));

        }


    }

    bool hit(const ray&r, interval ray_t, hit_record&rec) const override { return false; }


private:

    vector<vector<float>> vertex_list;
    vector<int> edge_list;
    vector<triangle> triangle_list;

    shared_ptr<material>  mat;

    point3 mesh_center;
    transform transform;

};

#endif //RAYTRACINGFROMSCRATCH_MESH_H