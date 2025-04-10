/**     @file ModelPart.h
  *     The following code shows the  header file which contains the class declerations for each of the model parts in a tree-view format.
  *     The code also incorporates orther header files to allows for the implementations to occur smoothly such as QString, QList and QVariant, as well as VTK libraries.
  *     The classes part of the code contains various methods and variables in order to create an effective tree view.
  *     The method includes: constructor, deconstructor and sets of getters and setters in order to manipulate properties such as color, visibility , parent and child of the model part.
  *     The varibales include: list of child items, visibility flag, RGB values and name
  *     At end of the classes part of the code there is a commented out section of variable and properties for the VTK, this because the VTK library has not been intsalled yet.
  *
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>


  /* VTK headers - will be needed when VTK used in next worksheet,
   * commented out for now
   *
   * Note that there are a few function definitions and variables
   * commented out below - this is because you haven't yet installed
   * the VTK library which is needed.
   */
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>
#include <vtkPolyDataMapper.h>

class ModelPart {
public:
    /** Constructor
     * @param data is a List (array) of strings for each property of this item (part name and visiblity in our case
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Deconstructor
      * Needs to free array of child items
      */
    ~ModelPart();

    /** Add a child to this item.
      * @param item Pointer to child object (must already be allocated using new)
      */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item
      * @param row is the row number (below this item)
      * @return pointer to the item requested.
      */
    ModelPart* child(int row);

    /** Return number of children to this item
      * @return number of children
      */
    int childCount() const;         /* Note on the 'const' keyword - it means that this function is valid for
                                     * constant instances of this class. If a class is declared 'const' then it
                                     * cannot be modifed, this means that 'set' type functions are usually not
                                     * valid, but 'get' type functions are.
                                     */

                                     /** Get number of data items (2 - part name and visibility string) in this case.
                                       * @return number of visible data columns
                                       */
    int columnCount() const;

    /** Return the data item at a particular column for this item.
      * i.e. either part name of visibility
      * used by Qt when displaying tree
      * @param column is column index
      * @return the QVariant (represents string)
      */
    QVariant data(int column) const;


    /** Default function required by Qt to allow setting of part
      * properties within treeview.
      * @param column is the index of the property to set
      * @param value is the value to apply
      */
    void set(int column, const QVariant& value);

    /** Get pointer to parent item
      * @return pointer to parent item
      */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item
      * @return row index
      */
    int row() const;


    /** Set colour
      * (0-255 RGB values as ints)
      */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    /** Set visible flag
      * @param isVisible sets visible/non-visible
      */
    void setVisible(bool isVisible);

    /** Get visible flag
      * @return visible flag as boolean
      */
    bool visible();

    /** Load STL file
      * @param fileName
      */
    void loadSTL(QString fileName);

    /** Return actor
      * @return pointer to default actor for GUI rendering
      */
    vtkSmartPointer<vtkActor> getActor();

    /** Return new actor for use in VR
      * @return pointer to new actor
      */
    vtkActor* getNewActor(int x, int y);

    /** The function returns a 'vtkSmartPointer<vtkMapper>' object 
    *   @brief It will return a smart pointer to the mapper which is used by the object
    *   The caller is used for managing the lifetime of the returned object
    *   @return smart pointer to the mapper used by the object
    */
    vtkSmartPointer<vtkMapper> getMapper();

    /** The function will set the filter and actor in relation to the object at the coordinates
    *   @brief will set the filter and the actor at the required coordinates
    *   @param x the x-coordinate of the object 
    *   @param y the y-cooridnate of the object
    */
    void setFilterAndActor(int x, int y);
    
    /** The function will return a boolean value in relation to the visibility of the object if vible it will be true if not it will be false
    *   @brief returns the viibility state of the object
    *   @return boolean value indicating whether the object is visible ('true') or not ('false')
    */
    bool get_visible();

    int get_shrink();
    void set_shrink(int shrink_);
    int get_clip();
    void set_clip(int clip_);

private:
    /**
    *   @brief it is m_childItem QList of a ModelPart item
    */
    QList<ModelPart*>                           m_childItems;       /** List (array) of child items */
    
    /**
    *   @brief it is m_itemdata QList of a ModelPart Item
    */
    QList<QVariant>                             m_itemData;         /** List (array of column data for item */

    /**
    *   @brief it is m_parentItem QList of a ModelPart Item
    */
    ModelPart* m_parentItem;       /**< Pointer to parent */

    /* These are some typical properties that I think the part will need, you might
     * want to add you own.
     */
    
    /**
    *   @brief gives the name of the ModelPart
    */
    QString name;
    
    /**
    * @brief Shows whether the model rendering is visble of not by indicating it via True/False
    */
    bool isVisible;          /** True/false to indicate if should be visible in model rendering */

    /**
    *   @brief RG1, RG2 and RG3 value for the ModelParts
    */
    unsigned char RGB1 = 0;
    unsigned char RGB2 = 0;
    unsigned char RGB3 = 0;

    /* These are vtk properties that will be used to load/render a model of this part,
     * commented out for now but will be used later
     */
    
    /** 
    *   @brief Datafile from which part loaded 
    */
    vtkSmartPointer<vtkSTLReader>               file;  

    /**
    *   @brief Mapper for rendering
    */
    vtkSmartPointer<vtkMapper>                  mapper;    

    /**
    *   @brief Actor for rendering
    */
    vtkSmartPointer<vtkActor>                   actor;             

    /**
    *   @brief User defineable colour
    */
    vtkColor3<unsigned char>                    colour;  

    int shrink = 0;
    int clip = 0;
};


#endif

