const https = require("https");
const fs = require("fs");
const path = require("path");
const express = require("express");

const dir = path.resolve(process.argv[2] ?? "./");

console.log("dir:", dir);

const app = express();
app.use((req, res, next) => {
  //Cross-Origin-Resource-Policy: cross-origin

  res.set("Cross-Origin-Opener-Policy", "same-origin");
  res.set("Cross-Origin-Resource-Policy", "cross-origin");
  res.set("Cross-Origin-Embedder-Policy", "require-corp");
  next();
});

console.log("serving from ", dir);
app.use(express.static(dir));

const options = {
  key: fs.readFileSync(path.join(__dirname, "key.pem")),
  cert: fs.readFileSync(path.join(__dirname, "cert.pem"))
};

// Serve...
const PORT = 3000;
console.log("Available on:");
console.log(`  https://localhost:${PORT}/blockworld.html`);
https.createServer(options, app).listen(PORT);
