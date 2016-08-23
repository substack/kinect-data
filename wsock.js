var http = require('http')
var server = http.createServer(function (req, res) {
  res.end('kinect websocket server\n')
})
server.listen(9000)

var pump = require('pump')
var spawn = require('child_process').spawn
var ps = spawn('./kdata', [])

var wsock = require('websocket-stream')
wsock.createServer({ server: server }, function (stream) {
  pump(ps.stdout, stream)
})
