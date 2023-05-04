/**		@file VRRenderThread.h
  *
  *		The following code depicts the header file for the class "VRRenderThread"  which is inherited from the QThread Class.
  *     The code allows one to run a parallel thread to the main thread for the QT framework.
  *     The class is designed in a format to handle the rendering of 3D visualization using VTTK libraries and OpenVR
  *     The class incooroated many private memeber variables which is used for the rendering, interaction with the window, camera and a group of actors.
  *     The use of the mutex and wait condition allows the data to be passed smoothing between the threads.
  *     The class also has several public functions: constructor, deconstructor, function to add more actors
  *     Additionally, the class also hasa  list of several commands which can be issued to the thread to allow the model to rotate along the three coordinate - planes
  */
#ifndef VR_RENDER_THREAD_H
#define VR_RENDER_THREAD_H

/* Project headers */

/** Qt headers */
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/** Vtk headers */
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>				
#include <vtkOpenVRRenderWindowInteractor.h>	
#include <vtkOpenVRRenderer.h>					
#include <vtkOpenVRCamera.h>	
#include <vtkActorCollection.h>
#include <vtkCommand.h>



/** Note that this class inherits from the Qt class QThread which allows it to be a parallel thread
 * to the main() thread.
 */
class VRRenderThread : public QThread {
    Q_OBJECT

public:
    /** List of command names */
    enum {
        END_RENDER,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z
    } Command;


    /**  Constructor
    * @param parent object of the trend
      */
    VRRenderThread(QObject* parent = nullptr);

    /**  Deconstructor
      */
    ~VRRenderThread();

    /** This allows actors to be added to the VR renderer BEFORE the VR
      * interactor has been started 
     */
    void addActorOffline(vtkActor* actor);


    /** This allows commands to be issued to the VR thread in a thread safe way. 
      * Function will set variables within the class to indicate the type of
      * action / animation / etc to perform. The rendering thread will then impelement this.
      */
    void issueCommand( int cmd, double value );


protected:
    /** This is a re-implementation of a QThread function 
      */
    void run() override;

private:
    /** Standard VTK VR Classes 
        */
    vtkSmartPointer<vtkOpenVRRenderWindow>              window;
    vtkSmartPointer<vtkOpenVRRenderWindowInteractor>    interactor;
    vtkSmartPointer<vtkOpenVRRenderer>                  renderer;
    vtkSmartPointer<vtkOpenVRCamera>                    camera;

    /**Use to synchronise passing of data to VR thread 
        */
    QMutex                                              mutex;      
    QWaitCondition                                      condition;

    /** List of actors that will need to be added to the VR scene 
        */
    vtkSmartPointer<vtkActorCollection>                 actors;

    /** A timer to help implement animations and visual effects 
        */
    std::chrono::time_point<std::chrono::steady_clock>  t_last;

    /** This will be set to false by the constructor, if it is set to true
      * by the GUI then the rendering will end 
      */
    bool                                                endRender; 

    /** Variable List
     *   Some variables to indicate animation actions to apply.
     */
    double rotateX;         
    /**
    * @brief Degrees to rotate around X axis (per time-step) 
    */
    double rotateY;        
    /**
    * @brief Degrees to rotate around Y axis (per time-step)
    */
    double rotateZ;
    /**
    *   @brief Degrees to rotate around Z axis (per time-step) 
    */
};



#endif
