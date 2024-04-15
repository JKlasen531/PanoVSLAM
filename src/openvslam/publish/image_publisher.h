#ifndef OPENVSLAM_PUBLISH_IMAGE_PUBLISHER_H
#define OPENVSLAM_PUBLISH_IMAGE_PUBLISHER_H

#include "openvslam/type.h"
#include <memory>

#include <mutex>

namespace openvslam {

class config;

namespace data {
class image_database;
class map_database;
class keyframe;
}

namespace publish {

class image_publisher {
public:

    /**
     * Constructor
     */
	image_publisher(data::image_database* img_db, data::map_database* map_db);
	
    /**
     * Destructor
     */
	virtual ~image_publisher();
	
	unsigned int get_images(std::vector<std::string>& paths);

    void test_image_db(std::vector<data::keyframe*>& keyfrms);
	
private:
    //! image database
    data::image_database* img_db_;
    data::map_database* map_db_;
};

} // namespace publish
} // namespace openvslam

#endif
