# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainUI.ui'
##
## Created by: Qt User Interface Compiler version 5.15.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import (QCoreApplication, QDate, QDateTime, QMetaObject,
    QObject, QPoint, QRect, QSize, QTime, QUrl, Qt)
from PySide2.QtGui import (QBrush, QColor, QConicalGradient, QCursor, QFont,
    QFontDatabase, QIcon, QKeySequence, QLinearGradient, QPalette, QPainter,
    QPixmap, QRadialGradient)
from PySide2.QtWidgets import *


class Ui_guiDlg(object):
    def setupUi(self, guiDlg):
        if not guiDlg.objectName():
            guiDlg.setObjectName(u"guiDlg")
        guiDlg.resize(281, 52)
        self.verticalLayout = QVBoxLayout(guiDlg)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.faceDet_checkbox = QCheckBox(guiDlg)
        self.faceDet_checkbox.setObjectName(u"faceDet_checkbox")

        self.verticalLayout.addWidget(self.faceDet_checkbox)


        self.retranslateUi(guiDlg)

        QMetaObject.connectSlotsByName(guiDlg)
    # setupUi

    def retranslateUi(self, guiDlg):
        guiDlg.setWindowTitle(QCoreApplication.translate("guiDlg", u"faceDetector", None))
        self.faceDet_checkbox.setText(QCoreApplication.translate("guiDlg", u"Face Detected", None))
    # retranslateUi

