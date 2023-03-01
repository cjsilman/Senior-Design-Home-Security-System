const button = document.querySelector('.arm_disarm');
const hubStatusRef = database.ref('hub/hubStatus');
const hubRef = database.ref('hub');
var state;
var statechange = false;

hubStatusRef.on("value", function(snap) {
    console.log("Updated to", snap.key, snap.val());
    state = snap.val().hubStatus;
    updateDisplay();
});

button.addEventListener("mouseover", (event)=>{
    console.log("Mouse entered");
    if (state == "DISARMED") {
        button.innerHTML = "ARM?";
        button.style.backgroundColor  = "grey";
    }

    if (state == "ARMED") {
        button.innerHTML = "DISARM?";
        button.style.backgroundColor  = "grey";
    }
    
});

button.addEventListener("mouseleave", (event)=>{
    console.log("Mouse left");
    if (state == "DISARMED") {
        button.innerHTML = "DISARMED";
        button.style.backgroundColor  = "#4CAF50";
    }

    if (state == "ARMED") {
        button.innerHTML = "ARMED";
        button.style.backgroundColor  = "#e63e32";
    }
    
});

button.addEventListener("click", (event)=>{
    if (state == "DISARMED" && statechange == false) {
        button.innerHTML = "ARMED";
        button.style.backgroundColor  = "#e63e32";
        hubStatusRef.child("hubStatus").set({
            hubStatus: "ARMED"
        });
        statechange = true;
    }

    if (state == "ARMED" && statechange == false) {
        button.innerHTML = "DISARMED";
        button.style.backgroundColor  = "#4CAF50";
        hubStatusRef.child("hubStatus").set({
            hubStatus: "DISARMED"
        });
        statechange =  true;
    }
    statechange = false;
});

function updateDisplay() {
    if (state == "DISARMED") {
        button.innerHTML = "DISARMED";
        button.style.backgroundColor  = "#4CAF50";
        return;
    }

    if (state == "ARMED") {
        button.innerHTML = "ARMED";
        button.style.backgroundColor  = "#e63e32";
        return;
    }
};