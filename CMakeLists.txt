cmake_minimum_required(VERSION 2.8.9)
project( lxqt-panel )

# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT CMAKE_BUILD_TYPE)
	set ( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)


macro( setByDefault VAR_NAME VAR_VALUE )
  if (NOT DEFINED ${VAR_NAME})
	set (${VAR_NAME} ${VAR_VALUE})
  endif (NOT DEFINED ${VAR_NAME})
endmacro()

# use gcc visibility feature to decrease unnecessary exported symbols
if (CMAKE_COMPILER_IS_GNUCXX)
  # set visibility to hidden to hide symbols, unlesss they're exporeted manually in the code
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fno-exceptions")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-no-undefined")
endif()


# *******************************************************************
# What plugins will be built, by default.
# You can enable/disable building of the plugin using cmake options.
#    cmake -DCLOCK_PLUGIN=Yes .. # Enable clock plugin
#    cmake -DCLOCK_PLUGIN=No ..  # Disable clock plugin

setByDefault(CLOCK_PLUGIN          Yes)
setByDefault(DESKTOPSWITCH_PLUGIN  Yes)
setByDefault(QUICKLAUNCH_PLUGIN    Yes)
setByDefault(TRAY_PLUGIN           Yes)
setByDefault(MAINMENU_PLUGIN       Yes)
setByDefault(TASKBAR_PLUGIN        Yes)
setByDefault(SCREENSAVER_PLUGIN    Yes)
setByDefault(SHOWDESKTOP_PLUGIN    Yes)
setByDefault(COLORPICKER_PLUGIN    Yes)
setByDefault(MOUNT_PLUGIN          Yes)
setByDefault(CPULOAD_PLUGIN        Yes)
setByDefault(NETWORKMONITOR_PLUGIN Yes)
setByDefault(SENSORS_PLUGIN        Yes)

setByDefault(VOLUME_PLUGIN         Yes)
# Options for the volume plugin.
setByDefault(VOLUME_USE_PULSEAUDIO Yes)
setByDefault(VOLUME_USE_ALSA       Yes)

setByDefault(SYSSTAT_PLUGIN        Yes)
setByDefault(KBINDICATOR_PLUGIN    Yes)
setByDefault(WORLDCLOCK_PLUGIN     Yes)

setByDefault(TEATIME_PLUGIN        No)

setByDefault(DOM_PLUGIN            Yes)
# *******************************************************************

include(LxQtLibSuffix)

add_definitions ( -Wall )

# Support Qt5
option(USE_QT5 "Build with Qt5." $ENV{USE_QT5})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_AUTOMOC ON)
if(USE_QT5)
  cmake_minimum_required(VERSION 2.8.11)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5DBus REQUIRED)
  find_package(Qt5LinguistTools REQUIRED QUIET)
  find_package(Qt5Xml REQUIRED QUIET)
  find_package(Qt5X11Extras REQUIRED QUIET)
  find_package(Qt5LinguistTools REQUIRED QUIET)
  message(STATUS "Building with Qt${Qt5Core_VERSION_STRING}")

  find_package(lxqt-qt5 REQUIRED)
  find_package(lxqtmount-qt5 REQUIRED )
  find_package(Qt5Xdg REQUIRED QUIET)
  find_package(lxqt-globalkeys-qt5 REQUIRED)
  find_package(lxqt-globalkeys-ui-qt5 REQUIRED)
  
  # xcb packages
  find_package(PkgConfig)
  pkg_check_modules(XCB REQUIRED xcb)
  pkg_check_modules(XCB_DAMAGE REQUIRED xcb-damage)
  pkg_check_modules(XCB_XKB REQUIRED xcb-xkb)
  include_directories(${XCB_INCLUDE_DIRS} ${XCB_DAMAGE_INCLUDE_DIRS} ${XCB_XKB_INCLUDE_DIRS})
else()
  set(QT_USE_QTXML 1)
  set(QT_USE_QTDBUS 1)
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  message(STATUS "Building with Qt${QTVERSION}")

  find_package(LXQT REQUIRED)
  find_package(LXQTMOUNT REQUIRED )
  find_package(QTXDG REQUIRED QUIET)
  find_package(LXQT_GLOBALKEYS REQUIRED)
  find_package(LXQT_GLOBALKEYS_UI REQUIRED)
endif()
include(${LXQT_USE_FILE})
include(${LXQTMOUNT_USE_FILE})
include(${QTXDG_USE_FILE})
include(${LXQT_GLOBALKEYS_USE_FILE})
include(${LXQT_GLOBALKEYS_UI_USE_FILE})

find_package ( X11 REQUIRED )

include_directories (
	${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_QTDBUS_INCLUDE_DIR}
	${X11_INCLUDE_DIR}
	${LXQT_INCLUDE_DIRS}
	${QTXDG_INCLUDE_DIRS}
	src)

set (PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/lxqt-panel/)
add_definitions(-DPLUGIN_DIR=\"${PLUGIN_DIR}\")
message(STATUS "Panel plugins location: ${PLUGIN_DIR}")

add_subdirectory(panel)

set(ENABLED_PLUGINS)

if (CLOCK_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "Clock")
  add_subdirectory( plugin-clock )
endif (CLOCK_PLUGIN)

if (DESKTOPSWITCH_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS} "DesktopSwitch")
  add_subdirectory( plugin-desktopswitch )
endif (DESKTOPSWITCH_PLUGIN)

if (QUICKLAUNCH_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "Quicklaunch")
  add_subdirectory( plugin-quicklaunch )
endif (QUICKLAUNCH_PLUGIN)

if (TRAY_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "System Tray")
  add_subdirectory( plugin-tray )
endif (TRAY_PLUGIN)

if (MAINMENU_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "Main menu")
  add_subdirectory( plugin-mainmenu )
endif (MAINMENU_PLUGIN)

if (MOUNT_PLUGIN)
	#  if (NOT UDEV_STABLE)
	#message(WARNING "lubudev is not stable - panel mount plugin is disabled")
	#else (NOT UDEV_STABLE)
	set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "Mount")
	add_subdirectory( plugin-mount )
	#endif (NOT UDEV_STABLE)
endif (MOUNT_PLUGIN)

if (TASKBAR_PLUGIN)
  set(ENABLED_PLUGINS ${ENABLED_PLUGINS}  "Taskbar")
  add_subdirectory( plugin-taskbar )
endif (TASKBAR_PLUGIN)

if (SCREENSAVER_PLUGIN)
	set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "Screensaver")
	add_subdirectory( plugin-screensaver )
endif (SCREENSAVER_PLUGIN)

if (SHOWDESKTOP_PLUGIN)
	set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "ShowDesktop")
	add_subdirectory(plugin-showdesktop)
endif (SHOWDESKTOP_PLUGIN)

if (COLORPICKER_PLUGIN)
	set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "Color Picker")
	add_subdirectory(plugin-colorpicker)
endif (COLORPICKER_PLUGIN)

if (HELLOWORLD_PLUGIN)
	set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "HelloWorld")
	add_subdirectory(plugin-helloworld)
endif (HELLOWORLD_PLUGIN)

if (CPULOAD_PLUGIN)
	FIND_LIBRARY(STATGRAB_LIB statgrab)

	if ( NOT(${STATGRAB_LIB} MATCHES "NOTFOUND" ))
		set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "CpuLoad")
		add_subdirectory(plugin-cpuload)
  else ()
		message(STATUS "")
		message(STATUS "panel's CPU load plugin requires libstatgrab")
		message(STATUS "")
	endif ( NOT(${STATGRAB_LIB} MATCHES "NOTFOUND" ))
endif (CPULOAD_PLUGIN)

if (NETWORKMONITOR_PLUGIN)
	FIND_LIBRARY(STATGRAB_LIB statgrab)

	if ( NOT(${STATGRAB_LIB} MATCHES "NOTFOUND" ))
		set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "NetworkMonitor")
		add_subdirectory(plugin-networkmonitor)
	else ()
		message(STATUS "")
		message(STATUS "Network monitor plugin requires libstatgrab")
		message(STATUS "")
	endif ( NOT(${STATGRAB_LIB} MATCHES "NOTFOUND" ))
endif (NETWORKMONITOR_PLUGIN)

if (SENSORS_PLUGIN)
  FIND_LIBRARY(SENSORS_LIB sensors)

  if ( NOT(${SENSORS_LIB} MATCHES "NOTFOUND" ))
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "Sensors")
    add_subdirectory(plugin-sensors)
  else ()
    message(STATUS "")
    message(STATUS "Sensors plugin requires lm_sensors")
    message(STATUS "")
  endif ( NOT(${SENSORS_LIB} MATCHES "NOTFOUND" ))

endif (SENSORS_PLUGIN)

if (VOLUME_PLUGIN)
  if (VOLUME_USE_PULSEAUDIO)
    FIND_PACKAGE(PulseAudio)
  endif (VOLUME_USE_PULSEAUDIO)

  if (VOLUME_USE_ALSA)
    FIND_PACKAGE(ALSA)
  endif (VOLUME_USE_ALSA)

  if (PULSEAUDIO_FOUND OR ALSA_FOUND)
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "Volume")
    message(STATUS "")
    message(STATUS "Volume plugin will be built")
    message(STATUS "    ALSA: ${ALSA_FOUND}")
    message(STATUS "    PulseAudio: ${PULSEAUDIO_FOUND}")
    message(STATUS "")
    add_subdirectory(plugin-volume)
  else ()
    message(STATUS "")
    message(STATUS "Volume plugin requires pulseaudio or alsa")
    message(STATUS "    ALSA: ${ALSA_FOUND}")
    message(STATUS "    PulseAudio: ${PULSEAUDIO_FOUND}")
    message(STATUS "")
  endif (PULSEAUDIO_FOUND OR ALSA_FOUND)

endif (VOLUME_PLUGIN)

if (KBINDICATOR_PLUGIN)
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "Keyboard indicator")
    add_subdirectory(plugin-kbindicator)
endif (KBINDICATOR_PLUGIN)


if (TEATIME_PLUGIN)
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "TeaTime")
    add_subdirectory(plugin-teatime)
endif (TEATIME_PLUGIN)

if (SYSSTAT_PLUGIN)
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "SysStat")
    add_subdirectory(plugin-sysstat)
endif (SYSSTAT_PLUGIN)

if (WORLDCLOCK_PLUGIN)
  FIND_PACKAGE(ICU 4.2 COMPONENTS i18n data)

  if(ICU_I18N_FOUND)
    message(STATUS "ICU version is ${ICU_MAJOR_VERSION}${ICU_MINOR_VERSION}")
    add_definitions(-DICU_VERSION=${ICU_MAJOR_VERSION}${ICU_MINOR_VERSION})
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "WorldClock")
    add_subdirectory(plugin-worldclock)
  else ()
    message(STATUS "")
    message(STATUS "WorldClock plugin requires ICU")
    message(STATUS "")
  endif (ICU_I18N_FOUND)

endif (WORLDCLOCK_PLUGIN)

if (DOM_PLUGIN)
    set(ENABLED_PLUGINS ${ENABLED_PLUGINS}   "DOM")
    add_subdirectory(plugin-dom)
endif (DOM_PLUGIN)

message(STATUS  "**************** The following plugins will be built ****************")
foreach (PLUGIN_STR ${ENABLED_PLUGINS})

  message(STATUS   "  ${PLUGIN_STR}")
endforeach()
message(STATUS  "*********************************************************************")

# building tarball with CPack -------------------------------------------------
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include (CPack)
