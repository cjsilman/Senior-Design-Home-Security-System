enum ResponseState {
    //HUB
    HUB_OK = 1,

    //Camera
    CAM_ERR = -1,
    CAM_OK = 1,
    CAM_RDY = 2,

    //Glassbreak Sensor
    GB_ERR = -1,
    GB_OK = 1,
    GB_NOISE = 2
};