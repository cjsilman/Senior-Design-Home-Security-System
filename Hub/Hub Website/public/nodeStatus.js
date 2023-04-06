// Nodes Database
var nodesRef = database.ref('nodes');
var notifRef = database.ref('server/notif');

//Whenever something is updated in nodes
nodesRef.on("child_added", function(snap) {
    console.log("Data check for", snap.key, snap.val());
});

nodesRef.on("value", function(snap) {
    snap.forEach((child) => {
    checkData(
            child.val().macAddr, 
            child.val().status, 
            child.val().data, 
            child.val().type);
    addToNotificationLog(
                child.key,
                child.val().macAddr, 
                child.val().status, 
                child.val().name, 
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
    if (status == 1) {
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
    if (status == 2 || status == 3) {
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

function addToNotificationLog(key, i_macAddr, i_status, i_name, i_value, i_type) {
    const datetime = new Date();
    date = datetime.toDateString();
    time = datetime.toLocaleTimeString();
    if(i_status == 2) {
        var updates = {};
        updates[key + '/' + 'status/' + 3];
        nodesRef.child(key).update({status: 3});

        notifRef.get().then((snapshot)=>{
            notifRef.update({number: snapshot.val().number+1, state: true});
        });

        

        if(i_type != "Temperature Sensor")
        {
            database.ref("notificationlog/" + date + "/" + time).set({
                macAddr: i_macAddr,
                type: i_type,
                message: `The device named "${i_name}" has detected something.`,
                new: true
            });
        }
        else
        {
            database.ref("notificationlog/" + date + "/" + time).set({
                macAddr: i_macAddr,
                type: i_type,
                message: `The device named "${i_name}" alarmed at a temperature of ${i_value}&degF`,
                new: true
            });
        }
        
    }
}