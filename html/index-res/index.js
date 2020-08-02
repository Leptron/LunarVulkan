var loaded = false;

var viewID = "";
var pWidth, pHeight;

function SetViewID(id) {
	viewID = id;
}

function SetViewDims(width, height) {
	pWidth  = width;
	pHeight = height;
}

const urlRoot = "http://localhost:5000/"

var msgboxNoClose = new MessageBox("#l-msg-area", {
	closeTime: 5000,
	hideCloseButton: false
});

function app() {
	LogMSG("Javascript has Loaded on View " + window.location);
	loaded = true;

	let xmlHTTP = new XMLHttpRequest();
	xmlHTTP.open("GET", urlRoot + "clear");
	xmlHTTP.send(null);

	//creation of the notifaction
	document.getElementById("switch-login").addEventListener("click", function() {
		document.getElementById("signupForm").classList.add("noshow");
		document.getElementById("loginForm").classList.remove("noshow");
	})

	document.getElementById("switch-signup").addEventListener("click", function() {
		document.getElementById("loginForm").classList.add("noshow");
		document.getElementById("signupForm").classList.remove("noshow");
	})
}

async function postData(url = '', data = {}) {
	// Default options are marked with *
	const response = await fetch(url, {
	  method: 'POST', // *GET, POST, PUT, DELETE, etc.
	  mode: 'cors', // no-cors, *cors, same-origin
	  cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
	  credentials: 'same-origin', // include, *same-origin, omit
	  headers: {
		'Content-Type': 'application/json'
		// 'Content-Type': 'application/x-www-form-urlencoded',
	  },
	  redirect: 'follow', // manual, *follow, error
	  referrerPolicy: 'no-referrer', // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
	  body: JSON.stringify(data) // body data type must match "Content-Type" header
	});
	return response.json(); // parses JSON response into native JavaScript objects
}

function validateCode(e) {
	e.preventDefault();

	//get the email
	let email = document.getElementById("email-text-verification").innerText;
	let code = document.getElementById("validate-text").value;
	let postJSON = {
		email: email,
		token: code
	};

	let url = urlRoot + "verify";
	postData(url, postJSON)
		.then(resp => {
			if(resp.user_logged == false) {
				document.getElementById("validationForm").classList.add("noshow");
				document.getElementById("loginForm").classList.remove("noshow");

				let xmlHTTP = new XMLHttpRequest();
				xmlHTTP.open("GET", urlRoot + "clear");
				xmlHTTP.send(null);
				return;
			}
			
			if(resp.verification_needed == false) {
				//switch to the project panel;
				window.location.assign("file:///projects.html")
				return;
			}

			if(resp.user_exists == false) {
				document.getElementById("validationForm").classList.add("noshow");
				document.getElementById("loginForm").classList.remove("noshow");

				let xmlHTTP = new XMLHttpRequest();
				xmlHTTP.open("GET", urlRoot + "clear");
				xmlHTTP.send(null);
				return;
			}

			if(resp.user_verified) {
				document.getElementById("validationForm").classList.add("noshow");
				document.getElementById("loginForm").classList.remove("noshow");

				let xmlHTTP = new XMLHttpRequest();
				xmlHTTP.open("GET", urlRoot + "clear");
				xmlHTTP.send(null);
				return;
			}			

			if(resp.token_valid == false) {
				message = new MessageBox("l-msg-area", "msgBoxLoc");
				message.create(5000, "Your token is not valid, try again or resend the email");
				return;
			}
		})
}

function submitLogin(e) {
	e.preventDefault();

	var loginJSON = {};
	var email = document.getElementById("login-email").value;
	var psswd = document.getElementById("login-password").value;

	loginJSON["email"] = email;
	loginJSON["psswd"] = psswd;

	let url = urlRoot + "login";
	postData(url, loginJSON)
		.then(resp => {
			if(resp.logged == true && resp.verified == false) {
				//switch to verification page
				document.getElementById("signupForm").classList.add("noshow");

				document.getElementById("email-text-verification").innerText = loginJSON["email"];
				document.getElementById("validationForm").classList.remove("noshow");
				return;
			} else if (resp.logged == true && resp.verified == true) {
				//switch to project page
				window.location.assign("file:///projects.html");
				return;
			}

			if(resp.user_exists == false) {
				message = new MessageBox("l-msg-area", "msgBoxLoc");
				message.create(5000, "User does not exists, try again with a different email or signup for our service");
				return;
			}

			if(resp.pass_match == false) {
				message = new MessageBox("l-msg-area", "msgBoxLoc");
				message.create(5000, "Your password doesnt match, please try again");
				return;
			}
		})
}

function submitSignup(e) {
	e.preventDefault();

	var email = document.getElementById("signup-email").value;
	var username = document.getElementById("signup-display-name").value;

	var passwordOne = document.getElementById("signup-password").value;
	var passwordConf = document.getElementById("signup-password-conf").value;
	if(passwordOne != passwordConf) {
		message = new MessageBox("l-msg-area", "msgBoxLoc");
		message.create(5000, "Passwords do not match, try again");
		return;
	}

	var loginJSON = {};
	loginJSON["email"] = email;
	loginJSON["username"] = username;
	loginJSON["psswd"] = passwordConf;
	
	let url = urlRoot + "register";
	postData(url, loginJSON)
		.then(resp => {
			if(resp.logged == true && resp.verified == false) {
				// switch to the verification page
				document.getElementById("signupForm").classList.add("noshow");

				document.getElementById("email-text-verification").innerText = loginJSON["email"];
				document.getElementById("validationForm").classList.remove("noshow");
				return;
			} else if (resp.logged = true && resp.verified == true) {
				// go to project page
				// TODO implement project page
				return;
			}

			if(resp.user_exists == true) {
				message = new MessageBox("l-msg-area", "msgBoxLoc");
				message.create(5000, "User already exists, try again with a different email or login");
				return;
			}

			document.getElementById("email-text-verification").innerText = loginJSON["email"];
			document.getElementById("signupForm").classList.add("noshow");
			document.getElementById("validationForm").classList.remove("noshow");
		})
}