#ifndef RAYTRACINGFROMSCRATCH_MESH_H
#define RAYTRACINGFROMSCRATCH_MESH_H

#include "triangle.h"

#include "hittable.h"
#include "hittable_list.h"

#include <sstream>
#include "mathLibrary.h"
#include "fstream"

using std::string;
using std::ifstream;
using std::vector;

class mesh : public hittable
{
public:
    mesh (string path, point3 position, hittable_list& world) {

        offset = position;
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
                        edge_list.push_back(int(s[0]) - 48);
                }
            }

            if (token == "v" && floatData.size() > 0)
                vertex_list.push_back(floatData);
        }

        loadObjFile.close();
    }

    void intializeTriangle(hittable_list& world)
    {
        //as we can assume all inputted meshes are triangulated
        for (int i = 0; i < edge_list.size(); i += 3)
        {
            auto material_center = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

            std::vector<point3> v;

            for (int j = 0; j <= 2; j ++)
            {
                std::vector<float> vertex = vertex_list[edge_list[i + j] - 1];
                v.push_back(point3(vertex[0], vertex[1], vertex[2]) + offset);
            }

            world.add(make_shared<triangle>( v, material_center));
        }


    }

    bool hit(const ray&r, interval ray_t, hit_record&rec) const override { return false; }
private:

    vector<vector<float>> vertex_list;
    vector<int> edge_list;
    vector<triangle> triangle_list;
    point3 offset;

};

#endif //RAYTRACINGFROMSCRATCH_MESH_H