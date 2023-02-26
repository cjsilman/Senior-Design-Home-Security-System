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
    deviceBox.innerHTML +=  `<div><button onclick="removeDevice('${deviceMac}')" class="remove-device-button">[delete]</button></div>`;
    deviceBox.innerHTML += `<p>${deviceMac}</p>`;
    deviceBox.innerHTML += `<p>${deviceType}</p>`;
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