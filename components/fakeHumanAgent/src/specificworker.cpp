
/*
 *    Copyright (C) 2016 by YOUR NAME HERE
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
//#include <qt4/QtGui/qdial.h>

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx, bool startup_check) : GenericWorker(mprx)
{
	active = false;
	worldModel = AGMModel::SPtr(new AGMModel());
	worldModel->name = "worldModel";
	innerModel = std::make_shared<InnerModel>();

	
	setWindowTitle("FakeHumanAgent");
	initializeUI();
//	lastJoystickEvent = QTime::currentTime();
	coordInItem.x = 0;
	coordInItem.y = 0;
	coordInItem.z = 0;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}

void SpecificWorker::initialize(int period)
{
    std::cout << "Initialize worker" << std::endl;

    try
    {
        RoboCompAGMWorldModel::World w = agmexecutive_proxy->getModel();
        AGMExecutiveTopic_structuralChange(w);
    }
    catch(...)
    {
        printf("The executive is probably not running, waiting for first AGM model publication...");
    }

    readPersonsFromAGM();

    this->Period = period;
    if(this->startup_check_flag)
    {
        this->startup_check();
    }
    else
    {
        timer.start(Period);
    }


}


int SpecificWorker::startup_check()
{
    std::cout << "Startup check" << std::endl;
    QTimer::singleShot(200, qApp, SLOT(quit()));
    return 0;
}

void SpecificWorker::readPersonsFromAGM()
{
	auto vectorPersons = worldModel->getSymbolsByType("person");

	for (auto p: vectorPersons)
	{
		TPerson person;

		auto id =  p->identifier;
		AGMModelSymbol::SPtr personParent = worldModel->getParentByLink(id, "RT");
		AGMModelEdge &edgeRT = worldModel->getEdgeByIdentifiers(personParent->identifier, id, "RT");

		person.personSymbolId = id;
		person.autoMovement = false;
		person.currentPoint = 0;
		person.speed = 0;
		person.name = p->attributes["imName"];
        qDebug()<<"PERSONA "<< QString::fromStdString (person.name);
        RoboCompInnerModelManager::Pose3D pose_person;
		pose_person.x = str2float(edgeRT.attributes["tx"]);
		pose_person.y = 0.f;
		pose_person.z = str2float(edgeRT.attributes["tz"]);
		pose_person.rx = 0.f;
		pose_person.ry = str2float(edgeRT.attributes["ry"]);
		pose_person.rz = 0.f;

		person.pose = pose_person;
        personMap[id] = person;

        int1_cb->addItem(QString::number(id));
        int2_cb->addItem(QString::number(id));
        person_cb->addItem(QString::number(id));
        int index = person_cb->findText(QString::number(id));
        person_cb->setCurrentIndex(index);
        updatePersonInterfaz(true);

	}


}


bool SpecificWorker::includeInRCIS(int id, const RoboCompInnerModelManager::Pose3D &pose, std::string meshName)
{
	printf("includeInRCIS begins\n");
	std::string name = "person" + std::to_string(id);
		
	RoboCompInnerModelManager::meshType mesh;
	mesh.pose.x  = 0;
	mesh.pose.y  = QString::fromStdString(translationy).toFloat();
	mesh.pose.z  = 0;
	mesh.pose.rx = 1.57079632679;
	mesh.pose.ry = 0;
	mesh.pose.rz = QString::fromStdString(rotationz).toFloat();
	
	mesh.scaleX = mesh.scaleY = mesh.scaleZ = QString::fromStdString(scale).toFloat();
	mesh.render = 0;
	mesh.meshPath = "/home/robocomp/robocomp/components/robocomp-viriato/files/osgModels/" + meshName;

    try
    {
        innermodelmanager_proxy->addTransform(name, "static", "root", pose);
    }
    catch (RoboCompInnerModelManager::InnerModelManagerError e)
    {
        if (e.err != RoboCompInnerModelManager::ErrorType::NodeAlreadyExists)
        {
            printf("Can't create fake peson\n");
            return false;
        }
    }
    try
    {
        innermodelmanager_proxy->addMesh(name+"_mesh", name, mesh);
    }
    catch (RoboCompInnerModelManager::InnerModelManagerError e)
    {
        if (e.err != RoboCompInnerModelManager::ErrorType::NodeAlreadyExists) {
            printf("Can't create fake peson\n");
            return false;
        }
    }
	printf("includeInRCIS ends\n");
	return true;
}

bool SpecificWorker::removeFromRCIS(int id)
{
	printf("removeFromRCIS begins\n");
	try
	{
		std::string name = personMap[id].name;
		std::cout<<"name "<<name<<std::endl;
		innermodelmanager_proxy->removeNode(name);
	}
	catch (std::exception& e)
	{
		std::cout<<"Can't delete fake person "<<e.what()<<std::endl;
		return false;
	}
	printf("removeFromRCIS ends\n");
	return true;
}

int SpecificWorker::includeInAGM(int id,const RoboCompInnerModelManager::Pose3D &pose, std::string mesh)
{
	printf("includeInAGM begins\n");

	std::string name = "person" ;
	std::string imName = "person" + std::to_string(id);
	int personSymbolId = -1;
	int idx=0;
	while ((personSymbolId = worldModel->getIdentifierByType(name, idx++)) != -1)
	{
		printf("%d %d\n", idx, personSymbolId);
		if (worldModel->getSymbolByIdentifier(personSymbolId)->getAttribute("imName") == imName)
		{
			printf("found %d!!\n", personSymbolId);
			break;
		}
	}
	if (personSymbolId != -1)
	{
		printf("Fake person already in the AGM model\n");
		return personSymbolId;
	}

	AGMModel::SPtr newModel(new AGMModel(worldModel));

	// Symbolic part
	AGMModelSymbol::SPtr person = newModel->newSymbol("person");
	personSymbolId = person->identifier;
	printf("Got personSymbolId: %d\n", personSymbolId);
	person->setAttribute("imName", imName);
	person->setAttribute("imType", "transform");
    person->setAttribute("polyline_intimate", "");
    person->setAttribute("polyline_personal", "");
    person->setAttribute("polyline_social", "");
    person->setAttribute("polyline_sharedWith","");

    newModel->addEdgeByIdentifiers(person->identifier, 3, "in");



	// Geometric part
	std::map<std::string, std::string> edgeRTAtrs;
	edgeRTAtrs["tx"] = std::to_string(pose.x);
	edgeRTAtrs["ty"] = "0";
	edgeRTAtrs["tz"] = std::to_string(pose.z);
	edgeRTAtrs["rx"] = "0";
	edgeRTAtrs["ry"] = std::to_string(pose.ry);
	edgeRTAtrs["rz"] = "0";
	newModel->addEdgeByIdentifiers(100, person->identifier, "RT", edgeRTAtrs);


	AGMModelSymbol::SPtr personMesh = newModel->newSymbol("mesh_"+name);
	printf("personMesh %d\n", personMesh->identifier);
	personMesh->setAttribute("collidable", "false");
	personMesh->setAttribute("imName", imName + "_Mesh");
	personMesh->setAttribute("imType", "mesh");
	std::string meshPath = "/home/robocomp/robocomp/components/robocomp-viriato/files/osgModels/" + mesh ;
	personMesh->setAttribute("path", meshPath);
	personMesh->setAttribute("render", "NormalRendering");
	personMesh->setAttribute("scalex", scale);
	personMesh->setAttribute("scaley", scale);
	personMesh->setAttribute("scalez", scale);

	edgeRTAtrs["tx"] = "0";
	edgeRTAtrs["ty"] = translationy;
	edgeRTAtrs["tz"] = "0";
	edgeRTAtrs["rx"] = "1.570796326794";
	edgeRTAtrs["ry"] = "0";
	edgeRTAtrs["rz"] = rotationz;
	newModel->addEdge(person, personMesh, "RT", edgeRTAtrs);

	while (true)
	{
		if(sendModificationProposal(worldModel, newModel))
		{
			break;
		}
		sleep(1);
	}
	printf("includeInAGM ends\n");
	return personSymbolId;
}

//Change 
void SpecificWorker::changePersonRoom(int personId, int roomId)
{
	std::cout<<"Change room: person "<<personId <<" new room: "<<roomId <<std::endl;
	// Get current roomId
	AGMModelSymbol::SPtr personAGM = worldModel->getSymbol(personId);
	int actualRoomId = -1;
	for (auto edge = personAGM->edgesBegin(worldModel); edge != personAGM->edgesEnd(worldModel); edge++)
	{
		const std::pair<int32_t, int32_t> symbolPair = edge->getSymbolPair();
		if (edge->getLabel() == "in")
		{
			const string secondType = worldModel->getSymbol(symbolPair.second)->symbolType;
			if (symbolPair.first == personId and secondType == "room")
			{
				actualRoomId = symbolPair.second;
				break;
			}
		}
	}
	// Modify IN edge
	try
	{
		AGMModel::SPtr newModel(new AGMModel(worldModel));
		if (actualRoomId != -1)
		{
			std::cout<<"remove edge "<<actualRoomId<<std::endl;			
			newModel->removeEdgeByIdentifiers(personId, actualRoomId, "in");
		}
		newModel->addEdgeByIdentifiers(personId, roomId, "in");
 		sendModificationProposal(worldModel, newModel);
	}
	catch (...)
	{
		printf("Can't update person in room... !!!\n");
	}
}

bool SpecificWorker::removeFromAGM(int id)
{
	printf("removeFromAGM begins\n");
	bool result = false;
	AGMModel::SPtr newModel(new AGMModel(worldModel));

	AGMModelSymbol::SPtr personAGM = worldModel->getSymbolByIdentifier(personMap[id].personSymbolId);

	newModel->removeEdgesRelatedToSymbol(personMap[id].personSymbolId);
	newModel->removeSymbol(personMap[id].personSymbolId);

	while (true)
	{
		try
		{
			sendModificationProposal(worldModel, newModel);
			result = true;
			break;
		}
		catch(const RoboCompAGMExecutive::Locked &e)
		{
			printf("agmexecutive locked...\n");
		}
		catch(const RoboCompAGMExecutive::OldModel &e)
		{
			printf("agmexecutive oldModel...\n");
		}
		catch(const RoboCompAGMExecutive::InvalidChange &e)
		{ 
			printf("agmexecutive InvalidChange...\n");
		}
		sleep(1);
	}


	printf("removeFromAGM ends\n");
	return result;
}


/*void SpecificWorker::receivedJoyStickEvent(int value, int type, int number)
{
	printf("*\n");
	fflush(stdout);
	if (type != 2)
		return;
	
	if (number == 0) // rot
	{
		humanRotVel = float(value)/32767.*1.;
		lastJoystickEvent = QTime::currentTime();
	}
	if (number == 1)
	{
		humanAdvVel = float(-value)/32767.*600.;
		lastJoystickEvent = QTime::currentTime();
	}
	printf("*");
	fflush(stdout);
}*/

void SpecificWorker::initializeUI(){
	//Teclado
	//UP
	connect(up,SIGNAL(pressed()),this,SLOT(upP()));
	connect(up,SIGNAL(released()),this,SLOT(upR()));
	//DOWN
	connect(down,SIGNAL(pressed()),this,SLOT(downP()));
	connect(down,SIGNAL(released()),this,SLOT(downR()));
	//RIGHT
	connect (right,SIGNAL(pressed()),this,SLOT(rightP()));
	connect (right,SIGNAL(released()),this,SLOT(rightR()));
	//LEFT
	connect(left,SIGNAL(pressed()),this,SLOT(leftP()));
	connect(left,SIGNAL(released()),this,SLOT(leftR()));
	//GIRO
	connect (giro,SIGNAL(valueChanged(int)),this,SLOT(rotar(int)));
	connect (giro,SIGNAL(sliderPressed()),this,SLOT(giroP()));
	connect (giro,SIGNAL(sliderReleased()),this,SLOT(giroR()));
	//giro->setNotchesVisible(true);
	giro->QAbstractSlider::setMinimum (0);
	giro->QAbstractSlider::setMaximum (360);
	giro->QAbstractSlider::setSliderPosition(0);

	connect(add_pb, SIGNAL(clicked()), this, SLOT(addPerson()));
	connect(del_pb, SIGNAL(clicked()), this, SLOT(delPerson()));
	connect(setPose_pb, SIGNAL(clicked()), this, SLOT(setPose()));
	
	connect(save_pb, SIGNAL(clicked()), this, SLOT(savePoints()));
	connect(load_pb, SIGNAL(clicked()), this, SLOT(loadPoints()));

	connect(person_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(cbIndexChanged(int)));

	connect(interaction_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(interactionChanged(int)));
	connect(autoM_cb, SIGNAL(clicked()),this, SLOT(autoMovement()));
	connect(rinteraction_pb, SIGNAL(clicked()),this, SLOT(removeEdgeAGM()));
	connect(ainteraction_pb, SIGNAL(clicked()),this, SLOT(addInteraction()));
//	connect(point_te, SIGNAL(textChanged()), this, SLOT(pointsChanged()));
    connect(random, SIGNAL(clicked()),this, SLOT(moverandom()));
	//disable interface elements 
	interactionChanged(0);
	updatePersonInterfaz(false);
	rinteraction_pb->setEnabled(interaction_lw->count() > 0);
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{


    outerRegion.setLeft(std::stoi(params["OuterRegionLeft"].value));
    outerRegion.setRight(std::stoi(params["OuterRegionRight"].value));
    outerRegion.setBottom(std::stoi(params["OuterRegionBottom"].value));
    outerRegion.setTop(std::stoi(params["OuterRegionTop"].value));

    hmin = std::min(outerRegion.left(), outerRegion.right()) ;
    hmax = std::max(outerRegion.left(), outerRegion.right()) ;
    vmin = std::min(outerRegion.top(), outerRegion.bottom()) ;
    vmax = std::max(outerRegion.top(), outerRegion.bottom()) -1000; //para que en las pruebas no se ponga en la pos destino

	// Joystick
	/*printf("Creating joystick...\n");
	joystick = new QJoyStick("/dev/input/js0");
	if (!joystick->openQJoy())
	{
		cout << "[" << PROGRAM_NAME << "]: Unable to open device: " << joystick->getDeviceName() << endl;
		return EXIT_FAILURE;
	}
	joystick->start();
	printf("Connecting joystick...\n");
	connect(joystick, SIGNAL(inputEvent(int, int, int)), this, SLOT(receivedJoyStickEvent(int, int, int)));
	
*/



	return true;
}

//UP
void SpecificWorker::upP(){
	coordInItem.x = 0;
	coordInItem.z = HUMANADVVEL;
    moveFlag = true;
}
void SpecificWorker::upR(){
	moveFlag = false;
}
//DOWN
void SpecificWorker::downP(){
	coordInItem.x = 0;
	coordInItem.z = -HUMANADVVEL;
	moveFlag = true;
}
void SpecificWorker::downR(){
	moveFlag = false;
}
//RIGHT
void SpecificWorker::rightP(){
  	coordInItem.x = HUMANADVVEL;
	coordInItem.z = 0;

    moveFlag = true;
}
void SpecificWorker::rightR(){
	moveFlag = false;
}
//LEFT
void SpecificWorker::leftP(){
  	coordInItem.x = -HUMANADVVEL;
	coordInItem.z = 0;

	moveFlag = true;
}
void SpecificWorker::leftR(){
	moveFlag = false;
}
//ROT
void SpecificWorker::rotar(int value){
	coordInItem.x = 0;
	coordInItem.z = 0;
	valorgiro = value * 0.0175;
//	valorgiro = value/100;
}
void SpecificWorker::giroP(){
	moveFlag = true;
}
void SpecificWorker::giroR(){ 
	moveFlag = false;
}

void SpecificWorker::cbIndexChanged(int index)
{
	qDebug()<<"cbIndexChanged";

	if (person_cb->currentText() != "")
	{
		TPerson *person = &personMap[person_cb->currentText().toInt()];
		giro->QAbstractSlider::setSliderPosition(person->pose.ry);

		qDebug() << person->pose.x << person->pose.z << person->pose.ry;

        x_sb->setValue(person->pose.x);
        z_sb->setValue(person->pose.z);
        rot_sb->setValue(person->pose.ry);

        x_lb->setText(  QString::number(person->pose.x) );
        z_lb->setText(  QString::number(person->pose.z));
        rot_lb->setText(  QString::number(person->pose.ry));
	}
}


void SpecificWorker::addPerson()
{

    qDebug()<<"add clicked";
	RoboCompInnerModelManager::Pose3D pose;
	pose.x = x_sb->value();
	pose.y = 0;
	pose.z = z_sb->value();
	pose.rx = 0.f;
	pose.ry = rot_sb->value();
	pose.rz = 0.f;
	int mesh = mesh_cb->currentText().toInt();

	translationy = "0";

	switch(mesh)
	{
		case 1:
            meshname = "nurse.ive";
            scale ="10";
            rotationz = "3.1415926535";
            break;
		case 2:
			meshname = "abuelita02.ive";
			scale = "1000";
			rotationz= "0";
			break;
		case 3:
            meshname = "human08.3DS";
            scale = "950";
            rotationz= "0";
            break;
        case 4:
			meshname = "human04.3ds";
			scale = "900";
			rotationz= "0";
			break;
		case 5:meshname = "human05.3ds";
			scale = "800";
			rotationz= "0";
			break;
		case 6:
			meshname = "human06.3ds";
			scale = "23";
			rotationz= "3.1415926535";
			break;
		case 7:
            meshname = "human01.3ds";
            scale = "12";
            rotationz= "3.1415926535";
            break;
		case 8:
            meshname = "human03.3ds";
            scale = "8";
            rotationz= "1.57079632679";
            break;

		default:
			qDebug()<< "Mesh error";
			return;
	}


	int id = personMap.size() + 1;
	// avoid inserting same element twice
	while (personMap.find(id) != personMap.end())
		id++;


	// Include person in RCIS
	if (includeInRCIS(id, pose, meshname))
	{
		// Include person in AGM
		int personSymbolId = includeInAGM(id, pose, meshname);
		if (personSymbolId != -1)
		{
			TPerson person;
			person.autoMovement = false;
			person.pose = pose;
			person.personSymbolId = personSymbolId;
			person.name = "person" + std::to_string(id);
			personMap.insert(std::pair<int,TPerson>(personSymbolId,person));
			//include in comboBox
			person_cb->addItem(QString::number(personSymbolId));
            int index = person_cb->findText(QString::number(personSymbolId));
            person_cb->setCurrentIndex(index);

			int1_cb->addItem(QString::number(personSymbolId));
			int2_cb->addItem(QString::number(personSymbolId));
			updatePersonInterfaz(true);

		}
	}
}
void SpecificWorker::delPerson()
{
	qDebug()<<"del clicked";
	if (person_cb->currentText() == ""){
		QMessageBox::information(this, "No person selected", "You have to select any person to delete");
	}
	else{
		qDebug()<<"Person selected" << person_cb->currentText();
		int personId = person_cb->currentText().toInt();
		removeFromRCIS(personId);
		removeFromAGM(personId);
		person_cb->removeItem(person_cb->currentIndex());
		int1_cb->removeItem(int1_cb->findText(QString::number(personId)));
		int2_cb->removeItem(int2_cb->findText(QString::number(personId)));
		cleanListWidget(personId);
		personMap.erase(personId);
		updatePersonInterfaz(personMap.size() > 0);
		rinteraction_pb->setEnabled(interaction_lw->count() > 0);
	}
}

//MOVE
void SpecificWorker::move()
{
	if (person_cb->currentText() == ""){
		std::cout<<"No selected person to move"<<std::endl;
	}
	else{



		TPerson *person = &personMap[person_cb->currentText().toInt()];

		if (prevIdSelected != person_cb->currentText()) { valorgiro = person->pose.ry ;}

		movePerson(person, coordInItem, setPoseFlag);

        x_sb->setValue(person->pose.x);
        z_sb->setValue(person->pose.z);
        rot_sb->setValue(person->pose.ry);

		x_lb->setText(  QString::number(person->pose.x) );
        z_lb->setText(  QString::number(person->pose.z));
        rot_lb->setText(  QString::number(person->pose.ry));

		prevIdSelected = person_cb->currentText();

    }
}

void SpecificWorker::movePerson(TPerson *person, RoboCompInnerModelManager::coord3D coordInItem ,bool global)
{
	bool changeRoom = false;
	RoboCompInnerModelManager::coord3D coordInWorld;
	try{
		if (global)
			innermodelmanager_proxy->transform("root", "world", coordInItem, coordInWorld);
		else
        {
            qDebug()<<"PERSON NAME IN MOVE" << QString::fromStdString(person->name);

            innermodelmanager_proxy->transform("root", person->name, coordInItem, coordInWorld);
        }
	}
	catch (std::exception& e)
	{
		std::cout<<"Exception retrieving transform from RCIS: "<<e.what()<<std::endl;
		return;
	}
	//check room change
//	if ((coordInWorld.z < 0 and person->pose.z >= 0) or (coordInWorld.z >= 0 and person->pose.z < 0))
//	{
//		changeRoom = true;
//	}

	RoboCompInnerModelManager::Pose3D pose;
	pose.x = coordInWorld.x;
	pose.y = coordInWorld.y;
	pose.z = coordInWorld.z;
	pose.rx = 0;
	pose.ry = valorgiro;
	pose.rz = 0;
	//store new position
	person->pose = pose;
	//move in RCIS


	try{
		innermodelmanager_proxy->setPoseFromParent(person->name, pose);
	}
	catch (std::exception& e)
	{
		std::cout<<"Exception moving person in RCIS: "<<e.what()<<std::endl;
		return;
	}
	//move in AGM
	AGMModelSymbol::SPtr personParent = worldModel->getParentByLink(person->personSymbolId, "RT");
	AGMModelEdge &edgeRT  = worldModel->getEdgeByIdentifiers(personParent->identifier, person->personSymbolId, "RT");
	edgeRT.attributes["tx"] = float2str(coordInWorld.x);
	edgeRT.attributes["ty"] = float2str(coordInWorld.y);
	edgeRT.attributes["tz"] = float2str(coordInWorld.z);
	edgeRT.attributes["rx"] = "0";
	edgeRT.attributes["ry"] = float2str(valorgiro);
	edgeRT.attributes["rz"] = "0";
	try
	{
		AGMMisc::publishEdgeUpdate(edgeRT, agmexecutive_proxy);
	}
	catch(std::exception& e)
	{
		std::cout<<"Exception moving in AGM: "<<e.what()<<std::endl;
	}
	
	//change room when needed
//	if(changeRoom)
//	{
//		int newRoom = 3;
//		//TODO ==> reevaluate to make it valid for any world
//		if (person->pose.z < 0 ){
//			newRoom = 5;
//		}
//		changePersonRoom(person->personSymbolId, newRoom);
//	}
}

void SpecificWorker::compute()
{
	QMutexLocker locker(mutex);
	static int firstTime=true;
	if (firstTime) //retrieve model after initialization
	{
		RoboCompAGMWorldModel::World w = agmexecutive_proxy->getModel();
		AGMExecutiveTopic_structuralChange(w);
		firstTime = false;
		//add robot ID to interaction comboBox
		try{
			robotID = worldModel->getIdentifierByType("robot");
			int2_cb->addItem(QString::number(robotID));
		}catch(...){
			std::cout<<"No robot found in AGM model"<<std::endl;
		}
		
	}
	//static QTime lastCompute = QTime::currentTime();
	
    try
	{
		if (moveFlag or setPoseFlag)
		{
			move();
			setPoseFlag = false;
		}

    }
    catch(...){}

	
	//move person
	for (auto iterator: personMap)
	{
		if (iterator.second.autoMovement)
		{
			RoboCompInnerModelManager::coord3D coordInItem = autoMovePerson(iterator.second);
			movePerson(&iterator.second, coordInItem);
		}
	}
	
	
	

	/*if (lastJoystickEvent.elapsed()  < 3000)
	{
		printf("vel: %f %f\n", humanAdvVel, humanRotVel);
		RoboCompInnerModelManager::coord3D coordInItem;
		coordInItem.x = 0;
		coordInItem.y = 0;
		coordInItem.z = humanAdvVel*0.001*lastCompute.elapsed();
		RoboCompInnerModelManager::coord3D coordInBase;
		printf("transform (%f, %f, %f) to fake person (%f, %f, %f)\n", coordInItem.x, coordInItem.y, coordInItem.z, coordInBase.x, coordInBase.y, coordInBase.z);
		innermodelmanager_proxy->transform("root", "fakeperson",  coordInItem, coordInBase);

		RoboCompInnerModelManager::Pose3D pose;
		humanRot += humanRotVel*0.001*lastCompute.elapsed(); 
		pose.x = coordInBase.x;
		pose.y = coordInBase.y;
		pose.z = coordInBase.z;
		pose.rx = 0;
		pose.ry = humanRot;
		pose.rz = 0;
		innermodelmanager_proxy->setPoseFromParent("fakeperson", pose);

		AGMModelSymbol::SPtr personParent = worldModel->getParentByLink(personSymbolId, "RT");
		AGMModelEdge &edgeRT  = worldModel->getEdgeByIdentifiers(personParent->identifier, personSymbolId, "RT");
		edgeRT.attributes["tx"] = float2str(coordInBase.x);
		edgeRT.attributes["ty"] = float2str(coordInBase.y);
		edgeRT.attributes["tz"] = float2str(coordInBase.z);
		edgeRT.attributes["rx"] = "0";
		edgeRT.attributes["ry"] = float2str(humanRot);
		edgeRT.attributes["rz"] = "0";
		printf("%d----[%f]--->%d\n", personParent->identifier, coordInBase.z, personSymbolId);
		AGMMisc::publishEdgeUpdate(edgeRT, agmexecutive_proxy);
	}
	else
	{
		printf(".");
		fflush(stdout);
	}



	lastCompute = QTime::currentTime();

*/

//#ifdef USE_QTGUI
//	if (innerModelViewer) innerModelViewer->update();
////	osgView->frame();
//#endif

}

void SpecificWorker::setPose()
{
	if (person_cb->currentText() == ""){
		QMessageBox::information(this, "No person selected", "You have to select any person to change its pose");
	}
	else{
		coordInItem.x = x_sb->value();
		coordInItem.z = z_sb->value();
		valorgiro = rot_sb->value();
		setPoseFlag = true;
	}
}

//***************************
//		AGENT RELATED
//***************************

bool SpecificWorker::AGMCommonBehavior_reloadConfigAgent()
{
	return true;
}

bool SpecificWorker::AGMCommonBehavior_activateAgent(const RoboCompAGMCommonBehavior::ParameterMap &prs)
{
	bool activated = false;
	if (setParametersAndPossibleActivation(prs, activated))
	{
		if (not activated)
		{
			return activate(p);
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool SpecificWorker::AGMCommonBehavior_setAgentParameters(const RoboCompAGMCommonBehavior::ParameterMap &prs)
{
	bool activated = false;
	return setParametersAndPossibleActivation(prs, activated);
}

RoboCompAGMCommonBehavior::ParameterMap SpecificWorker::AGMCommonBehavior_getAgentParameters()
{
	return params;
}

void SpecificWorker::AGMCommonBehavior_killAgent()
{

}

int SpecificWorker::AGMCommonBehavior_uptimeAgent()
{
	return 0;
}

bool SpecificWorker::AGMCommonBehavior_deactivateAgent()
{
	return deactivate();
}

RoboCompAGMCommonBehavior::StateStruct SpecificWorker::AGMCommonBehavior_getAgentState()
{
    RoboCompAGMCommonBehavior::StateStruct s;
	if (isActive())
	{
		s.state = RoboCompAGMCommonBehavior::Running;
	}
	else
	{
		s.state = RoboCompAGMCommonBehavior::Stopped;
	}
	s.info = p.action.name;
	return s;
}

void SpecificWorker::AGMExecutiveTopic_structuralChange(const RoboCompAGMWorldModel::World &w)
{
	mutex->lock();
	QMutexLocker lockIM(mutex);
 	AGMModelConverter::fromIceToInternal(w, worldModel);
 
	innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel));
//	regenerateInnerModelViewer();
	mutex->unlock();
}

//SUBSCRIPTION to selfEdgeAdded method from AGMExecutiveTopic interface
void SpecificWorker::AGMExecutiveTopic_selfEdgeAdded(const int nodeid, const string &edgeType, const RoboCompAGMWorldModel::StringDictionary &attributes)
{
//subscribesToCODE
	QMutexLocker lockIM(mutex);
 	try { worldModel->addEdgeByIdentifiers(nodeid, nodeid, edgeType, attributes); } catch(...){ printf("Couldn't add an edge. Duplicate?\n"); }
 
	try { innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel)); } catch(...) { printf("Can't extract an InnerModel from the current model.\n"); }
}

//SUBSCRIPTION to selfEdgeDeleted method from AGMExecutiveTopic interface
void SpecificWorker::AGMExecutiveTopic_selfEdgeDeleted(const int nodeid, const string &edgeType)
{
//subscribesToCODE
	QMutexLocker lockIM(mutex);
 	try { worldModel->removeEdgeByIdentifiers(nodeid, nodeid, edgeType); } catch(...) { printf("Couldn't remove an edge\n"); }
 
	try { innerModel = std::make_shared<InnerModel>(AGMInner::extractInnerModel(worldModel)); } catch(...) { printf("Can't extract an InnerModel from the current model.\n"); }
}
void SpecificWorker::AGMExecutiveTopic_edgesUpdated(const RoboCompAGMWorldModel::EdgeSequence &modifications)
{
	QMutexLocker locker(mutex);
	for (auto modification : modifications)
	{
		AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
		AGMInner::updateImNodeFromEdge(worldModel, modification, innerModel.get());
	}
}

void SpecificWorker::AGMExecutiveTopic_edgeUpdated(const RoboCompAGMWorldModel::Edge &modification)
{
	QMutexLocker locker(mutex);
	AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
	AGMInner::updateImNodeFromEdge(worldModel, modification, innerModel.get());
}

void SpecificWorker::AGMExecutiveTopic_symbolUpdated(const RoboCompAGMWorldModel::Node &modification)
{
	QMutexLocker locker(mutex);
	AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
}

void SpecificWorker::AGMExecutiveTopic_symbolsUpdated(const RoboCompAGMWorldModel::NodeSequence &modifications)
{
	QMutexLocker l(mutex);
	for (auto modification : modifications)
		AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
}

//void SpecificWorker::regenerateInnerModelViewer()
//{
////    if (innerModelViewer)
////    {
////        osgView->getRootGroup()->removeChild(innerModelViewer);
////    }
////
////    innerModelViewer = new InnerModelViewer(innerModel, "root", osgView->getRootGroup(), true);
//}


bool SpecificWorker::setParametersAndPossibleActivation(const RoboCompAGMCommonBehavior::ParameterMap &prs, bool &reactivated)
{
	QMutexLocker l(mutex);
	printf("<<< setParametersAndPossibleActivation\n");
	// We didn't reactivate the component
	reactivated = false;

	// Update parameters
	params.clear();
	params.clear();
	for (RoboCompAGMCommonBehavior::ParameterMap::const_iterator it=prs.begin(); it!=prs.end(); it++)
	{
		params[it->first] = it->second;
	}

	try
	{
		action = params["action"].value;
		std::transform(action.begin(), action.end(), action.begin(), ::tolower);
		//TYPE YOUR ACTION NAME
		if (action == "actionname")
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
		printf("exception in setParametersAndPossibleActivation\n");
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

bool SpecificWorker::sendModificationProposal(AGMModel::SPtr &worldModel, AGMModel::SPtr &newModel)
{
	bool result = false;

	try
	{	qDebug()<<"Intentando sendModificationProposal";
		AGMMisc::publishModification(newModel, agmexecutive_proxy, "fakeHumanAgentAgent");
		qDebug()<<"sendModificationProposal done";
		result = true;
	}
	catch(const RoboCompAGMExecutive::Locked &e)
	{
		printf("agmexecutive locked...\n");
	}
	catch(const RoboCompAGMExecutive::OldModel &e)
	{
		printf("agmexecutive oldModel...\n");
	}
	catch(const RoboCompAGMExecutive::InvalidChange &e)
	{ 
		printf("agmexecutive InvalidChange...\n");
	}
	catch(const Ice::Exception& e)
	{
	    printf("ICE Exception \n");
		exit(1);
	}

	return result;
}

// save points into file
void SpecificWorker::savePoints()
{
	QString nomFileH = QFileDialog::getSaveFileName(this, tr("File Name"), "",tr("Text file (*.txt)"));
	if (nomFileH != "") {
		QFile fileH(nomFileH);
		if (fileH.open(QFile::WriteOnly)) {
			QTextStream out(&fileH);
			out << point_te->toPlainText();
		}
	}
}
// load points from file
void SpecificWorker::loadPoints()
{
	if (person_cb->currentText() == ""){
		QMessageBox::information(this, "No person selected", "You have to select any person to load points");
	}
	else{
		QString nomFileH = QFileDialog::getOpenFileName(this, tr("File Name"), "",tr("Text file (*.txt)"));
		if (nomFileH != "") {
			QFile fileH(nomFileH);
			if (fileH.open(QFile::ReadOnly)) {
				QTextStream in(&fileH);
				QString line = in.readAll();
				point_te->clear();
				point_te->append(line);
			}
		}
	}
}

// Reload person information
void SpecificWorker::personChanged(int index)
{
	if (person_cb->currentText() == ""){
		QMessageBox::information(this, "No person selected", "You have to select any person to delete");
	}
	else{
		TPerson person = personMap[person_cb->currentText().toInt()];
		point_te->clear();
		for(const auto& s: person.points)
		{
			std::cout << s.x << std::endl;
			//point_te->append(line);
		}
	}
}
//enable/disable person automovement
void SpecificWorker::autoMovement()
{
	TPerson person = personMap[person_cb->currentText().toInt()];
	person.autoMovement = autoM_cb->isChecked();
	
	pointsChanged();
}


void SpecificWorker::pointsChanged()
{
	if (person_cb->currentText() == ""){
		std::cout<<"No selected person to move"<<std::endl;
	}
	else{
		autoM_cb->setChecked(false);
		TPerson person = personMap[person_cb->currentText().toInt()];
		person.points.clear();
		RoboCompInnerModelManager::Pose3D pose;
		QStringList data = point_te->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts);
		for (auto line: data)
		{
			try
			{
				QStringList aux = line.split(QRegExp("[;]"),QString::SkipEmptyParts);
				if (aux.size() == 3)
				{
					pose.x = aux[0].toFloat();
					pose.z = aux[1].toFloat();
					pose.y = 0;
					pose.rx = 0;
					pose.ry = 0;
					pose.rz = 0;
					person.points.push_back(pose);
				}
				else
				{
					std::cout << "Skipping no complete position: " << line.toStdString() << std::endl;
				}
			}catch(...)
			{
				std::cout << "Exception" << std::endl;
			}
		}
	}
}
#define MIN_DISTANCE 100
RoboCompInnerModelManager::coord3D SpecificWorker::autoMovePerson(TPerson person)
{
	std::cout << "Auto move person" << std::endl;
	RoboCompInnerModelManager::coord3D nextPose;
	//compute distance to next point
	QVec current = QVec::vec2(person.points[person.currentPoint].x, person.points[person.currentPoint].z);
	QVec actual = QVec::vec2(person.pose.x, person.pose.z);
	
	QVec next = (current - actual);
	
	
	
	// check if next point is already achieve
	if ( (current - actual).norm2() < MIN_DISTANCE){
		if (person.currentPoint == person.points.size()){
			person.currentPoint = 0;
		}else{
			person.currentPoint++;
		}
	}
	// BASIC_PERIOD 
	return nextPose;
}

//insert one person interaction edge in AGM
void SpecificWorker::addInteraction()
{
	int id1 = -1;
	int id2 = -1;
	std::string edgeName = "";
	QString listEntry;
	
	TInteraction option = string2Interaction(interaction_cb->currentText().toStdString());
	id1 = int1_cb->currentText().toInt();
	switch(option)
	{
		case isBusy:	edgeName = "isBusy";
						id2 = int1_cb->currentText().toInt();
						listEntry = int1_cb->currentText() + QString(" => isBusy");
						break;
		case isBlocking:id2 = robotID;
						edgeName = "is_blocking";
						listEntry = int1_cb->currentText() + QString(" => is_blocking => ") + QString::number(robotID);
						break;
		case softBlock:	id2 = robotID;
						edgeName = "softBlock";
						listEntry = int1_cb->currentText() + QString(" => softBlock => ") + QString::number(robotID);
						break;
		case interacting:	id2 = int2_cb->currentText().toInt();
							if (id1 == id2)
							{
								std::cout << "Person could not interact with himself" << std::endl;
								QMessageBox::information(this, "Interaction", "Person could not interact with himself");
								return;
							}
							edgeName = "interacting";
							listEntry = int1_cb->currentText() + QString(" => interacting => ") + int2_cb->currentText();
							break;
	    case isNear:
            id1 = robotID;
            id2 = int1_cb->currentText().toInt();
                        edgeName = "is_near";
						listEntry =  QString::number(robotID) + QString(" => is_near => ") + int1_cb->currentText();
                        break;

	    case front: id2 = robotID;
            edgeName = "front";
            listEntry =int1_cb->currentText() + QString(" => front => ") + QString::number(robotID) ;
            break;

	case unknown:	std::cout <<"Unknown interaction selected, please check TInteraction valid values" <<std::endl;
						return;
	}
	QList<QListWidgetItem*> list = interaction_lw->findItems(listEntry, Qt::MatchExactly);
	if (list.size() > 0)
	{
		std::cout << "Interaction is already used" << std::endl;
		QMessageBox::information(this, "Interaction", "Interaction is already used");
	}
	else
	{
		AGMModel::SPtr newModel(new AGMModel(worldModel));
		newModel->addEdgeByIdentifiers(id1, id2, edgeName);
		if(sendModificationProposal(worldModel, newModel))
		{
			try{
				AGMModelEdge edge = newModel->getEdgeByIdentifiers(id1, id2, edgeName);
				QListWidgetItem *item = new QListWidgetItem(listEntry);
				item->setData(Qt::UserRole, QVariant::fromValue<AGMModelEdge>(edge));
				interaction_lw->addItem(item);
			}catch(...)
			{
				std::cout << "Error retrieving " << edgeName << " edge from newModel" << std::endl;
			}
		}
		rinteraction_pb->setEnabled(interaction_lw->count() > 0);
	}
}
//Remove edge from AGM
void SpecificWorker::removeEdgeAGM()
{
	std::cout << "Remove edge " << std::endl;
	QListWidgetItem *item = interaction_lw->currentItem();
	if (item != NULL)
	{
		AGMModelEdge edge = item->data(Qt::UserRole).value<AGMModelEdge>();
		AGMModel::SPtr newModel(new AGMModel(worldModel));
		newModel->removeEdge(edge);
		if(sendModificationProposal(worldModel, newModel))
		{
			std::cout << "remove item from list" << std::endl;
			interaction_lw->removeItemWidget(item);
			delete item;
		}
	}
	else{
		QMessageBox::information(this, "Interaction", "No interaction is selected");
	}
	// disable button is no interaction left
	rinteraction_pb->setEnabled(interaction_lw->count() > 0);
}

//remove any entri related with personID (it is used when person is removed)
void SpecificWorker::cleanListWidget(int personID)
{
	QList<QListWidgetItem*> list = interaction_lw->findItems(QString::number(personID), Qt::MatchContains);
	for (auto item: list)
	{
		interaction_lw->removeItemWidget(item);
		delete item;
	}
}

//convert std::string to enum TInteraction
SpecificWorker::TInteraction SpecificWorker::string2Interaction(std::string interaction)
{
	if (interaction == "isBusy") 		return isBusy;
	if (interaction == "interacting") 	return interacting;
	if (interaction == "isBlocking") 	return isBlocking;
	if (interaction == "softBlock") 	return softBlock;
	if (interaction == "isNear") 	    return isNear;
    if (interaction == "front") 	    return front;

	return unknown;
}

void SpecificWorker::interactionChanged(int index)
{
	qDebug()<<"interactionChanged";
	TInteraction option = string2Interaction(interaction_cb->currentText().toStdString());
	switch(option)
	{
		case isBusy:	    int2_cb->setEnabled(false);
						    break;
		case isBlocking:    int2_cb->setCurrentIndex(int2_cb->findText(QString::number(robotID),Qt::MatchExactly));
						    int2_cb->setEnabled(false);
						    break;
		case softBlock:	    int2_cb->setCurrentIndex(int2_cb->findText(QString::number(robotID),Qt::MatchExactly));
						    int2_cb->setEnabled(false);
						    break;
		case interacting:	int2_cb->setEnabled(true);
							break;

	    case isNear:
                            int2_cb->setCurrentIndex(int2_cb->findText(QString::number(robotID),Qt::MatchExactly));
                            int2_cb->setEnabled(false);
                            break;

	    case front:
            int2_cb->setCurrentIndex(int2_cb->findText(QString::number(robotID),Qt::MatchExactly));
            int2_cb->setEnabled(false);
            break;

		case unknown:	std::cout <<"Unknown interaction selected, please check TInteraction valid values" <<std::endl;
						break;
	}
}

// enable/disable interface elements (used to disable all element when no person)
void SpecificWorker::updatePersonInterfaz(bool enable)
{
	move_gb->setEnabled(enable);
	interacion_gb->setEnabled(enable);
//	pose_gb->setEnabled(enable);
    setPose_pb->setEnabled(enable);
    random->setEnabled(enable);
	points_gb->setEnabled(enable);
}

void SpecificWorker::moverandom()
{
    if (person_cb->currentText() == ""){
        QMessageBox::information(this, "No person selected", "You have to select any person to change its pose");
    }
    else
    {
//        TPerson *person = &personMap[person_cb->currentText().toInt()];

        coordInItem.x = hmin + (double)rand() * (hmax - hmin)/ (double)RAND_MAX;
        coordInItem.z = vmin + (double)rand() * (vmax - vmin)/ (double)RAND_MAX;
        valorgiro = 0 + (double)rand() * (3.141516)/ (double)RAND_MAX;

//        movePerson(person, coordInItem, setPoseFlag);
        x_sb->setValue(coordInItem.x );
        z_sb->setValue(coordInItem.z);
        rot_sb->setValue(valorgiro);

        setPoseFlag = true;
        
    }


}
