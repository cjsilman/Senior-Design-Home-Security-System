var databaseNodes = database.ref('nodes');
var databaseServerData = database.ref('server');
const databaseServerNodes = database.ref('server/data/numberOfNodes');
const databaseHubAddress = database.ref('hub/hubAddress');
const databaseHubNodes = database.ref('hub/numberOfNodes');

var hubAddress;
var hubNodes;
var numberOfNodes;

databaseServerData.on("child_changed", function(snap) {
    console.log("Node change detected. Forcing update", snap.key, snap.val());
    if (snap.key = "numberOfNodes") { 
        updateNodesDescription();
    }
});

// When hub updates address, update website
databaseHubAddress.on('value', (snapshot)=> {
    hubAddress = snapshot.val();
    console.log(hubAddress);
    document.getElementById("hub-address").innerText = hubAddress;
});

// When hub updates how many nodes it sees, update website
databaseHubNodes.on('value', (snapshot)=> {
    hubNodes = snapshot.val();
    console.log(hubNodes);
    document.getElementById("hub-nodes").innerText = hubNodes;
    updateNodesDescription()
});

// When the website updates how many nodes it has, update website
databaseServerNodes.on('value', (snapshot)=> {
    numberOfNodes = snapshot.val();
    console.log(numberOfNodes);
    document.getElementById("server-nodes").innerText = `${numberOfNodes} devices defined`;
});

function updateNodesDescription() {
    var hubStatus = document.getElementById("hub-node-status")
    if (hubNodes == numberOfNodes) { //Subtract one to not count the add a device node
        hubStatus.innerText = "up-to-date"
        hubStatus.style.color='green';
    }
    else
    {
        hubStatus.innerText = "out-of-date"
        hubStatus.style.color='red';
    }
};