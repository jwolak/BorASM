#include "FileHandler.h"

#include <spdlog/spdlog.h>

#include "Tools.h"

namespace assembly_engine {
    FileHandler::FileHandler() {}

    FileHandler::~FileHandler() {
        spdlog::trace("[FileHandler] ~FileHandler() called [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Check is file opened... [{0}:{1}]", __FILENAME__, __LINE__);
        if (file.is_open()) {
            spdlog::debug("[FileHandler] Closeing file... [{0}:{1}]", __FILENAME__, __LINE__);
            file.close();
        }
        spdlog::debug("[FileHandler] File closed... [{0}:{1}]", __FILENAME__, __LINE__);
    }

    FileHandler& FileHandler::operator=(FileHandler&& other_file_holder) noexcept {
        spdlog::trace("[FileHandler] Move assignment operator called [{0}:{1}]", __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Check if this is the same object... [{0}:{1}]", __FILENAME__, __LINE__);
        if (this != &other_file_holder) {
            spdlog::debug("[FileHandler] Check is file opened... [{0}:{1}]", __FILENAME__, __LINE__);
            if (file.is_open()) {
                spdlog::debug("[FileHandler] Closeing file... [{0}:{1}]", __FILENAME__, __LINE__);
                file.close();
            }
            spdlog::debug("[FileHandler] Moveing file... [{0}:{1}]", __FILENAME__, __LINE__);
            file = std::move(other_file_holder.file);
        }

        spdlog::debug("[FileHandler] Move assignment operator completed [{0}:{1}]", __FILENAME__, __LINE__);
        return *this;
    }

    bool FileHandler::OpenFile(const std::string& file_path, std::ios::openmode mode) {
        spdlog::trace("[FileHandler] OpenFile() called with file_path: {0}, mode: {1} [{2}:{3}]", file_path, static_cast<int>(mode), __FILENAME__, __LINE__);

        spdlog::debug("[FileHandler] Attempting to open file... [{0}:{1}]", __FILENAME__, __LINE__);
        try {
            file.open(file_path, mode);
        } catch (const std::exception& e) {
            spdlog::error("[FileHandler] Failed to open file [{0}:{1}]: {2}", __FILENAME__, __LINE__, e.what());
            return false;
        }

        spdlog::debug("[FileHandler] File opened successfully... [{0}:{1}]", __FILENAME__, __LINE__);
        return file.is_open();
    }

    std::ifstream& FileHandler::GetFileStream() {
        spdlog::trace("[FileHandler] GetFileStream() called [{0}:{1}]", __FILENAME__, __LINE__);
        return file;
    }

}  // namespace assembly_engine