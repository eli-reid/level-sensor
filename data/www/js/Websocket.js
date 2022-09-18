

let gateway = `ws://${window.location.hostname}/ws`;

let websocket;

const cmdEnum={
    SAVE_CONFIG:0,
    SAVE_IP_CONFIG:1,
    GET_CONFIG:2,
    GET_WIFI:3,
    CON_WIFI:4
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
        websocket.onmessage = onMessage; 
        websocket.addEventListener("hello",function(e){
            console.log("hello", e.data);
        },false)
    }
}
function getConfig(){
    let cmd ="GET|" + cmdEnum.GET_CONFIG + "|tttt";
    sendcmd(cmd);
}

function postIPConfig(config){
   
    let cmd = "POST|" + cmdEnum.SAVE_IP_CONFIG + "|"+config;
    sendcmd(cmd);
}

function login(formData)
{
    JSON.stringify(formData)
    sendcmd("POST|loginRequest|" + JSON.stringify(formData));
}
function getWifiList(){
    const select = document.getElementById("Wifi_select");
    select.options.length = 1;
    document.getElementById("Wifi_select_label").innerHTML = "scanning..."
    sendcmd("GET|" +cmdEnum.GET_WIFI + "|GET" )
}