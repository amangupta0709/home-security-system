const express = require('express')
const decryption = require('./aesDecrypt.js')

const app = express()

app.use(express.text())

app.get('/', function(request, response) {
  console.log('GET /')
  let html = `
    <html>
        <body>
            <h1>Hello world!</h1>
        </body>
    </html>`
  response.writeHead(200, {'Content-Type': 'text/html'})
  response.end(html)
})

app.post('/', function(request, response) {
  console.log('POST /')
  //console.log(request.body)
  res = decryption(request.body)

  //console.log("Decoded password: "+decoded)
  response.writeHead(200, {'Content-Type': 'text/html'})
  response.end(res)
})

const port = 3000
app.listen(port)
console.log(`Listening at http://localhost:${port}`)