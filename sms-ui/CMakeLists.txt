#Qt5 GUI
project(sms-ui)

# --- Qt5 ---
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Requred QT packages
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)

# -----------
# Add resources
file (GLOB Resources *.qrc)
QT5_ADD_RESOURCES (ResourceSources ${Resources})

# Tell CMake to create the helloworld executable
add_executable(sms-ui main.cpp emuthread.cpp ${ResourceSources})

# Required Qt5 modules
qt5_use_modules(sms-ui Widgets)
qt5_use_modules(sms-ui Quick)
qt5_use_modules(sms-ui Qml)

# External libraries
target_link_libraries (sms-ui smsrom smsio smsram z80 psg peripherial sdsc vdp)
