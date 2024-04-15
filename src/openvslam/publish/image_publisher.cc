#include "openvslam/data/image_database.h"
#include "openvslam/publish/image_publisher.h"
#include "openvslam/data/map_database.h"
#include "openvslam/data/keyframe.h"

#include <spdlog/spdlog.h>

namespace openvslam {
namespace publish {

image_publisher::image_publisher(data::image_database* img_db, data::map_database* map_db)
      : img_db_(img_db), map_db_(map_db) {
    spdlog::debug("CONSTRUCT: publish::image_publisher");
}

image_publisher::~image_publisher() {
    spdlog::debug("DESTRUCT: publish::image_publisher");
}

unsigned int image_publisher::get_images(std::vector<std::string>& paths) {
  paths = img_db_->get_all_images();
  return img_db_->get_num_images();
}

void image_publisher::test_image_db(std::vector<data::keyframe*>& keyfrms) {
  img_db_->test_image_db();
}

} // namespace publish
} // namespace openvslam
