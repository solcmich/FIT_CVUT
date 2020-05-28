//
// Created by solcmich on 5/25/19.
//

#include "FileManager.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>

bool FileManager::ListFiles(std::string dir, std::vector<std::string> & files){
    DIR *d;
    struct dirent *dirp;

    if((d  = opendir(dir.c_str())) == NULL)
        return false;

    while ((dirp = readdir(d)) != NULL)
        files.push_back(std::string(dirp->d_name));

    closedir(d);
    return true;
}