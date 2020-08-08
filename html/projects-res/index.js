var loaded = false;
var projects;

const urlRoot = "http://localhost:5000/"

function app() {
    var project_string = GetProjects();
    projects = JSON.parse(project_string);

    for(let i = 0; i < projects["project_list"].length; i++) {
        let project = projects["project_list"][i];
        createProject(project);
    }
}

function createProject(project) {
    let parentLI = document.createElement("li");
    let projectContainer = document.createElement("div");
    let projectName = document.createElement("h2");
    let projectLoc = document.createElement("p");

    //classes
    projectContainer.classList.add("project");

    //text
    projectName.textContent = project["project_name"];
    projectLoc.textContent = project["project_location"];

    projectContainer.appendChild(projectName);
    projectContainer.appendChild(projectLoc);

    parentLI.append(projectContainer);

    document.getElementById("proj-append").appendChild(parentLI);
}