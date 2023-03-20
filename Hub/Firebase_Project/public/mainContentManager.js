const devices = document.querySelector(".main");
const info = document.querySelector(".main2");
const users = document.querySelector(".main3");




function homePageFunction() {
    const homePage = document.getElementById("homePage");
    devices.style.display = "inline";
    info.style.display = "none";
    users.style.display = "none";
    hubinfoPage.classList.remove("active");
    usersPage.classList.remove("active");
    homePage.classList.add("active");

};

function hubPageFunction() {
    const hubinfoPage = document.getElementById("hubinfoPage");
    devices.style.display = "none";
    info.style.display = "inline";
    users.style.display = "none";
    homePage.classList.remove("active");
    usersPage.classList.remove("active");
    hubinfoPage.classList.add("active");
};

function userPageFunction() {
    const usersPage = document.getElementById("usersPage");
    devices.style.display = "none";
    info.style.display = "none";
    users.style.display = "inline";
    homePage.classList.remove("active");
    hubinfoPage.classList.remove("active");
    usersPage.classList.add("active");
};