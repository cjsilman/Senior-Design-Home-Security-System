var videos = [];
var images = [];
var select = document.getElementById("vidSel");
var container = document.getElementById("video");
var timerLabel = document.getElementById("timeLabel");

var txt;
var vidRef;
var startUp = true;

var frame = 0;
var playing = false;
var delay;
var timer;

var btn1 = document.getElementById("PlayBTN");
var btn2 = document.getElementById("DeleteVid");
var btn3 = document.getElementById("getFrame");
var slider = document.getElementById("myRange");
var lastImage = 0;

slider.value = 0;
videoList();

var startImage = document.createElement("img");
startImage.setAttribute("id", "loading");
loadingRef.getDownloadURL().then((link)=> {
  startImage.setAttribute("src",link);
  startImage.setAttribute("class","displayImages");
  container.appendChild(startImage);
})


select.onchange = onChange;

slider.onchange = function() {
   clearInterval(delay);     
   pause();
}


slider.oninput = function() {
    
    var oldFrame = frame;
    clearInterval(delay);
    pause();
    document.getElementById(`image${lastImage}`).setAttribute("class", "hiddenImages");   
    frame = parseInt(slider.value);
    timer = frame * 0.095;
    if(timer < 1) {
        timerLabel.innerHTML = `${Number(timer.toPrecision(1))}`;
    }
    else if(timer > 10) {
        timerLabel.innerHTML = `${Number(timer.toPrecision(3))}`;
    }
    else {
        timerLabel.innerHTML = `${Number(timer.toPrecision(2))}`;
    }
    if(frame == 0) {
        document.getElementById(`image${oldFrame}`).setAttribute("class", "hiddenImages");
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    else {
    
        document.getElementById(`image${oldFrame}`).setAttribute("class", "hiddenImages");
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
        
    }
    if(frame > 149) {
        frame = 149;
    }
    
}

btn1.onclick = function(){
    if(playing == false) {
        play();
    }
    else {
        clearInterval(delay);
        pause();
    }
}

btn2.onclick = function() {
    var result = confirm("Delete Video?");
    if(result) {
        for(let z =0; z < 150; z++) {
        vidRef.child("image" + z + ".jpg").delete().then(() => {
            console.log("image " + z + " deleted");
        }).catch((error) => {

        })
    }
    }
    select.innerHTML = "";
    setTimeout(()=>{videoList();},1000);
}

btn3.onclick = function() {
    let stoppedImg = frame - 1;
    vidRef.child("image" + stoppedImg + ".jpg").getDownloadURL().then((photo) => {
        //console.log(photo);
        window.open(photo);
    })
}

function play() {
    document.getElementById('image0').setAttribute("class","hiddenImages");
    document.getElementById("getFrame").style.visibility = 'hidden';
    btn1.innerHTML = `<span id = "pausePlayIcon" class="fa fa-pause">`;
    playing = true;
    delay = setInterval(playSlides,95);
}

function pause() {
    document.getElementById("getFrame").style.visibility = 'visible';
    btn1.innerHTML = `<span id = "pausePlayIcon" class="fa fa-play">`;
    
    playing = false;
    
    
}
function playSlides() {
    slider.value = frame;
    lastImage = frame;
    //console.log(frame);
    timer = frame * 0.095;
    if(timer < 1) {
        timerLabel.innerHTML = `${Number(timer.toPrecision(1))}`;
    }
    else if(timer > 10) {
        timerLabel.innerHTML = `${Number(timer.toPrecision(3))}`;
    }
    else {
        timerLabel.innerHTML = `${Number(timer.toPrecision(2))}`;
    }
    if(frame > 0) {
        document.getElementById(`image${frame-1}`).setAttribute("class", "hiddenImages");
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    else {
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    
    
    frame = frame + 1;
    
    if(frame > 149) {
        document.getElementById(`image${frame-1}`).setAttribute("class", "hiddenImages");
        frame = 0;
    }
}

function videoList() {
    AllVidsRef.listAll().then((result)=>{
        result.prefixes.forEach((folderRef) => {   
            var opt = document.createElement("option");
            opt.textContent = folderRef.name;
            select.appendChild(opt);
            });
        }).catch(function(error){

        });
        txt = select.options[select.selectedIndex].text;
        vidRef = AllVidsRef.child(txt + '/');
}

function onChange() {
    
    frame = 0;
    value = select.value;
     txt = select.options[select.selectedIndex].text;
     vidRef = AllVidsRef.child(txt + '/');
     console.log(txt);

     
     if(startUp == false) {
        container.innerHTML = "";
        
     }
     else {
        
        onStart();
         
     }
     
    loadImages();  
}


function onStart() {
    document.getElementById('loading').setAttribute("class", "hiddenImages");
    document.getElementById('default').style.display= 'none';
    document.getElementById('DeleteVid').style.visibility = 'visible';
    document.getElementById('onVid-Controls').style.visibility = 'visible';
    startUp = false; 
}

function loadImages() {
    let j = 0;
    setInterval(()=>{if( j < 150) {
        
        var temp = document.createElement("img");
        if(j==0) {
            temp.setAttribute("class","displayImages")
        }
        else {
        temp.setAttribute("class", "hiddenImages");
        }
        temp.setAttribute("id", "image" + j);
        vidRef.child("image"+ j + ".jpg").getDownloadURL().then((url)=> { 
            temp.setAttribute("src", url);
            container.appendChild(temp)
       })
       j++;
      
      
   }},5);
   
}

    
function pausePlay() {
    if(playing==true) {
        clearInterval(delay);
        pause();
    }
    else {
        play();
    }
}


