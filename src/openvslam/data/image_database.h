#ifndef OPENVSLAM_DATA_IMAGE_DATABASE_H
#define OPENVSLAM_DATA_IMAGE_DATABASE_H

#include <mutex>
#include <vector>
#include <unordered_map>
#include <memory>

namespace openvslam {

namespace data {
class image;
class image_database {
public:
    image_database();
    
    ~image_database();
    
    void add_image(std::string path);
    
    void remove_image(std::string path);
    
    std::vector<std::string> get_all_images() const;
    
    unsigned int get_num_images() const;

    void test_image_db();
    
    void clear();
    
private:
    std::vector<std::string> image_paths_;
    mutable std::mutex mtx_img_;
};

}
}

#endif
