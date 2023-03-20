// References
const username = document.getElementById('userInp');
const pass = document.getElementById('passInp');
const submit = document.getElementById('sub_btn');

// Authenticate User
function AuthenticateUser() {
    const dbRef = database.ref();

    dbRef.child("UsersList").child(username.value).get().then((snapshot)=> {
        inputPassword = pass.value;
        if(inputPassword == null || inputPassword == '') {
            alert("Please input a password");
            return;
        }

        console.log(inputPassword);
        if(snapshot.exists()) {
            let dbpass = decPass(snapshot.val().password, inputPassword);
            console.log(dbpass);
            console.log(snapshot.val().password);
            console.log(inputPassword);
            if(dbpass == inputPassword) {
                login(snapshot.val());
            }
            else
            {
                alert("Username or password is invalid");
            }
        }
        else
        {
          alert("Username or password is invalid");
        }
    });
}

// Decrypt function
function decPass(dbpass, key){
    var pass12 = CryptoJS.AES.decrypt(dbpass, key);
    return pass12.toString(CryptoJS.enc.Utf8);
}

// Login
function login(user) {
    let keepLoggedIn = document.getElementById('customSwitch1').checked;

    if (!keepLoggedIn) {
        sessionStorage.setItem('user', JSON.stringify(user));
        window.location="index.html";
    }
    else{
        localStorage.setItem('keepLoggedIn', 'yes');
        localStorage.setItem('user', JSON.stringify(user));
        window.location="index.html";
    }
}

submit.addEventListener("click", ()=> {
    AuthenticateUser();
});

document.addEventListener("keyup", function(event) {
    if (event.keyCode === 13) {
        AuthenticateUser();
    }
});

//Show Password
function showPassword() {
    var inputbox = document.getElementById("passInp");
    if(inputbox.type == "password") {
        inputbox.type = "text";
    }
    else
    {
        inputbox.type = "password";
    }
}