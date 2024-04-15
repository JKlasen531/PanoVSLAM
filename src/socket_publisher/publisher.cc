#include "socket_publisher/publisher.h"

#include "openvslam/system.h"
#include "openvslam/publish/frame_publisher.h"
#include "openvslam/util/yaml.h"
#include <unistd.h>
#include <stdio.h>
#include <memory>

namespace socket_publisher {

publisher::publisher(const YAML::Node& yaml_node, openvslam::system* system,
                     const std::shared_ptr<openvslam::publish::frame_publisher>& frame_publisher,
                     const std::shared_ptr<openvslam::publish::map_publisher>& map_publisher,
                     const std::shared_ptr<openvslam::publish::image_publisher>& image_publisher,
                     const std::string& project_folder_path)
    : system_(system),
      emitting_interval_(yaml_node["emitting_interval"].as<unsigned int>(15000)),
      image_quality_(yaml_node["image_quality"].as<unsigned int>(20)),
      client_(new socket_client(yaml_node["server_uri"].as<std::string>("http://127.0.0.1:3000"))) {
    data_serializer_ = std::unique_ptr<data_serializer>(new data_serializer(
        frame_publisher, map_publisher, image_publisher, project_folder_path,
       yaml_node["publish_points"].as<bool>(true)));

    client_->set_signal_callback(std::bind(&publisher::callback, this, std::placeholders::_1));
}

void publisher::run() {
    is_terminated_ = false;
    is_paused_ = false;

    const auto serialized_reset_signal = data_serializer::serialized_reset_signal_;
    client_->emit("map_publish", serialized_reset_signal);
    while (true) {
        const auto t0 = std::chrono::system_clock::now();

        const auto serialized_map_data = data_serializer_->serialize_map_diff();
        if (!serialized_map_data.empty()) {
            client_->emit("map_publish", serialized_map_data);
        }

        const auto serialized_frame_data = data_serializer_->serialize_latest_frame(image_quality_);
        if (!serialized_frame_data.empty()) {
            client_->emit("frame_publish", serialized_frame_data);
        }

        // sleep until emitting interval time is past
        const auto t1 = std::chrono::system_clock::now();
        const auto elapse_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
        if (elapse_us < emitting_interval_) {
            const auto sleep_us = emitting_interval_ - elapse_us;
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_us));
        }

        if (check_and_execute_pause()) {
            while (is_paused()) {
                std::this_thread::sleep_for(std::chrono::microseconds(5000));
            }
        }

        if (terminate_is_requested()) {
            break;
        }
    }

    system_->request_terminate();
    terminate();
}

void publisher::init(const std::string& video_file , const std::string& project_folder_path, const std::string& tag, const double fps, const unsigned int frame_skip) {
    if(tag == "videoSlam") {
      const auto tags = std::vector<std::string>{"videoSlam","2", "project_folder", "fps*100", "frame_skip"};
      const auto messages = std::vector<std::string>{video_file, get_current_dir_name(), project_folder_path, std::to_string(fps*100), std::to_string(frame_skip)};
      const auto vid_message = data_serializer_->serialize_messages(tags, messages);
      client_->emit("video file", vid_message); /* change to a more fitting name */
    } else {
      const auto tags = std::vector<std::string>{"cameraSlam","2", "project_folder", "fps*100", "frame_skip"};
      const auto messages = std::vector<std::string>{video_file, get_current_dir_name() ,project_folder_path, std::to_string(fps*100), std::to_string(frame_skip)};
      const auto vid_message = data_serializer_->serialize_messages(tags, messages);
      client_->emit("video file", vid_message);
    }
}

void publisher::callback(const std::string& message) {
    if (message == "disable_mapping_mode") {
        system_->disable_mapping_module();
    }
    else if (message == "enable_mapping_mode") {
        system_->enable_mapping_module();
    }
    else if (message == "reset") {
        system_->request_reset();
    }
    else if (message == "terminate") {
        request_terminate();
    }
}

void publisher::send_status(std::string status) {
  const auto tag = std::vector<std::string>{"status"};
  const auto msg = std::vector<std::string>{status};
  const auto serialized_status = data_serializer_->serialize_messages(tag, msg);
  client_->emit("status", serialized_status);
}

void publisher::request_pause() {
    std::lock_guard<std::mutex> lock(mtx_pause_);
    if (!is_paused_) {
        pause_is_requested_ = true;
    }
}

bool publisher::is_paused() {
    std::lock_guard<std::mutex> lock(mtx_pause_);
    return is_paused_;
}

void publisher::resume() {
    std::lock_guard<std::mutex> lock(mtx_pause_);
    is_paused_ = false;
}

void publisher::request_terminate() {
    std::lock_guard<std::mutex> lock(mtx_terminate_);
    terminate_is_requested_ = true;
}

bool publisher::is_terminated() {
    std::lock_guard<std::mutex> lock(mtx_terminate_);
    return is_terminated_;
}

bool publisher::terminate_is_requested() {
    std::lock_guard<std::mutex> lock(mtx_terminate_);
    return terminate_is_requested_;
}

void publisher::terminate() {
    std::lock_guard<std::mutex> lock(mtx_terminate_);
    is_terminated_ = true;
}

bool publisher::check_and_execute_pause() {
    std::lock_guard<std::mutex> lock1(mtx_pause_);
    std::lock_guard<std::mutex> lock2(mtx_terminate_);

    if (terminate_is_requested_) {
        return false;
    }
    else if (pause_is_requested_) {
        is_paused_ = true;
        pause_is_requested_ = false;
        return true;
    }
    return false;
}

} // namespace socket_publisher