/**     @file optiondialog.h
  *     The following code depicts the header file for the class "optiondialog"  which is inherited from the QDialog Class.
  *     The class allows an effective user interface in order manipulate object properties such as name, visibility and colour.
  *     The code includes a public functions such as: explicit optionDialog(QWidget *parent = nullptr),  ~optionDialog(), QString objectNameChanged(), bool isVisible(), int getRGB1Value(), int getRGB2Value(), int getRGB3Value(), void setObjectName(QString dialogg), oid setIsVisible(bool checkBoxx),  void setRGB1Value(unsigned char RGB1),  void setRGB2Value(unsigned char RGB2),  void setRGB3Value(unsigned char RGB3)
  *     And finally, the code included the private function Ui::optionDialog which is used to allow access to the GUI element
  */


#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

// Include the QDiaglog class from QT
#include <QDialog>

// Create a namespace 
namespace Ui {
class optionDialog;
}

// Define the optionDialog Class
class optionDialog : public QDialog
{
    Q_OBJECT

public:
    /** Constructor
      *  Arguments are standard arguments for this type of class but are not used in this example.
      * @param data is not used
      * @param parent is used by the parent class constructor
      */
    // Constructor which has the QWidget as the parent and is set to nullptr in the case where there is no parent provided
    explicit optionDialog(QWidget *parent = nullptr);

    /** Deconstructor
      *  Frees root item allocated in constructor
      */
    // Deconstructor
    ~optionDialog();

    // Getter which is used to retrieve the information from the dialog
    QString objectNameChanged(); // Returns a new name for an object
    /**
        * @brief Returns a new name for the object by the user .
        * @return Name of the object.
    */
    bool isVisible(); // Returns to show whether the object should be vsible or not
    /**
        *@brief Returns whether or not the object is visible.
        *@return True if the object is visible, false otherwise.
    */
    int getRGB1Value(); // Returns a value for the first RGB component
    /**
        *@brief Returns a value for the first RGB component.
        *@return value for the first RGB component
    */
    int getRGB2Value(); // Returns a value for the second RCG component
    /**
       *@brief Returns a value for the second RGB component.
       *@return value for the second RGB component
    */
    int getRGB3Value(); // Returns a value for the third RGB component
    /**
        *@brief Returns a value for the third RGB component.
        *@return value for the third RGB component
    */

    // Setter which is used to set the information onto the dialog
    void setObjectName(QString dialogg); // Sets the name for the object
    /**
        * @brief sets a new name for the object by the user .
        * @param diaglog the name of object to be set.
    */

    void setIsVisible(bool checkBoxx); // Sets the value for the visibility of the object
    /**
        * @brief sets the visibility for the object  .
        * @param tick if True (Object is visible), false otherwise.
    */

    void setRGB1Value(unsigned char RGB1); // Sets the value for the first RGB value
    /**
        * @brief sets the value of first RGB component
        * @param sets the value of first RGB component
    */

    void setRGB2Value(unsigned char RGB2); // Sets the value for the second RCG value
    /**
        * @brief sets the value of second RGB component
        * @param sets the value of second RGB component
    */

    void setRGB3Value(unsigned char RGB3); // Sets the value for the third RGB value
    /**
        * @brief sets the value of third RGB component
        * @param sets the value of third RGB component
    */

// Pointer for the UI Object
    /** The private member variable will hold the value of the pointer to the Ui::optionDialog
    *   Access to the varibale is restricted to the class and its member function
    *   @brief Pointer to the user interface for the given option diaglog
    */
private:
    Ui::optionDialog *ui;
};

#endif // OPTIONDIALOG_H
