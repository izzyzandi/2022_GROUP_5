#include "VRRenderThread.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_optiondialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QTimer>
#include <QColor>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>
#include <vtkClipDataSet.h>
#include <vtkTransform.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Add an action to the context menu of the tree view
    ui->treeView->addAction(ui->actionItem_Options);

    // Connect signals from the two push buttons to slots in the main window class
    connect(ui->PushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(ui->checkBox, &QCheckBox::released, this, &MainWindow::checkbox1);
    connect(ui->checkBox_2, &QCheckBox::released, this, &MainWindow::checkbox2);
 

    // Connect the signal for when an item in the tree view is clicked to a slot in the main window class
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClick);

    // Connect the signal for when a status update message is emitted to the status bar in the user interface
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);


    /* create/allocate the ModelList */
    this->partList = new ModelPartList("PartsList");

    /* Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);

    /*Manually create a model tree - they're better and flexible
    eg nested functions*/
    ModelPart* rootItem = this->partList->getRootItem();

    QString name = QString("Model");
    QString visible("true");

    /* Creat child item */
    ModelPart* childItem = new ModelPart({ name,visible });

    /*Append to tree top-level*/
    rootItem->appendChild(childItem);

  
    // link a render window with qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);
    // add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer
    it will copy and paste cylinder example*/
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // mapper pushes geometry into graphics library and can do colour mapping
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // actor is a grouping mechanism. besides geometry mapper, it also has a property, transformation matrix 
    // and texture map
    // set colour and rotate is around the XY axes
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1, 0, 0.35);
    cylinderActor->RotateX(30);
    cylinderActor->RotateY(-45);

    renderer->AddActor(cylinderActor);

    // reset camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

  
}

void MainWindow::handleButton() {
    // Create a new VRRenderThread object
    VRRenderThread* VR = new VRRenderThread;

    // Get the currently selected item in a tree view
    QModelIndex index = ui->treeView->currentIndex();

    // Cast the selected item to a ModelPart object
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Emit a status update message indicating the selected part
    // emit statusUpdateMessage(QString("Selected Part: ") + selectedPart->data(0).toString() , 0);
    // Add a new actor to the VRRenderThread using the selectedPart
    VR->addActorOffline( selectedPart->getNewActor() );

    // Emit a status update message indicating that the VR button was clicked
    emit statusUpdateMessage(QString("VR button was clicked!"), 0); 

    // Start the VRRenderThread
    VR->start();
}

void MainWindow::handleButton2() {
    

    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::changeColour);
    timer->start(100); // update color every 100 milliseconds

    


  
}


void MainWindow::changeColour() {

    /* Get the index of the selected item in the tree view*/
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index*/
    //Get a pointer to the ModelPart object represented by the selected item
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    static int hue = 0; // hue ranges from 0 to 360 degrees
    QColor color = QColor::fromHsv(hue, 255, 255); // convert hue to RGB color
    int red, green, blue;
    color.getRgb(&red, &green, &blue); // get the RGB values of the color
    selectedPart->getActor()->GetProperty()->SetColor(red, blue, green);
    hue = (hue + 5) % 360; // increment hue by 5 degrees
    updateRender();
}

void MainWindow::checkbox1() {
    emit statusUpdateMessage(QString("check 1 was clicked!"), 0);

    /* Get the index of the selected item in the tree view*/
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index*/
    //Get a pointer to the ModelPart object represented by the selected item
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (ui->checkBox->isChecked()) {
        selectedPart->setFilterAndActor(x, 2);
        y = 2;
    }
    else
    {
        y = 0;
        selectedPart->setFilterAndActor(x, y);

    }
    updateRender();
}

void MainWindow::checkbox2() {
    emit statusUpdateMessage(QString("check 2 was clicked!"), 0);
    
    /* Get the index of the selected item in the tree view*/
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index*/
    //Get a pointer to the ModelPart object represented by the selected item
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    
    if (ui->checkBox_2->isChecked()) {
        selectedPart->setFilterAndActor(1, y);
        x = 1;
    }
    else
    {
        x = 0;
        selectedPart->setFilterAndActor(x, y);
    }
    updateRender();
}

void MainWindow::handleTreeClick() {
    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index*/
    // Here we're assuming that the model used by the tree view is a ModelPart

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* we will retrieve the name string from the internal QVariant data array*/
    // We assume that the name is stored in the first column of the model
    QString text = selectedPart->data(0).toString();

    // Emit a signal to update the status bar with the selected item's name
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

// Destructor for the main window
MainWindow::~MainWindow()
{
    // Free up any resources used by the UI
    delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{
    //Prompt the user in order to select a file
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    // return if no file has been selected
    if (fileName == "")
        return;
    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index*/
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Get information about the selected file
    QFileInfo fi(fileName);

    // when adding in CAD files 
    QString name = fi.fileName();
    QString visible("true");

    // Create a new model part for the selected file and add it as a child to the selected item
    ModelPart* childChildItem = new ModelPart({ name,visible });

    /* Append to parent*/
    selectedPart->appendChild(childChildItem);

    // Load the selected file into the new model part
    childChildItem->loadSTL(fileName);
    
    
   // selectedPart->set(0, fi.fileName());
    emit statusUpdateMessage(QString(fileName), 0);

    // Reset the camera settings for the renderer
    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetPosition(0, 0, 1000);
    renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
    renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
    renderer->ResetCameraClippingRange();

    // Update the renderer with the new changes
    updateRender();

    // makes the object auto white but change later to the actual colour
    childChildItem->setColour(1, 1, 1);
    
}

void MainWindow::on_actionItem_Options_triggered()
{
    // Create five optionDialog instances, each with a pointer to the main window as its parent
    optionDialog dialog(this);
    optionDialog visible(this);
    optionDialog RGB1(this);
    optionDialog RGB2(this);
    optionDialog RGB3(this);

    /* Get the index of the selected item in the tree view*/
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index*/
    //Get a pointer to the ModelPart object represented by the selected item
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Set the initial values of the option dialog to match the values of the selected ModelPart object
    dialog.setObjectName(selectedPart->data(0).toString());
    dialog.setRGB1Value(selectedPart->getColourR());
    dialog.setRGB2Value(selectedPart->getColourG());
    dialog.setRGB3Value(selectedPart->getColourB());
    dialog.setIsVisible(selectedPart->get_visible());

    // Open the option dialog and wait for the user to make changes
    if (dialog.exec() == QDialog::Accepted) {

        // gets values of model part to the inputted values from the option dialog
        QString name = dialog.objectNameChanged();
        int RGB1 = dialog.getRGB1Value();
        int RGB2 = dialog.getRGB2Value();
        int RGB3 = dialog.getRGB3Value();
        bool Visible = dialog.isVisible();

        // change the CAD file properties ie make the changes appear on the GUI
        // If the selected ModelPart object has a corresponding actor in the CAD file, update its properties
        if (selectedPart->getActor()) {
            selectedPart->getActor()->SetVisibility(Visible);
            selectedPart->getActor()->GetProperty()->SetColor(RGB1, RGB2, RGB3);
        }
        // If the selected ModelPart object has no corresponding actor, return without doing anything
        else return;
        

        // prints new variables in menu
        // Update the values of the selected ModelPart object to match the new values from the option dialog
        selectedPart->set(0, name);
        selectedPart->set(0, name);
        selectedPart->setColour(RGB1, RGB2, RGB3);
        selectedPart->setVisible(Visible);
        if (Visible)
        {
            selectedPart->set(1, "true");
        }
        else
        {
            selectedPart->set(1, "false");
        }


        // Emit a signal to update the status bar with the new values of the selected ModelPart object
        emit statusUpdateMessage(QString(name) + " " + QString::number(RGB1) + " " + QString::number(RGB2) + " " + QString::number(RGB3) + " " + (Visible ? "True" : "False"), 0);

        //update the CAD file renderer value
        updateRender();


    }
    else {
        // If the user clicked "Cancel", emit a signal to update the status bar with a message indicating that the dialog was rejected
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow::on_actionOpen_Directory_triggered()
{
    // Open a dialog box to get the selected directory
    QDir dir = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "C:\\"

    );
    // Create a QDir object for the selected directory
    QDir directory(dir.path());
    // Get a list of all files in the directory with the extension ".stl"
    QStringList files = directory.entryList(QStringList() << "*.stl", QDir::Files);
    // Load each file in the list of files
    foreach(QString fileName, files) {
        // Get the currently selected index in the tree view
        QModelIndex index = ui->treeView->currentIndex();

        // Get a pointer to the currently selected ModelPart object
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        // Create the address for the file to be loaded
        QString address = dir.path() + "/"+fileName;
        // Get the file info for the current file
        QFileInfo fi(fileName);
        
        // when adding in CAD files 
        // Set the name and visibility for the child ModelPart object
        QString name = fi.fileName();
        QString visible("true");
        ModelPart* childChildItem = new ModelPart({ name,visible });

        /* Append to parent*/
        // Append the child ModelPart object to the parent ModelPart object
        selectedPart->appendChild(childChildItem);

        // Print a debug message to indicate the file being added
//        qDebug() << "Trying to add file: " << address;

        // Load the STL file for the child ModelPart object
        childChildItem->loadSTL(address);

        childChildItem->setColour(1, 1, 1);
        

    }

    // Reset the camera position and other view parameters for the renderer
    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetPosition(0, 0, 1000);
    renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
    renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
    renderer->ResetCameraClippingRange();

    // Update the renderer
    updateRender();

    // Emit a signal to indicate the selected directory and set the status to 0
    emit statusUpdateMessage(QString(dir.path()), 0);
}


void MainWindow::updateRender() {
        // Remove all view props from the renderer
        renderer->RemoveAllViewProps();
        // Update the renderer with the actors of all parts in the tree model
        updateRenderFromTree(partList->index(0, 0, QModelIndex()));
        // Render the scene
        renderer->Render();
        // Get the render window from the renderer
        vtkSmartPointer <vtkRenderWindow> renderWindow = renderer->GetRenderWindow();
        // Render the render window
        renderWindow->Render();
        // Repaint the VTK widget
        ui->vtkWidget->repaint();
    }
void MainWindow::updateRenderFromTree(const QModelIndex & index) {
        // Check if the index is valid
        if (index.isValid()) {
            // Cast the internalPointer of the index to a ModelPart object
            ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
            // Add the selected part's actor to the renderer
            renderer->AddActor(selectedPart->getActor());
            /* Retrieve actor from selected part and add to renderer*/
        }
        // check to see if part has any children
        if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
            // If not, return
            return;
        }
        /*loop through children and add their actors*/
        int rows = partList->rowCount(index);
        for (int i = 0; i < rows; i++) {
            updateRenderFromTree(partList->index(i, 0, index));
        }
        
    }

