#include "openvslam/publish/map_publisher.h"
#include "openvslam/data/keyframe.h"
#include "openvslam/image_updater.h"

#include <opencv2/core/eigen.hpp>
#include <opencv2/imgcodecs.hpp>

namespace openvslam {

image_updater::image_updater(const std::shared_ptr<publish::map_publisher>& map_publisher, const std::string& image_output_path)
    : map_publisher_(map_publisher), image_output_path_(image_output_path) {
        spdlog::debug("CONSTRUCT: image_updater");
}

image_updater::~image_updater() {
    spdlog::debug("DESTRUCT: image_updater");
}

void image_updater::update_images() {
    std::vector<data::keyframe*> keyframes;
    auto number_of_images = map_publisher_->get_keyframes(keyframes);
    for(int i = 0; i < number_of_images; i++) {

    }
}

void save_keyfrm_img(cv::Mat& keyfrm_rgb, std::string &id, std::string &path_to_save) {

}

}
