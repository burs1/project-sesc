"""Main websocket instance script"""

import ssl
import tools
import asyncio
import threading
import websockets



class UserConn:
	""" User connection insance """

	def __init__(self, connection, ident:str):
		"""
			connection - websocket
			ident - unic temporary sha256 id

			#con:bool - registered flag
		"""

		self.connection = connection
		self.ident = ident

		self.connection_is_opened = True


	async def send_message(self, msg:str):
		""" Message sending """

		await self.connection.send(msg)


	async def close_conn(self):
		""" Gracefuly close websocket connection """

		self.connection_is_opened = False

		await self.connection.close()
		await self.connection.wait_closed()



class WsServer(threading.Thread):
	""" client interconnection ws server instance"""

	def __init__(self, host:str, port:int, msg_handler = None, ssl_context = None):
		"""
		Arguments:
			host, port - ip:port address
			connections - {temp_id:UserConn}

			msg_handler - function
		"""

		super().__init__(daemon = True)

		self.host = host
		self.port = port
		self.ssl_context = ssl_context

		self.msg_handler = msg_handler
		self.connections = {}

		self.task = None
		self.stopped = True


	async def process_messages(self, ident:str):
		""" Handle incoming messages """
		try:
			async for message in self.connections[ident].connection:
				if self.stopped:
					return

				await self.connections[ident].send_message(f"ECHO: {str(message)}")

		except websockets.exceptions.ConnectionClosedError:
			self.connections[ident].connection_is_opened = False


	async def register_connection(self, connection):
		""" new connection registration """

		if self.stopped:
			return

		ident = tools.gen_random_id()
		self.connections[ident] = UserConn(connection, ident = ident)

		await self.process_messages(ident)
		await self.connections[ident].close_conn()


	async def serve(self):
		""" Websocket server """
		print('starting')
		self.stopped = False

		kwargs = {"ws_handler":self.register_connection,
					"host":self.host,
					"port":self.port,
					}

		if self.ssl_context is not None:
			kwargs['ssl'] = self.ssl_context

		print(kwargs)


		try:
			async with websockets.serve(**kwargs):
				if self.stopped:
					return

				while True:
					await asyncio.sleep(3)

		except asyncio.exceptions.CancelledError:
			return


	async def start_serving_task(self):
		""" Running server as async task """

		self.task = asyncio.create_task(self.serve())
		await self.task


	def stop_task(self):
		""" Hard reset server """

		self.task.cancel()
		self.join()


	def run(self):
		"""Run the server thread"""

		asyncio.run(self.start_serving_task())


def start_server(host, port, use_ssl = False, ssl_certfile = None, ssl_keyfile = None):
	"""Use this for tests"""

	if use_ssl:
		ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
		ssl_context.load_cert_chain(ssl_certfile, keyfile=ssl_keyfile)

		server = WsServer(host, port, ssl_context = ssl_context)
	else:
		server = WsServer(host, port)

	asyncio.run(server.serve())

	# while server.stopped:
	# 	time.sleep(.05)

	#return server


#Testing

if __name__ == "__main__":
	start_server(**tools.load_server_config()) #asyncio.run(WsServer("127.0.0.1", "43758").serve())
