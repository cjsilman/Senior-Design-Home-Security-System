const button = document.querySelector('#armPage');
const hubStatusRef = database.ref('hub/hStatus');
const armPageLink = document.querySelector('#armPageLink');
const hubRef = database.ref('hub');
var state;
var statechange = false;
var mouseleft = true;

hubStatusRef.on("value", function(snap) {
    console.log("Updated to", snap.key, snap.val());
    state = snap.val().hubStatus;
    updateDisplay();
});

button.addEventListener("mouseenter", (event)=>{
    mouseleft = false;
    if (state == "DISARMED") {
        armPageLink.innerText = "Click to arm the system";
        armPageLink.style.color  = "white";
    }

    if (state == "ARMED") {
        armPageLink.innerText = "Click to disarm the system";
        armPageLink.style.color  = "white";
    }
});

button.addEventListener("mouseleave", (event)=>{
    mouseleft = true;
    if (state == "DISARMED") {
        armPageLink.innerText = `System Status: DISARMED`;
        armPageLink.style.color  = "#4CAF50";
    }

    if (state == "ARMED") {
        armPageLink.innerText = `System Status: ARMED`;
        armPageLink.style.color  = "#e63e32";
    }
    
});

button.addEventListener("click", (event)=>{
    if (state == "DISARMED" && statechange == false) {
        armPageLink.innerText = `System Status: ARMED`;
        armPageLink.style.color  = "#e63e32";
        hubRef.child("hStatus").set({
            hubStatus: "ARMED"
        });
        statechange = true;
    }

    if (state == "ARMED" && statechange == false) {
        armPageLink.innerText = `System Status: DISARMED`;
        armPageLink.style.color  = "white";
        hubRef.child("hStatus").set({
            hubStatus: "DISARMED"
        });
        statechange =  true;
    }
    statechange = false;
});

function updateDisplay() {
    if(mouseleft == false) {
        if (state == "DISARMED") {
            armPageLink.innerText = `System Status: DISARMED`;
            armPageLink.style.color  = "white";
            return;
        }
    } else {
        if (state == "DISARMED") {
            armPageLink.innerText = `System Status: DISARMED`;
            armPageLink.style.color  = "#4CAF50";
            return;
        }
    }
    

    if (state == "ARMED") {
        armPageLink.innerText = `System Status: ARMED`;
        armPageLink.style.color  = "#e63e32";
        return;
    }
};