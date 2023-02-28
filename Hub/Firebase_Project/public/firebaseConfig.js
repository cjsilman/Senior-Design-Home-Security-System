const firebaseConfig = {
    apiKey: "AIzaSyAsZSHG1-VpOdG63fJtrgE_7OovbOgBmkQ",
    authDomain: "esp32-firebase-demo-b5b71.firebaseapp.com",
    databaseURL: "https://esp32-firebase-demo-b5b71-default-rtdb.firebaseio.com",
    projectId: "esp32-firebase-demo-b5b71",
    storageBucket: "esp32-firebase-demo-b5b71.appspot.com",
    messagingSenderId: "264488333647",
    appId: "1:264488333647:web:64f355250b4f6e9cfd953a"
  };

// Initialize Firebase
firebase.initializeApp(firebaseConfig);

var database = firebase.database();