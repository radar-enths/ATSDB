
include_directories (
    "${CMAKE_CURRENT_LIST_DIR}"
    )

target_sources(atsdb
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/job.h"
        "${CMAKE_CURRENT_LIST_DIR}/jobmanager.h"
        "${CMAKE_CURRENT_LIST_DIR}/jobmanagerwidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/dboreaddbjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/allbuffercsvexportjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/buffercsvexportjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/dboactivedatasourcesdbjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/dbominmaxdbjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/finalizedboreadjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/insertbufferdbjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/updatebufferdbjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/readjsonfilepartjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/jsonparsejob.h"
        "${CMAKE_CURRENT_LIST_DIR}/jsonmappingjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/jsonmappingstubsjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/createartasassociationsjob.h"
        "${CMAKE_CURRENT_LIST_DIR}/dboreadassociationsjob.h"
    #        src/job/dbovariabledistinctstatisticsdbjob.h
    #        src/job/dbocountdbjob.h
    #        src/job/dboinfodbjob.h
    #        src/job/writebufferdbjob.h
    #        src/job/transformationjob.h
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/dboactivedatasourcesdbjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/dbominmaxdbjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/dboreaddbjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/finalizedboreadjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/allbuffercsvexportjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/buffercsvexportjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/insertbufferdbjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/updatebufferdbjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/jobmanager.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/jobmanagerwidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/readjsonfilepartjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/jsonparsejob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/jsonmappingjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/jsonmappingstubsjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/createartasassociationsjob.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/dboreadassociationsjob.cpp"
    #        src/job/dbovariabledistinctstatisticsdbjob.cpp
    #        src/job/dbocountdbjob.cpp
    #        src/job/dboinfodbjob.cpp
    #        src/job/writebufferdbjob.cpp
    #        src/job/transformationjob.cpp
)

IF (jASTERIX_FOUND)
	MESSAGE(STATUS "Including ASTERIX jobs")

target_sources(atsdb
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/asterixdecodejob.h"
#        "${CMAKE_CURRENT_LIST_DIR}/asterixextractrecordsjob.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/asterixdecodejob.cpp"
#        "${CMAKE_CURRENT_LIST_DIR}/asterixextractrecordsjob.cpp"
)
ENDIF()
