include("E:/csqt/build-mingw/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/oj-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtmultimedia")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "E:/csqt/build-mingw/oj.exe"
    GENERATE_QT_CONF
)
