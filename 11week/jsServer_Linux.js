// 11월 14일 목요일
// Node.js 기반 TCP 서버 구현

var net = require('net'); // net 모듈을 불러와 TCP 서버 기능 사용
var cntClient = 0; // 연결된 클라이언트 수를 카운트하기 위한 변수

// TCP 서버 생성 (클라이언트 연결 시 호출되는 콜백 함수 지정)
var server = net.createServer(function(client){
    
    // 새로운 클라이언트가 연결되었을 때 실행되는 부분
    console.log("Client " + cntClient++ +" "+ client.address().address + " connected. ");
    
    // 클라이언트로부터 데이터가 수신되었을 때 실행되는 이벤트 핸들러
    client.on('data', function(data){

        // 수신된 데이터를 문자열로 변환하여 콘솔에 출력
        console.log("Received from client: " + data.toString());
    });

    // 클라이언트와의 연결이 종료되었을 때 실행되는 이벤트 핸들러
    client.on('close', function(){
        console.log('client disconned.');
    });

});

// 서버를 특정 포트에서 리스닝 시작 (클라이언트의 연결 요청 대기)
// 여기서는 포트 번호 50001번을 사용
server.listen(50001);