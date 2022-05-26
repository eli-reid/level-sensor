
$(document).ready(function(){
        $('#Home').load("/status.html");
        $('#Config').load("/config.html");
    });
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var config={};
//Setup Event Listeners
if (!!window.EventSource) {
    var source = new EventSource('/events');
    source.addEventListener('open', function(e) {
        console.log("Events Connected");
        }, false);

    source.addEventListener('error', function(e) {
            if (e.target.readyState != EventSource.OPEN) {
                console.log("Events Disconnected");
            }
        }, false);

    source.addEventListener('message', function(e) {
        console.log("message", e.data);
        }, false);

    source.addEventListener('newdistance', function(e) {
        document.getElementById('dist').innerHTML= "Distance: " + Number(e.data).toFixed(2);
    }, false);
}
if (!!window.WebSocket){
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
    
}

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

// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();
window.addEventListener('load', onLoad);

function onOpen(event) {
    
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(function(){
            console.log('Trying to open a WebSocket connection...');
            websocket= new WebSocket(gateway);
    } , 2000);
}

function onMessage(event) {
    console.log(event.data); 
    
}

function onLoad(event) {
 
}

function sendcmd(cmd){ 
    websocket.send(cmd);
}

