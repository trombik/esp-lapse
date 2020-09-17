idf_component_register(SRCS
    main.c
    camera.c
    sdcard.c
    REQUIRES esp32-camera fatfs
    INCLUDE_DIRS ".")
