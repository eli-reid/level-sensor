let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

function onOpen(event) {
    
    console.log('Connection opened '+ event);
   
}

function onClose(event) {
    console.log('Connection closed' + event);
    setTimeout(function(){
            console.log('Trying to open a WebSocket connection...');
            wsConnect();
    } , 2000);
}

function onMessage(event) {
    console.log(event.data); 
    
}

function sendcmd(cmd){ 
    websocket.send(cmd);
}

function wsConnect(){
    if (!!window.WebSocket){
        console.log('Trying to open a WebSocket connection...');
        websocket = new WebSocket(gateway);
        websocket.onopen    = onOpen;
        websocket.onclose   = onClose;
        websocket.onmessage = onMessage; // <-- add this line
        websocket.addEventListener("hello",function(e){
            console.log("hello", e.data);
        },false)
    }
}
function getConfig(){
    let cmd ="GET|getConfig|tttt";
    sendcmd(cmd);
}

function saveConfig(config){
   
    let cmd = "POST|saveConfig|"+config;
    sendcmd(cmd);
}

function login(formData)
{
    JSON.stringify(formData)
    sendcmd("POST|loginRequest|" + JSON.stringify(formData));
}
