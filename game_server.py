"""

General server script

"""

import time
import tools
import websocket_server as ws_server_lib


class Player:
	def __init__(self, ident:str, connection_instance:ws_server_lib.UserConn, nickname:str):
		"""
			connection_instance:UserConn
		"""

		self.ident = ident
		self.connection_instance = connection_instance

		self.nickname = nickname

		self.last_package_time = time.time()


	async def game_data_handler(self, message:str) -> None:
		"""
			Processes incoming game data
		"""

		pass

	'''
	async def update_players_data(self):
		""" Consist's from two parts: 
			incoming data recieving
			outgoing data sending """
		pass
	'''



class Session:
	def __init__(self, session_id:int):
		"""
		self.players_connceted structure: {"ident":Player}
		"""

		self.session_id = session_id
		self.players_connceted = {}


	def get_players_data(self) -> dict:
		""" Collects all players game data
			Struct:
				{"ident":{data}}
		"""


	async def connect_player(self, player:Player) -> None:
		pass


	async def disconnect_player(self, player:Player) -> None:
		pass



class GameServer:
	def __init__(self, config = tools.load_server_config()):
		"""
		self.players = {'sender_id':Player}

		"""


		self.websocket_server = ws_server_lib.start_server(**config,
															msg_handler = self.message_handler)

		self.sessions = {}
		self.players = {}


	async def message_handler(self, message:str, sender_id:str) -> None:
		""" Decodes incoming data and process misc reqs """

		message = tools.decompose_request(message)

		if sender_id in self.players:
			self.players[sender_id].last_package_time = time.time()

		responce = (400, ())

		try:
			if message['flag'] == 'misc':
				if message['subflag'] == 'ping':
					responce = time.time()
				elif message['subflag'] == 'registration':
					responce = self.reg_user(args = message['args'], sender_id = sender_id)
		except Exception as e:
			responce = (400, ("Wrong arguments", str(e)))

		responce = tools.compose_responce(status_code=responce[0],
											flag=message['flag'],
											subflag=message['subflag'],
											args=responce[1])

		await self.websocket_server.connections[sender_id].send(responce)


	def reg_user(self, args:list, sender_id:str) -> tuple[int, tuple]:
		""" Registrates user to game server """

		if sender_id in self.players.keys():
			return (0, ('You are already registered',))

		user = Player(ident = tools.gen_random_id(6),
						connection_instance=self.websocket_server.connections[sender_id],
						nickname = args[0])

		self.players[sender_id] = user

		return (1, (sender_id,))


	def stop(self) -> None:
		""" Stops server """

		self.websocket_server.stop_server()

