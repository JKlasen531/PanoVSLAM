#ifndef OPENVSLAM_IMAGE_UPDATER_H
#define OPENVSLAM_IMAGE_UPDATER_H

namespace openvslam {
namespace data {
class keyframe;
}
namespace publish {
class map_publisher;
}
class image_updater {
public:
    image_updater(const std::shared_ptr<publish::map_publisher>& map_publisher, const std::string& image_output_path);

    ~image_updater();

    void update_images();

private:

    void save_keyfrm_img(cv::Mat& keyfrm_rgb, std::string &id, std::string &path_to_save);

    const std::shared_ptr<publish::map_publisher>& map_publisher_;

    const std::string image_output_path_;
};
}

#endif
