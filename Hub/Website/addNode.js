const nodeList = document.querySelector('.nodes');
const nodeCreator = nodeList.lastElementChild;
const deviceForm = document.querySelector(".device-form");
const addDeviceButton = document.querySelector("#add-device-button");

const deviceNameInput = document.querySelector("#device-name");
const deviceMacInput = document.querySelector("#mac-addr");
const deviceSelectorInput = document.querySelector("#type-selector");

addDeviceButton.addEventListener("click", ()=> {
    nodeCreator.style.display = 'none';
    nodeList.appendChild(deviceForm);
    deviceForm.style.display = 'flex'; 
});

deviceForm.addEventListener('submit', (e)=>{
    console.log("Submitted!");
    e.preventDefault();
    e.stopPropagation();
    const deviceName = deviceNameInput.value;
    const deviceMac = deviceMacInput.value;
    var deviceType = deviceSelectorInput.options[deviceSelectorInput.selectedIndex].text;
    
    const deviceBox = document.createElement("div");

    deviceBox.classList.add("node");
    deviceBox.innerHTML = `<h3>${deviceName}</h3>`;
    deviceBox.innerHTML +=  `<div class="delete-button"><button onclick="removeDevice('${deviceMac}')" class="remove-device-button">[delete]</button></div>`;
    if (deviceType == "Temperature Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">&#127777 Temp: <span id = "${deviceMac}-data">__</span></p>`;
    }
    if (deviceType == "Camera") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${deviceMac}-data">No detect</span></p>`;
    }
    if (deviceType == "Glassbreak Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${deviceMac}-data">Silent</span></p>`;
    }
    if (deviceType == "Door Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${deviceMac}-data">Closed</span></p>`;
    }
    deviceBox.innerHTML += `<p class="node_mac">${deviceMac}</p>`;
    deviceBox.innerHTML += `<p class="node_type">${deviceType}</p>`;
    deviceBox.id = deviceMac;
    
    nodeList.insertBefore(deviceBox, nodeCreator);
    deviceNameInput.value = "";
    deviceMacInput.value = "";
    deviceForm.style.display = "none";
    nodeCreator.style.display='block';
});

function removeDevice(deviceMac) {
    var tempDevice = document.getElementById(deviceMac);
    tempDevice.remove();
    console.log(`Removed ${deviceMac}!`);
};

deviceForm.addEventListener('reset', (e)=>{
    console.log("Cancelled!");
    e.preventDefault();
    e.stopPropagation();
    deviceForm.style.display = "none";
    nodeCreator.style.display='block';
});