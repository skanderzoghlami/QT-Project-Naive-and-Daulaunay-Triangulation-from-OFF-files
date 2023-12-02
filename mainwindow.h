#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::vector<float> zcoords;

private slots:
    void on_checkBox_stateChanged();
    void on_addVertexButton_clicked();
    void  Daulaunize();
    void  Flip();
    void  Elevate();
    void  RandomMesh();
    void  Save();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
