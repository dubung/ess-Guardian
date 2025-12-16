include("/home/ubuntu/ess_ws/2512_4th_proj_chanmi/src/ess_server/qt/qt_test/build/Desktop_Qt_6_8_3-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/qt_test-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /home/ubuntu/ess_ws/2512_4th_proj_chanmi/src/ess_server/qt/qt_test/build/Desktop_Qt_6_8_3-Debug/qt_test
    GENERATE_QT_CONF
)
