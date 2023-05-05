/**     @file ModelPartList.h
  *     The following code depicts the header file for the class "ModelPartList" which is inherited from the "QAbstractItemModel" used to create a tree view.
  *     The file incorportated header filles including ModelPart.h.
  *     The ModelPart class constains a private member variable "rootItem", which indicated the pointer to the root item in the tree.
  *     The class contains a constructors "QString" object and "QObject" pointer, on the other hand the deconstructor allows the root item to be allocated in the constructor.
  *     The class has several members: columnCount(), data(). flags(), headerData(), index(), parent(), rowCount(), getRootItem() and appendChild().
  */
  
#ifndef VIEWER_MODELPARTLIST_H
#define VIEWER_MODELPARTLIST_H


#include "ModelPart.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QList>

class ModelPart;

class ModelPartList : public QAbstractItemModel {
    Q_OBJECT        /**< A special Qt tag used to indicate that this is a special Qt class that might require preprocessing before compiling. */
public:
    /** Constructor
      *  Arguments are standard arguments for this type of class but are not used in this example.
      * @param data is not used
      * @param parent is used by the parent class constructor
      */
    ModelPartList( const QString& data, QObject* parent = NULL );

    /** Destructor
      *  Frees root item allocated in constructor
      */
    ~ModelPartList();

    /** Return column count
      * @param parent is not used
      * @return number of columns in the tree view - "Part" and "Visible", i.e. 2 in this case
      */
    int columnCount( const QModelIndex& parent ) const;

    /** This returns the value of a particular row (i.e. the item index) and 
      *  columns (i.e. either the "Part" or "Visible" property).
      *  It is used by QT internally - this is how Qt retrieves the text to display in the TreeView
      * @param index in a stucture Qt uses to specify the row and column it wants data for
      * @param role is how Qt specifies what it wants to do with the data
      * @return a QVariant which is a generic variable used to represent any Qt class type, in this case the QVariant will be a string
      */
    QVariant data( const QModelIndex& index, int role ) const;

    /** Standard function used by Qt internally.
      * @param index in a stucture Qt uses to specify the row and column it wants data for
      * @return a Qt item flags
      */
    Qt::ItemFlags flags( const QModelIndex& index ) const;


    /** Standard function used by Qt internally.
      */
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;


    /** Get a valid QModelIndex for a location in the tree (row is the row in the tree under "parent"
      * or under the root of the tree if parent isnt specified. Column is either 0 = "Part" or 1 = "Visible" 
      * in this example 
      * @param row is the item index
      * @param column is 0 or 1 - part name or visible stringstream
      * @param parent where the row is referenced from, usually the tree root
      * @return the QModelIndex structure
     */
    QModelIndex index( int row, int column, const QModelIndex& parent ) const;


    /** Take a QModelIndex for an item, get a QModel Index for its parent
      * @param index of item
      * @return index of parent
      */
    QModelIndex parent( const QModelIndex& index ) const;

    /** Get number of rows (items) under an item in tree
      *  @param is the parent, all items under this will be counted
      *  @return number of children
      */
    int rowCount( const QModelIndex& parent ) const;

    /** Get a pointer to the root item of the tree
      * @return the root item pointer
      */
    ModelPart* getRootItem();

    /** The function helps create a new index which shows the newly appended child and returns it
      * @brief Appends the child to the specific parent index with the given data
      * @param parent the parent index to the chld will be appended
      * @param data the data would be assigned to the new index which was created
     */
    QModelIndex appendChild( QModelIndex& parent, const QList<QVariant>& data );

    
/** The private member variable will have the pointer ModelPart object which is the root item of the model
*   @brief pointer for the root item of the model 
*/

private:
    ModelPart *rootItem;    /**< This is a pointer to the item at the base of the tree */
};
#endif

