//
// Created by solcmich on 5/25/19.
//

#ifndef PACMAN_FILEMANAGER_H
#define PACMAN_FILEMANAGER_H

#include <vector>
#include<string>

class FileManager {
public:
    /*!
     * Lists files in given directory
     * @param dir [in] path to dir
     * @param files [out] vector to store names of dir's files
     * @return success while reading directory
     */
    static bool ListFiles(std::string dir, std::vector<std::string> & files);
};
#endif //PACMAN_FILEMANAGER_H
