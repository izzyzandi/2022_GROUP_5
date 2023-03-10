#include "VRRenderThread.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_optiondialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QDirIterator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItem_Options);
    connect(ui->PushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClick);
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
    VRRenderThread* VR = new VRRenderThread;

    QModelIndex index = ui->treeView->currentIndex();

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

 //   emit statusUpdateMessage(QString("Selected Part: ") + selectedPart->data(0).toString() , 0);

    VR->addActorOffline( selectedPart->getNewActor() );

    emit statusUpdateMessage(QString("VR button was clicked!"), 0); 

    VR->start();
}

void MainWindow::handleButton2() {
    optionDialog dialog(this);
    optionDialog visible(this);
    optionDialog RGB1(this);
    optionDialog RGB2(this);
    optionDialog RGB3(this);



    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.objectNameChanged();
        int RGB1 = dialog.getRGB1Value();
        int RGB2 = dialog.getRGB1Value();
        int RGB3 = dialog.getRGB1Value();
        bool Visible = dialog.isVisible();
        

        emit statusUpdateMessage(QString(name)+" " + QString::number(RGB1) +" " + QString::number(RGB2) + " " + QString::number(RGB3) +" "+ (Visible ? "True" : "False"), 0);


        
        
        
    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }

    
}

void MainWindow::handleTreeClick() {
    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index*/

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* we will retrieve the name string from the internal QVariant data array*/
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));
    if (fileName == "")
        return;
    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index*/

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QFileInfo fi(fileName);

    // when adding in CAD files 

    QString name = fi.fileName();
    QString visible("true");
    ModelPart* childChildItem = new ModelPart({ name,visible });

    /* Append to parent*/
    selectedPart->appendChild(childChildItem);
    childChildItem->loadSTL(fileName);
   // selectedPart->set(0, fi.fileName());
    emit statusUpdateMessage(QString(fileName), 0);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetPosition(0, 0, 1000);
    renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
    renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
    renderer->ResetCameraClippingRange();

    updateRender();

}

void MainWindow::on_actionItem_Options_triggered()
{
    optionDialog dialog(this);
    optionDialog visible(this);
    optionDialog RGB1(this);
    optionDialog RGB2(this);
    optionDialog RGB3(this);

    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index*/
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    dialog.setObjectName(selectedPart->data(0).toString());
    dialog.setRGB1Value(selectedPart->getColourR());
    dialog.setRGB2Value(selectedPart->getColourG());
    dialog.setRGB3Value(selectedPart->getColourB());
    dialog.setIsVisible(selectedPart->get_visible());


    if (dialog.exec() == QDialog::Accepted) {

        // gets values of model part to the inputted values from the option dialog
        QString name = dialog.objectNameChanged();
        int RGB1 = dialog.getRGB1Value();
        int RGB2 = dialog.getRGB2Value();
        int RGB3 = dialog.getRGB3Value();
        bool Visible = dialog.isVisible();

        // change the CAD file properties ie make the changes appear on the GUI
        if (selectedPart->getActor()) {
            selectedPart->getActor()->SetVisibility(Visible);
            selectedPart->getActor()->GetProperty()->SetColor(RGB1, RGB2, RGB3);
        }
        else return;
        


        // prints new variables in menu
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



        emit statusUpdateMessage(QString(name) + " " + QString::number(RGB1) + " " + QString::number(RGB2) + " " + QString::number(RGB3) + " " + (Visible ? "True" : "False"), 0);


        updateRender();


    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow::on_actionOpen_Directory_triggered()
{
    QDir dir = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "C:\\"

    );
    // find directory path
    QDir directory(dir.path());
    // get the files in directory
    QStringList files = directory.entryList(QStringList() << "*.stl", QDir::Files);
    // load the files one by one
    foreach(QString fileName, files) {
        QModelIndex index = ui->treeView->currentIndex();

        /* Get a pointer to the item from the index*/

        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        QString address = dir.path() + "/"+fileName;
        QFileInfo fi(fileName);
        
        // when adding in CAD files 

        QString name = fi.fileName();
        QString visible("true");
        ModelPart* childChildItem = new ModelPart({ name,visible });

        /* Append to parent*/
        selectedPart->appendChild(childChildItem);
        qDebug() << "Trying to add file: " << address;
        childChildItem->loadSTL(address);
    }



    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetPosition(0, 0, 1000);
    renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
    renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
    renderer->ResetCameraClippingRange();

    updateRender();

    emit statusUpdateMessage(QString(dir.path()), 0);
}


void MainWindow::updateRender() {
        renderer->RemoveAllViewProps();
        updateRenderFromTree(partList->index(0, 0, QModelIndex()));
        renderer->Render();
        vtkSmartPointer <vtkRenderWindow> renderWindow = renderer->GetRenderWindow();
        renderWindow->Render();
        ui->vtkWidget->repaint();
    }
void MainWindow::updateRenderFromTree(const QModelIndex & index) {
        if (index.isValid()) {
            ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
            renderer->AddActor(selectedPart->getActor());
            /* Retrieve actor from selected part and add to renderer*/
        }
        // check to see if part has any children
        if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
            return;
        }
        /*loop through children and add their actors*/
        int rows = partList->rowCount(index);
        for (int i = 0; i < rows; i++) {
            updateRenderFromTree(partList->index(i, 0, index));
        }
        
    }

