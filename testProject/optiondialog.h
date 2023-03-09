#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class optionDialog;
}

class optionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit optionDialog(QWidget *parent = nullptr);
    ~optionDialog();

    QString objectNameChanged();
    bool isVisible();
    int getRGB1Value();
    int getRGB2Value();
    int getRGB3Value();

    void setObjectName(QString dialogg);
    void setIsVisible(bool checkBoxx);
    void setRGB1Value(unsigned char RGB1);
    void setRGB2Value(unsigned char RGB2);
    void setRGB3Value(unsigned char RGB3);


private:
    Ui::optionDialog *ui;
};

#endif // OPTIONDIALOG_H
