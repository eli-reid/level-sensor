
$(document).ready(function(){
        $('#Home').load("/status.html");
        $('#Config').load("/config.html");
    });

var config={};

//Tab COntrol
function openPage(pageName,elmnt,color) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablink");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].style.backgroundColor = "";
    }
    document.getElementById(pageName).style.display = "block";
    elmnt.style.backgroundColor = color;
}

function onLoad(event) {
    eventConnect();
    wsConnect();
}
// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();
window.addEventListener('load', onLoad);





