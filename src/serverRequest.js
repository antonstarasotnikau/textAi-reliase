document.addEventListener("DOMContentLoaded", function () {

    const sendButton = document.getElementById('send'); // button send
    sendButton.addEventListener('click', function () {
        let payload = 'name=' + encodeURIComponent(document.getElementById('value').value);
        const regExp = /[^(\d+(.\d+)?)]/g;

        const request = new XMLHttpRequest();
        request.open('POST', 'http://34.116.180.219:8080/cgi-bin/script.cgi', true);

        request.addEventListener('readystatechange', function () {
            console.log(parseInt(request.responseText.replace(regExp, '')));
            document.querySelector("#img").classList.add('opacity');
            document.querySelector("#result").innerHTML = `${parseInt(request.responseText.replace(regExp, ''))} % unique`;
        });

        request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        request.send(payload);
    })
})
