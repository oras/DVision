# -------------------------------------------------
# APM Planner - Micro Air Vehicle Groundstation
# Please see our website at <http://ardupilot.com>
# Maintainer:
# Bill Bonney <billbonney@communitech.com>
# Based of QGroundControl by Lorenz Meier <lm@inf.ethz.ch>
#
# (c) 2009-2011 QGroundControl Developers
# (c) 2013 Bill Bonney <billbonney@communistech.com>
# (c) 2013 Michael Carpenter <malcom2073@gmail.com>
#
# This file is part of the open groundstation project
# APM Planner is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# APM Planner is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with QGroundControl. If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------


message(Qt version $$[QT_VERSION])

# Setup our supported build types. We do this once here and then use the defined config scopes
# to allow us to easily modify suported build types in one place instead of duplicated throughout
# the project file.

DEFINES+=DISABLE_3DMOUSE    # Disable 3D mice support for now
#DEFINES+=ENABLE_CAMRAVIEW   # Example to include camraview

linux-g++-64 {
    message(Linux build x64_86)
    CONFIG += LinuxBuild
    DEFINES += Q_LINUX_64
    DEFINES += FLITE_AUDIO_ENABLED

    DISTRO = $$system(lsb_release -i)

    contains( DISTRO, "Ubuntu" ) {
        message(ArchLinux Build)
        DEFINES += Q_UBUNTU
    }

    contains( DISTRO, "Arch" ) {
        message(ArchLinux Build)
        DEFINES += Q_ARCHLINUX
    }

} else: linux-g++ {
    message(Linux build x86)
    CONFIG += LinuxBuild
    DEFINES += Q_LINUX_32
    DEFINES += FLITE_AUDIO_ENABLED

    DISTRO = $$system(lsb_release -i)

    contains( DISTRO, "Ubuntu" ) {
        message(ArchLinux Build)
        DEFINES += Q_UBUNTU
    }

    contains( DISTRO, "Arch" ) {
        message(ArchLinux Build)
        DEFINES += Q_ARCHLINUX
    }

} else : win32-msvc2012 | win32-msvc2013 {
    message(Windows build)
    CONFIG += WindowsBuild
}  else : win32-g++|win64-g++ {
    message(Windows Cross Build)
    CONFIG += WindowsCrossBuild
} else : macx-clang | macx-g++ {
    message(Mac build)
    CONFIG += MacBuild
} else {
    error(Unsupported build type)
}

# Setup our supported build flavors

CONFIG(debug, debug|release) {
    message(Debug flavor)
    CONFIG += DebugBuild
} else:CONFIG(release, debug|release) {
    message(Release flavor)
    CONFIG += ReleaseBuild
} else {
    error(Unsupported build flavor)
}

# Setup our build directories

BASEDIR = $${IN_PWD}
DebugBuild {
    DESTDIR = $${OUT_PWD}/debug
    BUILDDIR = $${OUT_PWD}/build-debug
}
ReleaseBuild {
    DESTDIR = $${OUT_PWD}/release
    BUILDDIR = $${OUT_PWD}/build-release
}
OBJECTS_DIR = $${BUILDDIR}/obj
MOC_DIR = $${BUILDDIR}/moc
UI_DIR = $${BUILDDIR}/ui
RCC_DIR = $${BUILDDIR}/rcc
LANGUAGE = C++

TEMPLATE = app
TARGET = apmplanner2

message(BASEDIR $$BASEDIR DESTDIR $$DESTDIR TARGET $$TARGET)

# Qt configuration
CONFIG += qt \
    thread

QT += network \
    opengl \
    svg \
    xml \
    sql \
    widgets \
    serialport \
    script\
    quick \
    printsupport \
    qml \
    quickwidgets

##  testlib is needed even in release flavor for QSignalSpy support
QT += testlib

#Not sure what we were doing here, will have to ask
#!NOTOUCH {
#    gittouch.commands = touch qgroundcontrol.pro
#    QMAKE_EXTRA_TARGETS += gittouch
#    POST_TARGETDEPS += gittouch
#}

# Turn off serial port warnings
DEFINES += _TTY_NOWARN_

#Turn on camera view
#DEFINES += AMERAVIEW
#
# Logging Library
#
include (QsLog/QsLog.pri)
#include (libs/mavlink/include/mavlink/v1.0-qt/mavlink.pri)
#
# OS Specific settings
#

RaspberryPiBuild {
   DEFINES -= CAMERAVIEW
}

MacBuild {
    QT += multimedia

    QMAKE_INFO_PLIST = Custom-Info.plist
    CONFIG += x86_64
    CONFIG -= x86
#    QMAKE_MAC_SDK = macosx10.11 # Required for Xcode7.0
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    ICON = $$BASEDIR/files/APMIcons/icon.icns
    QMAKE_INFO_PLIST = APMPlanner.plist   # Sets the pretty name for the build

    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)

    LIBS += -lz
    LIBS += -lssl -lcrypto
    LIBS += -framework ApplicationServices
}

LinuxBuild {
    DEFINES += __STDC_LIMIT_MACROS

    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)

    LIBS += -lsndfile -lasound
    LIBS += -lz
    LIBS += -lssl -lcrypto
    DEFINES += OPENSSL
}

WindowsBuild {
    DEFINES += __STDC_LIMIT_MACROS

    # Specify multi-process compilation within Visual Studio.
    # (drastically improves compilation times for multi-core computers)
    QMAKE_CXXFLAGS_DEBUG += -MP
    QMAKE_CXXFLAGS_RELEASE += -MP

    # QWebkit is not needed on MS-Windows compilation environment
    CONFIG -= webkit

    RC_FILE = $$BASEDIR/qgroundcontrol.rc

    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
}

WindowsCrossBuild {
    QT += script
    # Windows version cross compiled on linux using
    DEFINES += __STDC_LIMIT_MACROS

    # QWebkit is not needed on MS-Windows compilation environment
    CONFIG -= webkit

    RC_FILE = $$BASEDIR/qgroundcontrol.rc
    LIBS += -lz
    CONFIG += exceptions rtti

    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
}

#
# Build flavor specific settings
#

DebugBuild {
#Let console be defined by the IDE/Build process.
    #CONFIG += console
}

ReleaseBuild {
    DEFINES += QT_NO_DEBUG

    WindowsBuild {
        # Use link time code generation for beteer optimization (I believe this is supported in msvc express, but not 100% sure)
        QMAKE_LFLAGS_LTCG = /LTCG
        QMAKE_CFLAGS_LTCG = -GL
    }
}

#
# Unit Test specific configuration goes here (debug only)
#

#DebugBuild {
#    INCLUDEPATH += \
#        src/qgcunittest

#    HEADERS += \
#        src/qgcunittest/AutoTest.h \
#        src/qgcunittest/UASUnitTest.h \
#        src/qgcunittest/MockUASManager.h \
#        src/qgcunittest/MockUAS.h \
#        src/qgcunittest/MockQGCUASParamManager.h \
#        src/qgcunittest/MultiSignalSpy.h \
#        src/qgcunittest/TCPLinkTest.h \
#        src/qgcunittest/FlightModeConfigTest.h

#    SOURCES += \
#        src/qgcunittest/UASUnitTest.cc \
#        src/qgcunittest/MockUASManager.cc \
#        src/qgcunittest/MockUAS.cc \
#        src/qgcunittest/MockQGCUASParamManager.cc \
#        src/qgcunittest/MultiSignalSpy.cc \
#        src/qgcunittest/TCPLinkTest.cc \
#        src/qgcunittest/FlightModeConfigTest.cc
#}

#
# External library configuration
#

include(QGCExternalLibs.pri)

#
# Post link configuration
#

include(QGCSetup.pri)

#
# Main QGroundControl portion of project file
#

RESOURCES += qgroundcontrol.qrc

TRANSLATIONS += \
    es-MX.ts \
    en-US.ts

DEPENDPATH += \
    . \
    plugins

INCLUDEPATH += .

INCLUDEPATH += \
    src \
    src/ui \
    src/ui/linechart \
    src/ui/uas \
    src/ui/map \
    src/uas \
    src/comm \
    include/ui \
    src/input \
    src/lib/qmapcontrol \
    src/ui/mavlink \
    src/ui/param \
    src/ui/watchdog \
    src/ui/map3D \
    src/ui/mission \
    src/ui/designer \
    src/ui/configuration \
    src/output

FORMS += \
    src/ui/MainWindow.ui \
    src/ui/CommSettings.ui \
    src/ui/SerialSettings.ui \
    src/ui/UASControl.ui \
    src/ui/UASList.ui \
    src/ui/UASInfo.ui \
    src/ui/UASView.ui \
    src/ui/ParameterInterface.ui \
    src/ui/WaypointList.ui \
    src/ui/ObjectDetectionView.ui \
    src/ui/JoystickWidget.ui \
    src/ui/HDDisplay.ui \
    src/ui/MAVLinkSettingsWidget.ui \
    src/ui/AudioOutputWidget.ui \
    src/ui/QGCSensorSettingsWidget.ui \
    src/ui/watchdog/WatchdogControl.ui \
    src/ui/watchdog/WatchdogProcessView.ui \
    src/ui/watchdog/WatchdogView.ui \
    src/ui/QGCFirmwareUpdate.ui \
    src/ui/QGCPxImuFirmwareUpdate.ui \
    src/ui/QMap3D.ui \
    src/ui/SlugsDataSensorView.ui \
    src/ui/SlugsHilSim.ui \
    src/ui/SlugsPadCameraControl.ui \
    src/ui/uas/QGCUnconnectedInfoWidget.ui \
    src/ui/designer/QGCToolWidget.ui \
    src/ui/designer/QGCParamSlider.ui \
    src/ui/designer/QGCActionButton.ui \
    src/ui/designer/QGCCommandButton.ui \
    src/ui/QGCMAVLinkLogPlayer.ui \
    src/ui/QGCWaypointListMulti.ui \
    src/ui/QGCUDPLinkConfiguration.ui \
    src/ui/QGCUDPClientLinkConfiguration.ui \
    src/ui/QGCTCPLinkConfiguration.ui \
    src/ui/QGCSettingsWidget.ui \
    src/ui/map/QGCMapTool.ui \
    src/ui/map/QGCMapToolBar.ui \
    src/ui/QGCMAVLinkInspector.ui \
    src/ui/WaypointViewOnlyView.ui \
    src/ui/WaypointEditableView.ui \
    src/ui/UnconnectedUASInfoWidget.ui \
    src/ui/mavlink/QGCMAVLinkMessageSender.ui \
    src/ui/firmwareupdate/QGCFirmwareUpdateWidget.ui \
    src/ui/QGCPluginHost.ui \
    src/ui/firmwareupdate/QGCPX4FirmwareUpdate.ui \
    src/ui/mission/QGCMissionOther.ui \
    src/ui/mission/QGCMissionNavWaypoint.ui \
    src/ui/mission/QGCMissionDoJump.ui \
    src/ui/mission/QGCMissionDoSetServo.ui \
    src/ui/mission/QGCMissionDoRepeatServo.ui \
    src/ui/mission/QGCMissionDoDigicamControl.ui \
    src/ui/mission/QGCMissionDoChangeSpeed.ui \
    src/ui/mission/QGCMissionDoSetCamTriggDist.ui \
    src/ui/mission/QGCMissionDoSetHome.ui \
    src/ui/mission/QGCMissionDoSetRelay.ui \
    src/ui/mission/QGCMissionDoSetROI.ui \
    src/ui/mission/QGCMissionDoMountControl.ui \
    src/ui/mission/QGCMissionDoRepeatRelay.ui \
    src/ui/mission/QGCMissionConditionDelay.ui \
    src/ui/mission/QGCMissionConditionYaw.ui \
    src/ui/mission/QGCMissionConditionDistance.ui \
    src/ui/mission/QGCMissionNavLoiterUnlim.ui \
    src/ui/mission/QGCMissionNavLoiterTurns.ui \
    src/ui/mission/QGCMissionNavLoiterTime.ui \
    src/ui/mission/QGCMissionNavLoiterToAlt.ui \
    src/ui/mission/QGCMissionNavReturnToLaunch.ui \
    src/ui/mission/QGCMissionNavLand.ui \
    src/ui/mission/QGCMissionNavTakeoff.ui \
    src/ui/mission/QGCMissionNavSweep.ui \
    src/ui/mission/QGCMissionNavContinueChangeAlt.ui \
    src/ui/mission/QGCMissionDoStartSearch.ui \
    src/ui/mission/QGCMissionDoFinishSearch.ui \
    src/ui/QGCVehicleConfig.ui \
    src/ui/QGCHilConfiguration.ui \
    src/ui/QGCHilFlightGearConfiguration.ui \
    src/ui/QGCHilJSBSimConfiguration.ui \
#    src/ui/QGCHilXPlaneConfiguration.ui \
    src/ui/designer/QGCComboBox.ui \
    src/ui/designer/QGCTextLabel.ui \
    src/ui/uas/UASQuickView.ui \
    src/ui/uas/UASQuickViewItemSelect.ui \
    src/ui/uas/UASActionsWidget.ui \
    src/ui/QGCTabbedInfoView.ui \
    src/ui/UASRawStatusView.ui \
    src/ui/uas/QGCMessageView.ui \
    src/ui/configuration/ApmHardwareConfig.ui \
    src/ui/configuration/ApmSoftwareConfig.ui \
    src/ui/configuration/FrameTypeConfig.ui \
    src/ui/configuration/CompassConfig.ui \
    src/ui/configuration/AccelCalibrationConfig.ui \
    src/ui/configuration/RadioCalibrationConfig.ui \
    src/ui/configuration/FlightModeConfig.ui \
    src/ui/configuration/Radio3DRConfig.ui \
    src/ui/configuration/BatteryMonitorConfig.ui \
    src/ui/configuration/RangeFinderConfig.ui \
    src/ui/configuration/AirspeedConfig.ui \
    src/ui/configuration/OpticalFlowConfig.ui \
    src/ui/configuration/OsdConfig.ui \
    src/ui/configuration/AntennaTrackerConfig.ui \
    src/ui/configuration/CameraGimbalConfig.ui \
    src/ui/configuration/BasicPidConfig.ui \
    src/ui/configuration/StandardParamConfig.ui \
    src/ui/configuration/GeoFenceConfig.ui \
    src/ui/configuration/FailSafeConfig.ui \
    src/ui/configuration/AdvancedParamConfig.ui \
    src/ui/configuration/ArduCopterPidConfig.ui \
    src/ui/configuration/CopterPidConfig.ui \
    src/ui/configuration/ApmPlaneLevel.ui \
    src/ui/configuration/ParamWidget.ui \
    src/ui/configuration/ArduPlanePidConfig.ui \
    src/ui/configuration/AdvParameterList.ui \
    src/ui/configuration/ArduRoverPidConfig.ui \
    src/ui/configuration/TerminalConsole.ui \
    src/ui/configuration/LogConsole.ui \
    src/ui/configuration/SerialSettingsDialog.ui \
    src/ui/configuration/ApmFirmwareConfig.ui \
    src/ui/DebugOutput.ui \
    src/ui/configuration/SetupWarningMessage.ui \
    src/ui/uas/APMShortcutModesDialog.ui \
    src/ui/configuration/DownloadRemoteParamsDialog.ui \
    src/ui/configuration/ParamCompareDialog.ui \
    src/ui/AP2DataPlot2D.ui \
    src/ui/dataselectionscreen.ui \
    src/ui/AboutDialog.ui \
    src/ui/AP2DataPlotAxisDialog.ui \
    src/ui/AutoUpdateDialog.ui \
    src/uas/LogDownloadDialog.ui \
    src/ui/configuration/CompassMotorCalibrationDialog.ui \
    src/ui/MissionElevationDisplay.ui \
    src/ui/DroneshareUploadDialog.ui \
    src/ui/DroneshareDialog.ui \
    src/ui/uas/PreFlightCalibrationDialog.ui \
    src/ui/configuration/RadioFlashWizard.ui \
    src/ui/FireFlameReco.ui \
    src/ui/HorizonSettings.ui \
    src/ui/FireHeatReco.ui \
    src/ui/FireSmokeReco.ui \
    src/ui/VideoCapt.ui

HEADERS += \
    src/MG.h \
    src/QGCCore.h \
    src/uas/UASInterface.h \
    src/uas/UAS.h \
    src/uas/UASManager.h \
    src/comm/LinkManager.h \
    src/comm/LinkInterface.h \
    src/comm/SerialLinkInterface.h \
    src/comm/ProtocolInterface.h \
    src/comm/QGCFlightGearLink.h \
    src/comm/QGCJSBSimLink.h \
#    src/comm/QGCXPlaneLink.h \
    src/comm/serialconnection.h \
    src/ui/CommConfigurationWindow.h \
    src/ui/SerialConfigurationWindow.h \
    src/ui/MainWindow.h \
    src/ui/uas/UASControlWidget.h \
    src/ui/uas/UASListWidget.h \
    src/ui/uas/UASInfoWidget.h \
    src/ui/HUD.h \
    src/configuration.h \
    src/ui/uas/UASView.h \
    src/comm/MAVLinkSimulationLink.h \
    src/comm/UDPLink.h \
    src/comm/UDPClientLink.h \
    src/comm/TCPLink.h \
    src/ui/ParameterInterface.h \
    src/ui/WaypointList.h \
    src/ui/WaypointNavigation.h \
    src/Waypoint.h \
    src/ui/ObjectDetectionView.h \
    src/input/JoystickInput.h \
    src/ui/JoystickWidget.h \
    src/ui/HDDisplay.h \
    src/ui/MAVLinkSettingsWidget.h \
    src/ui/AudioOutputWidget.h \
    src/GAudioOutput.h \
    src/LogCompressor.h \
    src/ui/QGCParamWidget.h \
    src/ui/QGCSensorSettingsWidget.h \
    src/uas/SlugsMAV.h \
    src/uas/PxQuadMAV.h \
    src/uas/ArduPilotMegaMAV.h \
    src/uas/senseSoarMAV.h \
    src/ui/watchdog/WatchdogControl.h \
    src/ui/watchdog/WatchdogProcessView.h \
    src/ui/watchdog/WatchdogView.h \
    src/uas/UASWaypointManager.h \
    src/ui/HSIDisplay.h \
    src/QGC.h \
    src/ui/QGCFirmwareUpdate.h \
    src/ui/QGCPxImuFirmwareUpdate.h \
    src/ui/RadioCalibration/RadioCalibrationData.h \
    src/comm/QGCMAVLink.h \
    src/ui/SlugsDataSensorView.h \
    src/ui/SlugsHilSim.h \
    src/ui/SlugsPadCameraControl.h \
    src/ui/QGCMainWindowAPConfigurator.h \
    src/comm/MAVLinkSwarmSimulationLink.h \
    src/ui/uas/QGCUnconnectedInfoWidget.h \
    src/ui/designer/QGCToolWidget.h \
    src/ui/designer/QGCParamSlider.h \
    src/ui/designer/QGCCommandButton.h \
    src/ui/designer/QGCToolWidgetItem.h \
    src/ui/QGCMAVLinkLogPlayer.h \
    src/comm/MAVLinkSimulationWaypointPlanner.h \
    src/comm/MAVLinkSimulationMAV.h \
    src/uas/QGCMAVLinkUASFactory.h \
    src/ui/QGCWaypointListMulti.h \
    src/ui/QGCUDPLinkConfiguration.h \
    src/ui/QGCUDPClientLinkConfiguration.h \
    src/ui/QGCTCPLinkConfiguration.h \
    src/ui/QGCSettingsWidget.h \
    src/uas/QGCUASParamManager.h \
    src/ui/map/QGCMapWidget.h \
    src/ui/map/MAV2DIcon.h \
    src/ui/map/Waypoint2DIcon.h \
    src/ui/map/QGCMapTool.h \
    src/ui/map/QGCMapToolBar.h \
    src/QGCGeo.h \
    src/ui/QGCToolBar.h \
    src/ui/QGCStatusBar.h \
    src/ui/QGCMAVLinkInspector.h \
    src/ui/WaypointViewOnlyView.h \
    src/ui/WaypointEditableView.h \
    src/ui/UnconnectedUASInfoWidget.h \
    src/ui/QGCRGBDView.h \
    src/ui/mavlink/QGCMAVLinkMessageSender.h \
    src/ui/firmwareupdate/QGCFirmwareUpdateWidget.h \
    src/ui/QGCPluginHost.h \
    src/ui/firmwareupdate/QGCPX4FirmwareUpdate.h \
    src/ui/mission/QGCMissionOther.h \
    src/ui/mission/QGCMissionNavWaypoint.h \
    src/ui/mission/QGCMissionDoJump.h \
    src/ui/mission/QGCMissionDoSetServo.h \
    src/ui/mission/QGCMissionDoRepeatServo.h \
    src/ui/mission/QGCMissionDoDigicamControl.h \
    src/ui/mission/QGCMissionDoChangeSpeed.h \
    src/ui/mission/QGCMissionDoSetCamTriggDist.h \
    src/ui/mission/QGCMissionDoSetHome.h \
    src/ui/mission/QGCMissionDoSetRelay.h \
    src/ui/mission/QGCMissionDoSetROI.h \
    src/ui/mission/QGCMissionDoMountControl.h \
    src/ui/mission/QGCMissionDoRepeatRelay.h \
    src/ui/mission/QGCMissionConditionDelay.h \
    src/ui/mission/QGCMissionConditionYaw.h \
    src/ui/mission/QGCMissionConditionDistance.h \
    src/ui/mission/QGCMissionNavLoiterUnlim.h \
    src/ui/mission/QGCMissionNavLoiterTurns.h \
    src/ui/mission/QGCMissionNavLoiterTime.h \
    src/ui/mission/QGCMissionNavLoiterToAlt.h \
    src/ui/mission/QGCMissionNavReturnToLaunch.h \
    src/ui/mission/QGCMissionNavLand.h \
    src/ui/mission/QGCMissionNavTakeoff.h \
    src/ui/mission/QGCMissionNavSweep.h \
    src/ui/mission/QGCMissionNavContinueChangeAlt.h \
    src/ui/mission/QGCMissionDoStartSearch.h \
    src/ui/mission/QGCMissionDoFinishSearch.h \
    src/ui/QGCVehicleConfig.h \
    src/comm/QGCHilLink.h \
    src/ui/QGCHilConfiguration.h \
    src/ui/QGCHilFlightGearConfiguration.h \
    src/ui/QGCHilJSBSimConfiguration.h \
#    src/ui/QGCHilXPlaneConfiguration.h \
    src/ui/designer/QGCComboBox.h \
    src/ui/designer/QGCTextLabel.h \
    src/ui/submainwindow.h \
    src/ui/dockwidgettitlebareventfilter.h \
    src/ui/uas/UASQuickView.h \
    src/ui/uas/UASQuickViewItem.h \
    src/ui/uas/UASQuickViewItemSelect.h \
    src/ui/uas/UASQuickViewTextItem.h \
    src/ui/uas/UASQuickViewGaugeItem.h \
    src/ui/uas/UASActionsWidget.h \
    src/ui/designer/QGCRadioChannelDisplay.h \
    src/ui/QGCTabbedInfoView.h \
    src/ui/UASRawStatusView.h \
    src/ui/PrimaryFlightDisplay.h \
    src/ui/uas/QGCMessageView.h \
    src/ui/configuration/ApmHardwareConfig.h \
    src/ui/configuration/ApmSoftwareConfig.h \
    src/ui/configuration/FrameTypeConfig.h \
    src/ui/configuration/CompassConfig.h \
    src/ui/configuration/AccelCalibrationConfig.h \
    src/ui/configuration/RadioCalibrationConfig.h \
    src/ui/configuration/FlightModeConfig.h \
    src/ui/configuration/Radio3DRConfig.h \
    src/ui/configuration/BatteryMonitorConfig.h \
    src/ui/configuration/RangeFinderConfig.h \
    src/ui/configuration/AirspeedConfig.h \
    src/ui/configuration/OpticalFlowConfig.h \
    src/ui/configuration/OsdConfig.h \
    src/ui/configuration/AntennaTrackerConfig.h \
    src/ui/configuration/CameraGimbalConfig.h \
    src/ui/configuration/AP2ConfigWidget.h \
    src/ui/configuration/BasicPidConfig.h \
    src/ui/configuration/StandardParamConfig.h \
    src/ui/configuration/GeoFenceConfig.h \
    src/ui/configuration/FailSafeConfig.h \
    src/ui/configuration/AdvancedParamConfig.h \
    src/ui/configuration/ArduCopterPidConfig.h \
    src/ui/configuration/CopterPidConfig.h \
    src/ui/ApmToolBar.h \
    src/ui/configuration/PX4FirmwareUploader.h \
    src/ui/configuration/ApmPlaneLevel.h \
    src/ui/configuration/ParamWidget.h \
    src/ui/configuration/ArduPlanePidConfig.h \
    src/ui/configuration/AdvParameterList.h \
    src/ui/configuration/ArduRoverPidConfig.h \
    src/ui/configuration/Console.h \
    src/ui/configuration/SerialSettingsDialog.h \
    src/ui/configuration/TerminalConsole.h \
    src/ui/configuration/LogConsole.h \
    src/ui/configuration/ApmHighlighter.h \
    src/ui/configuration/ApmFirmwareConfig.h \
    src/ui/designer/QGCMouseWheelEventFilter.h \
    src/ui/DebugOutput.h \
    src/ui/configuration/APDoubleSpinBox.h \
    src/ui/configuration/APSpinBox.h \
    src/ui/configuration/Radio3DRSettings.h \
    src/ui/configuration/SetupWarningMessage.h \
    src/ui/uas/APMShortcutModesDialog.h \
    src/ui/configuration/DownloadRemoteParamsDialog.h \
    src/ui/configuration/ParamCompareDialog.h \
    src/uas/UASParameter.h \
    src/output/kmlcreator.h \
    src/output/logdata.h \
    src/ui/AP2DataPlot2D.h \
    src/ui/AP2DataPlotThread.h \
    src/ui/dataselectionscreen.h \
    src/ui/qcustomplot.h \
    src/globalobject.h \
    src/ui/AboutDialog.h \
    src/ui/uas/UASQuickViewTextLabel.h \
    src/uas/ApmUiHelpers.h \
    src/ui/AP2DataPlotAxisDialog.h \
    src/comm/arduino_intelhex.h \
    src/comm/arduinoflash.h \
    src/audio/AlsaAudio.h \
    src/ui/AutoUpdateCheck.h \
    src/ui/AutoUpdateDialog.h \
    src/uas/LogDownloadDialog.h \
    src/comm/TLogReplayLink.h \
    src/ui/PrimaryFlightDisplayQML.h \
    src/ui/configuration/CompassMotorCalibrationDialog.h \
    src/comm/MAVLinkDecoder.h \
    src/comm/MAVLinkProtocol.h \
    src/ui/MissionElevationDisplay.h \
    src/ui/GoogleElevationData.h \
    src/ui/DroneshareUploadDialog.h \
    src/ui/DroneshareUpload.h \
    src/ui/DroneshareDialog.h \
    src/ui/LoginDialog.h \
    src/ui/DroneshareAPIBroker.h \
    src/comm/UASObject.h \
    src/comm/VehicleOverview.h \
    src/comm/RelPositionOverview.h \
    src/comm/AbsPositionOverview.h \
    src/comm/MissionOverview.h \
    src/ui/AP2DataPlot2DModel.h \
    src/ui/uas/PreFlightCalibrationDialog.h \
    src/ui/configuration/RadioFlashWizard.h \
    src/ui/GraphTreeWidgetItem.h \
    src/comm/LinkManagerFactory.h \
    src/ui/VibrationMonitor.h \
    src/ui/EKFMonitor.h \
    src/Settings.h \
    src/ImgRecoTool.h \
    src/ui/FireFlameReco.h \
    src/EventsDetection.h \
    src/Event.h \
    src/Position.h \
    src/Horizon.h \
    src/EventFactory.h \
    src/ui/HorizonSettings.h \
    src/ui/FireHeatReco.h \
    src/ui/FireSmokeReco.h \
    src/ui/VideoCapt.h \
    libs/alglib/src/dataanalysis.h \
    libs/alglib/src/fasttransforms.h \
    libs/alglib/src/specialfunctions.h \
    libs/alglib/src/statistics.h \
    libs/alglib/src/stdafx.h \
    libs/lib/Frameworks/SDL2.framework/Headers/begin_code.h \
    libs/lib/Frameworks/SDL2.framework/Headers/close_code.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_assert.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_atomic.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_audio.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_bits.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_blendmode.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_clipboard.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_config.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_config_macosx.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_copying.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_cpuinfo.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_endian.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_error.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_events.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_filesystem.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_gamecontroller.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_gesture.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_haptic.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_hints.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_joystick.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_keyboard.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_keycode.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_loadso.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_log.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_main.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_messagebox.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_mouse.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_mutex.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_name.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_opengl.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_opengles.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_opengles2.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_pixels.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_platform.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_power.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_quit.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_rect.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_render.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_revision.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_rwops.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_scancode.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_shape.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_stdinc.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_surface.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_system.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_syswm.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_thread.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_timer.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_touch.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_types.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_version.h \
    libs/lib/Frameworks/SDL2.framework/Headers/SDL_video.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/begin_code.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/close_code.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_assert.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_atomic.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_audio.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_bits.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_blendmode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_clipboard.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_config.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_config_macosx.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_copying.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_cpuinfo.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_endian.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_error.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_events.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_filesystem.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_gamecontroller.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_gesture.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_haptic.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_hints.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_joystick.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_keyboard.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_keycode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_loadso.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_log.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_main.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_messagebox.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_mouse.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_mutex.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_name.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_opengl.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_opengles.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_opengles2.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_pixels.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_platform.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_power.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_quit.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_rect.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_render.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_revision.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_rwops.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_scancode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_shape.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_stdinc.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_surface.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_system.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_syswm.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_thread.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_timer.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_touch.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_types.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_version.h \
    libs/lib/Frameworks/SDL2.framework/Versions/A/Headers/SDL_video.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/begin_code.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/close_code.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_assert.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_atomic.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_audio.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_bits.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_blendmode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_clipboard.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_config.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_config_macosx.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_copying.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_cpuinfo.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_endian.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_error.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_events.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_filesystem.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_gamecontroller.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_gesture.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_haptic.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_hints.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_joystick.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_keyboard.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_keycode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_loadso.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_log.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_main.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_messagebox.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_mouse.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_mutex.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_name.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_opengl.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_opengles.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_opengles2.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_pixels.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_platform.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_power.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_quit.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_rect.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_render.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_revision.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_rwops.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_scancode.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_shape.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_stdinc.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_surface.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_system.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_syswm.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_thread.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_timer.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_touch.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_types.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_version.h \
    libs/lib/Frameworks/SDL2.framework/Versions/Current/Headers/SDL_video.h \
    libs/lib/msinttypes/inttypes.h \
    libs/lib/msinttypes/stdint.h \
    libs/lib/sdl/include/SDL2/begin_code.h \
    libs/lib/sdl/include/SDL2/close_code.h \
    libs/lib/sdl/include/SDL2/SDL.h \
    libs/lib/sdl/include/SDL2/SDL_assert.h \
    libs/lib/sdl/include/SDL2/SDL_atomic.h \
    libs/lib/sdl/include/SDL2/SDL_audio.h \
    libs/lib/sdl/include/SDL2/SDL_bits.h \
    libs/lib/sdl/include/SDL2/SDL_blendmode.h \
    libs/lib/sdl/include/SDL2/SDL_clipboard.h \
    libs/lib/sdl/include/SDL2/SDL_config.h \
    libs/lib/sdl/include/SDL2/SDL_cpuinfo.h \
    libs/lib/sdl/include/SDL2/SDL_endian.h \
    libs/lib/sdl/include/SDL2/SDL_error.h \
    libs/lib/sdl/include/SDL2/SDL_events.h \
    libs/lib/sdl/include/SDL2/SDL_filesystem.h \
    libs/lib/sdl/include/SDL2/SDL_gamecontroller.h \
    libs/lib/sdl/include/SDL2/SDL_gesture.h \
    libs/lib/sdl/include/SDL2/SDL_haptic.h \
    libs/lib/sdl/include/SDL2/SDL_hints.h \
    libs/lib/sdl/include/SDL2/SDL_joystick.h \
    libs/lib/sdl/include/SDL2/SDL_keyboard.h \
    libs/lib/sdl/include/SDL2/SDL_keycode.h \
    libs/lib/sdl/include/SDL2/SDL_loadso.h \
    libs/lib/sdl/include/SDL2/SDL_log.h \
    libs/lib/sdl/include/SDL2/SDL_main.h \
    libs/lib/sdl/include/SDL2/SDL_messagebox.h \
    libs/lib/sdl/include/SDL2/SDL_mouse.h \
    libs/lib/sdl/include/SDL2/SDL_mutex.h \
    libs/lib/sdl/include/SDL2/SDL_name.h \
    libs/lib/sdl/include/SDL2/SDL_opengl.h \
    libs/lib/sdl/include/SDL2/SDL_opengles.h \
    libs/lib/sdl/include/SDL2/SDL_opengles2.h \
    libs/lib/sdl/include/SDL2/SDL_pixels.h \
    libs/lib/sdl/include/SDL2/SDL_platform.h \
    libs/lib/sdl/include/SDL2/SDL_power.h \
    libs/lib/sdl/include/SDL2/SDL_quit.h \
    libs/lib/sdl/include/SDL2/SDL_rect.h \
    libs/lib/sdl/include/SDL2/SDL_render.h \
    libs/lib/sdl/include/SDL2/SDL_revision.h \
    libs/lib/sdl/include/SDL2/SDL_rwops.h \
    libs/lib/sdl/include/SDL2/SDL_scancode.h \
    libs/lib/sdl/include/SDL2/SDL_shape.h \
    libs/lib/sdl/include/SDL2/SDL_stdinc.h \
    libs/lib/sdl/include/SDL2/SDL_surface.h \
    libs/lib/sdl/include/SDL2/SDL_system.h \
    libs/lib/sdl/include/SDL2/SDL_syswm.h \
    libs/lib/sdl/include/SDL2/SDL_test.h \
    libs/lib/sdl/include/SDL2/SDL_test_assert.h \
    libs/lib/sdl/include/SDL2/SDL_test_common.h \
    libs/lib/sdl/include/SDL2/SDL_test_compare.h \
    libs/lib/sdl/include/SDL2/SDL_test_crc32.h \
    libs/lib/sdl/include/SDL2/SDL_test_font.h \
    libs/lib/sdl/include/SDL2/SDL_test_fuzzer.h \
    libs/lib/sdl/include/SDL2/SDL_test_harness.h \
    libs/lib/sdl/include/SDL2/SDL_test_images.h \
    libs/lib/sdl/include/SDL2/SDL_test_log.h \
    libs/lib/sdl/include/SDL2/SDL_test_md5.h \
    libs/lib/sdl/include/SDL2/SDL_test_random.h \
    libs/lib/sdl/include/SDL2/SDL_thread.h \
    libs/lib/sdl/include/SDL2/SDL_timer.h \
    libs/lib/sdl/include/SDL2/SDL_touch.h \
    libs/lib/sdl/include/SDL2/SDL_types.h \
    libs/lib/sdl/include/SDL2/SDL_version.h \
    libs/lib/sdl/include/SDL2/SDL_video.h \
    libs/lib/sdl/msvc/include/SDL2/begin_code.h \
    libs/lib/sdl/msvc/include/SDL2/close_code.h \
    libs/lib/sdl/msvc/include/SDL2/SDL.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_assert.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_atomic.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_audio.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_bits.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_blendmode.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_clipboard.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_config.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_cpuinfo.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_endian.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_error.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_events.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_filesystem.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_gamecontroller.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_gesture.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_haptic.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_hints.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_joystick.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_keyboard.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_keycode.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_loadso.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_log.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_main.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_messagebox.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_mouse.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_mutex.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_name.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_opengl.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_opengles.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_opengles2.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_pixels.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_platform.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_power.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_quit.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_rect.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_render.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_revision.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_rwops.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_scancode.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_shape.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_stdinc.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_surface.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_system.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_syswm.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_assert.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_common.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_compare.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_crc32.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_font.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_fuzzer.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_harness.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_images.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_log.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_md5.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_test_random.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_thread.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_timer.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_touch.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_types.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_version.h \
    libs/lib/sdl/msvc/include/SDL2/SDL_video.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/ardupilotmega.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_ahrs.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_ahrs2.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_ahrs3.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_airspeed_autocal.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_ap_adc.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_autopilot_version_request.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_battery2.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_camera_feedback.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_camera_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_compassmot_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_data16.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_data32.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_data64.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_data96.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_digicam_configure.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_digicam_control.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_ekf_status_report.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_fence_fetch_point.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_fence_point.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_fence_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_axis_calibration_progress.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_control.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_erase_firmware_and_config.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_factory_parameters_loaded.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_home_offset_calibration_result.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_perform_factory_tests.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_report.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_report_axis_calibration_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_report_factory_tests_progress.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_request_axis_calibration.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_request_axis_calibration_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_reset.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_set_factory_parameters.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gimbal_set_home_offsets.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gopro_get_request.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gopro_get_response.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gopro_heartbeat.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gopro_set_request.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_gopro_set_response.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_hwstatus.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_led_control.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_limits_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_mag_cal_progress.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_mag_cal_report.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_meminfo.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_mount_configure.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_mount_control.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_mount_status.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_pid_tuning.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_radio.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_rally_fetch_point.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_rally_point.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_rangefinder.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_sensor_offsets.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_set_mag_offsets.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_simstate.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/mavlink_msg_wind.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/testsuite.h \
    libs/mavlink/include/mavlink/v1.0/ardupilotmega/version.h \
    libs/mavlink/include/mavlink/v1.0/common/common.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_actuator_control_target.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_att_pos_mocap.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_attitude.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_attitude_quaternion.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_attitude_quaternion_cov.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_attitude_target.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_auth_key.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_autopilot_version.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_battery_status.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_change_operator_control.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_change_operator_control_ack.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_command_ack.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_command_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_command_long.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_data_stream.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_data_transmission_handshake.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_debug.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_debug_vect.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_distance_sensor.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_encapsulated_data.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_file_transfer_protocol.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_global_position_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_global_position_int_cov.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_global_vision_position_estimate.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps2_raw.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps2_rtk.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps_global_origin.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps_inject_data.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps_raw_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps_rtk.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_gps_status.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_heartbeat.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_highres_imu.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_controls.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_gps.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_optical_flow.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_rc_inputs_raw.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_sensor.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_state.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_hil_state_quaternion.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_landing_target.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_local_position_ned.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_local_position_ned_cov.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_local_position_ned_system_global_offset.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_data.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_entry.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_erase.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_request_data.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_request_end.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_log_request_list.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_manual_control.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_manual_setpoint.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_memory_vect.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_ack.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_clear_all.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_count.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_current.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_item.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_item_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_item_reached.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_request.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_request_list.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_request_partial_list.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_set_current.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_mission_write_partial_list.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_named_value_float.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_named_value_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_nav_controller_output.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_optical_flow.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_optical_flow_rad.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_param_map_rc.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_param_request_list.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_param_request_read.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_param_set.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_param_value.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_ping.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_position_target_global_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_position_target_local_ned.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_power_status.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_radio_status.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_raw_imu.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_raw_pressure.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_rc_channels.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_rc_channels_override.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_rc_channels_raw.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_rc_channels_scaled.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_request_data_stream.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_safety_allowed_area.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_safety_set_allowed_area.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_scaled_imu.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_scaled_imu2.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_scaled_imu3.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_scaled_pressure.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_scaled_pressure2.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_serial_control.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_servo_output_raw.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_actuator_control_target.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_attitude_target.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_gps_global_origin.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_mode.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_position_target_global_int.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_set_position_target_local_ned.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_sim_state.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_statustext.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_sys_status.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_system_time.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_terrain_check.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_terrain_data.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_terrain_report.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_terrain_request.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_timesync.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_v2_extension.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_vfr_hud.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_vibration.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_vicon_position_estimate.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_vision_position_estimate.h \
    libs/mavlink/include/mavlink/v1.0/common/mavlink_msg_vision_speed_estimate.h \
    libs/mavlink/include/mavlink/v1.0/common/testsuite.h \
    libs/mavlink/include/mavlink/v1.0/common/version.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/matrixpilot.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_airspeeds.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_altitudes.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_buffer_function.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_buffer_function_ack.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_command.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_command_ack.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_directory.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_directory_ack.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_read_req.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_flexifunction_set.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f13.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f14.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f15.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f16.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f2_a.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f2_b.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f4.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f5.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f6.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f7.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/mavlink_msg_serial_udb_extra_f8.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/testsuite.h \
    libs/mavlink/include/mavlink/v1.0/matrixpilot/version.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_attitude_control.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_brief_feature.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_data_transmission_handshake.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_encapsulated_data.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_image_available.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_image_trigger_control.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_image_triggered.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_marker.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_pattern_detected.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_point_of_interest.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_point_of_interest_connection.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_position_control_setpoint.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_raw_aux.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_set_cam_shutter.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_set_position_control_offset.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_watchdog_command.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_watchdog_heartbeat.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_watchdog_process_info.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/mavlink_msg_watchdog_process_status.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/pixhawk.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/testsuite.h \
    libs/mavlink/include/mavlink/v1.0/pixhawk/version.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_cmd_airspeed_ack.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_cmd_airspeed_chng.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_filt_rot_vel.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_llc_out.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_air_temp.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_air_velocity.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_attitude.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_bias.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_position.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_qff.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_velocity.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_obs_wind.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_pm_elec.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/mavlink_msg_sys_stat.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/sensesoar.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/testsuite.h \
    libs/mavlink/include/mavlink/v1.0/sensesoar/version.h \
    libs/mavlink/include/mavlink/v1.0/checksum.h \
    libs/mavlink/include/mavlink/v1.0/mavlink_conversions.h \
    libs/mavlink/include/mavlink/v1.0/mavlink_helpers.h \
    libs/mavlink/include/mavlink/v1.0/mavlink_protobuf_manager.hpp \
    libs/mavlink/include/mavlink/v1.0/mavlink_types.h \
    libs/mavlink/include/mavlink/v1.0/protocol.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/test/mavlink.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/test/mavlink_msg_test_types.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/test/test.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/test/testsuite.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/test/version.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/checksum.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/mavlink_helpers.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/mavlink_types.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v0.9/protocol.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/pixhawk/pixhawk.pb.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/test/mavlink.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/test/mavlink_msg_test_types.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/test/test.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/test/testsuite.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/test/version.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/checksum.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/mavlink_helpers.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/mavlink_protobuf_manager.hpp \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/mavlink_types.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/include_v1.0/protocol.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/test/windows/stdafx.h \
    libs/mavlink/share/pyshared/pymavlink/generator/C/test/windows/targetver.h \
    libs/qtconcurrent/multitask.h \
    libs/qtconcurrent/qtconcurrent_global.h \
    libs/qtconcurrent/runextensions.h \
    libs/qwt/qwt.h \
    libs/qwt/qwt_abstract_scale.h \
    libs/qwt/qwt_abstract_scale_draw.h \
    libs/qwt/qwt_abstract_slider.h \
    libs/qwt/qwt_analog_clock.h \
    libs/qwt/qwt_array.h \
    libs/qwt/qwt_arrow_button.h \
    libs/qwt/qwt_clipper.h \
    libs/qwt/qwt_color_map.h \
    libs/qwt/qwt_compass.h \
    libs/qwt/qwt_compass_rose.h \
    libs/qwt/qwt_counter.h \
    libs/qwt/qwt_curve_fitter.h \
    libs/qwt/qwt_data.h \
    libs/qwt/qwt_dial.h \
    libs/qwt/qwt_dial_needle.h \
    libs/qwt/qwt_double_interval.h \
    libs/qwt/qwt_double_range.h \
    libs/qwt/qwt_double_rect.h \
    libs/qwt/qwt_dyngrid_layout.h \
    libs/qwt/qwt_event_pattern.h \
    libs/qwt/qwt_global.h \
    libs/qwt/qwt_interval_data.h \
    libs/qwt/qwt_knob.h \
    libs/qwt/qwt_layout_metrics.h \
    libs/qwt/qwt_legend.h \
    libs/qwt/qwt_legend_item.h \
    libs/qwt/qwt_legend_itemmanager.h \
    libs/qwt/qwt_magnifier.h \
    libs/qwt/qwt_math.h \
    libs/qwt/qwt_paint_buffer.h \
    libs/qwt/qwt_painter.h \
    libs/qwt/qwt_panner.h \
    libs/qwt/qwt_picker.h \
    libs/qwt/qwt_picker_machine.h \
    libs/qwt/qwt_plot.h \
    libs/qwt/qwt_plot_canvas.h \
    libs/qwt/qwt_plot_curve.h \
    libs/qwt/qwt_plot_dict.h \
    libs/qwt/qwt_plot_grid.h \
    libs/qwt/qwt_plot_item.h \
    libs/qwt/qwt_plot_layout.h \
    libs/qwt/qwt_plot_magnifier.h \
    libs/qwt/qwt_plot_marker.h \
    libs/qwt/qwt_plot_panner.h \
    libs/qwt/qwt_plot_picker.h \
    libs/qwt/qwt_plot_printfilter.h \
    libs/qwt/qwt_plot_rasteritem.h \
    libs/qwt/qwt_plot_scaleitem.h \
    libs/qwt/qwt_plot_spectrogram.h \
    libs/qwt/qwt_plot_svgitem.h \
    libs/qwt/qwt_plot_zoomer.h \
    libs/qwt/qwt_polygon.h \
    libs/qwt/qwt_raster_data.h \
    libs/qwt/qwt_rect.h \
    libs/qwt/qwt_round_scale_draw.h \
    libs/qwt/qwt_scale_div.h \
    libs/qwt/qwt_scale_draw.h \
    libs/qwt/qwt_scale_engine.h \
    libs/qwt/qwt_scale_map.h \
    libs/qwt/qwt_scale_widget.h \
    libs/qwt/qwt_slider.h \
    libs/qwt/qwt_spline.h \
    libs/qwt/qwt_symbol.h \
    libs/qwt/qwt_text.h \
    libs/qwt/qwt_text_engine.h \
    libs/qwt/qwt_text_label.h \
    libs/qwt/qwt_thermo.h \
    libs/qwt/qwt_valuelist.h \
    libs/qwt/qwt_wheel.h \
    libs/thirdParty/libxbee/xsys/linux.h \
    libs/thirdParty/libxbee/xsys/win32.h \
    libs/thirdParty/libxbee/api.h \
    libs/thirdParty/libxbee/xbee.h \
    libs/thirdParty/quazip/quagzipfile.h \
    libs/thirdParty/quazip/quaziodevice.h \
    libs/thirdParty/quazip/quazip_global.h \
    libs/thirdParty/quazip/quazipdir.h \
    libs/utils/abstractprocess.h \
    libs/utils/basevalidatinglineedit.h \
    libs/utils/checkablemessagebox.h \
    libs/utils/classnamevalidatinglineedit.h \
    libs/utils/codegeneration.h \
    libs/utils/consoleprocess.h \
    libs/utils/coordinateconversions.h \
    libs/utils/detailsbutton.h \
    libs/utils/detailswidget.h \
    libs/utils/fancylineedit.h \
    libs/utils/fancymainwindow.h \
    libs/utils/filenamevalidatinglineedit.h \
    libs/utils/filesearch.h \
    libs/utils/filewizarddialog.h \
    libs/utils/filewizardpage.h \
    libs/utils/homelocationutil.h \
    libs/utils/iwelcomepage.h \
    libs/utils/linecolumnlabel.h \
    libs/utils/listutils.h \
    libs/utils/newclasswidget.h \
    libs/utils/parameteraction.h \
    libs/utils/pathchooser.h \
    libs/utils/pathlisteditor.h \
    libs/utils/pathutils.h \
    libs/utils/projectintropage.h \
    libs/utils/projectnamevalidatinglineedit.h \
    libs/utils/qtcassert.h \
    libs/utils/qtcolorbutton.h \
    libs/utils/qwineventnotifier_p.h \
    libs/utils/reloadpromptutils.h \
    libs/utils/savedaction.h \
    libs/utils/settingsutils.h \
    libs/utils/styledbar.h \
    libs/utils/stylehelper.h \
    libs/utils/submiteditorwidget.h \
    libs/utils/submitfieldwidget.h \
    libs/utils/synchronousprocess.h \
    libs/utils/treewidgetcolumnstretcher.h \
    libs/utils/uncommentselection.h \
    libs/utils/utils_global.h \
    libs/utils/welcomemodetreewidget.h \
    libs/utils/winutils.h \
    libs/utils/worldmagmodel.h \
    libs/utils/xmlconfig.h \
    QsLog/QsLogDest.h \
    src/apps/mavlinkgen/generator/MAVLinkXMLParser.h \
    src/apps/mavlinkgen/generator/MAVLinkXMLParserV10.h \
    src/apps/mavlinkgen/msinttypes/inttypes.h \
    src/apps/mavlinkgen/msinttypes/stdint.h \
    src/apps/mavlinkgen/template/checksum.h \
    src/apps/mavlinkgen/template/mavlink_checksum.h \
    src/apps/mavlinkgen/template/mavlink_data.h \
    src/apps/mavlinkgen/template/mavlink_options.h \
    src/apps/mavlinkgen/template/mavlink_protocol.h \
    src/apps/mavlinkgen/template/mavlink_types.h \
    src/apps/mavlinkgen/template/protocol.h \
    src/apps/mavlinkgen/ui/DomItem.h \
    src/apps/mavlinkgen/ui/DomModel.h \
    src/apps/mavlinkgen/ui/QGCMAVLinkTextEdit.h \
    src/apps/mavlinkgen/ui/XMLCommProtocolWidget.h \
    src/apps/mavlinkgen/MAVLinkGen.h \
    src/apps/qgcvideo/QGCVideoApp.h \
    src/apps/qgcvideo/QGCVideoMainWindow.h \
    src/apps/qgcvideo/QGCVideoWidget.h \
    src/apps/qupgrade/QUpgradeApp.h \
    src/apps/qupgrade/QUpgradeMainWindow.h \
    src/apps/qupgrade/uploader.h \
    src/comm/CallConv.h \
    src/comm/HexSpinBox.h \
    src/comm/QGCXPlaneLink.h \
    src/comm/SerialInterface.h \
    src/comm/XbeeLink.h \
    src/comm/XbeeLinkInterface.h \
    src/qgcunittest/AutoTest.h \
    src/qgcunittest/UASUnitTest.h \
    src/ui/generated/AudioOutputWidget.h \
    src/ui/generated/CommSettings.h \
    src/ui/generated/DebugConsole.h \
    src/ui/generated/HDDisplay.h \
    src/ui/generated/JoystickWidget.h \
    src/ui/generated/LineChart.h \
    src/ui/generated/MainWindow.h \
    src/ui/generated/MapWidget.h \
    src/ui/generated/MAVLinkSettingsWidget.h \
    src/ui/generated/ObjectDetectionView.h \
    src/ui/generated/ParameterInterface.h \
    src/ui/generated/QGCSensorSettingsWidget.h \
    src/ui/generated/SerialSettings.h \
    src/ui/generated/UASControl.h \
    src/ui/generated/UASInfo.h \
    src/ui/generated/UASList.h \
    src/ui/generated/UASView.h \
    src/ui/generated/WatchdogControl.h \
    src/ui/generated/WatchdogProcessView.h \
    src/ui/generated/WatchdogView.h \
    src/ui/generated/WaypointList.h \
    src/ui/generated/WaypointView.h \
    src/ui/generated/XMLCommProtocolWidget.h \
    src/ui/linechart/IncrementalPlot.h \
    src/ui/linechart/LinechartPlot.h \
    src/ui/linechart/Linecharts.h \
    src/ui/linechart/LinechartWidget.h \
    src/ui/linechart/Scrollbar.h \
    src/ui/linechart/ScrollZoomer.h \
    src/ui/map3D/QGCGlut.h \
    src/ui/map3D/QMap3D.h \
    src/ui/MapWidget.h \
    src/ui/OgreWidget.h \
    src/ui/PX4FirmwareUpgrader.h \
    src/ui/QGCHilXPlaneConfiguration.h \
    src/ui/QGCWelcomeWidget.h \
    src/ui/XbeeConfigurationWindow.h \
    src/PX4FirmwareUpgradeWorker.h

SOURCES += src/main.cc \
    src/QGCCore.cc \
    src/uas/UASManager.cc \
    src/uas/UAS.cc \
    src/comm/LinkManager.cc \
    src/comm/LinkInterface.cpp \
    src/comm/QGCFlightGearLink.cc \
    src/comm/QGCJSBSimLink.cc \
#    src/comm/QGCXPlaneLink.cc \
    src/comm/serialconnection.cc \
    src/ui/CommConfigurationWindow.cc \
    src/ui/SerialConfigurationWindow.cc \
    src/ui/MainWindow.cc \
    src/ui/uas/UASControlWidget.cc \
    src/ui/uas/UASListWidget.cc \
    src/ui/uas/UASInfoWidget.cc \
    src/ui/HUD.cc \
    src/ui/uas/UASView.cc \
    src/comm/MAVLinkSimulationLink.cc \
    src/comm/UDPLink.cc \
    src/comm/UDPClientLink.cc \
    src/comm/TCPLink.cc \
    src/ui/ParameterInterface.cc \
    src/ui/WaypointList.cc \
    src/ui/WaypointNavigation.cc \
    src/Waypoint.cc \
    src/ui/ObjectDetectionView.cc \
    src/input/JoystickInput.cc \
    src/ui/JoystickWidget.cc \
    src/ui/HDDisplay.cc \
    src/ui/MAVLinkSettingsWidget.cc \
    src/ui/AudioOutputWidget.cc \
    src/GAudioOutput.cc \
    src/LogCompressor.cc \
    src/ui/QGCParamWidget.cc \
    src/ui/QGCSensorSettingsWidget.cc \
    src/uas/SlugsMAV.cc \
    src/uas/PxQuadMAV.cc \
    src/uas/ArduPilotMegaMAV.cc \
    src/uas/senseSoarMAV.cpp \
    src/ui/watchdog/WatchdogControl.cc \
    src/ui/watchdog/WatchdogProcessView.cc \
    src/ui/watchdog/WatchdogView.cc \
    src/uas/UASWaypointManager.cc \
    src/ui/HSIDisplay.cc \
    src/QGC.cc \
    src/ui/QGCFirmwareUpdate.cc \
    src/ui/QGCPxImuFirmwareUpdate.cc \
    src/ui/RadioCalibration/RadioCalibrationData.cc \
    src/ui/SlugsDataSensorView.cc \
    src/ui/SlugsHilSim.cc \
    src/ui/SlugsPadCameraControl.cpp \
    src/ui/QGCMainWindowAPConfigurator.cc \
    src/comm/MAVLinkSwarmSimulationLink.cc \
    src/ui/uas/QGCUnconnectedInfoWidget.cc \
    src/ui/designer/QGCToolWidget.cc \
    src/ui/designer/QGCParamSlider.cc \
    src/ui/designer/QGCCommandButton.cc \
    src/ui/designer/QGCToolWidgetItem.cc \
    src/ui/QGCMAVLinkLogPlayer.cc \
    src/comm/MAVLinkSimulationWaypointPlanner.cc \
    src/comm/MAVLinkSimulationMAV.cc \
    src/uas/QGCMAVLinkUASFactory.cc \
    src/ui/QGCWaypointListMulti.cc \
    src/ui/QGCUDPLinkConfiguration.cc \
    src/ui/QGCUDPClientLinkConfiguration.cc \
    src/ui/QGCTCPLinkConfiguration.cc \
    src/ui/QGCSettingsWidget.cc \
    src/uas/QGCUASParamManager.cc \
    src/ui/map/QGCMapWidget.cc \
    src/ui/map/MAV2DIcon.cc \
    src/ui/map/Waypoint2DIcon.cc \
    src/ui/map/QGCMapTool.cc \
    src/ui/map/QGCMapToolBar.cc \
    src/QGCGeo.cc \
    src/ui/QGCToolBar.cc \
    src/ui/QGCStatusBar.cc \
    src/ui/QGCMAVLinkInspector.cc \
    src/ui/WaypointViewOnlyView.cc \
    src/ui/WaypointEditableView.cc \
    src/ui/UnconnectedUASInfoWidget.cc \
    src/ui/QGCRGBDView.cc \
    src/ui/mavlink/QGCMAVLinkMessageSender.cc \
    src/ui/firmwareupdate/QGCFirmwareUpdateWidget.cc \
    src/ui/QGCPluginHost.cc \
    src/ui/firmwareupdate/QGCPX4FirmwareUpdate.cc \
    src/ui/mission/QGCMissionOther.cc \
    src/ui/mission/QGCMissionNavWaypoint.cc \
    src/ui/mission/QGCMissionDoJump.cc \
    src/ui/mission/QGCMissionDoSetServo.cc \
    src/ui/mission/QGCMissionDoRepeatServo.cc \
    src/ui/mission/QGCMissionDoDigicamControl.cc \
    src/ui/mission/QGCMissionDoChangeSpeed.cc \
    src/ui/mission/QGCMissionDoSetCamTriggDist.cc \
    src/ui/mission/QGCMissionDoSetHome.cc \
    src/ui/mission/QGCMissionDoSetRelay.cc \
    src/ui/mission/QGCMissionDoSetROI.cc \
    src/ui/mission/QGCMissionDoMountControl.cc \
    src/ui/mission/QGCMissionDoRepeatRelay.cc \
    src/ui/mission/QGCMissionConditionDelay.cc \
    src/ui/mission/QGCMissionConditionYaw.cc \
    src/ui/mission/QGCMissionConditionDistance.cc \
    src/ui/mission/QGCMissionNavLoiterUnlim.cc \
    src/ui/mission/QGCMissionNavLoiterTurns.cc \
    src/ui/mission/QGCMissionNavLoiterTime.cc \
    src/ui/mission/QGCMissionNavLoiterToAlt.cc \
    src/ui/mission/QGCMissionNavReturnToLaunch.cc \
    src/ui/mission/QGCMissionNavLand.cc \
    src/ui/mission/QGCMissionNavTakeoff.cc \
    src/ui/mission/QGCMissionNavSweep.cc \
    src/ui/mission/QGCMissionNavContinueChangeAlt.cc \
    src/ui/mission/QGCMissionDoStartSearch.cc \
    src/ui/mission/QGCMissionDoFinishSearch.cc \
    src/ui/QGCVehicleConfig.cc \
    src/ui/QGCHilConfiguration.cc \
    src/ui/QGCHilFlightGearConfiguration.cc \
    src/ui/QGCHilJSBSimConfiguration.cc \
#    src/ui/QGCHilXPlaneConfiguration.cc \
    src/ui/designer/QGCComboBox.cc \
    src/ui/designer/QGCTextLabel.cc \
    src/ui/submainwindow.cpp \
    src/ui/dockwidgettitlebareventfilter.cpp \
    src/ui/uas/UASQuickViewItem.cc \
    src/ui/uas/UASQuickView.cc \
    src/ui/uas/UASQuickViewTextItem.cc \
    src/ui/uas/UASQuickViewGaugeItem.cc \
    src/ui/uas/UASQuickViewItemSelect.cc \
    src/ui/uas/UASActionsWidget.cpp \
    src/ui/designer/QGCRadioChannelDisplay.cpp \
    src/ui/QGCTabbedInfoView.cpp \
    src/ui/UASRawStatusView.cpp \
    src/ui/PrimaryFlightDisplay.cc \
    src/ui/uas/QGCMessageView.cc \
    src/ui/configuration/ApmHardwareConfig.cc \
    src/ui/configuration/ApmSoftwareConfig.cc \
    src/ui/configuration/FrameTypeConfig.cc \
    src/ui/configuration/CompassConfig.cc \
    src/ui/configuration/AccelCalibrationConfig.cc \
    src/ui/configuration/RadioCalibrationConfig.cc \
    src/ui/configuration/FlightModeConfig.cc \
    src/ui/configuration/Radio3DRConfig.cc \
    src/ui/configuration/BatteryMonitorConfig.cc \
    src/ui/configuration/RangeFinderConfig.cc \
    src/ui/configuration/AirspeedConfig.cc \
    src/ui/configuration/OpticalFlowConfig.cc \
    src/ui/configuration/OsdConfig.cc \
    src/ui/configuration/AntennaTrackerConfig.cc \
    src/ui/configuration/CameraGimbalConfig.cc \
    src/ui/configuration/AP2ConfigWidget.cc \
    src/ui/configuration/BasicPidConfig.cc \
    src/ui/configuration/StandardParamConfig.cc \
    src/ui/configuration/GeoFenceConfig.cc \
    src/ui/configuration/FailSafeConfig.cc \
    src/ui/configuration/AdvancedParamConfig.cc \
    src/ui/configuration/ArduCopterPidConfig.cc \
    src/ui/configuration/CopterPidConfig.cc \
    src/ui/ApmToolBar.cc \
    src/ui/configuration/PX4FirmwareUploader.cc \
    src/ui/configuration/ApmPlaneLevel.cc \
    src/ui/configuration/ParamWidget.cc \
    src/ui/configuration/ArduPlanePidConfig.cc \
    src/ui/configuration/AdvParameterList.cc \
    src/ui/configuration/ArduRoverPidConfig.cc \
    src/ui/configuration/TerminalConsole.cc \
    src/ui/configuration/LogConsole.cc \
    src/ui/configuration/Console.cc \
    src/ui/configuration/SerialSettingsDialog.cc \
    src/ui/configuration/ApmHighlighter.cc \
    src/ui/configuration/ApmFirmwareConfig.cc \
    src/ui/designer/QGCMouseWheelEventFilter.cc \
    src/ui/DebugOutput.cc \
    src/ui/configuration/APDoubleSpinBox.cc \
    src/ui/configuration/APSpinBox.cc \
    src/ui/configuration/Radio3DRSettings.cc \
    src/ui/configuration/SetupWarningMessage.cc \
    src/ui/uas/APMShortcutModesDialog.cpp \
    src/ui/configuration/DownloadRemoteParamsDialog.cc \
    src/ui/configuration/ParamCompareDialog.cpp \
    src/uas/UASParameter.cpp \
    src/output/kmlcreator.cc \
    src/output/logdata.cc \
    src/ui/AP2DataPlot2D.cpp \
    src/ui/AP2DataPlotThread.cc \
    src/ui/dataselectionscreen.cpp \
    src/ui/qcustomplot.cpp \
    src/globalobject.cc \
    src/ui/AboutDialog.cc \
    src/ui/uas/UASQuickViewTextLabel.cc \
    src/uas/ApmUiHelpers.cc \
    src/ui/AP2DataPlotAxisDialog.cc \
    src/comm/arduino_intelhex.cpp \
    src/comm/arduinoflash.cc \
    src/audio/AlsaAudio.cc \
    src/ui/AutoUpdateCheck.cc \
    src/ui/AutoUpdateDialog.cc \
    src/uas/LogDownloadDialog.cc \
    src/comm/TLogReplayLink.cc \
    src/ui/PrimaryFlightDisplayQML.cpp \
    src/ui/configuration/CompassMotorCalibrationDialog.cpp \
    src/comm/MAVLinkDecoder.cc \
    src/comm/MAVLinkProtocol.cc \
    src/ui/MissionElevationDisplay.cpp \
    src/ui/GoogleElevationData.cpp \
    src/ui/DroneshareUploadDialog.cpp \
    src/ui/DroneshareUpload.cpp \
    src/ui/DroneshareDialog.cc \
    src/ui/LoginDialog.cpp \
    src/ui/DroneshareAPIBroker.cpp \
    src/comm/UASObject.cc \
    src/comm/VehicleOverview.cc \
    src/comm/RelPositionOverview.cc \
    src/comm/AbsPositionOverview.cc \
    src/comm/MissionOverview.cc \
    src/ui/AP2DataPlot2DModel.cc \
    src/ui/uas/PreFlightCalibrationDialog.cpp \
    src/ui/configuration/RadioFlashWizard.cpp \
    src/ui/GraphTreeWidgetItem.cc \
    src/comm/LinkManagerFactory.cpp \
    src/ui/VibrationMonitor.cpp \
    src/ui/EKFMonitor.cpp \
    src/Settings.cpp \
    src/ImgRecoTool.cpp \
    src/ui/FireFlameReco.cc \
    src/EventsDetection.cpp \
    src/Event.cpp \
    src/Position.cpp \
    src/Horizon.cpp \
    src/ui/HorizonSettings.cc \
    src/ui/FireHeatReco.cpp \
    src/ui/FireSmokeReco.cc \
    src/ui/VideoCapt.cc \
    libs/alglib/src/dataanalysis.cpp \
    libs/alglib/src/fasttransforms.cpp \
    libs/alglib/src/statistics.cpp \
    libs/alglib/tests/test_c.cpp \
    libs/alglib/tests/test_i.cpp \
    libs/alglib/tests/test_x.cpp \
    libs/mavlink/share/pyshared/pymavlink/generator/C/src_v1.0/pixhawk/pixhawk.pb.cc \
    libs/mavlink/share/pyshared/pymavlink/generator/C/test/windows/stdafx.cpp \
    libs/mavlink/share/pyshared/pymavlink/generator/C/test/windows/testmav.cpp \
    libs/qwt/qwt_abstract_scale.cpp \
    libs/qwt/qwt_abstract_scale_draw.cpp \
    libs/qwt/qwt_abstract_slider.cpp \
    libs/qwt/qwt_analog_clock.cpp \
    libs/qwt/qwt_arrow_button.cpp \
    libs/qwt/qwt_clipper.cpp \
    libs/qwt/qwt_color_map.cpp \
    libs/qwt/qwt_compass.cpp \
    libs/qwt/qwt_compass_rose.cpp \
    libs/qwt/qwt_counter.cpp \
    libs/qwt/qwt_curve_fitter.cpp \
    libs/qwt/qwt_data.cpp \
    libs/qwt/qwt_dial.cpp \
    libs/qwt/qwt_dial_needle.cpp \
    libs/qwt/qwt_double_interval.cpp \
    libs/qwt/qwt_double_range.cpp \
    libs/qwt/qwt_double_rect.cpp \
    libs/qwt/qwt_dyngrid_layout.cpp \
    libs/qwt/qwt_event_pattern.cpp \
    libs/qwt/qwt_interval_data.cpp \
    libs/qwt/qwt_knob.cpp \
    libs/qwt/qwt_layout_metrics.cpp \
    libs/qwt/qwt_legend.cpp \
    libs/qwt/qwt_legend_item.cpp \
    libs/qwt/qwt_magnifier.cpp \
    libs/qwt/qwt_math.cpp \
    libs/qwt/qwt_paint_buffer.cpp \
    libs/qwt/qwt_painter.cpp \
    libs/qwt/qwt_panner.cpp \
    libs/qwt/qwt_picker.cpp \
    libs/qwt/qwt_picker_machine.cpp \
    libs/qwt/qwt_plot.cpp \
    libs/qwt/qwt_plot_axis.cpp \
    libs/qwt/qwt_plot_canvas.cpp \
    libs/qwt/qwt_plot_curve.cpp \
    libs/qwt/qwt_plot_dict.cpp \
    libs/qwt/qwt_plot_grid.cpp \
    libs/qwt/qwt_plot_item.cpp \
    libs/qwt/qwt_plot_layout.cpp \
    libs/qwt/qwt_plot_magnifier.cpp \
    libs/qwt/qwt_plot_marker.cpp \
    libs/qwt/qwt_plot_panner.cpp \
    libs/qwt/qwt_plot_picker.cpp \
    libs/qwt/qwt_plot_print.cpp \
    libs/qwt/qwt_plot_printfilter.cpp \
    libs/qwt/qwt_plot_rasteritem.cpp \
    libs/qwt/qwt_plot_scaleitem.cpp \
    libs/qwt/qwt_plot_spectrogram.cpp \
    libs/qwt/qwt_plot_svgitem.cpp \
    libs/qwt/qwt_plot_xml.cpp \
    libs/qwt/qwt_plot_zoomer.cpp \
    libs/qwt/qwt_raster_data.cpp \
    libs/qwt/qwt_rect.cpp \
    libs/qwt/qwt_round_scale_draw.cpp \
    libs/qwt/qwt_scale_div.cpp \
    libs/qwt/qwt_scale_draw.cpp \
    libs/qwt/qwt_scale_engine.cpp \
    libs/qwt/qwt_scale_map.cpp \
    libs/qwt/qwt_scale_widget.cpp \
    libs/qwt/qwt_slider.cpp \
    libs/qwt/qwt_spline.cpp \
    libs/qwt/qwt_symbol.cpp \
    libs/qwt/qwt_text.cpp \
    libs/qwt/qwt_text_engine.cpp \
    libs/qwt/qwt_text_label.cpp \
    libs/qwt/qwt_thermo.cpp \
    libs/qwt/qwt_wheel.cpp \
    libs/thirdParty/quazip/quagzipfile.cpp \
    libs/thirdParty/quazip/quaziodevice.cpp \
    libs/thirdParty/quazip/quazipdir.cpp \
    libs/utils/abstractprocess_win.cpp \
    libs/utils/basevalidatinglineedit.cpp \
    libs/utils/checkablemessagebox.cpp \
    libs/utils/classnamevalidatinglineedit.cpp \
    libs/utils/codegeneration.cpp \
    libs/utils/consoleprocess.cpp \
    libs/utils/consoleprocess_unix.cpp \
    libs/utils/consoleprocess_win.cpp \
    libs/utils/coordinateconversions.cpp \
    libs/utils/detailsbutton.cpp \
    libs/utils/detailswidget.cpp \
    libs/utils/fancylineedit.cpp \
    libs/utils/fancymainwindow.cpp \
    libs/utils/filenamevalidatinglineedit.cpp \
    libs/utils/filesearch.cpp \
    libs/utils/filewizarddialog.cpp \
    libs/utils/filewizardpage.cpp \
    libs/utils/homelocationutil.cpp \
    libs/utils/iwelcomepage.cpp \
    libs/utils/linecolumnlabel.cpp \
    libs/utils/newclasswidget.cpp \
    libs/utils/parameteraction.cpp \
    libs/utils/pathchooser.cpp \
    libs/utils/pathlisteditor.cpp \
    libs/utils/pathutils.cpp \
    libs/utils/projectintropage.cpp \
    libs/utils/projectnamevalidatinglineedit.cpp \
    libs/utils/qtcolorbutton.cpp \
    libs/utils/reloadpromptutils.cpp \
    libs/utils/savedaction.cpp \
    libs/utils/settingsutils.cpp \
    libs/utils/styledbar.cpp \
    libs/utils/stylehelper.cpp \
    libs/utils/submiteditorwidget.cpp \
    libs/utils/submitfieldwidget.cpp \
    libs/utils/synchronousprocess.cpp \
    libs/utils/treewidgetcolumnstretcher.cpp \
    libs/utils/uncommentselection.cpp \
    libs/utils/welcomemodetreewidget.cpp \
    libs/utils/winutils.cpp \
    libs/utils/worldmagmodel.cpp \
    libs/utils/xmlconfig.cpp \
    QsLog/examplemain.cpp \
    src/apps/mavlinkgen/generator/MAVLinkXMLParser.cc \
    src/apps/mavlinkgen/generator/MAVLinkXMLParserV10.cc \
    src/apps/mavlinkgen/ui/DomItem.cc \
    src/apps/mavlinkgen/ui/DomModel.cc \
    src/apps/mavlinkgen/ui/QGCMAVLinkTextEdit.cc \
    src/apps/mavlinkgen/ui/XMLCommProtocolWidget.cc \
    src/apps/mavlinkgen/main.cc \
    src/apps/mavlinkgen/MAVLinkGen.cc \
    src/apps/qgcvideo/main.cc \
    src/apps/qgcvideo/QGCVideoApp.cc \
    src/apps/qgcvideo/QGCVideoMainWindow.cc \
    src/apps/qgcvideo/QGCVideoWidget.cc \
    src/apps/qupgrade/main.cc \
    src/apps/qupgrade/QUpgradeApp.cc \
    src/apps/qupgrade/QUpgradeMainWindow.cc \
    src/apps/qupgrade/uploader.cpp \
    src/comm/HexSpinBox.cpp \
    src/comm/QGCXPlaneLink.cc \
    src/comm/XbeeLink.cpp \
    src/qgcunittest/testSuite.cc \
    src/qgcunittest/UASUnitTest.cc \
    src/ui/designer/QGCParamSliderPlugin.cc \
    src/ui/linechart/IncrementalPlot.cc \
    src/ui/linechart/LinechartPlot.cc \
    src/ui/linechart/Linecharts.cc \
    src/ui/linechart/LinechartWidget.cc \
    src/ui/linechart/Scrollbar.cc \
    src/ui/linechart/ScrollZoomer.cc \
    src/ui/InputConfiguration.cc \
    src/ui/MapWidget.cc \
    src/ui/OgreWidget.cc \
    src/ui/PX4FirmwareUpgrader.cc \
    src/ui/QGCHilXPlaneConfiguration.cc \
    src/ui/QGCWelcomeWidget.cc \
    src/ui/SlugsVideoCamControl.cpp \
    src/ui/XbeeConfigurationWindow.cpp \
    src/AudioOutput.cc \
    src/PX4FirmwareUpgradeWorker.cc \
    libs/mavlink/share/pyshared/pymavlink/generator/C/test/posix/testmav.c \
    libs/thirdParty/libxbee/sample/analog.c \
    libs/thirdParty/libxbee/sample/api.c \
    libs/thirdParty/libxbee/sample/atis.c \
    libs/thirdParty/libxbee/sample/atsetup.c \
    libs/thirdParty/libxbee/sample/callback.c \
    libs/thirdParty/libxbee/sample/digital.c \
    libs/thirdParty/libxbee/sample/digitalout.c \
    libs/thirdParty/libxbee/sample/multi.c \
    libs/thirdParty/libxbee/sample/scan.c \
    libs/thirdParty/libxbee/sample/scan_adv.c \
    libs/thirdParty/libxbee/sample/simple.c \
    libs/thirdParty/libxbee/sample/talk_to_me.c \
    libs/thirdParty/libxbee/sample/xbee2_rx.c \
    libs/thirdParty/libxbee/sample/xbee2_tx.c \
    libs/thirdParty/libxbee/xsys/linux.c \
    libs/thirdParty/libxbee/xsys/win32.c \
    libs/thirdParty/libxbee/xsys/win32.dll.c \
    libs/thirdParty/libxbee/api.c \
    libs/thirdParty/libxbee/main.c

MacBuild | WindowsBuild : contains(GOOGLEEARTH, enable) { #fix this to make sense ;)
    message(Including support for Google Earth)
    QT +=  webkit webkitwidgets
    HEADERS +=  src/ui/map3D/QGCWebPage.h
    SOURCES +=  src/ui/map3D/QGCWebPage.cc
} else {
    message(Skipping support for Google Earth)
}

contains(DEFINES, ENABLE_CAMRAVIEW){
    message(Including support for Camera View)
    HEADERS += src/ui/CameraView.h
    SOURCES += src/ui/CameraView.cc
} else {
    message(Skipping support for Camera View)
}


OTHER_FILES += \
    qml/components/DigitalDisplay.qml \
    qml/components/StatusDisplay.qml \
    qml/components/ModeDisplay.qml \
    qml/components/HeartbeatDisplay.qml \
    qml/PrimaryFlightDisplayQML.qml \
    qml/components/RollPitchIndicator.qml \
    qml/components/AltitudeIndicator.qml \
    qml/components/SpeedIndicator.qml \
    qml/components/CompassIndicator.qml \
    qml/components/PitchIndicator.qml \
    qml/components/StatusMessageIndicator.qml \
    qml/components/InformationOverlayIndicator.qml \

OTHER_FILES += \
    qml/ApmToolBar.qml \
    qml/components/Button.qml \
    qml/components/TextButton.qml \
    qml/resources/apmplanner/toolbar/connect.png \
    qml/resources/apmplanner/toolbar/flightplanner.png \
    qml/resources/apmplanner/toolbar/helpwizard.png \
    qml/resources/apmplanner/toolbar/light_initialsetup_icon.png \
    qml/resources/apmplanner/toolbar/terminal.png \
    qml/resources/apmplanner/toolbar/simulation.png \
    qml/resources/apmplanner/toolbar/light_tuningconfig_icon.png \
    qml/resources/apmplanner/toolbar/flightdata.png \
    qml/resources/apmplanner/toolbar/disconnect.png \
    qml/resources/apmplanner/toolbar/donate.png

OTHER_FILES += \
    qml/VibrationMonitor.qml \
    qml/EKFMonitor.qml

# Command Line Tools
OTHER_FILES += \
    libs/sik_uploader/sik_uploader.py

#qmlcomponents.path    += $${DESTDIR}$${TARGET}/components
#qmlcomponents.files   += ./components/Button.qml

#sources.files       += ApmToolBar.qml
#sources.path        += $$DESTDIR/qml
#target.path         += apmplanner2
#INSTALLS            += sources target

DISTFILES += \
    qml/components/BarGauge.qml \
    apm_planner_version.json \
    GoogleEarthLinux.bin \
    Installation instructions.rtf \
    mgwhelp.dll \
    exchndl.dll \
    drmingw.exe \
    qgroundcontrol.pro.user \
    user_config.pri.dist \
    testlog2.txt \
    testlog.txt \
    release_notes.txt \
    qgroundcontrol.rc \
    license.txt \
    README.md \
    CONTRIBUTING.md

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

QT += core gui multimedia

QT += multimediawidgets

SUBDIRS += \
    qupgrade.pro \
    qgcvideo.pro \
    qgcunittest.pro
