#include "mesh.h"
#include <ctime>
#include <iostream >
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "Mesh.h"
#include "Vector3D.h"
#include <set>
#include <deque>

using std::cout;
using std::endl;
using std::vector;
using namespace std;




GeometricWorld::GeometricWorld()
{
    double width=0.5, depth=0.6, height=0.8;
    _bBox.push_back(Point(-0.5*width,-0.5*depth,-0.5*height)); //0
    _bBox.push_back(Point(-0.5*width,0.5*depth,-0.5*height)); // 1
    _bBox.push_back(Point(0.5*width,-0.5*depth,-0.5*height)); // 2
    _bBox.push_back(Point(-0.5*width,-0.5*depth,0.5*height)); // 3
}

// The following functions could be displaced into a module OpenGLDisplayGeometricWorld that would include mesh.h

// Draw a Point
void glPointDraw(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}

//Example with a bBox
void GeometricWorld::draw() {
    glColor3d(1,0,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glPointDraw(_bBox[2]);
    glEnd();

    glColor3d(0,1,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glPointDraw(_bBox[3]);
    glEnd();

    glColor3d(0,0,1);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glPointDraw(_bBox[1]);
    glEnd();

    //glColor3d(1,1,0);
}

//Example with a wireframe bBox
void GeometricWorld::drawWireFrame() {
    glColor3d(0,1,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glEnd();
    glColor3d(0,0,1);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glEnd();
    glColor3d(1,0,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glEnd();
}


// ------------------------------- Partie Creation de La structure de données ------------------
// Mr. Not really interesting Constructor
Mesh::Mesh(const std::vector<Vertex> &vertex_init, const std::vector<Face> &faces_init)
    : vertices(vertex_init), faces(faces_init)
{
}
// Name Says it all
void Mesh::readOFFFile(const string &filename, std::vector<Vertex> &vertices, std::vector<Face> &faces)
{
    std::ifstream file(filename);

    int numVertices, numFaces, numEdges;
    string holder;
    file >> holder;
    file >> numVertices >> numFaces >> numEdges;
    vertices.resize(numVertices);
    faces.resize(numFaces);


    for (int i = 0; i < numVertices; ++i)
    {
        Vertex vertex;
        file >> vertex.x >> vertex.y >> vertex.z;
        if(!VertexMask[i]){
            vertex.front_index = -1;
            vertices[i] = vertex;}
    }

    for (int i = 0; i < numFaces; ++i)
    {
        Face face;
        face.f1 = -1;
        face.f2 = -1;
        face.f3 = -1;
        int numVerticesInFace;

        file >> numVerticesInFace >> face.v1 >> face.v2 >> face.v3;
        if ( true )// !faceMask[i])
        {
            vertices[face.v1].front_index = i;
            vertices[face.v2].front_index = i;
            vertices[face.v3].front_index = i;
            faces[i] = face;
            faceMask[i]=1 ;
            //std::map<std::pair<int, int>, std::pair<int, int>>::iterator iter = indexing_map.find({face.v1, face.v2});
            std::pair<int, int> keyToFind = {face.v1, face.v2};
            std::map<std::pair<int, int>, std::pair<int, int>>::iterator iter;
            for ( iter = indexing_map.begin(); iter != indexing_map.end(); ++iter) {
                // Check if the key matches the one to find
                if (iter->first == keyToFind) {
                    // Check additional conditions if needed
                    int otherFace = iter->second.first;
                    if (otherFace!=i)
                        break;
                }
            }
            if (iter != indexing_map.end())
            {
                int otherFace = iter->second.first;
                int otherVertex = iter->second.second;
                switch (otherVertex)
                {
                case 1:
                    faces[otherFace].f1 = i;
                    break;
                case 2:
                    faces[otherFace].f2 = i;
                    break;
                case 3:
                    faces[otherFace].f3 = i;
                    break;
                }
                faces[i].f3 = otherFace;
            }
            else
            {
                indexing_map[{face.v1, face.v2}] = {i, 3};
                indexing_map[{face.v2, face.v1}] = {i, 3};
            }

            //iter = indexing_map.find({face.v1, face.v3});
            keyToFind={face.v1,face.v3};
            for ( iter = indexing_map.begin(); iter != indexing_map.end(); ++iter) {
                // Check if the key matches the one to find

                if (iter->first == keyToFind ) {
                    // Check additional conditions if needed
                    int otherFace = iter->second.first;
                    if (otherFace!=i)
                        break;
                }
            }
            if (iter != indexing_map.end())
            {
                int otherFace = iter->second.first;
                int otherVertex = iter->second.second;
                switch (otherVertex)
                {
                case 1:
                    faces[otherFace].f1 = i;
                    break;
                case 2:
                    faces[otherFace].f2 = i;
                    break;
                case 3:
                    faces[otherFace].f3 = i;
                    break;
                }
                faces[i].f2 = otherFace;
            }
            else
            {
                indexing_map[{face.v1, face.v3}] = {i, 2};
                indexing_map[{face.v3, face.v1}] = {i, 2};
            }

            //iter = indexing_map.find({face.v2, face.v3});
            keyToFind={face.v2,face.v3};
            for ( iter = indexing_map.begin(); iter != indexing_map.end(); ++iter) {
                // Check if the key matches the one to find

                if (iter->first == keyToFind ) {
                    // Check additional conditions if needed
                    int otherFace = iter->second.first;
                    if (otherFace!=i)
                        break;
                }
            }
            if (iter != indexing_map.end())
            {
                int otherFace = iter->second.first;
                int otherVertex = iter->second.second;
                switch (otherVertex)
                {
                case 1:
                    faces[otherFace].f1 = i;
                    break;
                case 2:
                    faces[otherFace].f2 = i;
                    break;
                case 3:
                    faces[otherFace].f3 = i;
                    break;
                }
                faces[i].f1 = otherFace;
            }
            else
            {
                indexing_map[{face.v2, face.v3}] = {i, 1};
                indexing_map[{face.v3, face.v2}] = {i, 1};
            }
        }
    }
    file.close();
}
// Checking if two faces are neighbours, return the vertex common between them
bool areNeighbors(const Face &face1, const Face &face2, bool &v1Shared, bool &v2Shared, bool &v3Shared)
{
    v1Shared = (face1.v1 == face2.v1 || face1.v1 == face2.v2 || face1.v1 == face2.v3);
    v2Shared = (face1.v2 == face2.v1 || face1.v2 == face2.v2 || face1.v2 == face2.v3);
    v3Shared = (face1.v3 == face2.v1 || face1.v3 == face2.v2 || face1.v3 == face2.v3);
    int sharedVertexCount = 0;
    if (v1Shared) {
        sharedVertexCount++;
    }
    if (v2Shared) {
        sharedVertexCount++;
    }
    if (v3Shared) {
        sharedVertexCount++;
    }
    return sharedVertexCount == 2;
}
// NAme says it all
void Mesh::saveOFFFile(const vector<Vertex> &vertices, const vector<Face> &faces, const string &filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    outFile << "OFF" << endl;
    outFile << vertices.size() << " " << faces.size() << " 0" << endl;
    for (const Vertex &vertex : vertices)
    {
        outFile << vertex.x << " " << vertex.y << " " << vertex.z << endl;
    }
    for (const Face &face : faces)
    {
        outFile << "3 " << face.v1 << " " << face.v2 << " " << face.v3 << endl;
    }
    outFile.close();
}

// ----------------------------- Partie Insertion naive ------------------------
// Aka: Split Triangle into two, when the vertex is on an edge
void Mesh::splitFace(int indiceFace, Vertex &new_vetex ,int l)
{
    // Vertices Configuration
    new_vetex.front_index = indiceFace;
    this->vertices.push_back(new_vetex);
    this->VertexMask.push_back(1);

    int v4 = vertices.size() - 1;
    Face &nf0 = this->faces[indiceFace];
    Face nf1;
    int v1 =nf0.v1 , v2=nf0.v2 , v3=nf0.v3 ;
    switch (l){
    case 1 :
        nf1.v1 = v1, nf1.v2 = v4, nf1.v3 = v3;
        nf0.v1 =  v4 , nf0.v2 =  v2 ,  nf0.v3 =  v3 ;
        break ;
    case 2:
        nf1.v1 = v1, nf1.v2 = v2, nf1.v3 = v4;
        nf0.v1 =  v1 , nf0.v2 =  v4 ,  nf0.v3 =  v3 ;
        break;
    case 3:
        nf1.v1 = v1, nf1.v2 = v2, nf1.v3 = v4;
        nf0.v1 =  v2 , nf0.v2 =  v3 ,  nf0.v3 =  v4 ;
        break;
    }

    this->faces.push_back(nf1);
    faceMask.push_back(0);
    faceMask[indiceFace] = 0;
    std::cout << "Initiating File Save" << std::endl;
    saveOFFFile(vertices, faces, "temp.off");
    std::cout << "Finishing File Load and changing VerticeS/faces" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    readOFFFile("temp.off", vertices, faces);
}


// SplitFace The face into 3 Parts
void Mesh::splitFaceV2(int indiceFace, Vertex &new_vetex)
{
    // Vertices Configuration
    new_vetex.front_index = indiceFace;
    this->VertexMask.push_back(1);
    this->vertices.push_back(new_vetex);
    int new_v_place = vertices.size() - 1;
    Face &nf0 = this->faces[indiceFace];
    Face nf1;
    nf1.v1 = nf0.v2, nf1.v2 = nf0.v3, nf1.v3 = new_v_place;
    Face nf2;
    nf2.v1 = nf0.v3, nf2.v2 = nf0.v1, nf2.v3 = new_v_place;
    nf0.v3 = new_v_place;
    this->faceMask.push_back(0);
    this->faceMask.push_back(0);
    this->faces.push_back(nf1);
    this->faces.push_back(nf2);
    this->faceMask[indiceFace]=0;
    // Faces configuraton
    std::cout << "Initiating File Save" << std::endl;
    saveOFFFile(vertices, faces, "temp.off");
    std::cout << "Finishing File Load and changing VerticeS/faces" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    readOFFFile("temp.off", vertices, faces);
}

// The Long Not fully Tested way
void Mesh::edgeFlip(int f1, int f2)
{
    Face &t1 = this->faces[f1];
    Face &t2 = this->faces[f2];

    // Find the shared vertices between the two faces.
    int sharedV1 = -1, sharedV2 = -1, faceV11 = -1, faceV12 = -1, faceV21 = -1, faceV22 = -1;

    if (t1.v1 == t2.v1 || t1.v1 == t2.v2 || t1.v1 == t2.v3)
    {
        sharedV1 = t1.v1;
        faceV11 = t1.f1;
        if (t1.v1 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v1 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v1 == t2.v3)
            faceV12 = t2.f3;
    }
    if (t1.v2 == t2.v1 || t1.v2 == t2.v2 || t1.v2 == t2.v3)
    {
        sharedV1 = t1.v2;
        faceV11 = t1.f2;
        if (t1.v2 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v2 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v2 == t2.v3)
            faceV12 = t2.f3;
    }
    if (t1.v3 == t2.v1 || t1.v3 == t2.v2 || t1.v3 == t2.v3)
    {
        sharedV1 = t1.v3;
        faceV11 = t1.f3;
        if (t1.v3 == t2.v1)
            faceV12 = t2.f1;
        if (t1.v3 == t2.v2)
            faceV12 = t2.f2;
        if (t1.v3 == t2.v3)
            faceV12 = t2.f3;
    }

    // Find the other shared vertex.
    if (t2.v1 != sharedV1 && (t2.v1 == t1.v1 || t2.v1 == t1.v2 || t2.v1 == t1.v3))
    {
        sharedV2 = t2.v1;
        faceV21 = t2.f1;
        if (t2.v1 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v1 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v1 == t1.v3)
            faceV22 = t1.f3;
    }
    if (t2.v2 != sharedV1 && (t2.v2 == t1.v1 || t2.v2 == t1.v2 || t2.v2 == t1.v3))
    {
        sharedV2 = t2.v2;
        faceV21 = t2.f2;
        if (t2.v2 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v2 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v2 == t1.v3)
            faceV22 = t1.f3;
    }
    if (t2.v3 != sharedV1 && (t2.v3 == t1.v1 || t2.v3 == t1.v2 || t2.v3 == t1.v3))
    {
        sharedV2 = t2.v3;
        faceV21 = t2.f3;
        if (t2.v3 == t1.v1)
            faceV22 = t1.f1;
        if (t2.v3 == t1.v2)
            faceV22 = t1.f2;
        if (t2.v3 == t1.v3)
            faceV22 = t1.f3;
    }
    int nonShared1, nonShared2;
    if ((t1.v1 != sharedV1) && (t1.v1 != sharedV2))
        nonShared1 = t1.v1;
    if ((t1.v2 != sharedV1) && (t1.v2 != sharedV2))
        nonShared1 = t1.v2;
    if ((t1.v3 != sharedV1) && (t1.v3 != sharedV2))
        nonShared1 = t1.v3;

    if ((t2.v1 != sharedV1) && (t2.v1 != sharedV2))
        nonShared2 = t2.v1;
    if ((t2.v2 != sharedV1) && (t2.v2 != sharedV2))
        nonShared2 = t2.v2;
    if ((t2.v3 != sharedV1) && (t2.v3 != sharedV2))
        nonShared2 = t2.v3;
    if (Mesh::test_orientation(vertices[nonShared1], vertices[sharedV1], vertices[nonShared2]) > 0)
    {
        t1.v1 = nonShared1;
        t1.v2 = sharedV1;
        t1.v3 = nonShared2;

        t2.v1 = nonShared2;
        t2.v2 = sharedV2;
        t2.v3 = nonShared1;
    }
    else
    {
        t1.v1 = nonShared2;
        t1.v2 = sharedV1;
        t1.v3 = nonShared1;

        t2.v1 = nonShared1;
        t2.v2 = sharedV2;
        t2.v3 = nonShared2;
    }

    int n[4] = {faceV11, faceV12, faceV21, faceV22};
    for (int i = 0; i < 4; i++)
    {
        bool b1 = false , b2 = false , b3 = false;
        if (areNeighbors(t1, faces[n[i]], b1, b2, b3))
        {
            if (b1)
                t1.f1 = n[i];
            if (b2)
                t1.f2 = n[i];
            if (b3)
                t1.f3 = n[i];
        }
        b1 = false , b2 = false , b3 = false;
        if (areNeighbors(t2, faces[n[i]], b1, b2, b3))
        {
            if (b1)
                t2.f1 = n[i];
            if (b2)
                t2.f2 = n[i];
            if (b3)
                t2.f3 = n[i];
        }
    }
}
// The Easy Way
void Mesh::edgeFlipV2(int f1, int f2)
{

    Face &t1 = this->faces[f1];
    faceMask[f1]=0 ;
    Face &t2 = this->faces[f2];
    faceMask[f2]=0 ;

    // Find the shared vertices between the two faces.
    int sharedV1 = -1, sharedV2 = -1;

    if (t1.v1 == t2.v1 || t1.v1 == t2.v2 || t1.v1 == t2.v3)
    {
        sharedV1 = t1.v1;
    }
    if (t1.v2 == t2.v1 || t1.v2 == t2.v2 || t1.v2 == t2.v3)
    {
        sharedV1 = t1.v2;
    }
    if (t1.v3 == t2.v1 || t1.v3 == t2.v2 || t1.v3 == t2.v3)
    {
        sharedV1 = t1.v3;
    }

    // Find the other shared vertex.
    if (t2.v1 != sharedV1 && (t2.v1 == t1.v1 || t2.v1 == t1.v2 || t2.v1 == t1.v3))
    {
        sharedV2 = t2.v1;
    }
    if (t2.v2 != sharedV1 && (t2.v2 == t1.v1 || t2.v2 == t1.v2 || t2.v2 == t1.v3))
    {
        sharedV2 = t2.v2;
    }
    if (t2.v3 != sharedV1 && (t2.v3 == t1.v1 || t2.v3 == t1.v2 || t2.v3 == t1.v3))
    {
        sharedV2 = t2.v3;
    }
    int nonShared1, nonShared2;
    if ((t1.v1 != sharedV1) && (t1.v1 != sharedV2))
        nonShared1 = t1.v1;
    if ((t1.v2 != sharedV1) && (t1.v2 != sharedV2))
        nonShared1 = t1.v2;
    if ((t1.v3 != sharedV1) && (t1.v3 != sharedV2))
        nonShared1 = t1.v3;
    if ((t2.v1 != sharedV1) && (t2.v1 != sharedV2))
        nonShared2 = t2.v1;
    if ((t2.v2 != sharedV1) && (t2.v2 != sharedV2))
        nonShared2 = t2.v2;
    if ((t2.v3 != sharedV1) && (t2.v3 != sharedV2))
        nonShared2 = t2.v3;
    if (Mesh::test_orientation(vertices[nonShared1], vertices[sharedV1], vertices[nonShared2]) > 0)
    {
        t1.v1 = nonShared1;
        t1.v2 = sharedV1;
        t1.v3 = nonShared2;

        t2.v1 = nonShared2;
        t2.v2 = sharedV2;
        t2.v3 = nonShared1;
    }
    else
    {
        t1.v1 = nonShared2;
        t1.v2 = sharedV1;
        t1.v3 = nonShared1;

        t2.v1 = nonShared1;
        t2.v2 = sharedV2;
        t2.v3 = nonShared2;
    }
    std::cout << "Initiating File Save" << std::endl;
    saveOFFFile(vertices, faces, "temp.off");
    std::cout << "Finishing File Load and changing VerticeS/faces" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    readOFFFile("temp.off", vertices, faces);
}
// z < 0 => -1 ./.  z > 0 => 1   ./. z==0 => 0                                                                 : 0;
int Mesh::test_orientation(Vertex v1, Vertex v2, Vertex v3)
{
    Vector3D v1v2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
    Vector3D v1v3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
    Vector3D orientation_tester = v1v2.cross(v1v3);
    return orientation_tester.z < 0 ? -1 : orientation_tester.z > 0 ? 1
                                                                    : 0;
}
// Checking if a point is Inside a Triangle
int Mesh::pointInTriangle(Face &f, Vertex &P, int& l)
{
    Vertex A = vertices[f.v1];
    Vertex B = vertices[f.v2];
    Vertex C = vertices[f.v3];
    int orientation1 = this->test_orientation(A, B, P);
    int orientation2 = this->test_orientation(B, C, P);
    int orientation3 = this->test_orientation(C, A, P);

    if (orientation1 == 0 && ( (orientation2 > 0 && orientation3 > 0) ))
    {
        l=1; // In AB
        return 0 ;
    }
    else if (orientation2 == 0 && ( (orientation1 > 0 && orientation3 > 0)))
    {
        l=2;// In BC
        return 0 ;
    }
    else if (orientation3 == 0 && ( (orientation1 > 0 && orientation2 > 0) ))
    {
        l=3; // In CA
        return 0 ;
    }
    else if (orientation1 > 0 && orientation2 > 0 && orientation3 > 0)
    {
        return 1; // Inside
    }

    else
    {
        return -1; // Outside
    }
}
// insert Point inside/outside the convexHull
void Mesh::InsertPointInMesh(Vertex &A)
{
    // Inside the convexHull
    int isInside = 0;
    int isOnEdge = 0;
    std::vector<std::vector<int>> vectorOfVertices;
    std::cout << "Initiating: Checking if point is inside Convex hull" << std::endl;
    int i = 0;
    while (i < faces.size() && isInside == 0 && isOnEdge <=  2)
    {
        Face face = faces[i];
        int l ;
        int k = pointInTriangle(face, A , l) ;
        if (k == 1)
        {
            isInside = 1;
            splitFaceV2(i, A);
            std::cout << "Found The Point inside Face: " << i << std::endl;
        }else if(k==0){
            isOnEdge += 1;
            splitFace(i, A , l);
            std::cout << "Found The Point in an edge: " << i << std::endl;
        }
        if (face.f1 == -1)
        {
            vectorOfVertices.push_back({face.v2, face.v3,i,1});
        }
        if (face.f2 == -1)
        {
            vectorOfVertices.push_back({face.v3, face.v1,i,2});
        }
        if (face.f3 == -1)
        {
            vectorOfVertices.push_back({face.v1, face.v2,i,3});
        }
        i++;
    }

    if (isInside)
    {
        std::cout << "Point is Inside Convex hull, Leaving" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        return;

    }
    if (isOnEdge>=1){
        std::cout << "Point is inside "<< isOnEdge << " Edges "<< std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        return;
    }

    std::cout << "Point is outside Convex hull" << std::endl;
    // Outside
    vertices.push_back(A);
    VertexMask.push_back(1);
    for (const std::vector<int> &verticesPair : vectorOfVertices)
    {
        int p;
        switch(verticesPair[3]){
           case 1: //BC
            p= test_orientation( vertices[verticesPair[0]], vertices[verticesPair[1]] ,A)  ;
         break;
           case 2: //CA
         p= test_orientation(vertices[verticesPair[0]], vertices[verticesPair[1]] ,A)  ;

        break;
           case 3: // AB
        p= test_orientation(vertices[verticesPair[0]], vertices[verticesPair[1]],A)  ;
        break;
           }
        if ( p<0)
        {
            Face newFace;
            newFace.v1 = verticesPair[0];
            newFace.v2 = verticesPair[1];
            newFace.v3 = vertices.size() - 1;
            if (test_orientation(vertices[newFace.v1], vertices[newFace.v2], vertices[newFace.v3]) < 0)
            {
                std::swap(newFace.v2, newFace.v3);
            }
            faceMask.push_back(0);
            faces.push_back(newFace);
            faceMask[verticesPair[2]] = 0 ;
            std::cout << "Found a visible edge" << newFace.v1 << newFace.v2 << std::endl;
        }
    }
    // addFaces(affectedFaces, newFaces);
    std::cout << "Finished Adding outside the convex hull" << std::endl;
    std::cout << "Initiating File Save" << std::endl;
    saveOFFFile(vertices, faces, "temp.off");
    //    std::vector<Vertex> newVertices;
    //    std::vector<Face> newFaces;
    std::cout << "Finishing File Load and changing VerticeS/faces" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    readOFFFile("temp.off", vertices, faces);
}

// ----------------------- Partie Daulaunay -------------------------------------
// Projection vers la paraboloide
Vector3D Mesh::getPhiVector(Vertex A, Vertex B)
{
    Vector3D v(1, 1, 1);
    v.x = B.x - A.x;
    v.y = B.y - A.y;
    v.z = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
    return v;
}
// Edge Daulaunay Check
bool Mesh::isLocallyDaulaunay(Edge edge)
{
    Vector3D phiAB = getPhiVector(vertices[edge.A], vertices[edge.B]);
    Vector3D phiAC = getPhiVector(vertices[edge.A], vertices[edge.C]);
    Vector3D phiAD = getPhiVector(vertices[edge.A], vertices[edge.D]);
    return (phiAB.cross(phiAC).dot(phiAD)) > 0;
}

std::deque<Edge> Mesh::getEdges()
{
    std::deque<Edge> edges;
    std::set<std::pair<int, int>> addedEdges; // Use a set to track added edges

    for (int i = 0; i < faces.size(); ++i)
    {
        Face &currentFace = faces[i];
        currentFace.fillTables();
        for (int j = 0; j < 3; ++j)
        {
            int vertexA = currentFace.v1v2v3[j];
            int vertexB = currentFace.v1v2v3[(j + 1) % 3];
            int vertexC = currentFace.v1v2v3[(j + 2) % 3];
            int neighborFaceIndex = currentFace.f1f2f3[(j + 2) % 3];

            int vertexD = -1;
            if (neighborFaceIndex != -1)
            {
                Face &neighborFace = faces[neighborFaceIndex];
                neighborFace.fillTables();
                for (int k = 0; k < 3; ++k)
                {
                    if (neighborFace.v1v2v3[k] != vertexA && neighborFace.v1v2v3[k] != vertexB)
                    {
                        vertexD = neighborFace.v1v2v3[k];
                        break;
                    }
                }
            }
            std::pair<int, int> edgeIndices(std::min(vertexA, vertexB), std::max(vertexA, vertexB));
            if (addedEdges.find(edgeIndices) == addedEdges.end())
            {
                Edge edge;
                edge.A = vertexA;
                edge.B = vertexB;
                edge.C = vertexC;
                edge.D = vertexD;
                edge.f1 = i;
                edge.f2 = neighborFaceIndex;
                if (edge.f1 != -1 && edge.f2 != -1)
                {
                    edges.push_back(edge);
                }
                addedEdges.insert(edgeIndices);
            }
        }
    }
    return edges;
}
// This function checks if the mesh is daulaunay, if it isnt it returns the edge that is not daulaunay
bool Mesh::isDaulaunay(Edge& a , int& ind){
    // We iterate through all faces and check each edge
    // print faces size
    std::cout<<"Number of Faces: "<<faces.size()<<std::endl;
    for (int i = ind; i < faces.size(); ++i)
    {

        std::cout<<"Still Looking we at Face: "<<i<<std::endl;
        Face &currentFace = faces[i];
        currentFace.fillTables();
        for (int j = 0; j < 3; ++j)
        {
            int vertexA = currentFace.v1v2v3[j];
            int vertexB = currentFace.v1v2v3[(j + 1) % 3];
            int vertexC = currentFace.v1v2v3[(j + 2) % 3];
            int neighborFaceIndex = currentFace.f1f2f3[(j + 2) % 3];

            int vertexD = -1;
            // if there is a neighbor face we get the vertexD, else we skip this iteration
            if (neighborFaceIndex != -1)
            {
                Face &neighborFace = faces[neighborFaceIndex];
                neighborFace.fillTables();
                for (int k = 0; k < 3; ++k)
                {
                    if (neighborFace.v1v2v3[k] != vertexA && neighborFace.v1v2v3[k] != vertexB && neighborFace.v1v2v3[k] != vertexC)
                    {
                        vertexD = neighborFace.v1v2v3[k];
                        break;
                    }
                }
            } else {
                continue;
            }
            Edge edge;
            edge.A = vertexA;
            edge.B = vertexB;
            edge.C = vertexC;
            edge.D = vertexD;
            edge.f1 = i;
            edge.f2 = neighborFaceIndex;
            if (edge.f1 != -1 && edge.f2 != -1 && edge.A!=-1 && edge.B!=-1 && edge.C!=-1 && edge.D!=-1  )
            {
                if(!isLocallyDaulaunay(edge) ){
                    // if the edge is flippable we return it otherwise de do nothing
                    std::cout<<"Edge is not locally daulaunay "<<isLocallyDaulaunay(edge)<<std::endl;
                    //if(isFlippable(edge)){
                    a = edge;
                    ind = i;
                    return false;
                    //}
                }
            }
        }
    }
    // If all edges are daulany we return True
    return true;
}


bool Mesh::isFlippable(Edge edge){
    auto &f1 = faces[edge.f1];
    auto &f2 = faces[edge.f2];
    int s1 = f1.getOpposite(edge.f1);
    int s2 = f2.getOpposite(edge.f2);
    if(s1 < 0 || s2 < 0) return false;
    return true;
}



int Mesh::lawson() {
    Edge a;
    int cnt = 0;
    std::cout << "Started Lawson" << std::endl;
    int ind = 0;

    int outerLoopCounter = 0;  // Counter for the outer loop

    while (!isDaulaunay(a, ind)) {
        int start = ind;
        ind = 0;

        while (!isDaulaunay(a, start)) {
            std::cout << "Flipping Edge: " << a.A << " " << a.B << " " << a.C << " " << a.D << " " << a.f1 << " " << a.f2 << std::endl;
            edgeFlipV2(a.f1, a.f2);
            cnt += 1;        }
        outerLoopCounter += 1;
    }

    std::cout << "Finished Lawson after " << cnt << " iterations" << std::endl;
    return cnt;
}



void Mesh::drawWireFrame() const
{
    glBegin(GL_TRIANGLES);
    //srand(static_cast<unsigned>(time(nullptr)));
    srand(42);
    for ( Face face : faces)
    {
        face.fillTables();
        // Generate random color for each face
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;
        glColor3f(r, g, b);
        for (const int vertexIndex : face.v1v2v3)
        {
            const Vertex& vertex = vertices[vertexIndex];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }

    glEnd();
}


void Mesh::edgeFlipv3( int fId1,  int fId2)
{
    Face  &f1 = faces[fId1];
    Face  &f2 = faces[fId2];
    int s1 = f1.getOpposite(fId2);
    int s2 = f2.getOpposite(fId1);

    if(s1 < 0 || s2 < 0) return;
    unsigned int s1n = (s1 + 1) % 3, s1nn = (s1 + 2) % 3;
    unsigned int s2n = (s2 + 1) % 3, s2nn = (s2 + 2) % 3;
    f1.fillTables();
    f2.fillTables();
    // change opposite
    f1.f1f2f3[s1] = f2.f1f2f3[s2nn];
    f2.f1f2f3[s2] = f1.f1f2f3[s1nn];
    f1.f1f2f3[s1nn] = fId2;
    f2.f1f2f3[s2nn] = fId1;
    // change incident
    vertices[f1.v1v2v3[s1n]].front_index = fId2;
    vertices[f2.v1v2v3[s2n]].front_index = fId1;
    // flip edge
    f1.v1v2v3[s1n] = f2.v1v2v3[s2];
    f2.v1v2v3[s2n] = f1.v1v2v3[s1];
    f1.fillScalars();
    f2.fillScalars();
    // update opposite in opposite face
    int sb = faces[f1.f1f2f3[s1]].getOpposite(fId2); // b
    int sc = faces[f2.f1f2f3[s2]].getOpposite(fId1); // c
    faces[f1.f1f2f3[s1]].f1f2f3[sb] = fId1;
    faces[f2.f1f2f3[s2]].f1f2f3[sc] = fId2;
}

