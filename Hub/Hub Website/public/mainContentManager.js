const devices = document.querySelector(".main");
const info = document.querySelector(".main2");
const users = document.querySelector(".main3");
const log = document.querySelector(".main4");
const camVids = document.querySelector(".main5");




function homePageFunction() {
    const homePage = document.getElementById("homePage");
    devices.style.display = "inline";
    info.style.display = "none";
    users.style.display = "none";
    log.style.display = "none";
    camVids.style.display = "none";
    hubinfoPage.classList.remove("active");
    usersPage.classList.remove("active");
    homePage.classList.add("active");
    logPage.classList.remove("active");
    cameraPage.classList.remove("active");

};

function hubPageFunction() {
    const hubinfoPage = document.getElementById("hubinfoPage");
    devices.style.display = "none";
    info.style.display = "inline";
    users.style.display = "none";
    log.style.display = "none";
    camVids.style.display = "none";
    homePage.classList.remove("active");
    usersPage.classList.remove("active");
    hubinfoPage.classList.add("active");
    logPage.classList.remove("active");
    cameraPage.classList.remove("active");
};

function userPageFunction() {
    const usersPage = document.getElementById("usersPage");
    devices.style.display = "none";
    info.style.display = "none";
    users.style.display = "inline";
    log.style.display = "none";
    camVids.style.display = "none";
    homePage.classList.remove("active");
    hubinfoPage.classList.remove("active");
    usersPage.classList.add("active");
    logPage.classList.remove("active");
    cameraPage.classList.remove("active");
};



function logPageFunction() {
    const logPage = document.getElementById("logPage");
    devices.style.display = "none";
    info.style.display = "none";
    users.style.display = "none";
    log.style.display = "inline";
    camVids.style.display = "none";
    homePage.classList.remove("active");
    hubinfoPage.classList.remove("active");
    usersPage.classList.remove("active");
    logPage.classList.add("active");
    cameraPage.classList.remove("active");
};

function cameraPageFunction() {
    const logPage = document.getElementById("logPage");
    devices.style.display = "none";
    info.style.display = "none";
    users.style.display = "none";
    log.style.display = "none";
    camVids.style.display = "flex";
    homePage.classList.remove("active");
    hubinfoPage.classList.remove("active");
    usersPage.classList.remove("active");
    logPage.classList.remove("active");
    cameraPage.classList.add("active");
};