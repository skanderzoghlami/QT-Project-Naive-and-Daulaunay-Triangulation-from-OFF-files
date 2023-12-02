#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>
#include <iostream>  // for std::cerr
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addVertex, SIGNAL(clicked()), this, SLOT(on_addVertexButton_clicked()));
    connect(ui->Daulaunize, SIGNAL(clicked()), this, SLOT(Daulaunize()));
    connect(ui->Flip, SIGNAL(clicked()), this, SLOT(Flip()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_checkBox_stateChanged()));
    connect(ui->rmesh, SIGNAL(clicked()), this, SLOT(RandomMesh()));
    connect(ui->elevate, SIGNAL(clicked()), this, SLOT(Elevate()));
    connect(ui->Save, SIGNAL(clicked()), this, SLOT(Save()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_stateChanged()
{
    GLDisplayWidget* glWidget = ui->widget;
    // Print vertices
    std::cout << "Vertices:" << std::endl;
    for (const Vertex& vertex : glWidget->_mesh.vertices) {
        std::cout << "Vertex (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }

    // Print faces
    std::cout << "Faces:" << std::endl;
    for ( Face& face : glWidget->_mesh.faces) {
        std::cout << "Face: ";
        face.fillTables();
        for (int vertexIndex : face.v1v2v3) {
            const Vertex& vertex = glWidget->_mesh.vertices[vertexIndex];
            std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ") ";
        }
            std::cout << "Neighbours = (" << face.f1 << ", " << face.f2 << ", " << face.f3 << ") ";
        std::cout << std::endl;
    }

}

void  MainWindow::Daulaunize(){
    GLDisplayWidget* glWidget = ui->widget;
    glWidget->_mesh.lawson();
    ui->widget->updateGL();  // UpgradeQt6: ui->widget->update();
}
void  MainWindow::Save(){
    GLDisplayWidget* glWidget = ui->widget;
    glWidget->_mesh.saveOFFFile(glWidget->_mesh.vertices ,glWidget->_mesh.faces , "Output.off" );
}

void  MainWindow::Flip(){
    GLDisplayWidget* glWidget = ui->widget;
    double x = ui->summet1->text().toInt();
    double y = ui->Summet2->text().toInt();
    glWidget->_mesh.edgeFlip(x, y);
    ui->widget->updateGL();  // UpgradeQt6: ui->widget->update();
}

void MainWindow::on_addVertexButton_clicked()
{
    // Get coordinates from line edits
    double x = ui->lineEdit->text().toDouble();
    double y = ui->lineEdit_2->text().toDouble();

    GLDisplayWidget* glWidget = ui->widget;
    // Create a new vertex
    Vertex newVertex(x, y, 0.0);  // You may adjust the third parameter based on your needs
//    // Add the new vertex to the mesh
    glWidget->_mesh.InsertPointInMesh(newVertex);
//    // Redraw the scene
    ui->widget->updateGL();  // UpgradeQt6: ui->widget->update();
}
void MainWindow::Elevate()
{
    GLDisplayWidget* glWidget = ui->widget;

    if (glWidget->_mesh.vertices.size() != zcoords.size())
    {
        qDebug() << "Error: Size mismatch between mesh vertices and zcoords vector";

        // Resize the zcoords vector to match the size of vertices
        zcoords.resize(glWidget->_mesh.vertices.size());

        // Generate random values for the missing zcoords with small variations
        for (size_t i = 0; i < zcoords.size(); ++i)
        {
            if (i >= glWidget->_mesh.vertices.size())
            {
                // Generate a random value with a small variation (for missing vertices)
                double smallVariation = 5 * ((rand() % 100) / 100.0);
                zcoords[i] = zcoords[i - 1] + smallVariation;
            }
        }
    }

    for (size_t i = 0; i < glWidget->_mesh.vertices.size(); ++i)
    {
        // Elevate each vertex with its corresponding Z coordinate from zcoords vector
        glWidget->_mesh.vertices[i].z =  5 +zcoords[i];
    }

    // Call updateGL() to redraw the scene
    glWidget->updateGL();
}

void  MainWindow::RandomMesh(){
    GLDisplayWidget* glWidget = ui->widget;
    // Seed the random number generator
    std::srand(std::time(0));

    // Generate 10 random vertices and insert them into the mesh
    for (int i = 0; i < 10; ++i)
    {
        // Generate random x and y coordinates (adjust the range as needed)
        float x = static_cast<float>(std::rand() % 100) / 10.0f;
        float y = static_cast<float>(std::rand() % 100) / 10.0f;

        // Create a vertex with the random coordinates
        Vertex newVertex(x, y, 0.0f);

        // Insert the vertex into the mesh
        glWidget->_mesh.InsertPointInMesh(newVertex);

        // Optionally, you can call updateGL() here to redraw the scene
        ui->widget->updateGL();
    }
}

//void  MainWindow::RandomMesh(){
//{
//    //QString currentPath = QCoreApplication::applicationDirPath(); // Get the current working directory

//    GLDisplayWidget* glWidget = ui->widget;
//    // Open the file
//    QString filePath = "alpes_poisson.txt";
//    std::ifstream file(filePath.toStdString());

//    if (!file.is_open())
//    {
//        qDebug() << "Error: Unable to open file" << filePath;
//        return;
//    }

//    // Read the number of vertices
//    int numVertices;
//    file >> numVertices;

//    // Read vertex coordinates and insert them into the mesh
//    zcoords.clear();
//    zcoords={1647.61 ,1096.62 , 1633};
//    for (int i = 0; i < numVertices; ++i)
//    {
//        float x, y, z;
//        file >> x >> y >> z;
//        zcoords.push_back(z);
//        // Create a vertex with the coordinates
//        Vertex newVertex(x, y, 0);

//        // Insert the vertex into the mesh
//        glWidget->_mesh.InsertPointInMesh(newVertex);
//    }

//    // Close the file
//    file.close();
//    // Optionally, you can call updateGL() here to redraw the scene
//    glWidget->updateGL();
//}
//}
