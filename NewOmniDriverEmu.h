/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_CONTROLLER_OMNIEMU_H
#define SOFA_COMPONENT_CONTROLLER_OMNIEMU_H

//Sensable include
#include <sofa/helper/LCPcalc.h>
#include <sofa/defaulttype/SolidTypes.h>

#include <sofa/core/behavior/BaseController.h>
#include <SofaOpenglVisual/OglModel.h>
#include <SofaUserInteraction/Controller.h>

#include <sofa/helper/system/thread/CTime.h>

namespace sofa
{
namespace simulation { class Node; }

namespace component
{
namespace visualModel { class OglModel; }

namespace controller
{

class ForceFeedback;


using namespace sofa::defaulttype;
using namespace helper::system::thread;
using core::objectmodel::Data;

/** Holds data retrieved from HDAPI. */
typedef struct
{
    unsigned int id;
    int nupdates;
    int m_buttonState;					/* Has the device button has been pressed. */
    //hduVector3Dd m_devicePosition;	/* Current device coordinates. */
    //HDErrorInfo m_error;
    Vec3d pos;
    Quat quat;
    bool ready;
    bool stop;
} DeviceData;

typedef struct
{
    ForceFeedback* forceFeedback;
    simulation::Node *context;

    sofa::defaulttype::SolidTypes<double>::Transform endOmni_H_virtualTool;
    //Transform baseOmni_H_endOmni;
    sofa::defaulttype::SolidTypes<double>::Transform world_H_baseOmni;
    double forceScale;
    double scale;
    bool permanent_feedback;

    // API OMNI //
    DeviceData servoDeviceData;  // for the haptic loop
    DeviceData deviceData;		 // for the simulation loop

} OmniData;

/**
* Omni driver
*/
class NewOmniDriverEmu : public Controller
{

public:
    typedef Rigid3dTypes::Coord Coord;
    typedef Rigid3dTypes::VecCoord VecCoord;

    SOFA_CLASS(NewOmniDriverEmu, Controller);
    Data<double> scale; ///< Default scale applied to the Phantom Coordinates. 
    Data<double> forceScale; ///< Default forceScale applied to the force feedback. 
    Data<int> simuFreq; ///< frequency of the "simulated Omni"
    Data<Vec3d> positionBase; ///< Position of the interface base in the scene world coordinates
    Data<Quat> orientationBase; ///< Orientation of the interface base in the scene world coordinates
    Data<Vec3d> positionTool; ///< Position of the tool in the omni end effector frame
    Data<Quat> orientationTool; ///< Orientation of the tool in the omni end effector frame
    Data<bool> permanent; ///< Apply the force feedback permanently
    Data<bool> omniVisu; ///< Visualize the position of the interface in the virtual scene
    Data<bool> simulateTranslation; ///< do very naive "translation simulation" of omni, with constant orientation <0 0 0 1>

    OmniData	data;


    NewOmniDriverEmu();
    virtual ~NewOmniDriverEmu();

    virtual void init();
    virtual void bwdInit();
    virtual void reset();
    void reinit();

    int initDevice(OmniData& data);

    void cleanup();
    virtual void draw();

    void onKeyPressedEvent(core::objectmodel::KeypressedEvent *);
    void onKeyReleasedEvent(core::objectmodel::KeyreleasedEvent *);

    void setDataValue();
    void reinitVisual();

    bool afterFirstStep;
    SolidTypes<double>::Transform prevPosition;

    //neede for "omni simulation"
    CTime *thTimer;
    double lastStep;
    bool executeAsynchro;
    Data<VecCoord> trajPts; ///< Trajectory positions
    Data<helper::vector<double> > trajTim; ///< Trajectory timing

private:
    void handleEvent(core::objectmodel::Event *);
    void copyDeviceDataCallback(OmniData *pUserData);
    void stopCallback(OmniData *pUserData);
    sofa::component::visualmodel::OglModel *visu_base, *visu_end;
    bool noDevice;

    bool moveOmniBase;
    Vec3d positionBase_buf;
    bool omniSimThreadCreated;




};


} // namespace controller

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_CONTROLLER_OMNIEMU_H
