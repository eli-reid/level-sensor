let configJSON="";
function eventConnect(){
    if (!!window.EventSource) {
        let eventCon = new EventSource('/events');
        eventCon.onopen = onEventOpen;
        eventCon.onerror = onEventError;
        eventCon.onmessage = onEventMessage;
        eventCon.addEventListener('newdistance', updateDistance, false);
        eventCon.addEventListener('config', updatedConfig, false);
    }
}

function onEventOpen(event){

}

function onEventMessage(event){

}

function onEventError(event){
    if (event.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
        setTimeout(function(){
            console.log('Trying to open a Events connection...');
            eventConnect();
        } , 2000);
    }
}

function updateDistance(event){
    
    document.getElementById('dist').innerText = "Distance: " + Number(event.data).toFixed(2);
}

function updatedConfig(event){
    console.log("we are getting a config event")
    console.log(event.data);
    configJSON = JSON.parse(event.data);
    document.getElementById('ipadd').innerText = configJSON.WIFI_STA_CONFIG.IP_ADDR
    document.getElementById('subnet').innerText = configJSON.WIFI_STA_CONFIG.SUBNET
    document.getElementById('gateway').innerText = configJSON.WIFI_STA_CONFIG.DEFAULT_GATEWAY
    document.getElementById('dns1').innerText = configJSON.WIFI_STA_CONFIG.DNS_SERVER1
    document.getElementById('dns2').innerText = configJSON.WIFI_STA_CONFIG.DNS_SERVER2
    document.getElementById('IP_addrText').value = configJSON.WIFI_STA_CONFIG.IP_ADDR;
    document.getElementById('SubnetText').value = configJSON.WIFI_STA_CONFIG.SUBNET;
    document.getElementById('GatewayText').value = configJSON.WIFI_STA_CONFIG.DEFAULT_GATEWAY;
    document.getElementById('DNSText1').value = configJSON.WIFI_STA_CONFIG.DNS_SERVER1;
    document.getElementById('DNSText2').value = configJSON.WIFI_STA_CONFIG.DNS_SERVER2;

    if(configJSON.WIFI_STA_CONFIG.DCHP_ENABLED){
        document.getElementById('DHCP').checked = true;
        document.getElementById('StaticIP').hidden=true;
        document.getElementById('DHCPIP').hidden=false;

    }
    else{
        document.getElementById('Static').checked = true;
        document.getElementById('StaticIP').hidden=false;
        document.getElementById('DHCPIP').hidden=true;

    }
}