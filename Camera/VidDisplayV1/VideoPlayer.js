var videos = [];
var images = [];
var select = document.getElementById("vidSel");
var container = document.getElementById("video");

var txt;
var vidRef;
var startUp = true;

var frame = 0;
var playing = false;
var delay;


var btn1 = document.getElementById("PlayBTN");
var btn2 = document.getElementById("DeleteVid");
var slider = document.getElementById("myRange");
var current;

slider.value = 0;
videoList();

var startImage = document.getElementById("init");
loadingRef.getDownloadURL().then((link)=> {
  startImage.setAttribute("src",link);
  startImage.setAttribute("class","displayImages");

})


select.onchange = onChange;

slider.onchange = function() {
   clearInterval(delay);
   pause();
   
   

}
slider.oninput = function() {;
    clearInterval(delay);
    pause();
    document.getElementById(`image${frame}`).setAttribute("class", "hiddenImages");   
    frame = parseInt(slider.value);
    //frame = parseInt(slider.value);
    if(frame == 0) {
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    else {
    
        document.getElementById(`image${frame-1}`).setAttribute("class", "hiddenImages");
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
        document.getElementById(`image${frame+1}`).setAttribute("class", "hiddenImages");
    }
    if(frame > 149) {
        frame = 149;
    }

}

btn1.onclick = function(){
    if(playing == false) {
        //btn1.innerHTML = 'Pause';
        play();
    }
    else {
        clearInterval(delay);
        //btn1.innerHTML = 'Play';
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

function play() {
    document.getElementById('image0').setAttribute("class","hiddenImages");
    btn1.innerHTML = `<span id = "pausePlayIcon" class="fa fa-pause">`;
    playing = true;
    delay = setInterval(playSlides,68);
}
function pause() {
    btn1.innerHTML = `<span id = "pausePlayIcon" class="fa fa-play">`;
    
    playing = false;
    
    
}
function playSlides() {
    slider.value = frame;
    console.log(frame);
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
            //console.log(folderRef.name);
            select.appendChild(opt);
            //videos.push(folderRef.name);
            });
            //console.log(videos.length);
        }).catch(function(error){

        });
        txt = select.options[select.selectedIndex].text;
        vidRef = AllVidsRef.child(txt + '/');
        //console.log(txt);
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
    document.getElementById('init').setAttribute("class", "hiddenImages");
    document.getElementById('default').style.display= 'none';
    document.getElementById('PlayBTN').style.visibility = 'visible';
    document.getElementById('DeleteVid').style.visibility = 'visible';
    document.getElementById("slidecontainer").style.display = 'flex';
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
            //container.innerHTML=`<img class="hiddenImgs" id = "image${i}" src = "${url}">`
       })
       j++;
      
      
   }},5);
   
}

    
function pausePlay() {
    if(playing==true) {
        clearInterval(delay);
        //btn1.innerHTML = 'Play';
        pause();
    }
    else {
        play();
    }
}
