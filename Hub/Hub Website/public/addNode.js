// Nodes Database
var nodesRef = database.ref('nodes');

//Server database
var databaseServerNum = database.ref('server');

//Constants
const nodeList = document.querySelector('.nodes');
const nodeCreator = nodeList.lastElementChild;
const deviceForm = document.querySelector(".device-form");
const addDeviceButton = document.querySelector("#add-device-button");

const deviceNameInput = document.querySelector("#device-name");
const deviceMacInput = document.querySelector("#mac-addr");
const deviceSelectorInput = document.querySelector("#type-selector");
const deviceFormTempInput = document.querySelector("#device-form-temp-input");
const deviceLowTempInput = document.querySelector("#lower-temp");
const deviceHighTempInput = document.querySelector("#higher-temp")

//Nodes List
var totalNodes = 0;

// Load older nodes
nodesRef.on("child_added", function(snap) {
    console.log("Added", snap.key, snap.val());
    nodeList.insertBefore(nodeHtmlFromObject(snap.val()), nodeCreator);
    updateTotalNodes();
});

// When add device + is clicked, show form
addDeviceButton.addEventListener("click", ()=> {
    nodeCreator.style.display = 'none';
    nodeList.appendChild(deviceForm);
    deviceForm.style.display = 'flex'; 
});

// If temp sensor is selected, add in the temp range input
function updateDeviceForm() {
    if(deviceSelectorInput.options[deviceSelectorInput.selectedIndex].text == "Temperature Sensor") {
        deviceFormTempInput.style.display = "inline";
    }
    else
    {
        deviceFormTempInput.style.display = "none";
    }
}

deviceForm.addEventListener('submit', (e)=>{
    console.log("Submitted!");
    e.preventDefault();
    e.stopPropagation();
    const deviceName = deviceNameInput.value;
    const deviceMac = deviceMacInput.value;
    let deviceLowTemp = deviceLowTempInput.value;
    let deviceHighTemp = deviceHighTempInput.value;
    var deviceType = deviceSelectorInput.options[deviceSelectorInput.selectedIndex].text;
    
    if(deviceType != "Temperature Sensor") {
        deviceLowTemp = 0.0;
        deviceHighTemp = 30.0;
    }   

    nodesRef.push({
        name: deviceName,
        macAddr: deviceMac,
        type: deviceType,
        status: -1,
        data: -1,
        lowTemp: parseFloat(deviceLowTemp),
        highTemp: parseFloat(deviceHighTemp),
        batLvl: 0.0
    });

    deviceNameInput.value = "";
    deviceMacInput.value = "";
    deviceLowTemp.value = "";
    deviceHighTemp.value = "";
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


//Creates html object
function nodeHtmlFromObject(node) {
    const deviceBox = document.createElement("div");
    deviceBox.classList.add("node");
    deviceBox.innerHTML = `<h3>${node.name}</h3>`;
    deviceBox.innerHTML +=  
        `
        <i class="fa fa-battery-empty" id = "${node.macAddr}-bat"></i>
        <div class="node-del">
            <button onclick="removeDevice('${node.macAddr}')" class="remove-device-button">[delete]</button>
        </div>`;
    if (node.type == "Temperature Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">&#127777 Temp: <span  id = "${node.macAddr}-data">__</span></p>`;
        deviceBox.innerHTML += `
        <div>
            <p class = "node-temp-range">Range: ${node.lowTemp} - ${node.highTemp}&degF</p>
        </div>
        `;
    }
    if (node.type == "Camera") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${node.macAddr}-data">No detect</span></p>`;
    }
    if (node.type == "Glassbreak Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${node.macAddr}-data">Silent</span></p>`;
    }
    if (node.type == "Door Sensor") {
        deviceBox.innerHTML += `<p class = "node_data">Status: <span  id = "${node.macAddr}-data">Closed</span></p>`;
    }
    deviceBox.innerHTML += `<p class="node_mac">${node.macAddr}</p>`;
    deviceBox.innerHTML += `<p class="node_type">${node.type}</p>`;
    deviceBox.id = `${node.macAddr}`;
    return deviceBox;
};

function removeDevice(nodeAddr) {
    var databaseAddr;
    var nodeKey;
    nodesRef.on("value", function(snap) {
        snap.forEach((child) => {
            databaseAddr = child.val().macAddr;
            nodeKey = child.key;

            if (databaseAddr == nodeAddr) {
                console.log(`${databaseAddr} = ${nodeAddr}`);
                console.log(`Found match, removing: ${nodeAddr}`);
                var tempDevice = document.getElementById(nodeAddr);

                nodeAddr="";

                tempDevice.remove();
                console.log(`Removed ${nodeKey}!`);
                nodesRef.child(nodeKey).remove();
                updateTotalNodes();
            }
        })
    });
}


//Node counter
function updateTotalNodes() {
    console.log("Updating total nodes...")
    totalNodes = ((document.querySelectorAll('.node').length)-1);
    console.log("Now ", totalNodes);
    databaseServerNum.child("data").set({
        numberOfNodes: totalNodes
    });
};

