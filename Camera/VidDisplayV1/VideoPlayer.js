var videos = [];
var images = [];
var select = document.getElementById("vidSel");
var hidden = document.getElementById("storedImgs");

var txt;
var vidRef;
var startUp = true;


videoList();
select.onchange = onChange;




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
    

    value = select.value;
     txt = select.options[select.selectedIndex].text;
     vidRef = AllVidsRef.child(txt + '/');
     console.log(txt);
     if(startUp = true) {
        setTimeout(()=>{onStart();},100);
        startUp = false; 
     }
    loadImages();  
}


function onStart() {
    document.getElementById('default').style.display= 'none';
    document.getElementById('PlayBTN').style.visibility = 'visible';
    document.getElementById('PauseBTN').style.visibility = 'visible';
}

function loadImages() {
    images = [];
    vidRef.listAll().then((res)=> {
        res.items.forEach((imgRef)=>{
            images.push(imgRef.name);
        });
    });
    
    console.log(images);
    setTimeout(()=>{toWebpage();},200);
    
}

function toWebpage() {
    
    console.log(images.length);
    var i = 0;
    while(i < images.length) {
        var temp = document.createElement("img");
        temp.setAttribute("id", images[i]);
        temp.setAttribute("height",400);
        temp.setAttribute("width",400);
        vidRef.child(images[i]).getDownloadURL().then((url)=> { 
             temp.setAttribute("src", url);
             console.log(url);
             console.log(temp);
             hidden.appendChild(temp);
        })
        i++;
       
    }
}