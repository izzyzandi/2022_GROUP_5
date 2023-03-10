#include "optiondialog.h"
#include "ui_optiondialog.h"

// Constructor for optionDialog class with QWidget pointer as argument
optionDialog::optionDialog(QWidget *parent) :
    QDialog(parent),
    // Create new Ui object
    ui(new Ui::optionDialog)
{
    // Call setupUi function from Ui object to set up dialog user interface
    ui->setupUi(this);
}

// Destructor for optionDialog class
optionDialog::~optionDialog()
{
    // Delete Ui object created in constructor
    delete ui;
}

// Member function to get the text in the name field of the dialog
QString optionDialog::objectNameChanged() {
    // Return the text in the name field
    return ui->name->text();
}

// Member function to check if the dialog is visible
bool optionDialog::isVisible() {
    // Return the check state of the visible checkbox
    return ui->visible->checkState();

}

// Member function to get the value of the first RGB input
int optionDialog::getRGB1Value() {
    // Return the value of the RGB1 input
    return ui->RGB1->value();
}

// Member function to get the value of the second RGB input
int optionDialog::getRGB2Value() {
    // Return the value of the RGB2 input
    return ui->RGB2->value();
}

// Member function to get the value of the third RGB input
int optionDialog::getRGB3Value() {
    // Return the value of the RGB3 input
    return ui->RGB3->value();
}

// Member function to set the name of the dialog
void optionDialog::setObjectName(QString dialogg) {
    // Set the text of the name field to the given string
    ui->name->setText(dialogg);
}

// Member function to set the visibility of the dialog
void optionDialog::setIsVisible(bool checkBoxx) {
    // Set the check state of the visible checkbox to the given boolean
    ui->visible->setChecked(checkBoxx);
}

// Member function to set the value of the first RGB input
void optionDialog::setRGB1Value(unsigned char RGB1_) {
    // Set the value of the RGB1 input to the given integer
    ui->RGB1->setValue(RGB1_);
}

// Member function to set the value of the second RGB input
void optionDialog::setRGB2Value(unsigned char RGB2_) {
    // Set the value of the RGB2 input to the given integer
    ui->RGB2->setValue(RGB2_);
}

// Member function to set the value of the third RGB input
void optionDialog::setRGB3Value(unsigned char RGB3_) {
    // Set the value of the RGB3 input to the given integer
    ui->RGB3->setValue(RGB3_);
}
