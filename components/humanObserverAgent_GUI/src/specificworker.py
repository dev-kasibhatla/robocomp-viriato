#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
#    Copyright (C) 2020 by YOUR NAME HERE
#
#    This file is part of RoboComp
#
#    RoboComp is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RoboComp is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
#
from genericworker import *
from datetime import date, datetime
from google_calender import CalenderApi
from PySide2.QtCore import QDateTime
from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *

# AGM related imports
import AGMModelConversion
from AGGL import *

import json
import sys
import math

try:
    from ui_activity_form import *
except:
    print("Can't import ui_activity_form UI file")

try:
    from ui_dailyActivity import *
except ImportError:
    print("Can't import ui_dailyActivity UI file")

try:
    from ui_viewLaser import *
except ImportError:
    print("Can't import ui_viewLaser UI file")

try:
    from ui_cameraViewer import *
except ImportError:
    print("Can't import ui_cameraViewer UI file")

try:
    import speech_recognition as sr
    import pyaudio
    import wave
except:
    print("Can't import speech_recognition or pyaudio or wave")
# If RoboComp was compiled with Python bindings you can use InnerModel in Python
# sys.path.append('/opt/robocomp/lib')
# import librobocomp_qmat
# import librobocomp_osgviewer
# import librobocomp_innermodel


class OpenActivityForm(QDialog):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.parent = parent
        self.ui = Ui_activityForm()
        self.ui.setupUi(self)
        self.setModal(True)
        self.ui.pushButton.clicked.connect(self.button_ok)
        self.ui.pushButton_2.clicked.connect(self.button_cancel)
        self.ui.comboBox_4.currentIndexChanged.connect(self.type_check)
        self.ui.comboBox_5.activated.connect(self.location_check)
        self.ui.textEdit_3.setText("Activity " + str(self.parent.ui.comboBox_3.count() + 1))
        self.ui.comboBox_6.addItems({"Stretcher", "Robot"})

    # save the data when ok button is pressed
    def button_ok(self):
        print("ok button")
        self.parent.ui.comboBox_3.addItem(self.ui.textEdit_3.toPlainText())
        start = str(self.parent.ui.dateEdit.date().toPython()) + "T" + str(self.ui.timeEdit.time().toPython()) + "Z"
        end = str(self.parent.ui.dateEdit.date().toPython()) + "T" + str(self.ui.timeEdit_2.time().toPython()) + "Z"
        description_data = {"Type": self.ui.comboBox_4.currentText(),
                            "IndividualName": self.ui.textEdit.toPlainText(),
                            "TherapistName": self.ui.textEdit_2.toPlainText(),
                            "Location": self.ui.comboBox_5.currentText(),
                            "Element": self.ui.comboBox_6.currentText(),
                            "Notification": self.ui.checkBox.isChecked(),
                            }
        body_content = {"summary": self.ui.textEdit_3.toPlainText(),
                        "description": json.dumps(description_data),
                        "start": {"dateTime": start},
                        "end": {"dateTime": end},
                        }
        print(body_content)
        self.parent.calendarApiObj.createEvent(bodyContent=body_content)
        self.hide()

    # exit the form and discard all the data
    def button_cancel(self):
        self.hide()
        start = str(self.parent.ui.dateEdit.date().toPython()) + "T" + str(self.ui.timeEdit.time().toPython()) + "Z"
        end = str(self.parent.ui.dateEdit.date().toPython()) + "T" + str(self.ui.timeEdit_2.time().toPython()) + "Z"
        body = {"summary": self.ui.textEdit_3.toPlainText(),
                "description": {"Type": self.ui.comboBox_4.currentText(),
                                "IndividualName": self.ui.textEdit.toPlainText(),
                                "TherapistName": self.ui.textEdit_2.toPlainText(),
                                "Location": self.ui.comboBox_5.currentText(),
                                "Element": self.ui.comboBox_6.currentText(),
                                "Notification": self.ui.checkBox.isChecked(),
                                },
                "start": {"dateTime": start},
                "end": {"dateTime": end},
                }
        print(body)

    # check the type of the activity
    def type_check(self, index):
        if self.ui.comboBox_4.itemText(index) == "Individual":
            self.ui.label_9.show()
            self.ui.textEdit.show()
        else:
            self.ui.label_9.hide()
            self.ui.textEdit.hide()

    # check the location
    def location_check(self, index):
        if self.ui.comboBox_5.itemText(index) == "Physical therapy room":
            self.ui.comboBox_6.clear()
            self.ui.comboBox_6.addItems({"Stretcher", "Robot"})
        else:
            self.ui.comboBox_6.clear()
            self.ui.comboBox_6.addItems({"Table", "Robot", "TV"})


class DailyActivity(QDialog):
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.parent = parent
        self.ui = Ui_DailyActivity()
        self.ui.setupUi(self)
        self.setModal(True)
        self.ui.tableWidget.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)

    def closeEvent(self, arg__1):
        self.parent.ui.label.setText(" ")


class ViewLaser(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self)
        self.parentObj = parent
        self.ui = Ui_LaserViewer()
        self.ui.setupUi(self)
        self.ui.doubleSpinBox.valueChanged.connect(self.repaint)
        self.ui.horizontalSlider.valueChanged.connect(self.updatePeriod)
        self.count = 0

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.updateValues)
        self.Period = 200
        self.laser_data, laser_dumy = [],[]
        self.timer.start(200)
        self.ui.horizontalSlider.setValue(5)

    def updatePeriod(self):
        period = int(self.ui.label_3.text())
        self.timer.setInterval(1000//period)

    def updateValues(self):
        self.update()

    def paintEvent(self, event=None):
        try:
            self.laser_data, laser_angle = self.parentObj.laser_proxy.getLaserAndBStateData()
        except:
            print("No laser is connected")
            print("exiting")
            self.close()

        xOff = self.width() / 2.
        yOff = self.height() / 2.
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)

        for point in self.laser_data:
            newCoor = self.measure2coord(point)
            painter.drawRect(newCoor[0] - 1, newCoor[1] - 1, 2, 2)

        for wm in range(10):
            w = 1000. * (1. + wm) * self.ui.doubleSpinBox.value()
            painter.drawEllipse(QRectF(0.5 * self.width() - w / 2., 0.5 * self.height() - w / 2., w, w))
        for wm in range(5):
            w = 200. * (1. + wm) * self.ui.doubleSpinBox.value()
            painter.drawEllipse(QRectF(0.5 * self.width() - w / 2., 0.5 * self.height() - w / 2., w, w))

        painter.end()
        painter = None

    def measure2coord(self, measure):
        const_mul = self.ui.doubleSpinBox.value()
        x = math.cos(measure.angle - 0.5 * math.pi) * measure.dist * const_mul + (0.5 * self.width())
        y = math.sin(measure.angle - 0.5 * math.pi) * measure.dist * const_mul + (0.5 * self.height())
        return x, y


class CameraViewer(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self)
        self.parent = parent
        self.ui = Ui_CamViewer()
        self.ui.setupUi(self)

        # timer init
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.repaint)
        self.timer.start(30)

        # variable init
        self.color, self.depth, self.headState, self.baseState = None,None,None,None

    def paintEvent(self, event=None):
        try:
            self.color, self.depth, self.headState, self.baseState = self.parent.rgbd_proxy.getData()
            if (len(self.color) == 3 * 640 * 480):
                color_image_width = 640
                color_image_height = 480
            elif (len(self.color) == 3 * 320 * 240):
                color_image_width = 320
                color_image_height = 240
            else:
                print('Undetermined Color image size in getRGB: we shall not paint! %d' % (len(self.color)))
                return

            painter = QPainter(self)
            painter.setRenderHint(QPainter.Antialiasing, True)

            if color_image_width != 0 and color_image_height != 0:
                image = QImage(self.color, color_image_width, color_image_height, QImage.Format_RGB888)
                painter.drawImage(QPointF(self.ui.frame.x(), self.ui.frame.y()), image)
            painter.end()
            painter = None
        except:
            print("No Camera is connected")
            print("exiting")
            self.close()

class Human():
    def __init__(self,id = None,name = ""):
        self.id = id
        self.name = ""
        self.age = "Age<30"
        self.userType = "Clinician"
        self.PhysicalDep = 0
        self.CognitiveDep = 0
        self.emotionalState = "Neutral"
        self.activity = "Rest"
        self.pose = Pose3D()
        self.photo = ""


class SpecificWorker(GenericWorker):
    def __init__(self, proxy_map):
        super(SpecificWorker, self).__init__(proxy_map)
        # calling the calendar api object
        try:
            self.calendarApiObj = CalenderApi()
        except:
            print("No internet connection detected")
            print("Restart the application to to enable Activity Calendar")
            self.ui.CalendarEvents.setEnabled(False)

        # Person's list
        self.persons = []
        self.timer.timeout.connect(self.compute)
        self.Period = 2000
        self.timer.start(self.Period)
        self.ui.pushButton_10.clicked.connect(self.newActivity)
        self.ui.pushButton_14.clicked.connect(self.viewAgenda)

        # for debugging
        # self.ui.check.clicked.connect(self.debug_click)

        # setting the date to today
        self.ui.dateEdit.setDateTime(QDateTime.currentDateTimeUtc())

        # init functions for robot navigation and control part #
        self.ui.pushButton_8.clicked.connect(self.gotoPoint)
        self.ui.pushButton_9.clicked.connect(self.keyboardControl)
        self.ui.pushButton_4.clicked.connect(self.stopRobotB)
        self.ui.pushButton_5.clicked.connect(self.resetRobotB)

        # init control buttons
        self.ui.left_button.clicked.connect(self.moveLeft)
        self.ui.right_button.clicked.connect(self.moveRight)
        self.ui.up_button.clicked.connect(self.moveForward)
        self.ui.down_button.clicked.connect(self.moveBackward)
        self.ui.stop_button.clicked.connect(self.stopRobotB)
        self.ui.cc_button.clicked.connect(self.rotCounterClockwise)
        self.ui.c_button.clicked.connect(self.rotClockwise)

        # limits
        self.translationSpeedLimit = 2000
        self.rotationSpeedLimit = 2

        # robot velocities variables
        self.velX = 0
        self.velZ = 0
        self.velRot = 0

        # Agm related initialization
        self.AGMinit()


        # view Laser method
        self.ui.viewLaser_button.clicked.connect(self.viewLaserB)
        self.laser_data, laser_dummy = [], []
        # view Camera method
        self.ui.view_camera_button.clicked.connect(self.viewCameraB)

        # ui initialize for human interaction
        self.ui.newHuman_button.clicked.connect(self.newHumanB)
        self.ui.setHuman_button.clicked.connect(self.setHumanB)
        self.ui.addPhotoB.clicked.connect(self.addPhoto)
        # self.ui.photo_viewer.connect(self.photo_change)
        self.ui.label_18.hide()
        self.ui.label_19.hide()
        self.ui.H_phyDep.hide()
        self.ui.H_cogDep.hide()
        self.ui.H_userType.currentIndexChanged.connect(self.userTypeChange)
        # def mousePressEvent(self, event:PySide2.QtGui.QMouseEvent):
        self.ui.id_list.currentIndexChanged.connect(self.updateHumanInfo)
        self.ui.setPose_pb.clicked.connect(self.setPose)
        self.currentImagePath = ""

        # interaction related
        self.ui.interaction_cb.currentTextChanged.connect(self.interactionChanged)
        self.ui.int2_cb.setEnabled(False)
        # self.ui.rinteraction_pb.setEnabled(False)
        self.ui.ainteraction_pb.clicked.connect(self.addInteraction)
        self.ui.rinteraction_pb.clicked.connect(self.removeEdgeAGM)
        # self.ui.interaction_cb_2.currentTextChanged.connect(self.interactionChanged2)
        self.ui.interaction_cb_2.hide()

        # initialisation related to TTS/ASR
        self.ui.speak_button.clicked.connect(self.speak_button_press)
        self.ui.listen_button.clicked.connect(self.listen_button_press)
        self.CHUNK = 1024
        self.FORMAT = pyaudio.paInt16
        self.CHANNELS = 1
        self.RATE = 44100
        self.RECORD_SECONDS = 5
        self.WAVE_OUTPUT_FILENAME = "output.wav"

        # initialisation of the face emmotion
        self.ui.FacialExpr.currentIndexChanged.connect(self.facialChange)
        self.facialChange()

    def userTypeChange(self,val):
        print(val," type")
        if self.ui.H_userType.currentText()=="Caregiving User":
            self.ui.label_18.show()
            self.ui.label_19.show()
            self.ui.H_phyDep.show()
            self.ui.H_cogDep.show()
        else:
            self.ui.label_18.hide()
            self.ui.label_19.hide()
            self.ui.H_phyDep.hide()
            self.ui.H_cogDep.hide()

    # initialization of AGM Proxy and DSR
    def AGMinit(self):
        self.worldModel = AGMGraph()
        try:
            w = self.agmexecutive_proxy.getModel()
            self.AGMExecutiveTopic_structuralChange(w)
            # get all the list of persons
            self.getPersonList_AGM()

        except:
            print("The executive is probably not running, waiting for first AGM model publication...")

    def __del__(self):
        print('SpecificWorker destructor')


    def setParams(self, params):
        # try:
        #	self.innermodel = InnerModel(params["InnerModelPath"])
        # except:
        #	traceback.print_exc()
        #	print("Error reading config params")
        return True

    @QtCore.Slot()
    def compute(self):
        # w = World()
        # w = self.agmexecutive_proxy.getModel()
        # print('SpecificWorker.compute...')
        # computeCODE
        # try:
        #     self.laser_data, laser_dummy = self.laser_proxy.getLaserAndBStateData()
        #     self.viewLaser.update()
        # except Ice.Exception as e:
        #     print(e)

        # The API of python-innermodel is not exactly the same as the C++ version
        # self.innermodel.updateTransformValues('head_rot_tilt_pose', 0, 0, 0, 1.3, 0, 0)
        # z = librobocomp_qmat.QVec(3,0)
        # r = self.innermodel.transform('rgbd', z, 'laser')
        # r.printvector('d')
        # print(r[0], r[1], r[2])

        return True

    #
    # SUBSCRIPTION to edgeUpdated method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_edgeUpdated(self, modification):
        #
        # subscribesToCODE
        #
        pass

    #
    # SUBSCRIPTION to edgesUpdated method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_edgesUpdated(self, modifications):
        #
        # subscribesToCODE
        #
        pass

    #
    # SUBSCRIPTION to selfEdgeAdded method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_selfEdgeAdded(self, nodeid, edgeType, attributes):
        #
        # subscribesToCODE
        #
        pass

    #
    # SUBSCRIPTION to selfEdgeDeleted method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_selfEdgeDeleted(self, nodeid, edgeType):
        #
        # subscribesToCODE
        #
        pass

    #
    # SUBSCRIPTION to structuralChange method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_structuralChange(self, w):
        self.mutex.lock()
        # print("before")
        # print(type(self.worldModel), type(w))
        self.worldModel = AGMModelConversion.fromIceToInternal_model(w)
        # print("after")
        # print(type(self.worldModel), type(w))
        # print(self.worldModel)
        # fromIceToInternal(w, worldModel)
        self.mutex.unlock()


    #
    # SUBSCRIPTION to symbolUpdated method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_symbolUpdated(self, modification):
        #
        # subscribesToCODE
        #
        pass

    #
    # SUBSCRIPTION to symbolsUpdated method from AGMExecutiveTopic interface
    #
    def AGMExecutiveTopic_symbolsUpdated(self, modifications):
        #
        # subscribesToCODE
        #
        pass

    # =============== Methods for Component Implements ==================
    # ===================================================================

    #
    # activateAgent
    #
    def AGMCommonBehavior_activateAgent(self, prs):
        ret = bool()
        #
        # implementCODE
        #
        return ret

    #
    # deactivateAgent
    #
    def AGMCommonBehavior_deactivateAgent(self):
        ret = bool()
        #
        # implementCODE
        #
        return ret

    #
    # getAgentParameters
    #
    def AGMCommonBehavior_getAgentParameters(self):
        ret = ParameterMap()
        #
        # implementCODE
        #
        return ret

    #
    # getAgentState
    #
    def AGMCommonBehavior_getAgentState(self):
        ret = StateStruct()
        #
        # implementCODE
        #
        return ret

    #
    # killAgent
    #
    def AGMCommonBehavior_killAgent(self):
        #
        # implementCODE
        #
        pass

    #
    # reloadConfigAgent
    #
    def AGMCommonBehavior_reloadConfigAgent(self):
        ret = bool()
        #
        # implementCODE
        #
        return ret

    #
    # setAgentParameters
    #
    def AGMCommonBehavior_setAgentParameters(self, prs):
        ret = bool()
        #
        # implementCODE
        #
        return ret

    #
    # uptimeAgent
    #
    def AGMCommonBehavior_uptimeAgent(self):
        ret = int()
        #
        # implementCODE
        #
        return ret

    # ===================================================================
    # ===================================================================

    # ======functions for ui=============
    def newActivity(self):
        print("button Clicked2")
        self.activityForm = OpenActivityForm(self)
        self.activityForm.show()

    def viewAgenda(self):
        print("view Agenda clicked")
        self.ui.label.show()
        self.ui.label.setText("Loading...")
        self.dailyActivity = DailyActivity(self)
        self.dailyActivity.ui.tableWidget.clearContents()
        self.dailyActivity.ui.tableWidget.setRowCount(1)
        self.fetchEvents()
        self.dailyActivity.show()

    def fetchEvents(self):
        self.ui.comboBox_3.clear()
        date_req = self.ui.dateEdit.date().toPython()
        eventList = self.calendarApiObj.getEvents(date_req)
        for event in eventList:
            try:
                type_string, IndividualName_string, TherapistName_string, Location_string, Element_string, Notification_string = '', '', '', '', '', ''
                # print(event)
                summary_string = str(event.get('summary', 'NoTitle'))
                # show only time not the date part of the string
                start_time = event.get('start', {})
                end_time = event.get('end', {})

                description_data = json.loads(event.get('description', '{}'))
                try:
                    type_string = description_data.get('Type', "Not Specified")
                    IndividualName_string = description_data.get('IndividualName', "Not Specified")
                    TherapistName_string = description_data.get('TherapistName', "Not Specified")
                    Location_string = description_data.get('Location', "Not Specified")
                    Element_string = description_data.get('Element', "Not Specified")
                    Notification_string = description_data.get('Notification', "Not Specified")
                    if Notification_string:
                        Notification_string = "True"
                    elif not Notification_string:
                        Notification_string = "False"
                except:
                    print("Unable to parse description")

                # if time is not specified meant the event is for full day
                start_string = start_time.get("dateTime", "           Full Day")[11:]
                end_string = end_time.get("dateTime", "           Full Day")[11:]

                # print(jsonObj["Type"], jsonObj["Type"], jsonObj["Type"])
                self.ui.comboBox_3.addItem(summary_string)
                rowNumber = self.dailyActivity.ui.tableWidget.rowCount()
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 0, QTableWidgetItem(summary_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 1, QTableWidgetItem(start_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 2, QTableWidgetItem(end_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 3, QTableWidgetItem(type_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 4, QTableWidgetItem(IndividualName_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 5, QTableWidgetItem(TherapistName_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 6, QTableWidgetItem(Location_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 7, QTableWidgetItem(Element_string))
                self.dailyActivity.ui.tableWidget.setItem(rowNumber - 1, 8, QTableWidgetItem(Notification_string))
                self.dailyActivity.ui.tableWidget.setRowCount(rowNumber + 1)

            except:
                print("unable to parse the event")
                print("Unexpected error:", sys.exc_info()[0])

    def gotoPoint(self):
        robotPose = Pose3D()
        robotPose.x = float(self.ui.lineEdit.text())
        robotPose.y = 0
        robotPose.z = float(self.ui.lineEdit_2.text())
        robotPose.rx = 0
        robotPose.ry = float(self.ui.lineEdit_3.text())
        robotPose.rz = 0
        self.innermodelmanager_proxy.setPoseFromParent("robot", robotPose)
        print("goto")
        # print(float(self.ui.lineEdit.text())+2)

    def keyboardControl(self):
        if self.ui.pushButton_9.text() == 'Keyboard Control Off':
            self.ui.pushButton_9.setStyleSheet(u"background-color: rgb(41, 239, 41);")
            self.ui.pushButton_9.setText('Keyboard Control On')
            print("Keyboard Control On")
        else:
            self.ui.pushButton_9.setStyleSheet(u"background-color: rgb(239, 41, 41);")
            self.ui.pushButton_9.setText('Keyboard Control Off')
            print("Keyboard Control Off")

    # function for robot movement
    def moveForward(self):
        if self.velZ > self.translationSpeedLimit:
            self.velZ = self.velZ
        else:
            self.velZ += 20
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def moveBackward(self):
        if self.velZ < -1 * self.translationSpeedLimit:
            self.velZ = self.velZ
        else:
            self.velZ -= 20
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def moveLeft(self):
        if self.velX < -1 * self.translationSpeedLimit:
            self.velX = self.velX
        else:
            self.velX -= 20
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def moveRight(self):
        if self.velX > self.translationSpeedLimit:
            self.velX = self.velX
        else:
            self.velX += 20
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def rotCounterClockwise(self):
        if self.velRot < -1 * self.rotationSpeedLimit:
            self.velRot = self.velRot
        else:
            self.velRot -= 0.1
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def rotClockwise(self):
        if self.velRot > self.rotationSpeedLimit:
            self.velRot = self.velRot
        else:
            self.velRot += 0.1
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    def stopRobot(self):
        self.velX = 0
        self.velZ = 0
        self.velRot = 0
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    # overriding keyPressEvent from QWidget
    def keyPressEvent(self, event):
        if self.ui.pushButton_9.text() == 'Keyboard Control On':
            if event.key() == QtCore.Qt.Key_W:
                self.moveForward()
                print("w button")
            elif event.key() == QtCore.Qt.Key_S:
                self.moveBackward()
                print("s button")
            elif event.key() == QtCore.Qt.Key_A:
                self.moveLeft()
                print("a button")
            elif event.key() == QtCore.Qt.Key_D:
                self.moveRight()
                print("d button")
            elif event.key() == QtCore.Qt.Key_F:
                self.rotCounterClockwise()
                print("left button")
            elif event.key() == QtCore.Qt.Key_G:
                self.rotClockwise()
                print("right button")
            elif event.key() == QtCore.Qt.Key_Backspace:
                self.stopRobot()
                print("stop button")

    # method to stop the robot
    def stopRobotB(self):
        print("stop button")
        self.velX = 0
        self.velZ = 0
        self.velRot = 0
        try:
            self.omnirobot_proxy.setSpeedBase(self.velX, self.velZ, self.velRot)
        except:
            print("no robot found")

    # method to reset the position of the robot
    def resetRobotB(self):
        print("reset button")
        robotPose = Pose3D()
        robotPose.x = 0
        robotPose.y = 0
        robotPose.z = 0
        robotPose.rx = 0
        robotPose.ry = 0
        robotPose.rz = 0
        self.innermodelmanager_proxy.setPoseFromParent("robot", robotPose)

    def viewCameraB(self):
        self.viewCamera = CameraViewer(parent=self)
        self.viewCamera.show()

    def viewLaserB(self):
        self.viewLaser = ViewLaser(parent=self)
        self.viewLaser.show()
        # print(self.laser_data)

    # function to add a person in AGM
    def includeInAGM(self, Id, pose, mesh):
        print("includeInAGM begins\n")
        # name = "person"
        imName = "person" + str(Id)
        # personSymbolId = -1
        # idx = 0
        attribute2 = dict()
        attribute2["imName"] = imName
        attribute2["imType"] = "transform"

        self.worldModel.addNode(0, 0, Id, "person", attribute2)
        self.worldModel.addEdge(Id, 3, "in")

        edgeRTAtrs2 = dict()
        edgeRTAtrs2["tx"] = "0"
        edgeRTAtrs2["ty"] = "0"
        edgeRTAtrs2["tz"] = "0"
        edgeRTAtrs2["rx"] = "0"
        edgeRTAtrs2["ry"] = "0"
        edgeRTAtrs2["rz"] = "0"
        self.worldModel.addEdge(100, Id, "RT", edgeRTAtrs2)

        attribute = dict()
        attribute["collidable"] = "false"
        attribute["imName"] = imName + "_Mesh"
        attribute["imType"] = "mesh"
        meshPath = "/home/robocomp/robocomp/components/robocomp-viriato/files/osgModels/" + mesh
        attribute["path"] = str(meshPath)
        attribute["render"] = "NormalRendering"
        attribute["scalex"] = str(12)
        attribute["scaley"] = str(12)
        attribute["scalez"] = str(12)
        self.worldModel.addNode(0, 0, Id + 1, "mesh_person", attribute)
        # self.worldModel.addNode(0, 0, temp_id + 1, "personMesh")

        edgeRTAtrs = dict()
        edgeRTAtrs["tx"] = "0"
        edgeRTAtrs["ty"] = "0"
        edgeRTAtrs["tz"] = "0"
        edgeRTAtrs["rx"] = "1.570796326794"
        edgeRTAtrs["ry"] = "0"
        edgeRTAtrs["rz"] = "3.1415926535"
        # id used in addEdge should be int type
        self.worldModel.addEdge(Id, Id + 1, "RT", edgeRTAtrs)

        self.newModel = AGMModelConversion.fromInternalToIce(self.worldModel)
        self.agmexecutive_proxy.structuralChangeProposal(self.newModel, "gui1", "log2")
        print("includeInAGM ends\n")
        return Id

    # function to add a person in RCIS
    def includeInRCIS(self, id_val, pose, meshName):
        print("includeInRCIS begins")
        name = "person" + str(id_val)
        mesh = meshType()
        mesh.pose.x = 0
        mesh.pose.y = 0
        mesh.pose.z = 0
        mesh.pose.rx = 1.57079632679
        mesh.pose.ry = 0
        mesh.pose.rz = 3.1415926535
        mesh.scaleX = mesh.scaleY = mesh.scaleZ = 12
        mesh.render = 0
        mesh.meshPath = "/home/robocomp/robocomp/components/robocomp-viriato/files/osgModels/" + meshName
        try:
            self.innermodelmanager_proxy.addTransform(name, "static", "root", pose)
        except:
            print("Can't create fake person name ")
            return False
        try:
            self.innermodelmanager_proxy.addMesh(name + "_mesh", name, mesh)
        except:
            print("Can't create fake person mesh")
            return False

        print("includeInRCIS ends")
        return True

    # function to add the person information in in-memory database
    def updatePersons(self):
        temp_human = Human()
        temp_id = int(self.ui.id_list.currentText())
        for p in self.persons:
            if p.id==temp_id:
                p.name = self.ui.H_name.text()
                p.age = self.ui.H_age.currentText()
                p.userType = self.ui.H_userType.currentText()
                p.PhysicalDep = self.ui.H_phyDep.value()
                p.CognitiveDep = self.ui.H_cogDep.value()
                p.emotionalState = self.ui.H_emoSate.currentText()
                p.activity = self.ui.H_activity.currentText()
                p.pose.x = self.ui.x_sb.value()
                p.pose.z = self.ui.z_sb.value()
                p.pose.rx = self.ui.rot_sb.value()
                p.photo = self.currentImagePath
                return
        temp_human.id = temp_id
        temp_human.name = self.ui.H_name.text()
        temp_human.age = self.ui.H_age.currentText()
        temp_human.userType = self.ui.H_userType.currentText()
        temp_human.PhysicalDep = self.ui.H_phyDep.value()
        temp_human.CognitiveDep = self.ui.H_cogDep.value()
        temp_human.emotionalState = self.ui.H_emoSate.currentText()
        temp_human.activity = self.ui.H_activity.currentText()
        temp_human.pose.x = self.ui.x_sb.value()
        temp_human.pose.z = self.ui.z_sb.value()
        temp_human.pose.rx = self.ui.rot_sb.value()
        temp_human.photo = self.currentImagePath
        self.persons.append(temp_human)

    def updateHumanInfo(self):
        print(int(self.ui.id_list.currentText()))
        currID = int(self.ui.id_list.currentText())
        self.load_image("")
        for human in list(self.persons):
            if human.id == currID:
                self.ui.H_name.setText(human.name)
                self.ui.H_age.setCurrentText(human.age)
                self.ui.H_userType.setCurrentText(human.userType)
                self.ui.H_phyDep.setValue(human.PhysicalDep)
                self.ui.H_cogDep.setValue(human.CognitiveDep)
                self.ui.H_emoSate.setCurrentText(human.emotionalState)
                self.ui.H_activity.setCurrentText( human.activity)
                self.ui.x_sb.setValue(human.pose.x)
                self.ui.z_sb.setValue(human.pose.z)
                self.ui.rot_sb.setValue(human.pose.rx)
                self.currentImagePath = human.photo
                self.load_image(human.photo)
                break
        # self.load_image("")

    # to create a new Person Node in DSR
    def newHumanB(self):
        print("newHuman")
        pose = Pose3D()
        pose.x = 0
        pose.y = 0
        pose.z = 0
        pose.rx = 0
        pose.ry = 0
        pose.rz = 0
        userid = self.getLatestId()
        # t = self.getLatestId()
        # print(type(t),t)
        meshname = "human01.3ds"
        scale = "12"
        rotationz = "3.1415926535"
        if self.includeInRCIS(userid, pose, meshname):
            self.includeInAGM(userid, pose, meshname)
            self.ui.id_list.addItem(str(userid))
            self.ui.int1_cb.addItem(str(userid))
            self.ui.int2_cb.addItem(str(userid))
            self.ui.id_list.setCurrentIndex(self.ui.id_list.count() - 1)
            self.currentImagePath = ""
            # self.updatePersons()
            temp_human = Human(id=userid)
            self.persons.append(temp_human)
            self.updateHumanInfo()

        else:
            print("error creating in RCIS")

    # Set or update the human information with the corresponding id
    # selected in the id combo box
    def setHumanB(self):
        print("set human")
        self.updatePersons()


    def addPhoto(self):
        path_to_file, _ = QFileDialog.getOpenFileName(self, "Load Image", "~/Desktop/", "Images (*.jpg)")
        self.currentImagePath = path_to_file
        self.load_image(path_to_file)

    def load_image(self, image_path):
        self.scene_obj = QGraphicsScene()
        self.ui.photo_viewer.setScene(self.scene_obj)
        pixmap_obj = QPixmap(image_path)
        self.scene_obj.addPixmap(pixmap_obj)
        self.ui.photo_viewer.fitInView(QRectF(0, 0, pixmap_obj.width(), pixmap_obj.height()),Qt.KeepAspectRatio)
        self.scene_obj.update()

    # get list of all person currently in the DSR
    def getPersonList_AGM(self):
        src = self.worldModel
        for nodeSrc in list(src.nodes.values()):
            if nodeSrc.sType == 'person':
                self.ui.id_list.addItem(nodeSrc.name)
                self.ui.int1_cb.addItem(nodeSrc.name)
                self.ui.int2_cb.addItem(nodeSrc.name)
            if nodeSrc.sType == 'object':
                self.ui.int1_cb.addItem(nodeSrc.name)
                self.ui.int2_cb.addItem(nodeSrc.name)

                # temp_person = Human(name=str(nodeSrc.name))
                # self.persons.append(temp_person)

    def getLatestId(self):
        src = self.worldModel
        last_id = 0
        for nodeSrc in list(src.nodes.values()):
            last_id = max(last_id,int(nodeSrc.name))
            # print(nodeSrc.name)
        for link in list(src.links):
            last_id = max(last_id, int(link.a))
            last_id = max(last_id, int(link.b))
        # the latest available id that can be assigned to any node or link
        return last_id + 1

    def toDSR(self):
        print("todsr")
        print(self.persons)
        # print(self.worldModel)

    def setPose(self):
        if self.ui.id_list.currentText() == "":
            print("No selected person to move")
        else:
            temp_id = int(self.ui.id_list.currentText())
            name = "person" + str(temp_id)
            pose = Pose3D()
            pose.x = self.ui.x_sb.value()
            pose.y = 0
            pose.z = self.ui.z_sb.value()
            pose.rx = 0
            pose.ry = self.ui.rot_sb.value()
            pose.rz = 0
            # move the person in RCIS
            try:
                self.innermodelmanager_proxy.setPoseFromParent(name,pose)
            except:
                print("Exception moving person in RCIS: ")
                return

            # move the person in AGM
            edgeRTAtrs = dict()
            edgeRTAtrs["tx"] = str(self.ui.x_sb.value())
            edgeRTAtrs["ty"] = "0"
            edgeRTAtrs["tz"] = str(self.ui.z_sb.value())
            edgeRTAtrs["rx"] = "0"
            edgeRTAtrs["ry"] = str(self.ui.rot_sb.value())
            edgeRTAtrs["rz"] = "0"
            self.newModel = AGMModelConversion.fromInternalToIce(self.worldModel)
            self.edgeUpdate(100, temp_id, "RT",edgeRTAtrs)
            try:
                self.agmexecutive_proxy.structuralChangeProposal(self.newModel, "gui1", "logger")
            except:
                print("Exception moving in AGM")

    def edgeUpdate(self,a, b, linkLabel='',attr=None):
        src = self.newModel
        for srcEdge in src.edges:
            if srcEdge.a==a and srcEdge.b==b:
                if srcEdge.edgeType == linkLabel:
                    srcEdge.attributes = attr
                    break


    def interactionChanged(self,index):
        # print("inter",index)
        if index=="isBusy":
            self.ui.int2_cb.setEnabled(False)
            self.ui.interaction_cb_2.hide()
        elif index=="block":
            self.ui.int2_cb.setEnabled(False)
            self.ui.interaction_cb_2.hide()
        elif index=="softBlock":
            self.ui.int2_cb.setEnabled(False)
            self.ui.interaction_cb_2.hide()
        elif index=="interacting":
            self.ui.int2_cb.setEnabled(True)
            self.ui.interaction_cb_2.show()
        else:
            print("Unknown interaction selected")


    def addInteraction(self):
        curr_text = self.ui.interaction_cb.currentText()
        id1 = self.ui.int1_cb.currentText()
        id2 = self.ui.int2_cb.currentText()
        robotID = "1"
        if curr_text == "isBusy":
            self.ui.int2_cb.setEnabled(False)
            id2 = id1
            listEntry = id1 + " => " + curr_text
        elif curr_text == "block":
            self.ui.int2_cb.setEnabled(False)
            id2 = robotID
            listEntry = id1 + " => " + curr_text + " " + id2
        elif curr_text == "softBlock":
            self.ui.int2_cb.setEnabled(False)
            id2 = robotID
            listEntry = id1 + " => " + curr_text + " " + id2
        elif curr_text == "interacting":
            self.ui.int2_cb.setEnabled(True)
            if id1 == id2:
                msgBox = QMessageBox()
                msgBox.setText('Person could not interact with himself')
                msgBox.exec_()
                return
            listEntry = id1 + " => " + curr_text + " " + id2
        else:
            print("Unknown interaction selected")
            return

        # check if the link is already added or not
        listNum = self.ui.interaction_lw.findItems(listEntry,Qt.MatchExactly)
        if len(listNum)>0:
            msgBox = QMessageBox()
            msgBox.setText('Interaction is already used')
            msgBox.exec_()
        else:
            if self.ui.interaction_cb.currentText()== "interacting":
                attrs = {
                    'type': str(self.ui.interaction_cb_2.currentText()),
                    'timeStarted': str(datetime.now()),
                         }
            else:
                attrs = None
            self.worldModel.addEdge(id1, id2, curr_text,attrs)
            try:
                edge = AGMLink(id1, id2, curr_text,attrs, enabled=True)
                self.newModel = AGMModelConversion.fromInternalToIce(self.worldModel)
                item = QListWidgetItem(listEntry)
                # adding the edge information to the item
                item.setData(Qt.UserRole,edge)
                self.agmexecutive_proxy.structuralChangeProposal(self.newModel, "gui1", "log2")
                self.ui.interaction_lw.addItem(item)
                self.ui.rinteraction_pb.setEnabled(True)
            except:
                print("error retrieving edge from newModel")

    def removeEdgeAGM(self):
        print("remove")
        itemR = self.ui.interaction_lw.currentRow()
        item1 = self.ui.interaction_lw.currentItem()
        if not item1 is None:
            edge = item1.data(Qt.UserRole)
            print(edge)
            # print(self.worldModel)
            src = self.worldModel
            print(src.links[-1],type(src.links[0].a))
            print(edge,type(edge.a))
            try:
                if self.worldModel.removeEdge(edge.a,edge.b,edge.linkType):
                    print("success")
                    self.newModel = AGMModelConversion.fromInternalToIce(self.worldModel)
                    self.agmexecutive_proxy.structuralChangeProposal(self.newModel, "gui1", "log2")
                    self.ui.interaction_lw.takeItem(itemR)
                    del item1
                    del itemR
                else:
                    print("fail")
            except:
                print("error deleting edge from worldModel")
        else:
            msgBox = QMessageBox()
            msgBox.setText('No interaction is selected')
            msgBox.exec_()

    def _say_with_festival(self, text):
        shellcommand = "echo \"%s\" | iconv -f utf-8 -t iso-8859-1 | padsp festival --tts --language spanish" % text
        print('Order: ' + text)
        print('Shell: "' + shellcommand + '"')
        if os.system(shellcommand):
            print("install festivale using the below command")
            print("sudo apt-get install festival festvox-kallpc16k")

    def speak_button_press(self):
        print("speak")
        speak_text = self.ui.tts_edit.toPlainText()
        self._say_with_festival(speak_text)

    def listen_button_press(self):
        try:
            p = pyaudio.PyAudio()
            stream = p.open(format=self.FORMAT,
                            channels=self.CHANNELS,
                            rate=self.RATE,
                            input=True,
                            frames_per_buffer=self.CHUNK)
            self.ui.listen_status.setText("recording..")
            self.ui.listen_status.repaint()
            print("* recording")

            frames = []

            for i in range(0, int(self.RATE / self.CHUNK * self.RECORD_SECONDS)):
                data = stream.read(self.CHUNK)
                frames.append(data)

            self.ui.listen_status.setText("Done recording")
            self.ui.listen_status.repaint()
            print("* done recording")

            stream.stop_stream()
            stream.close()
            p.terminate()

            wf = wave.open(self.WAVE_OUTPUT_FILENAME, 'wb')
            wf.setnchannels(self.CHANNELS)
            wf.setsampwidth(p.get_sample_size(self.FORMAT))
            wf.setframerate(self.RATE)
            wf.writeframes(b''.join(frames))
            wf.close()

            r = sr.Recognizer()
            with sr.Microphone() as source:  # use the default microphone as the audio source
                audio = r.adjust_for_ambient_noise(
                    source)  # listen for 1 second to calibrate the energy threshold for ambient noise levels

            with sr.WavFile("output.wav") as source:
                audio = r.record(source)
            try:
                self.ui.listen_status.setText("Analysing")
                self.ui.listen_status.repaint()
                command = r.recognize_google(audio,
                                             language="es-ES")  # recognize speech using Google Speech Recognition
                print(command)
                self.ui.asr_edit.setText(command)
                self.ui.listen_status.setText("")
                self.ui.listen_status.repaint()
            except LookupError:  # speech is unintelligible
                self.ui.listen_status.setText("Say Again..")
                self.ui.listen_status.repaint()
                print("Could not understand audio")
                self.ui.asr_edit.setText("Error, not understand audio")
        except:
            print("error ")

    def facialChange(self):
        print("index changed")
        face = self.ui.FacialExpr.currentText()
        image_path = ":/faces/faces/" + face + ".png"
        self.scene_obj = QGraphicsScene()
        self.ui.graphicsView_4.setScene(self.scene_obj)
        pixmap_obj = QPixmap(image_path)
        self.scene_obj.addPixmap(pixmap_obj)
        self.ui.graphicsView_4.fitInView(QRectF(0, 0, pixmap_obj.width(), pixmap_obj.height()), Qt.KeepAspectRatio)
        self.scene_obj.update()