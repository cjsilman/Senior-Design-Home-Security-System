var videos = [];
var images = [];
const select = document.getElementById("vidSel");
const container = document.getElementById("video");
const timerLabel = document.getElementById("timeLabel");

var txt;
var vidRef;
var startUp = true;

var frame = 0;
var playing = false;
var delay;
var timer;

const playBtn = document.getElementById("PlayBTN");
const deleteBtn = document.getElementById("DeleteVid");
const getFrameBtn = document.getElementById("getFrame");
const slider = document.getElementById("myRange");
const videoPlayer = document.querySelector("#video-player");
const videoControls = document.querySelector("#onVid-Controls");
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

playBtn.onclick = function(){
    if(playing == false) {
        play();
    }
    else {
        clearInterval(delay);
        pause();
    }
}

deleteBtn.onclick = function() {
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

getFrameBtn.onclick = function() {
    let stoppedImg = frame - 1;
    vidRef.child("image" + stoppedImg + ".jpg").getDownloadURL().then((photo) => {
        //console.log(photo);
        window.open(photo);
    })
}

function play() {
    //document.getElementById('image0').setAttribute("class","hiddenImages");
    document.getElementById("getFrame").style.visibility = 'hidden';
    playBtn.innerHTML = `<span id = "pausePlayIcon" class="fa fa-pause">`;
    playing = true;
    delay = setInterval(playSlides,95);

    timerLabel.style.visibility = "visible";
}

function pause() {
    document.getElementById("getFrame").style.visibility = 'visible';
    playBtn.innerHTML = `<span id = "pausePlayIcon" class="fa fa-play">`;
    
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
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
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
    document.getElementById('default').style.display= 'none';
    document.getElementById('DeleteVid').style.visibility = 'visible';
    document.getElementById('onVid-Controls').style.visibility = 'visible';
}

function loadImages() {
    let j = 0;

    setInterval(()=>{if( j < 150) {
        
        var temp = document.createElement("img");
        temp.setAttribute("class", "hiddenImages");
        temp.setAttribute("id", "image" + j);

        vidRef.child("image"+ j + ".jpg").getDownloadURL().then((url)=> { 
            temp.setAttribute("src", url);
            container.appendChild(temp);
        }).catch((err)=> {
            errorImgRef.getDownloadURL().then((url)=> { 
                temp.setAttribute("src", url);
                container.appendChild(temp);
            })
        }) 

       j++;

       if(j==70) {
        timerLabel.innerHTML = "0.0";
        if(playing == false) {
            document.getElementById('image0').setAttribute("class", "displayImages");
        }
        if (startUp == true) {
            document.getElementById('loading').setAttribute("class", "hiddenImages");
            startUp = false; 
        }
       }
      
      
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

// Mouse Enter/Leave Video Controls
videoPlayer.addEventListener("mouseenter", (e)=>{
    videoControls.style.opacity = "0.8";
})

videoPlayer.addEventListener("mouseleave", (e)=>{
    videoControls.style.opacity = "0.3";
})

