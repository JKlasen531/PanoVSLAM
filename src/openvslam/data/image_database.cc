#include "openvslam/data/image.h"
#include "openvslam/data/image_database.h"

#include <spdlog/spdlog.h>
#include <iostream>

namespace openvslam {
namespace data {

image_database::image_database() {
    spdlog::debug("CONSTRUCT: data::image_database");
}

image_database::~image_database() {
    clear();
    spdlog::debug("DESTRUCT: data::image_database");
}

void image_database::add_image(std::string path) {
    std::lock_guard<std::mutex> lock(mtx_img_);
    image_paths_.push_back(path);
}

void image_database::remove_image(std::string path) {
    std::lock_guard<std::mutex> lock(mtx_img_);
    //image_paths_.erase(path);
}

std::vector<std::string> image_database::get_all_images() const {
    std::lock_guard<std::mutex> lock(mtx_img_);
    std::vector<std::string> images;
    images.reserve(image_paths_.size());
    for (const auto img_path : image_paths_) {
        images.push_back(img_path);
    }
    return images;
}

unsigned int image_database::get_num_images() const {
    std::lock_guard<std::mutex> lock(mtx_img_);
    return image_paths_.size();
}

void image_database::test_image_db() {
    std::cout << "image database tested" << std::endl;
    std::cout << image_paths_.size() << std::endl;
}

void image_database::clear() {
    std::lock_guard<std::mutex> lock(mtx_img_);
    //for (auto& img : image_paths_) {
    //    delete img;
    //   img = nullptr;
    //}
    image_paths_.clear();
    
    spdlog::info("clear image database");
}
} // namespace data
} // namespace openvslam
