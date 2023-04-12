let userlink = document.getElementById('userlink');
let signoutlink = document.getElementById('signoutlink');
let userSettingsName = document.querySelector('.user-settings-username');
var currentUser = null;

function getUsername() {
    let keepLoggedIn = localStorage.getItem("keepLoggedIn");

    if(keepLoggedIn == "yes") {
        currentUser = JSON.parse(localStorage.getItem('user'));
    }
    else
    {
        currentUser = JSON.parse(sessionStorage.getItem('user'));
    }
}

function Signout() {
    sessionStorage.removeItem('user');
    localStorage.removeItem('user');
    localStorage.removeItem('keepLoggedIn');
    window.location = "login.html";
}

//Window load
window.onload = function(){
    getUsername();
    if(currentUser == null) {
        window.location = "login.html";
    }
    else
    {
        userlink.innerText=`Welcome, ${currentUser.username}`;
        userSettingsName.innerText=`${currentUser.username}`;
        userlink.href = "#";

        signoutlink.innerText="Not you? Sign Out";
        signoutlink.href = "javascript:Signout()";
    }
}