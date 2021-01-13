const express = require('express');
const app = express();
const http = require('http');
const WebSocket = require('ws');
const path = require('path');
const server = http.createServer(app);
const socket = new WebSocket.Server({ server });
const PORT = 4000;

require('dns').lookup(require('os').hostname(), (err, add, fam) => {
	console.log(`Server IP Address : ${add}`);
});

socket.on('connection', (ws, req) => {
	const ip = req.socket.remoteAddress.slice(7);
	ws.on('message', (message) => {
		console.log(`Received: ${message}`);
		socket.clients.forEach((client) => {
			if (client != ws && client.readyState) {
				client.send(`[*] Server (Broadcast) : ${message}`);
			}
		});
		ws.send(`[*] Server : ${message}`);
	});

	ws.on('close', () => {
		console.log(`${ip} disconnected from server`);
	});

	console.log(`${ip} connected to server`);
	ws.send(`${ip} connected to server`);
});

server.listen(PORT, () => console.log(`Server running on port ${PORT}`));
