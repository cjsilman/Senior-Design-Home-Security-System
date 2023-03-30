var videos = [];
var images = [];
var select = document.getElementById("vidSel");
let btn1 = document.querySelector(".PlayBTN");
var beginProcess = false;
var beginDisplay = false;
var refRetrieved = false;

videosNameRetrieve();

//console.log(typeof videos);
//console.log(videos);
//console.log("Video length is " + videos.length);

setTimeout(()=>{listAdd();},200);


if(beginProcess = true) {
    var value = select.value;
    var txt = select.options[select.selectedIndex].text;
    var  vidRef = AllVidsRef.child(txt + '/');
    refRetrieved = false;
    vidRef.listAll().then((result)=> {
        result.prefixes.forEach((imageRef) => {
            images.push(imageRef.name);
        });
     }).catch(function(error){

     });

    select.onchange = onChange; 
    onChange();


     while(refRetrieved=false) {
        console.log("loading...");
     };

        let i = 0;
        btn1.addEventListener('click' ,e=>{
                vidRef.child('image'+ i + '.jpg').getDownloadURL().then((url)=> {
                document.querySelector('img').src = url; 
                //console.log(url);
            }).catch((error) => {       
        }); 
         if( i < images.length){  
            i++;
            setTimeout(() => { btn1.click(); },200);   
        }
         else {
         i = 0;
         console.log('video done');
   }
});

    
};

function listAdd() {
    for(var j = 0;j < videos.length; j++){
        var opt = videos[j];
        var el = document.createElement("option");
        el.textContent = opt;
        el.value = opt;
        select.appendChild(el);
        //console.log(select[j]);
    }
    beginProcess = true;
}
   
    
function onChange() {
     value = select.value;
<<<<<<< HEAD
     txt = select.options[select.selectedIndex].text;
     vidRef = AllVidsRef.child(txt + '/');
     retImageNumber();
=======
     text = select.options[select.selectedIndex].text;
     vidRef = storageRef.child(text + '/');
    console.log(value, text);
>>>>>>> 5ffe4331dc2dbf675a82e0e209bfa6f7ce9226b5
  }

function videosNameRetrieve() {

    //Retrieve "Video" folder name and add to video array as a string
    
    AllVidsRef.listAll().then((result)=>{
    result.prefixes.forEach((folderRef) => {   
        videos.push(folderRef.name);
        
        });
    }).catch(function(error){

    });
   
  }

  function retImageNumber() {
    vidRef.listAll().then((result)=> {
        result.prefixes.forEach((imageRef) => {
            images.push(imageRef.name);
        });
     }).catch(function(error){

     });
     refRetrieved = true;
     console.log(images);
  }