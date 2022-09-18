
$(document).ready(function(){
        $('#Home').load("/status.html");
        $('#Config').load("/config.html");
        $('#Wifi').load("/wifi.html");
     
    });


//Tab Control
function openPage(pageName,elmnt,color) {
    let i, tabcontent, tablinks;
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

function saveIPConfig(){
    configJSON.WIFI_STA_CONFIG.DCHP_ENABLED = document.getElementById('DHCP').checked;
    if(document.getElementById('Static').checked){
        configJSON.WIFI_STA_CONFIG.IP_ADDR = document.getElementById('IP_addrText').value;
        configJSON.WIFI_STA_CONFIG.SUBNET = document.getElementById('SubnetText').value;
        configJSON.WIFI_STA_CONFIG.DEFAULT_GATEWAY = document.getElementById('GatewayText').value;
        configJSON.WIFI_STA_CONFIG.DNS_SERVER1 = document.getElementById('DNSText1').value;
        configJSON.WIFI_STA_CONFIG.DNS_SERVER2 = document.getElementById('DNSText2').value;
    }
    console.log("saving config")
    console.log(JSON.stringify(configJSON));
    postIPConfig(JSON.stringify(configJSON));
    getConfig();
}









// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();
window.addEventListener('load', onLoad);