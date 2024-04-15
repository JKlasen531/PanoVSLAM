#PanoVSLAM
Modified OpenVSLAM to create an interactive panorama model.

## Dependencies
- Docker [(installation guide)](https://docs.docker.com/engine/install/)

## Installation
```
git clone --recursive https://github.com/JKlasen531/PanoVSLAM.git
cd PanoVSLAM
docker build -t panovslam -f Dockerfile.socket . --build-arg NUM_THREADS=$(nproc)
```
    
## Running
Start VSLAM container
```
docker run -it --rm --ipc=host --ulimit memlock=-1 --ulimit stack=67108864 -p 3002:3002 --name=panovslam -v ${HOST_DATA_PATH:?Set path to directory on the host system to keep input and output data.}:/data panovslam
```
should volume mount orb vocab, vide file, config file and optionally a mask file
e.g.
docker run \
  -it --rm --ipc=host \
  --ulimit memlock=-1 \
  --ulimit stack=67108864 \
  -p 3002:3002 \
  --name=vslam_pano \
  -v $(pwd)/../dataset/orb_vocab.fbow:/orb_vocab.fbow \
  -v $(pwd)/../dataset/aist/mask.png:/aist/mask.png \
  -v $(pwd)/../dataset/aist_entrance_hall_1/video.mp4:/aist/video.mp4 \
  -v $(pwd)/../dataset/aist/equirectangular.yaml:/aist/config.yaml \
  vslam_pano
```
./run_video_slam -v /data/orb_vocab.fbow -c "/data/${PATH_TO_CONFIG:?}" -m "/data/${PATH_TO_VIDEO:?}" --mask "/data/${PATH_TO_MASK:?}" --project-folder "${PATH_TO_OUTPUT_FOLDER}"
```
