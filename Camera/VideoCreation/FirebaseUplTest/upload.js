var videos = ["20", "19", "21"];
var select = document.getElementById("vidSel");
let btn1 = document.querySelector(".PlayBTN");

listAdd();

//var value = select.value;
//var text = select.options[select.selectedIndex].text;
//var vidRef = storageRef.child(text + '/');
//console.log(value, text);
var value = select.value;
var text = select.options[select.selectedIndex].text;
var vidRef = storageRef.child(text + '/');
select.onchange = onChange; 
onChange();



let i = 0;
btn1.addEventListener('click' ,e=>{
        vidRef.child('image'+ i + '.jpg').getDownloadURL().then((url)=> {
        document.querySelector('img').src = url; 
        //console.log(url);
    }).catch((error) => {       
}); 
   if( i < 149){  
    i++;
    setTimeout(() => { btn1.click(); },200);   
   }
   else {
    i = 0;
    console.log('video done');
   }
});


function listAdd() {
    for(var j = 0;j < videos.length; j++){
        var opt = videos[j];
        var el = document.createElement("option");
        el.textContent = opt;
        el.value = opt;
        select.appendChild(el);
        console.log(select[j]);
    }
}
   
    
function onChange() {
    var value = select.value;
    var text = select.options[select.selectedIndex].text;
    var vidRef = storageRef.child(text + '/');
    console.log(value, text);
  }
