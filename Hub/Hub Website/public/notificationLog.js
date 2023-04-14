//Variables
const datetime = new Date();
date = datetime.toDateString();
var count = 1;

//Databases
const logRef = database.ref('notificationlog/'+date);
const logNotifRef = database.ref('server/notif');

//DOM objects
const logTable = document.querySelector('.log-table');
const logTime = document.querySelector('#log-time')
const msgArea = document.getElementById("invalid-msg");
const logButton = document.getElementById("log-link");
const logButtonText = document.getElementById("notification-amount");
const logDateHeader = document.getElementById("log-date-header");

logDateHeader.innerHTML = `${date}`;

logRef.orderByKey().on("child_added", function(snap) {
    addRowToLogTable(snap.key, snap.val());
});

logNotifRef.on("value", function(snap) {
    if (snap.val().number != 0) {
        logButtonText.innerHTML = `&nbsp(&nbsp${snap.val().number}&nbsp)`;
        logButton.classList.add("log-link-active");
    }
    else
    {
        logButtonText.innerHTML = ``;
        logButton.classList.remove("log-link-active");
    }
    
})


function addRowToLogTable(time, info) {
    var row = logTable.insertRow();
    var d_t = row.insertCell(0);
    var device = row.insertCell(1);
    var msg = row.insertCell(2);

    if (info.new == true) {
        row.classList.add("log-new-notif");
    }

    d_t.innerHTML = time;
    device.innerHTML = info.macAddr;
    msg.innerHTML = info.message;
    count += 1;
};

function downloadJSON() {
    const logDate = document.querySelector('#log-date');
    console.log(logDate);
    const dateObj = new Date(logDate.value + " " + new Date().toLocaleTimeString());
    var dateToGetData = dateObj.toDateString();
    if((isNaN(logDate.value))) {
        window.open(`https://home-security-system-4c4fa-default-rtdb.firebaseio.com/notificationlog/${dateToGetData}.json?download=${dateToGetData}.json`, "_blank");
        msgArea.innerHTML = "";
    }
    else
    {
        msgArea.style.color = "Red";
        msgArea.style.padding = "0px";
        msgArea.style.margin = "0px";
        msgArea.innerHTML = "Invalid date, please enter a valid date";
    }
}

function viewJSON() {
    const logDate = document.querySelector('#log-date');
    console.log(logDate);
    const dateObj = new Date(logDate.value + " " + new Date().toLocaleTimeString());
    var dateToGetData = dateObj.toDateString();
    if((isNaN(logDate.value)))
    {
        window.open(`https://home-security-system-4c4fa-default-rtdb.firebaseio.com/notificationlog/${dateToGetData}.json?print=pretty`, "_blank");
        msgArea.innerHTML = "";
    }
    else
    {
        msgArea.style.color = "Red";
        msgArea.style.padding = "0px";
        msgArea.style.margin = "0px";
        msgArea.innerHTML = "Invalid date, please enter a valid date";
    }
    
}

function clearNotifs() {
    logNotifRef.update({number: 0, state: false});
    nodesRef.get('value').then((snap) => {
        snap.forEach((child) => {
            nodesRef.child(child.key).update({status: 1});
        })
    })

    const allNewLogs = document.querySelectorAll(".log-new-notif");
    allNewLogs.forEach((child) => {
        child.classList.remove("log-new-notif");
    }) 

    logRef.get('value').then((snap)=> {
        snap.forEach((child) => {
            logRef.child(child.key).update({new: false});
        })
    })
    
}