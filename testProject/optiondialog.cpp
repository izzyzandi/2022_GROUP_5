#include "optiondialog.h"
#include "ui_optiondialog.h"

optionDialog::optionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionDialog)
{
    ui->setupUi(this);
}

optionDialog::~optionDialog()
{
    delete ui;
}

QString optionDialog::objectNameChanged() {
    return ui->name->text();
}

bool optionDialog::isVisible() {
    return ui->visible->checkState();

}

int optionDialog::getRGB1Value() {
    return ui->RGB1->value();
}

int optionDialog::getRGB2Value() {
    return ui->RGB2->value();
}

int optionDialog::getRGB3Value() {
    return ui->RGB3->value();
}

void optionDialog::setObjectName(QString dialogg) {
    ui->name->setText(dialogg);
}

void optionDialog::setIsVisible(bool checkBoxx) {
    ui->visible->setChecked(checkBoxx);
}

void optionDialog::setRGB1Value(unsigned char RGB1_) {
    ui->RGB1->setValue(RGB1_);
}

void optionDialog::setRGB2Value(unsigned char RGB2_) {
    ui->RGB2->setValue(RGB2_);
}

void optionDialog::setRGB3Value(unsigned char RGB3_) {
    ui->RGB3->setValue(RGB3_);
}