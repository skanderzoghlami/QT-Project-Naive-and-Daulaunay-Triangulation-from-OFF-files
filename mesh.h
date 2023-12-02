#ifndef MESH_H
#define MESH_H

#include <QGLWidget> //UpgradeQt6: #include <QOpenGLWidget>
#include <map>
// TO MODIFY
class Point
{
public:
    double _x;
    double _y;
    double _z;

    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
};


//** TP : TO MODIFY

#include <vector>
#include <string>
#include "Vector3D.h"
#include <deque>
struct Vertex
{
    Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z)
    {}
    float x, y, z;
    float xyz[3] ;
    int front_index;
    bool isFinite = true ;
    bool operator==(const Vertex &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    // Most  of the functions work on x,y,z coordinates directly, however if it's easier to work with tables just
    // Call these two functions before and after manipulating them
    void fillTables(){
        xyz[0] = x ;xyz[1] = y ;xyz[2] = z ;
    }
    void fillScalars(){
        x = xyz[0]  ; y = xyz[1]  ;z = xyz[2]  ;
    }
};

struct Face
{
    Face(int x = 0, int y = 0, int z = 0)
        : v1(x), v2(y), v3(z) , f1(-1) , f2(-1) , f3(-1)
    {}
    int v1, v2, v3;
    int f1, f2, f3;
    int v1v2v3[3] ;
    int f1f2f3[3] ;
    bool isFinite = true ;
    bool operator==(const Face &other) const
    {
        return v1 == other.v1 && v2 == other.v2 && v3 == other.v3;
    }
    bool operator!=(const Face &other) const
    {
        return v1 != other.v1 || v2 != other.v2 || v3 != other.v3;
    }
    void fillTables(){
        v1v2v3[0] = v1 ;v1v2v3[1] = v2 ;v1v2v3[2] = v3 ;
        f1f2f3[0] = f1 ;f1f2f3[1] = f2 ;f1f2f3[2] = f3 ;
    }
    void fillScalars(){
        v1 = v1v2v3[0]  ; v2= v1v2v3[1]  ;v3 = v1v2v3[2] ;
        f1 = f1f2f3[0]  ;f2 = f1f2f3[1]  ;f3 = f1f2f3[2]  ;
    }
    int getOpposite( int vId1,  int vId2) {
        int relId1 = getRelativeId(vId1);
        int relId2 = getRelativeId(vId2);
        if(relId1 < 0 || relId2 < 0)
            return -1;
        // equivalent à (relId1 +1) % 3 == relId2 ? (relId1 +1) % 3 : (relId1 +2) % 3;
        // on cherche le 3 id relatif entre 0 1 et 2
        return (relId1 | relId2) ^ 3;
    }
    int getRelativeId( int vertexId) {
        fillTables();
        return  v1v2v3[0] == vertexId ? 0 :
                   v1v2v3[1] == vertexId ? 1 :
                   v1v2v3[2] == vertexId ? 2 : -1;
    }
    int getOpposite( int face) {
        fillTables();
        return f1f2f3[0] == face ? 0 :
                   f1f2f3[1] == face ? 1 :
                   f1f2f3[2] == face ? 2 : -1;
    }
    int getNeighborFace(int vertexId) {
        fillTables();

        int relId = getRelativeId(vertexId);
        if (relId < 0)
            return -1;
        return f1f2f3[(relId + 1) % 3];
    }
    void drawWireFrame() const;
};

struct Edge{
    int A,B,C,D ;
    int f1,f2 ;
};


using std::string;
using std::vector;

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertex_init, const std::vector<Face> &faces_init);
    Mesh(){}
    void saveOFFFile(const vector<Vertex> &vertices, const vector<Face> &faces, const string &filename);
    void readOFFFile(const string &filename, std::vector<Vertex> &vertices, std::vector<Face> &faces );
    void splitFace(int indiceFace , Vertex& new_vetex );
    void edgeFlip(int f1 , int f2);
    void edgeFlipV2(int f1, int f2);
    void edgeFlipv3( int fId1,  int fId2);
    int test_orientation(Vertex v1 , Vertex v2 , Vertex v3) ;
    int pointInTriangle(Face &f, Vertex &P);
    void InsertPointInMesh(Vertex& A ) ;
    void splitFaceV2(int indiceFace, Vertex &new_vetex);
    Vector3D getPhiVector(Vertex A , Vertex B );
    bool isLocallyDaulaunay(Edge edge);
    std::deque<Edge> getEdges();
    int lawson();
    void drawWireFrame() const;


    void addFaces(const std::vector<int>& affectedFaces, const std::vector<int>& newFaces);
    void connectFaces(int faceIndex, int v1, int v2, std::map<std::pair<int, int>, std::pair<int, int>>& indexing_map);


    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::map<std::pair<int, int>, std::pair<int, int>> indexing_map;
    vector<int> faceMask ;
    vector<int> VertexMask ;

    bool isDaulaunay(Edge& a) ;
    int delaunizeV2() ;
    bool isFlippable(Edge edge);
    // Mr Iterator For Vertices
    class Iterator_on_vertices
    {
    public:
        // Constructor
        Iterator_on_vertices(std::vector<Vertex>::iterator it) : it_(it) {}

        // operators
        Iterator_on_vertices &operator++()
        {
            ++it_;
            return *this;
        }

        Iterator_on_vertices operator++(int)
        {
            Iterator_on_vertices temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_vertices &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_vertices &other) const
        {
            return it_ != other.it_;
        }

        Vertex &operator*()
        {
            return *it_;
        }

    private:
        std::vector<Vertex>::iterator it_;
    };
    Iterator_on_vertices vertices_begin()
    {
        return Iterator_on_vertices(vertices.begin());
    }
    Iterator_on_vertices vertices_past_the_end()
    {
        return Iterator_on_vertices(vertices.end());
    }
    // Mr Iterator For Faces
    class Iterator_on_faces
    {
    public:
        // Constructor
        Iterator_on_faces(std::vector<Face>::iterator it) : it_(it) {}

        // operators
        Iterator_on_faces &operator++()
        {
            ++it_;
            return *this;
        }

        Iterator_on_faces operator++(int)
        {
            Iterator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_on_faces &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator_on_faces &other) const
        {
            return it_ != other.it_;
        }

        Face &operator*()
        {
            return *it_;
        }

    private:
        std::vector<Face>::iterator it_;
    };
    Iterator_on_faces faces_begin()
    {
        return Iterator_on_faces(faces.begin());
    }
    Iterator_on_faces faces_past_the_end()
    {
        return Iterator_on_faces(faces.end());
    }

    // Mr Circulator on faces
    class Circulator_on_faces
    {
    public:
        // Constructor
        Circulator_on_faces(Face &it, Vertex &vertex, Mesh &mesh) : it_(it), sommet(vertex), mesh(mesh) {}

        // operators
        Circulator_on_faces operator++()
        {
            if (mesh.vertices[it_.v1] == sommet)
            {
                it_ = mesh.faces[it_.f2];

                return *this;
            }
            else if (mesh.vertices[it_.v2] == sommet)
            {
                it_ = mesh.faces[it_.f3];
                return *this;
            }
            it_ = mesh.faces[it_.f1];
            return *this;
        }

        Circulator_on_faces operator++(int)
        {
            Circulator_on_faces temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Circulator_on_faces &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Circulator_on_faces &other) const
        {
            return it_ != other.it_;
        }
        Face &operator*()
        {
            return it_;
        }

    public:
        Face it_;
        Vertex sommet;
        Mesh &mesh;
    };
    // Return the the first face that contains the vertex
    Mesh::Circulator_on_faces incident_faces(Vertex &v)
    {
        for (Mesh::Iterator_on_faces it = faces_begin(); it != faces_past_the_end(); ++it)
        {
            Face face = *it;
            if ((vertices[face.v1] == v) || (vertices[face.v2] == v) || (vertices[face.v3] == v))
            {
                return Mesh::Circulator_on_faces(face, v, *this);
            }
        }
    }
};
// --------------------------------------

class GeometricWorld //Here used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box
public :
  GeometricWorld();
  void draw();
  void drawWireFrame();
  // ** TP Can be extended with further elements;
   Mesh _mesh;
};


#endif // MESH_H
