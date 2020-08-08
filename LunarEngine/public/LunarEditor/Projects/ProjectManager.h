#pragma once
#include "ProjectData.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <fstream>
#include <sys/stat.h>

namespace LunarEditor {
    class ProjectManager {
    private:
        static ProjectManager* instance;
        
        ProjectManager();

        bool fileExists(const std::string& filename);
    public:
        ~ProjectManager();
        static ProjectManager* getInstance();

        void updateManifest();
        void CreateProjectDir(std::string projectLocation, std::string projectName, std::string projectDescription);
        std::string Serialize();
    private:
        void loadProjects();

        std::vector<UserProject> _projects;
        int _projectCount;
    };
}