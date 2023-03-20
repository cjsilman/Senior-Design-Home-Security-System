// References
const name = document.getElementById('nameInp');
const email = document.getElementById('emailInp');
const username = document.getElementById('userInp');
const pass = document.getElementById('passInp');
const roleSelectorInput = document.querySelector("#typeInp");
const submit = document.getElementById('sub_btn');

// Validation

function isEmptyOrSpaces(str) {
    return str === null || str.match(/^ *$/) !== null;
}

function Validation() {
    let nameregex = /^[a-z A-Z]+$/;
    let emailregex = /^[a-zA-Z0-9]+@(gmail|yahoo|outlook)\.com$/;
    let userregex = /^[a-zA-Z0-9]{5,}$/;

    if(isEmptyOrSpaces(name.value) || 
       isEmptyOrSpaces(email.value) || 
       isEmptyOrSpaces(username.value) || 
       isEmptyOrSpaces(pass.value)) 
    {
        alert("You cannot leave any fields blank");
        return false;
    }

    if(!nameregex.test(name.value)) {
        alert("The name should only contain letters");
        return false;
    }

    if(!emailregex.test(email.value)) {
        alert("Enter a valid email");
        return false;
    }

    if(!userregex.test(username.value)) {
        alert("-username can only be alphanumeric\n-username must be atleast 5 characters\n-username cannot contain spaces")
        return false;
    }

    return true;
}

// Register user to firebase

function RegisterUser() {
    if (Validation() == false) {
        return;
    };
    const dbRef = database.ref();

    dbRef.child("UsersList").child(username.value).get().then((snapshot)=> {
        if(snapshot.exists()) {
            alert("Account Already Exists!");
        }
        else
        {
           database.ref('UsersList/' + username.value).set({
                fullname: name.value,
                email: email.value,
                username: username.value,
                password: encPass(),
                role: roleSelectorInput.options[roleSelectorInput.selectedIndex].text
           })
           .then(()=>{
                alert("User added successfully");
            })
            .catch((error)=>{
                alert("error" + error);
            })
        }
    });
}

//Encryption
function encPass(){
    var pass12 = CryptoJS.AES.encrypt(pass.value, pass.value);
    return pass12.toString();
}

// Submit Button
submit.addEventListener("click", ()=> {
    RegisterUser()
    window.location = "index.html";
});
