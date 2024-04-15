#include "openvslam/data/image.h"

namespace openvslam {
namespace data {

image::image(std::string* path)
    : path_(path) {}

std::string image::get_path() {
    return *path_;
}

}
}
