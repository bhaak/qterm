file(GLOB TRANSLATIONS_FILES *.ts)
if(QT5)
    qt5_add_translation(QM_FILES ${TRANSLATIONS_FILES})
else(QT5)
    qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
endif(QT5)
add_custom_target(translations_target DEPENDS ${QM_FILES})
if (NOT APPLE)
  install(FILES ${QM_FILES} DESTINATION share/qterm/po)
endif()
