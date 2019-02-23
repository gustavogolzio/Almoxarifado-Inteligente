import json

from flask import Flask, render_template, redirect, request
from flask_socketio import SocketIO, send, emit
from flask_cors import CORS

app = Flask(__name__)
socketio = SocketIO(app)
CORS(app)


estado = {
	"TEMP" : 0,
	"PIR" : 0,
	"LED01" : 0,
	"LED02" : 0
}

change=0

@app.route("/")
def rota_inicial():
	print(estado)
	return render_template("index.html")

@app.route("/login")
def rota_login():
	print(estado)
	return render_template("login.html")

@app.route("/estoque")
def rota_estoque():
	print(estado)
	return render_template("estoque.html")

@app.route("/colaboradores")
def rota_colaboradores():
	print(estado)
	return render_template("colaboradores.html")
	
@app.route("/upload", methods=["POST"])
def rota_data():
	global estado
	estado = request.get_json()
	socketio.emit("atualiza",estado)
	return "200"

@app.route("/download", methods=["GET"])
def rota_download():
	global estado
	global change
	if change==1:
		change=0
		return json.dumps(estado)
	return"1"

@socketio.on('fechadura_destravar')
def fechadura_destravar():
	global estado
	global change
	change=1;
	estado["LED01"] = 1

@socketio.on('ligar_luz')
def ligar_luz():
	global estado
	global change
	change=1;
	estado["LED02"] = 1
	
@socketio.on('desligar_luz')
def desligar_luz():
	global estado, change
	global change
	change=1;
	estado["LED02"] = 0
