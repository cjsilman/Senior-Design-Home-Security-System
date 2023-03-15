// Nodes Database
var nodesRef = database.ref('nodes');

//Whenever something is updated in nodes
nodesRef.on("child_added", function(snap) {
    console.log("Data check for", snap.key, snap.val());
    checkData(snap.val().macAddr, 
              snap.val().status, 
              snap.val().data, 
              snap.val().type);
});

nodesRef.on("value", function(snap) {
    snap.forEach((child) => {
    checkData(child.val().macAddr, 
              child.val().status, 
              child.val().data, 
              child.val().type);
    })
});

function checkData(macAddr, status, value, type) {
    data = document.getElementById(`${macAddr}-data`);
    if (status == -1) {
        switch(type) {
            case "Camera":
                data.innerHTML = "NO CONN";
                data.style.color = "Red";
                break;
            case "Glassbreak Sensor":
                data.innerHTML = "NO CONN";
                data.style.color = "Red";
                break;
            case "Door Sensor":
                data.innerHTML = "NO CONN";
                data.style.color = "Red";
                break;
            case "Temperature Sensor":
                data.innerHTML = "NO CONN";
                data.style.color = "Red";
                break;
            default:
                data.innerHTML = "null";
        }
    }
    if (status == 0) {
        switch(type) {
            case "Camera":
                data.innerHTML = "No detect";
                data.style.color = "Green";
                break;
            case "Glassbreak Sensor":
                data.innerHTML = "Silent";
                data.style.color = "Green";
                break;
            case "Door Sensor":
                data.innerHTML = "Closed";
                data.style.color = "Green";
                break;
            case "Temperature Sensor":
                data.innerHTML = `${value}&degF`;
                data.style.color = "#444";
                break;
            default:
                data.innerHTML = "null";
        }
    }
    if (status == 1) {
        switch(type) {
            case "Camera":
                data.innerHTML = "Detection";
                data.style.color = "Red";
                break;
            case "Glassbreak Sensor":
                data.innerHTML = "Noise";
                data.style.color = "Red";
                break;
            case "Door Sensor":
                data.innerHTML = "Open";
                data.style.color = "Red";
                break;
            case "Temperature Sensor":
                data.innerHTML = `${value}&degF`;
                data.style.color = "#444";
                break;
            default:
                data.innerHTML = "null";
        }
    }
}