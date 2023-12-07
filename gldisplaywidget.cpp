#include "gldisplaywidget.h"
#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include "QDebug"
GLDisplayWidget::GLDisplayWidget( QWidget *parent) : QGLWidget(parent), _X(0.f), _Y(0.f), _Z(0.f), _angle(0.f)
{
    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16);
}
void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    std::vector<Vertex> vertices = {
        Vertex(0.0f, 0.0f, 0.0f),
        Vertex(2.0f, 0.0f, 0.0f),
        Vertex(0.0f, 2.0f, 0.0f)
    };
//    std::vector<Vertex> vertices = {
//        Vertex(-1296.33 , -942.784 , 0.0),
//        Vertex(-5862.13 ,  2288.89,  0.0),
//        Vertex(3009.41 ,  -4990.08 ,  0)
//    };


    std::vector<Face> faces = {
        Face(0, 1, 2) // Assuming Face is a class representing a triangle with vertex indices
    };

    // Initialize the Mesh with vertices and faces
    _mesh = Mesh(vertices, faces);
    _mesh.faceMask = {0};
    _mesh.VertexMask = {1,1,1};

}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,  0,0,0,   0,1,0); //gluLookAt(eye, center, up)  //deprecated
                                       // Returns a 4x4 matrix that transforms world coordinates to eye coordinates.
    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angle, 1.0f, 1.0f, 0.0f);

    // Color for your _geomWorld
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    _geomWorld.drawWireFrame();
    //_geomWorld.draw();
    glColor3f(1, 0, 0); // Set color for the mesh
    _mesh.drawWireFrame();
}

void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height); //Viewport in the world window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL(); //UpgradeQt6: update();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    // int dy = event->y() - lastPosMouse.y();

    if( event != NULL )
    {
        _angle += dx;
        _lastPosMouse = event->pos();

        updateGL(); //UpgradeQt6: update();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.5;
    if (!numDegrees.isNull())
    {
      _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}
