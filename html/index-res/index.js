var loaded = false;

function app() {
	LogMSG("Javascript has Loaded on View");
	loaded = true;
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

function submitLogin() {
	
	var loginJSON = {};
	var email = document.getElementById("login-email").value;
	var psswd = document.getElementById("login-password").value;

	loginJSON["email"] = email;
	loginJSON["psswd"] = psswd;

	let url = "http://localhost:3000/login";
	postData(url, {email: email, psswd: psswd})
		.then(data => {
			if(data.user_exists == false) {
				LogMSG("User does not exist");
			}
		})
}