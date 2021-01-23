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

/**
	\brief
	@author authorname
*/

#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include "grid.h"
#include "grid.cpp"
#include "navigation.h"
#include "navigation.cpp"
#include <controller.h>
#include <doublebuffer/DoubleBuffer.h>
#include <myscene.h>
#include <Eigen/Dense>

#define USE_QTGUI
#ifdef USE_QTGUI
	#include "innerviewer.h"
#endif


class SpecificWorker : public GenericWorker
{
    using Point = std::pair<float, float>;
    Q_OBJECT
    public:
        SpecificWorker(TuplePrx tprx, bool startup_check);
        ~SpecificWorker();
        bool setParams(RoboCompCommonBehavior::ParameterList params);

        //#ifdef USE_QTGUI
        //    using InnerViewerPtr = std::shared_ptr<InnerViewer>;
        //#endif

    public slots:
        void compute();
        int startup_check();
        void initialize(int period);

    private:
        std::shared_ptr<InnerModel>innerModel;
        Grid<>::Dimensions dim;
        //#ifdef USE_QTGUI
        //    InnerViewerPtr viewer;
        //#endif
        bool startup_check_flag;
        Navigation<Grid<>,Controller> navigation;
        std::shared_ptr<RoboCompCommonBehavior::ParameterList> confParams;

        //robot
        RoboCompGenericBase::TBaseState read_base();
        std::tuple<QPolygonF,RoboCompLaser::TLaserData> read_laser();
        const float MAX_SPIKING_ANGLE_rads = 0.2;
        const float MAX_RDP_DEVIATION_mm  =  70;
        void ramer_douglas_peucker(const vector<Point> &pointList, double epsilon, vector<Point> &out);
        void stop_robot();

        //2d scene
        const float ROBOT_LENGTH = 400;
        MyScene scene;
        QGraphicsItem *robot_polygon = nullptr;

        // Target
        using Target = Navigation<Grid<>,Controller>::Target;
        Target target;
        DoubleBuffer<QPointF, Target> target_buffer;

        // path
        void draw_path(const std::vector<QPointF> &path);
        bool atTarget = true;
        void init_drawing( Grid<>::Dimensions dim);
        QGraphicsEllipseItem *target_draw = nullptr;
        void draw_target(const RoboCompGenericBase::TBaseState &bState, QPointF t);
        void draw_laser(const QPolygonF &poly);
        void initializeWorld();
        std::vector<QGraphicsItem *> boxes;
};

#endif
