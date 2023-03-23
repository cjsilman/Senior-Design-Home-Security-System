
const firebaseConfig = {
    apiKey: "AIzaSyCwasbPktOgE6gTIMXMlVcpc0aPidCtDgs",
    authDomain: "esp32-cam-photo-upload.firebaseapp.com",
    databaseURL: "https://esp32-cam-photo-upload-default-rtdb.firebaseio.com",
    projectId: "esp32-cam-photo-upload",
    storageBucket: "esp32-cam-photo-upload.appspot.com",
    messagingSenderId: "1099283553426",
    appId: "1:1099283553426:web:6dcb5fbce2d39f59f2da4b"
  };

  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);
  
  var database = firebase.database();
  var stor = firebase.storage();
 
  var storageRef = stor.ref();
  //var AllVidsRef = storageRef.child('videos/');
  
