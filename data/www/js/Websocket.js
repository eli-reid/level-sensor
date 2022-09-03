let gateway = `ws://${window.location.hostname}/ws`;
let websocket;
const cmdEnum={
    saveConfig_e:0,
    getConfig_e:1,
    getWIFI_e:3,
    connectWIFI_e:4
};
Object.freeze(cmdEnum);
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
    let cmd ="GET|" + cmdEnum.getConfig_e + "|tttt";
    sendcmd(cmd);
}

function saveConfig(config){
   
    let cmd = "POST|" + cmdEnum.saveConfig_e + "|"+config;
    sendcmd(cmd);
}

function login(formData)
{
    JSON.stringify(formData)
    sendcmd("POST|loginRequest|" + JSON.stringify(formData));
}
