/**		@file mainwindow.h
  *
  *		The following code depicts the header file for the class "MainWindow" which is inherited from the "QMainWindow".
  *     The code includes serval header files: "ModelPart.h", "ModelPartList.h" and "optiondialog.h".
  *     The class containes many public and private member functions: updateRender(), handleButton(), on_actionOpen_File_triggered() and changeColour().
  *     The class also contains many private member variables namely pointers such as: Ui::MainWindow and ModelPartList as well as certain VTK objects which contain renderers and render windows
  */



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// List of headers which are included
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

/**
* @brief  Contains the MainWindow class as a subclass of the QMainWindow
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**  Constructor
    * @param the parent pointer of the parent widget
    */
    // Constructor which initializes teh QWidget parent in order create a 3D render it also initializes the ModelPartList
    MainWindow(QWidget *parent = nullptr);

    /**Denstructor
    */
    // Deconstructor will help to free any dynamic allocated memory
    ~MainWindow();

    /** 
    *   @brief Is used to update the renderer window
    *   @param updates the renderer window
    */
    // It is a function used to updat the 3D visulaization if any changes are made to the model part
    void updateRender();

    /** 
    *   @brief Is used to update the renderer window depending on the selected tree view
    *   @param index representing the tree-view
    */
    void updateRenderFromTree(const QModelIndex& index);

public slots:
    /** Private slots
    * The slots are used to handle user interface with the help of buttons and the tree-view
    */
    void handleButton();
    void handleButton2();
    void handleTreeClick();
signals:
    /** It is a signal which is used to send status updates on to the main window
    */
    void statusUpdateMessage(const QString &message, int timeout);

private slots:
    /**Private slots
        * slot opens the file dialog box, option dialog boxand directory dialog box
    */
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();
    void on_actionOpen_Directory_triggered();

/** Shows the memeber variables
*/
private:
    Ui::MainWindow *ui;

    ModelPartList* partList;

    vtkSmartPointer<vtkRenderer> renderer; // render the 3D visualization
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow; // provide an independent interface with OpenGL render window
    
    /**Used to change the colour of the presently selected model part in the form of a 3D visulaization
    */
    void changeColour();
   // VRRenderThread* VRThread;
};
#endif // MAINWINDOW_H
