/*		SOCIAL NAVIGATION NUEVO
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"
#include <math.h>

#define PI M_PI

/**
 * \brief Default constructor
 */

SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	qDebug() << __FUNCTION__ ;
	active = false;
	worldModel = AGMModel::SPtr(new AGMModel());
	worldModel->name = "worldModel";
	haveTarget = false;
	innerModel = std::make_shared<InnerModel>();


	//Timed slot to read TrajectoryRobot2D state
//	connect(&trajReader, SIGNAL(timeout()), &aE, SLOT(readTrajState()));
	connect(gaussiana,SIGNAL(clicked()),&socialrules, SLOT(calculateGauss()));
//	connect(por,SIGNAL(clicked()),&socialrules, SLOT(PassOnRight()));
	connect(objint,SIGNAL(clicked()),&socialrules, SLOT(checkObjectAffordance()));
	connect(datos,SIGNAL(clicked()),&socialrules, SLOT(saveData()));
	
	connect(gotoperson,SIGNAL(clicked()),&socialrules, SLOT(goToPerson()));
	//trajReader.start(1000);
    connect(follow, SIGNAL (clicked()),&socialrules,SLOT(checkstate()));
    connect(accompany, SIGNAL (clicked()),&socialrules,SLOT(checkstate()));
    connect(por, SIGNAL (clicked()),&socialrules,SLOT(checkstate()));

    socialrules.idselected = idselect;
    socialrules.follow = follow;
    socialrules.accompany = accompany;
    socialrules.por = por;

}
/**
 * \brief Default destructor
 */
SpecificWorker::~SpecificWorker()
{
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList paramsL)
{
    //Extract robot name
    try{ robotname = paramsL.at("RobotName").value;}
    catch(const std::exception &e){ std::cout << e.what() << "SpecificWorker::SpecificWorker - Robot name defined in config. Using default 'robot' " << std::endl;}

    confParams  = std::make_shared<RoboCompCommonBehavior::ParameterList>(paramsL);

	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;

#ifdef USE_QTGUI
    viewer = std::make_shared<InnerViewer>(innerModel, "Social Navigation");  //InnerViewer copies internally innerModel so it has to be resynchronized
#endif

    try
    {
        RoboCompAGMWorldModel::World w = agmexecutive_proxy->getModel();
        AGMExecutiveTopic_structuralChange(w);
        // Initializing PathFinder
        pathfinder.initialize(innerModel, viewer, confParams, laser_proxy, omnirobot_proxy);
        // Initializing SocialRules
        socialrules.initialize(socialnavigationgaussian_proxy, agmexecutive_proxy, mutex, &pathfinder, worldModel, innerModel);

        AGMExecutiveTopic_structuralChange(w);
        rDebug2(("Leaving Structural Change"));
    }

        catch(...)
        {	rDebug2(("SetParams ---- The executive is probably not running, waiting for first AGM model publication...")); }


    rDebug2(("NavigationAgent started"));
    rDebug2(("Leaving setParams"));


    this->Period = period;
    timer.start(Period);
}


/**
 * \brief Check if persons are included in the AGM. 
 * Then, their pose is stored.
 * Everytime a person has moved, its position is updated
 */
void SpecificWorker::compute() {

	static bool first = true;
	if (first) {
		rDebug2(("navigationAgent started"));
		qDebug()<<"saving innerModel";
		first = false;
	}


	pathfinder.run();
    viewer->run();


    if (worldModelChanged) {
        socialrules.update(worldModel);
        socialrules.checkRobotmov();
        worldModelChanged = false;
	}

	if (!socialrules.pSymbolId.empty()) {
		checkHumanBlock();
	}

	first = false;

}


void SpecificWorker::checkHumanBlock()
{
    int robotID = -1;
    AGMModel::SPtr newModel(new AGMModel(worldModel));

    vector <int32_t> pId_blocking = pathfinder.getHumanBlocking();
	vector <int32_t> pId_softblocking = pathfinder.getHumanSoftBlocking();
    vector <int32_t> pId_affblocking = pathfinder.getAffordanceBlocking();

    if ((previous_blockinglist != pId_blocking) or (previous_softblockinglist != pId_softblocking) or (previous_affordanceslist != pId_affblocking))
    {
        try
        {
            robotID = worldModel->getIdentifierByType("robot");
        }

        catch(...)
        {
            std::cout<<__FUNCTION__<<" No robot found in AGM model"<<std::endl;
            return;
        }

        for (auto id1:previous_blockinglist)
        {
			try
			{
				newModel->removeEdgeByIdentifiers(id1, robotID, "block");
                qDebug ()<<"Se elimina el enlace block a la persona  " << id1;
			}

			catch(...)
			{
				std::cout<<__FUNCTION__<<"No existe el enlace"<<std::endl;

			}
        }

        for (auto id1:pId_blocking)
        {

			try
			{
				newModel->addEdgeByIdentifiers(id1, robotID, "block");
				qDebug ()<<"Se añade el enlace block a la persona  " << id1;
			}

			catch(...)
			{
				std::cout<<__FUNCTION__<<"No se puede añadir el enlace"<<std::endl;
			}

        }


		for (auto id1:previous_softblockinglist)
		{
			try
			{
				newModel->removeEdgeByIdentifiers(id1, robotID, "softBlock");
				qDebug ()<<"Se elimina el enlace softblock a la persona  " << id1;
			}

			catch(...)
			{
				std::cout<<__FUNCTION__<<"No existe el enlace"<<std::endl;

			}
		}

		for (auto id1:pId_softblocking)
		{

			try
			{
				newModel->addEdgeByIdentifiers(id1, robotID,  "softBlock");
				qDebug ()<<"Se añade el enlace softblock a la persona  " << id1;
			}

			catch(...)
			{
				std::cout<<__FUNCTION__<<"No se puede añadir el enlace"<<std::endl;
			}

		}

        for (auto id1:previous_affordanceslist)
        {
            try
            {
                newModel->removeEdgeByIdentifiers(id1, robotID, "affordanceBlock");
                qDebug ()<<"Se elimina el enlace affordanceBlock a la persona  " << id1;
            }

            catch(...)
            {
                std::cout<<__FUNCTION__<<"No existe el enlace"<<std::endl;

            }
        }

        for (auto id1:pId_affblocking)
        {

            try
            {
                newModel->addEdgeByIdentifiers(id1, robotID,  "affordanceBlock");
                qDebug ()<<"Se añade el enlace affordanceblock a la persona  " << id1;
            }

            catch(...)
            {
                std::cout<<__FUNCTION__<<"No se puede añadir el enlace"<<std::endl;
            }

        }

        try
        {
            sendModificationProposal(worldModel,newModel, "block");
        }
        catch(...)
        {
            std::cout<<"No se puede actualizar worldModel"<<std::endl;
        }
    }

    previous_blockinglist = pId_blocking;
	previous_softblockinglist = pId_softblocking;
	previous_affordanceslist = pId_affblocking;
}

float SpecificWorker::TrajectoryRobot2D_go(const TargetPose &target)
{
	if (target.doRotation)
	{
		pathfinder.go_rot(target.x, target.z, target.ry);
	}
	else
	{
		pathfinder.go(target.x, target.z);
	}
	return 0.0;
};




// *****************************************************************************************
// AGENT RELATED
// *****************************************************************************************

bool SpecificWorker::AGMCommonBehavior_activateAgent(const ParameterMap& prs)
{
	bool activated = false;
	printf("<<activateAgent\n");
	if (setParametersAndPossibleActivation(prs, activated))
	{
		if (not activated)
		{
			printf("activateAgent 0 >>\n");
			return activate(p);
		}
	}
	else
	{
		printf("activateAgent 1 >>\n");
		return false;
	}
	printf("activateAgent 2 >>\n");
	return true;
}

bool SpecificWorker::AGMCommonBehavior_deactivateAgent()
{
	return deactivate();
}

StateStruct SpecificWorker::AGMCommonBehavior_getAgentState()
{
	StateStruct s;
	if (isActive())
	{
		s.state = Running;
	}
	else
	{
		s.state = Stopped;
	}
	s.info = p.action.name;
	return s;
}

ParameterMap SpecificWorker::AGMCommonBehavior_getAgentParameters()
{
	return params;
}

bool SpecificWorker::AGMCommonBehavior_setAgentParameters(const ParameterMap& prs)
{
	bool activated = false;
	return setParametersAndPossibleActivation(prs, activated);
}

void SpecificWorker::AGMCommonBehavior_killAgent()
{
}

Ice::Int SpecificWorker::AGMCommonBehavior_uptimeAgent()
{
	return 0;
}

bool SpecificWorker::AGMCommonBehavior_reloadConfigAgent()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpecificWorker::AGMExecutiveTopic_structuralChange(const RoboCompAGMWorldModel::World& modification)
{
	qDebug()<<"StructuralChange";
	QMutexLocker l(mutex);
	static bool first = true;
	
	AGMModelConverter::fromIceToInternal(modification, worldModel);
	innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel));


	if (!first)
	{
		socialrules.update(worldModel);
		innerModel->save("/home/robocomp/robocomp/components/robocomp-viriato/etcSim/innermodel.xml");
	}
	else
		first = false;
	
	
	viewer->reloadInnerModel(innerModel);
	
	printf("FIN structuralChange>>\n");
}


void SpecificWorker::AGMExecutiveTopic_symbolUpdated(const RoboCompAGMWorldModel::Node& modification)
{
	qDebug()<<"symbolUpdated";
	QMutexLocker l(mutex);
	AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
}

void SpecificWorker::AGMExecutiveTopic_symbolsUpdated(const RoboCompAGMWorldModel::NodeSequence &modifications)
{
	//qDebug()<<"symbolsUpdated";
	QMutexLocker l(mutex);

	for (auto modification : modifications)
		AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
}


void SpecificWorker::AGMExecutiveTopic_edgesUpdated(const RoboCompAGMWorldModel::EdgeSequence &modifications)
{
 	qDebug()<<"edgesUpdated";
	worldModelChanged=true;

	QMutexLocker lockIM(mutex);
	for (auto modification : modifications)
	{
		AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
		AGMModelEdge edge;
		AGMModelConverter::fromIceToInternal(modification, edge);
//TODO Guardar si el cambio se refiere a una persona?¿
		//Update InnerModel values
		if (edge->getLabel()=="RT" )
		{
			try{
				std::string songName= (worldModel->getSymbol( edge->getSymbolPair().second) )->getAttribute("imName");
				
				QVec vec = QVec::vec6();
				vec[0] = str2float(edge->getAttribute("tx"));
				vec[1] = str2float(edge->getAttribute("ty"));
				vec[2] = str2float(edge->getAttribute("tz"));
				vec[3] = str2float(edge->getAttribute("rx"));
				vec[4] = str2float(edge->getAttribute("ry"));
				vec[5] = str2float(edge->getAttribute("rz"));
				innerModel->updateTransformValues(QString::fromStdString(songName), vec);
                viewer->ts_updateTransformValues(QString::fromStdString(songName), vec);
            }
			catch (...)
			{
				qDebug()<<"EXCEPTION,RT label connect to a symbol without imName\n";
				std::cout<<(worldModel->getSymbol( edge->getSymbolPair().second))->toString(true);
			}
		}
	}
}

/**
 * \brief ACTUALIZACION DEL ENLACE EN INNERMODEL
 */ 


void SpecificWorker::AGMExecutiveTopic_selfEdgeAdded(const int nodeid, const string &edgeType, const RoboCompAGMWorldModel::StringDictionary &attributes)
{
//subscribesToCODE
	QMutexLocker lockIM(mutex);
 	try { worldModel->addEdgeByIdentifiers(nodeid, nodeid, edgeType, attributes); } catch(...){ printf("Couldn't add an edge. Duplicate?\n"); }
 
	try { innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel)); } catch(...) { printf("Can't extract an InnerModel from the current model.\n"); }
}

void SpecificWorker::AGMExecutiveTopic_selfEdgeDeleted(const int nodeid, const string &edgeType)
{
//subscribesToCODE
	QMutexLocker lockIM(mutex);
 	try { worldModel->removeEdgeByIdentifiers(nodeid, nodeid, edgeType); } catch(...) { printf("Couldn't remove an edge\n"); }
 
	try { innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel)); } catch(...) { printf("Can't extract an InnerModel from the current model.\n"); }
}


void SpecificWorker::AGMExecutiveTopic_edgeUpdated(const RoboCompAGMWorldModel::Edge& modification)
{
 	qDebug() << "edgeUpdated";
	worldModelChanged = true;
	QMutexLocker lockIM(mutex);
	AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
	AGMModelEdge edge;
	AGMModelConverter::fromIceToInternal(modification, edge);
//TODO Guardar si el cambio se refiere a una persona?¿
	//Update InnerModel values
	if (edge->getLabel()=="RT" )
	{
		try{
			std::string songName= (worldModel->getSymbol( edge->getSymbolPair().second) )->getAttribute("imName");
			
			QVec vec = QVec::vec6();
			vec[0] = str2float(edge->getAttribute("tx"));
			vec[1] = str2float(edge->getAttribute("ty"));
			vec[2] = str2float(edge->getAttribute("tz"));
			vec[3] = str2float(edge->getAttribute("rx"));
			vec[4] = str2float(edge->getAttribute("ry"));
			vec[5] = str2float(edge->getAttribute("rz"));
			innerModel->updateTransformValues(QString::fromStdString(songName), vec);
			viewer->ts_updateTransformValues(QString::fromStdString(songName), vec);
		}
		catch (...)
		{
			qDebug()<<"EXCEPTION,RT label connect to a symbol without imName\n";
			std::cout<<(worldModel->getSymbol( edge->getSymbolPair().second))->toString(true);
		}
	}
}

bool SpecificWorker::setParametersAndPossibleActivation(const ParameterMap &prs, bool &reactivated)
{
	printf("<<< setParametersAndPossibleActivation\n");
	// We didn't reactivate the component
	reactivated = false;

	// Update parameters
	params.clear();
	for (ParameterMap::const_iterator it=prs.begin(); it!=prs.end(); it++)
	{
		printf("param:%s   value:%s\n", it->first.c_str(), it->second.value.c_str());
		params[it->first] = it->second;
	}
	printf("----\n");

	try
	{
		action = params["action"].value;
		std::transform(action.begin(), action.end(), action.begin(), ::tolower);

		if (action == "graspobject")
		{
			active = true;
		}
		else
		{
			active = true;
		}
	}
	catch (...)
	{
		printf("exception in setParametersAndPossibleActivation %d\n", __LINE__);
		return false;
	}

	// Check if we should reactivate the component
	if (active)
	{
		active = true;
		reactivated = true;
	}

	printf("setParametersAndPossibleActivation >>>\n");

	return true;
}


void SpecificWorker::sendModificationProposal(AGMModel::SPtr &worldModel, AGMModel::SPtr &newModel,std::string m)
{
	QMutexLocker locker(mutex);
	
	try
	{
		AGMMisc::publishModification(newModel, agmexecutive_proxy, std::string( "SocialnavigationAgent")+m);
	}
	catch(const RoboCompAGMExecutive::Locked &e)
	{
	}
	catch(const RoboCompAGMExecutive::OldModel &e)
	{
		printf("modelo viejo\n");
	}
	catch(const RoboCompAGMExecutive::InvalidChange &e)
	{
		printf("modelo invalido\n");
	}
	catch(const Ice::Exception& e)
	{
		exit(1);
	}
}







