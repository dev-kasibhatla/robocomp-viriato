/*
 *    Copyright (C) 2020 by YOUR NAME HERE
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
#include "trajectoryrobot2dI.h"

TrajectoryRobot2DI::TrajectoryRobot2DI(GenericWorker *_worker)
{
	worker = _worker;
}


TrajectoryRobot2DI::~TrajectoryRobot2DI()
{
}

float TrajectoryRobot2DI::changeTarget(const TargetPose  &target, const Ice::Current&)
{
	return worker->TrajectoryRobot2D_changeTarget(target);
}

NavState TrajectoryRobot2DI::getState(const Ice::Current&)
{
	return worker->TrajectoryRobot2D_getState();
}

float TrajectoryRobot2DI::go(const TargetPose  &target, const Ice::Current&)
{
	return worker->TrajectoryRobot2D_go(target);
}

float TrajectoryRobot2DI::goBackwards(const TargetPose  &target, const Ice::Current&)
{
	return worker->TrajectoryRobot2D_goBackwards(target);
}

float TrajectoryRobot2DI::goReferenced(const TargetPose  &target, const float  xRef, const float  zRef, const float  threshold, const Ice::Current&)
{
	return worker->TrajectoryRobot2D_goReferenced(target, xRef, zRef, threshold);
}

void TrajectoryRobot2DI::mapBasedTarget(const NavigationParameterMap  &parameters, const Ice::Current&)
{
	worker->TrajectoryRobot2D_mapBasedTarget(parameters);
}

void TrajectoryRobot2DI::setHumanSpace(const PolyLineList  &polyList, const Ice::Current&)
{
	worker->TrajectoryRobot2D_setHumanSpace(polyList);
}

void TrajectoryRobot2DI::stop(const Ice::Current&)
{
	worker->TrajectoryRobot2D_stop();
}

