// Nodes Database
var nodesRef = database.ref('nodes');

//Constants
const nodeList = document.querySelector('.nodes');
const nodeCreator = nodeList.lastElementChild;
const deviceForm = document.querySelector(".device-form");
const addDeviceButton = document.querySelector("#add-device-button");

const deviceNameInput = document.querySelector("#device-name");
const deviceMacInput = document.querySelector("#mac-addr");
const deviceSelectorInput = document.querySelector("#type-selector");

// Load older nodes
nodesRef.on("child_added", function(snap) {
    console.log("Added", snap.key, snap.val());
    nodeList.insertBefore(nodeHtmlFromObject(snap.val()), nodeCreator);
});

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
    
    nodesRef.push({
        name: deviceName,
        macAddr: deviceMac,
        type: deviceType
    });
    
    nodeList.insertBefore(deviceBox, nodeCreator);
    deviceNameInput.value = "";
    deviceMacInput.value = "";
    deviceForm.style.display = "none";
    nodeCreator.style.display='block';
});

deviceForm.addEventListener('reset', (e)=>{
    console.log("Cancelled!");
    e.preventDefault();
    e.stopPropagation();
    deviceForm.style.display = "none";
    nodeCreator.style.display='block';
});

//Creates html
function nodeHtmlFromObject(node) {
    console.log(node);
    const deviceBox = document.createElement("div");
    deviceBox.classList.add("node");
    deviceBox.innerHTML = `<h3>${node.name}</h3>`;
    deviceBox.innerHTML += `<p>${node.macAddr}</p>`;
    deviceBox.innerHTML += `<p>${node.type}</p>`;
    return deviceBox;
}