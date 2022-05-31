function eventConnect(){
    if (!!window.EventSource) {
        var eventCon = new EventSource('/events');
        eventCon.onopen = onEventOpen;
        eventCon.onerror = onEventError;
        eventCon.onmessage = onEventMessage;
        eventCon.addEventListener('newdistance', updateDistance, false);
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
    document.getElementById('dist').innerHTML= "Distance: " + Number(event.data).toFixed(2);
}