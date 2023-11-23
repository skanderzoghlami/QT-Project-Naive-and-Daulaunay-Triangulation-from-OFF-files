#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addVertex, SIGNAL(clicked()), this, SLOT(on_addVertexButton_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->widget->adjustSize();
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
