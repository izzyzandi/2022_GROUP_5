#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"
#include <QFileDialog>
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
public slots:
    void handleButton();
    void handleButton2();
    void handleTreeClick();
signals:
    void statusUpdateMessage(const QString &message, int timeout);
private slots:
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;

    ModelPartList* partList;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    
   // VRRenderThread* VRThread;
};
#endif // MAINWINDOW_H
