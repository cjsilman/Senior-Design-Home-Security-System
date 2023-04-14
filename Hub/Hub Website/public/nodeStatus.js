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
            child.key,
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

function checkData(key, macAddr, status, value, type) {
    let data = document.getElementById(`${macAddr}-data`);
    let bat = document.getElementById(`${macAddr}-bat`);
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
    //Bat Voltage
    if (status == 5) {
        switch(type) {
            case "Camera":
                calculateBatIcon(key, value, bat);
                break;
            case "Glassbreak Sensor":
                calculateBatIcon(key, value, bat);
                break;
            case "Door Sensor":
                calculateBatIcon(key, value, bat);
                break;
            case "Temperature Sensor":
                calculateBatIcon(key, value, bat);
                break;
            default:
                bat.innerHTML = "null";
        }
    }
}

function calculateBatIcon(key, batValue, nodeObject) {
    nodeObject.innerHTML = ` <span style="color:#333; font-size:12px;">${batValue}%</span>`;
    if(batValue >= 90) {
        
        nodeObject.setAttribute('class', 'fa fa-battery-full');
        nodeObject.setAttribute('style', 'color:#333;');
    } 
    else if (batValue < 90 && batValue >= 75) {
        nodeObject.setAttribute('class', 'fa fa-battery-three-quarters');
        nodeObject.setAttribute('style', 'color:#333;');
    } 
    else if (batValue < 75 && batValue >= 50) {
        nodeObject.setAttribute('class', 'fa fa-battery-half');
        nodeObject.setAttribute('style', 'color:#333;');
    } 
    else if (batValue < 50 && batValue >= 8) {
        nodeObject.setAttribute('class', 'fa fa-battery-quarter');
        nodeObject.setAttribute('style', 'color:#333;');
    } 
    else if (batValue < 8 && batValue >= 5) {
        nodeObject.setAttribute('class', 'fa fa-battery-empty');
        nodeObject.setAttribute('style', 'color:orange;');
    }
    else {
        nodeObject.setAttribute('class', 'fa fa-battery-empty');
        nodeObject.setAttribute('style', 'color:red;');
    }

    let databaseNode = database.ref(`nodes/${key}`);
    databaseNode.update({batLvl: batValue});
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