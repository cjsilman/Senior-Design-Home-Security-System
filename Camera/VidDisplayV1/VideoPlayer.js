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
var slider = document.getElementById("myRange");

videoList();

select.onchange = onChange;

slider.oninput = function() {
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

function play() {
    btn1.innerHTML = 'Pause';
    playing = true;
    delay = setInterval(playSlides,68);
}
function pause() {
    btn1.innerHTML = 'Play';
    playing = false;
    
    
}
function playSlides() {
    console.log(frame);
    if(frame > 0) {
        document.getElementById(`image${frame-1}`).setAttribute("class", "hiddenImages");
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    else {
        document.getElementById(`image${frame}`).setAttribute("class", "displayImages");
    }
    slider.value = frame;
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
    document.getElementById('default').style.display= 'none';
    document.getElementById('PlayBTN').style.visibility = 'visible';
    document.getElementById("slidecontainer").style.display = 'flex';
    startUp = false; 
}

function loadImages() {
    let j = 0;
    setInterval(()=>{if( j < 150) {
        
        var temp = document.createElement("img");
        temp.setAttribute("class", "hiddenImages");
        temp.setAttribute("id", "image" + j);
        vidRef.child("image"+ j + ".jpg").getDownloadURL().then((url)=> { 
            temp.setAttribute("src", url);
            container.appendChild(temp)
            //container.innerHTML=`<img class="hiddenImgs" id = "image${i}" src = "${url}">`
       })
       j++;
      
      
   }},5);
}

    
/*
function playSelected() {
   
   var i = 0;
   setInterval(()=> {if(i < 150) {
        if(i > 0) {
            //let j = i - 1;
            let previous = document.getElementById(`image${i-1}`);
            previous.setAttribute("class", "hiddenImages");
        }
        let current = document.getElementById(`image${i}`);
        current.setAttribute("class", "displayImages");
        i++;

    };},67);
   

   

  
}
*/
