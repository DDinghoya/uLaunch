
#pragma once
#include <ul_Include.hpp>

namespace fs {

    template<mode_t Mode>
    inline bool ExistsBase(const std::string &path) {
        struct stat st;
        if(stat(path.c_str(), &st) == 0) {
            return st.st_mode & Mode;
        }
        else {
            return false;
        }
    }

    inline bool ExistsFile(const std::string &path) {
        return ExistsBase<S_IFREG>(path);
    }

    inline bool ExistsDirectory(const std::string &path) {
        return ExistsBase<S_IFDIR>(path);
    }

    inline void CreateDirectory(const std::string &path) {
        mkdir(path.c_str(), 777);
    }

    inline void CreateFileBase(const std::string &path, u32 opt) {
        fsdevCreateFile(path.c_str(), 0, opt);
    }

    inline void CreateFile(const std::string &path) {
        CreateFileBase(path, 0);
    }

    inline void CreateConcatenationFile(const std::string &path) {
        CreateFileBase(path, FsCreateOption_BigFile);
    }

    inline void DeleteDirectory(const std::string &path) {
        fsdevDeleteDirectoryRecursively(path.c_str());
    }

    inline void DeleteFile(const std::string &path) {
        remove(path.c_str());
    }

    inline void CleanDirectory(const std::string &path) {
        DeleteDirectory(path);
        CreateDirectory(path);
    }

    inline bool WriteFile(const std::string &path, const void *data, const size_t size, const bool overwrite) {
        auto f = fopen(path.c_str(), overwrite ? "wb" : "ab+");
        if(f) {
            fwrite(data, 1, size, f);
            fclose(f);
            return true;
        }
        else {
            return false;
        }
    }

    inline bool ReadFile(const std::string &path, void *data, const size_t size) {
        auto f = fopen(path.c_str(), "rb");
        if(f) {
            fread(data, 1, size, f);
            fclose(f);
            return true;
        }
        else {
            return false;
        }
    }

    inline size_t GetFileSize(const std::string &path) {
        struct stat st;
        if(stat(path.c_str(), &st) == 0) {
            return st.st_size;
        }
        else {
            return 0;
        }
    }

    #define UL_FS_FOR(dir, name_var, path_var, ...) ({ \
        const std::string dir_str = dir; \
        auto dp = opendir(dir_str.c_str()); \
        if(dp) { \
            while(true) { \
                auto dt = readdir(dp); \
                if(dt == nullptr) { \
                    break; \
                } \
                const std::string name_var = dt->d_name; \
                const std::string path_var = dir_str + "/" + dt->d_name; \
                __VA_ARGS__ \
            } \
            closedir(dp); \
        } \
    })

}