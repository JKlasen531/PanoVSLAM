Modified OpenVSLAM to create an interactive panorama model.

*********************************************************
Dependencies:
Same as regular OpenVSLAM

*********************************************************
Build instructions:

-cd /path/to/OnlineOpenVslamPano
    mkdir build && cd build
    cmake \
        -DUSE_PANGOLIN_VIEWER=OFF \
        -DUSE_SOCKET_PUBLISHER=ON \
        -DUSE_STACK_TRACE_LOGGER=ON \
        -DBUILD_TESTS=ON \
        -DBUILD_EXAMPLES=ON \
        ..
    make -j4
    
*********************************************************
Installation for PanoViewer:

$ cd /path/to/PanoViewer
$ npm install

Then launch the server with:
$ cd /path/to/PanoViewer
$ node app.js
WebSocket: listening on *:3000
HTTP server: listening on *:3002

After launching, please access to ``http://localhost:3002/`` to check whether the server is correctly launched.

*********************************************************

Example invocation:

In main dir: ./build/run_video_slam -v ./orb_vocab/orb_vocab.fbow -m path/to/sciebo/Daten/2021-09-20_25-Integrationssprint/7-Donnerstag-FPV/VID_20180101_012305_00_004.mp4 -c ./example/panoviewer/config-large.yaml --mask path/to/sciebo/Daten/2021-09-20_25-Integrationssprint/7-Donnerstag-FPV/mask.png --frame-skip 1 -p ./build/tube.msg --img-output /imageoutputpath

*adjust paths accordingly

*********************************************************
generally you should first launch the panoviewer, connect to ``http://localhost:3002/`` and then run video slam or camera slam
