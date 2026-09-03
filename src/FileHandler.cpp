#include "FileHandler.h"

#include "Tools.h"
#include "EquinoxLogger.hpp"

namespace assembly_engine {
    FileHandler::FileHandler() {}

    FileHandler::~FileHandler() {
        LOG_TRACE("[FileHandler] ~FileHandler() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_DEBUG("[FileHandler] Check is file opened... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (file_to_read_.is_open()) {
            LOG_DEBUG("[FileHandler] Closeing file... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            file_to_read_.close();
        }
        LOG_DEBUG("[FileHandler] File closed... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        if (file_to_write_.is_open()) {
            LOG_DEBUG("[FileHandler] Closing file to write... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            file_to_write_.close();
        }
        LOG_DEBUG("[FileHandler] File to write closed... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
    }

    FileHandler& FileHandler::operator=(FileHandler&& other_file_holder) noexcept {
        LOG_TRACE("[FileHandler] Move assignment operator called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_DEBUG("[FileHandler] Check if this is the same object... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        if (this != &other_file_holder) {
            LOG_DEBUG("[FileHandler] Check is file opened... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            if (file_to_read_.is_open()) {
                LOG_DEBUG("[FileHandler] Closeing file... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
                file_to_read_.close();
            }
            LOG_DEBUG("[FileHandler] Moveing file... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
            file_to_read_ = std::move(other_file_holder.file_to_read_);
        }

        LOG_DEBUG("[FileHandler] Move assignment operator completed [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return *this;
    }

    bool FileHandler::OpenFileToRead(const std::string& file_path) {
        LOG_TRACE("[FileHandler] OpenFile() called with file_path: " + file_path + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_DEBUG("[FileHandler] Attempting to open file... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        try {
            file_to_read_.open(file_path, std::ios::in | std::ios::binary);
        } catch (const std::exception& e) {
            LOG_ERROR("[FileHandler] Failed to open file [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]: " + e.what());
            return false;
        }

        LOG_DEBUG("[FileHandler] File opened successfully... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return file_to_read_.is_open();
    }

    std::ifstream& FileHandler::GetFileToReadStream() {
        LOG_TRACE("[FileHandler] GetFileToReadStream() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return file_to_read_;
    }

    bool FileHandler::OpenFileToWrite(const std::string& file_path) {
        LOG_TRACE("[FileHandler] OpenFileToWrite() called with file_path: " + file_path + " [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");

        LOG_DEBUG("[FileHandler] Attempting to open file to write... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        try {
            file_to_write_.open(file_path, std::ios::out | std::ios::binary);
        } catch (const std::exception& e) {
            LOG_ERROR("[FileHandler] Failed to open file to write [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]: " + e.what());
            return false;
        }

        LOG_DEBUG("[FileHandler] File to write opened successfully... [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return file_to_write_.is_open();
    }

    std::ofstream& FileHandler::GetFileToWriteStream() {
        LOG_TRACE("[FileHandler] GetFileToWriteStream() called [" + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + "]");
        return file_to_write_;
    }

}  // namespace assembly_engine