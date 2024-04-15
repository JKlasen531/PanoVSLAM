#ifndef OPENVSLAM_DATA_IMAGE_H
#define OPENVSLAM_DATA_IMAGE_H

#include <mutex>
#include <atomic>

namespace openvslam {

namespace data{
class image {
public:
    image(std::string* path);

    std::string get_path();

private:
    std::string* path_;
};
}
}

#endif