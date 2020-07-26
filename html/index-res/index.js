var loaded = false;

var msgboxNoClose = new MessageBox("#l-msg-area", {
	closeTime: 5000,
	hideCloseButton: false
});

function app() {
	LogMSG("Javascript has Loaded on View");
	loaded = true;

	//creation of the notifaction
	
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

function submitLogin(e) {
	e.preventDefault();

	var loginJSON = {};
	var email = document.getElementById("login-email").value;
	var psswd = document.getElementById("login-password").value;

	loginJSON["email"] = email;
	loginJSON["psswd"] = psswd;

	let url = "http://localhost:3000/login";
	postData(url, {email: email, psswd: psswd})
		.then(data => {
			if(data.user_exists == false) {
				//notification
				message = new MessageBox("l-msg-area", "msgBoxLoc");
				message.create(10000, "User is Not Found, try again or Signup for our Service.");
			}
		})
}

function submitSignup(e) {
	e.preventDefault();
}