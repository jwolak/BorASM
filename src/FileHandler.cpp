#include "FileHandler.h"

#include "Tools.h"
#include "spdlog/spdlog.h"

namespace assembly_engine {
    FileHandler::FileHandler() {}

    FileHandler::~FileHandler() {
        spdlog::trace("[FileHandler] ~FileHandler() called [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Check is file opened... [{0}:{1}]", __FILENAME__, __LINE__);
        if (file_to_read_.is_open()) {
            spdlog::debug("[FileHandler] Closeing file... [{0}:{1}]", __FILENAME__, __LINE__);
            file_to_read_.close();
        }
        spdlog::debug("[FileHandler] File closed... [{0}:{1}]", __FILENAME__, __LINE__);

        if (file_to_write_.is_open()) {
            spdlog::debug("[FileHandler] Closing file to write... [{0}:{1}]", __FILENAME__, __LINE__);
            file_to_write_.close();
        }
        spdlog::debug("[FileHandler] File to write closed... [{0}:{1}]", __FILENAME__, __LINE__);
    }

    FileHandler& FileHandler::operator=(FileHandler&& other_file_holder) noexcept {
        spdlog::trace("[FileHandler] Move assignment operator called [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Check if this is the same object... [{0}:{1}]", __FILENAME__, __LINE__);
        if (this != &other_file_holder) {
            spdlog::debug("[FileHandler] Check is file opened... [{0}:{1}]", __FILENAME__, __LINE__);
            if (file_to_read_.is_open()) {
                spdlog::debug("[FileHandler] Closeing file... [{0}:{1}]", __FILENAME__, __LINE__);
                file_to_read_.close();
            }
            spdlog::debug("[FileHandler] Moveing file... [{0}:{1}]", __FILENAME__, __LINE__);
            file_to_read_ = std::move(other_file_holder.file_to_read_);
        }

        spdlog::debug("[FileHandler] Move assignment operator completed [{0}:{1}]", __FILENAME__, __LINE__);
        return *this;
    }

    bool FileHandler::OpenFileToRead(const std::string& file_path) {
        spdlog::trace("[FileHandler] OpenFile() called with file_path: {0} [{1}:{2}]", file_path, __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Attempting to open file... [{0}:{1}]", __FILENAME__, __LINE__);
        try {
            file_to_read_.open(file_path, std::ios::in | std::ios::out | std::ios::binary);
        } catch (const std::exception& e) {
            spdlog::error("[FileHandler] Failed to open file [{0}:{1}]: {2}", __FILENAME__, __LINE__, e.what());
            return false;
        }

        spdlog::debug("[FileHandler] File opened successfully... [{0}:{1}]", __FILENAME__, __LINE__);
        return file_to_read_.is_open();
    }

    std::ifstream& FileHandler::GetFileStream() {
        spdlog::trace("[FileHandler] GetFileStream() called [{0}:{1}]", __FILENAME__, __LINE__);
        return file_to_read_;
    }

    bool FileHandler::OpenFileToWrite(const std::string& file_path) {
        spdlog::trace("[FileHandler] OpenFileToWrite() called with file_path: {0} [{1}:{2}]", file_path, __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Attempting to open file to write... [{0}:{1}]", __FILENAME__, __LINE__);
        try {
            file_to_write_.open(file_path, std::ios::out | std::ios::binary);
        } catch (const std::exception& e) {
            spdlog::error("[FileHandler] Failed to open file to write [{0}:{1}]: {2}", __FILENAME__, __LINE__, e.what());
            return false;
        }

        spdlog::debug("[FileHandler] File to write opened successfully... [{0}:{1}]", __FILENAME__, __LINE__);
        return file_to_write_.is_open();
    }

    std::ofstream& FileHandler::GetFileToWriteStream() {
        spdlog::trace("[FileHandler] GetFileToWriteStream() called [{0}:{1}]", __FILENAME__, __LINE__);
        return file_to_write_;
    }

}  // namespace assembly_engine