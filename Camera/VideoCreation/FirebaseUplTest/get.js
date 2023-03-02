let input = document.querySelector("input");
let btn = document.querySelector("button");

btn.addEventListener('click' ,e=>{
    let input = document.querySelector("input").files[0];
    console.log(input);
    
    

    task.on('state_change',

    function progress(snap) {
        console.log(snap.bytesTransferred / snap.totalBytes * 100);
    },
    function error(err) {
        console.log("ERROR");
    },
    function completed() {
        getDownloadURL().then(url =>{
            let body = document.querySelector('body')
            body.innerHTML += `<img src={url} alt="">`
        });
    });
});
