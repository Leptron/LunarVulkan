#include "../../../public/LunarEditor/Projects/ProjectManager.h"

namespace LunarEditor {
    ProjectManager* ProjectManager::instance = nullptr;

    ProjectManager::ProjectManager() {
        loadProjects();
    }

    ProjectManager::~ProjectManager() {

    }

    ProjectManager* ProjectManager::getInstance() {
        if(instance == nullptr)
            instance = new ProjectManager;
        
        return instance;
    }

    void ProjectManager::loadProjects() {
        std::string manifest_loc = "project_man.bin";

        if(!fileExists(manifest_loc)) {
            int itemNum = 0;

            std::ofstream out(manifest_loc, std::ios::out | std::ios::binary | std::ios::trunc);
            out.write((char*)&itemNum, sizeof(itemNum));
            out.close();
        }

        int itemNum;
        std::ifstream in(manifest_loc, std::ios::binary | std::ios::in);
        in.read((char*)&itemNum, sizeof(int));
        
        _projectCount = itemNum;

        for(int i = 0; i < _projectCount; i++) {
            std::string loc, desc, name;
            size_t locS, descS, nameS;

            in.read((char*)&locS, sizeof(size_t));
            loc.resize(locS);
            in.read((char*)&loc[0], locS);


            in.read((char*)&nameS, sizeof(size_t));
            name.resize(nameS);
            in.read((char*)&name[0], nameS);

            in.read((char*)&descS, sizeof(size_t));
            desc.resize(descS);
            in.read((char*)&desc[0], descS);

            UserProject x = {};
            x.projectName = name;
            x.projectLocation = loc;
            x.projectDescription = desc;

            _projects.push_back(x);
            _projectCount = _projects.size();
        }

        if(_projectCount == 0) {
            UserProject x = {};

            x.projectLocation = "gaming";
            x.projectDescription = "gamingWithYo";
            x.projectName = "mamma";

            _projects.push_back(x);
            _projectCount = _projects.size();
        }
    }

    void ProjectManager::CreateProjectDir(std::string projectLocation, std::string projectName, std::string projectDescription) {
        UserProject _proj = {};

        _proj.projectName = projectName;
        _proj.projectLocation = projectLocation;
        _proj.projectDescription = projectDescription;

        _projects.push_back(_proj);
        _projectCount += _projects.size();
    }

    std::string ProjectManager::Serialize() {
        std::stringstream output_stream;
        output_stream << "{\"project_list\": [";
       
        for(int i = 0; i < _projects.size(); i++) {
            UserProject project = _projects[i];
            output_stream << "{";

            output_stream << "\"project_name\": \"" << project.projectName << "\",";
            output_stream << "\"project_location\": \"" << project.projectLocation << "\",";
            output_stream << "\"project_description\": \"" << project.projectDescription << "\"";

            if(i == _projects.size() - 1) {
                output_stream << "}";
            } else {
                output_stream << "},";
            }
        }

        output_stream << "]}";

        return output_stream.str();
    }

    void ProjectManager::updateManifest() {
        std::string manifest_loc = "project_man.bin";

        std::ofstream out(manifest_loc, std::ios::out | std::ios::binary | std::ios::trunc);
        out.write((char*)&_projectCount, sizeof(_projectCount));

        for(int i = 0; i < _projectCount; i++) {
            UserProject project = _projects[i];

            size_t locSize = project.projectLocation.size();
            out.write((char*)&locSize, sizeof(size_t));
            out.write((char*)project.projectLocation.c_str(), locSize);

            size_t nameSize = project.projectName.size();
            out.write((char*)&nameSize, sizeof(size_t));
            out.write((char*)project.projectName.c_str(), nameSize);

            size_t descSize = project.projectDescription.size();
            out.write((char*)&descSize, sizeof(size_t));
            out.write((char*)project.projectDescription.c_str(), descSize);
        }
    }

    bool ProjectManager::fileExists(const std::string& filename) {
        struct stat buffer;
        return (stat (filename.c_str(), &buffer) == 0);
    }
}