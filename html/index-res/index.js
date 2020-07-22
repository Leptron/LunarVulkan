var loaded = false;

function app() {
	LogMSG("Javascript has Loaded on View");
	loaded = true;
}

function submitLogin() {
	
	var loginJSON = {};
	var email = document.getElementById("login-email").value;
	var psswd = document.getElementById("login-password").value;

	loginJSON["email"] = email;
	loginJSON["psswd"] = psswd;

	//submit to the epic 
}