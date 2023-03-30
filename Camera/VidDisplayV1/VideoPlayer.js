var videos = [];
var images = [];
var select = document.getElementById("vidSel");
var container = document.getElementById("video");

var txt;
var vidRef;
var startUp = true;
var pause = false;

var btn1 = document.querySelector(".PlayBTN");
var btn2 = document.querySelector(".PauseBTN");

videoList();

select.onchange = onChange;

btn1.addEventListener('click' ,e=>{
    playSelected();
});

btn2.addEventListener('click', e=> {
    pause = true;
});


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

     
     if(startUp == false) {
        container.innerHTML = "";
        
     }
     else {
        
        onStart();
        //startUp = false; 
     }
     
    loadImages();  
}


function onStart() {
    document.getElementById('default').style.display= 'none';
    document.getElementById('PlayBTN').style.visibility = 'visible';
    document.getElementById('PauseBTN').style.visibility = 'visible';
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
            //onsole.log(url);
            //console.log(temp);
            container.appendChild(temp)
            //container.innerHTML=`<img class="hiddenImgs" id = "image${i}" src = "${url}">`
       })
       j++;
      
      
   }},5);
}

    

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
