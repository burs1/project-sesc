"""

General server script

"""

import time
import tools
import websocket_server as ws_server_lib


class Player:
	def __init__(self, connection_instance:ws_server_lib.UserConn, nickname:str):
		"""
			connection_instance:UserConn
		"""

		self.connection_instance = connection_instance
		self.ident = connection_instance.ident

		self.nickname = nickname

		self.last_package_time = time.time()
		self.session_id = None


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
	def __init__(self, ident:str, name:int, max_players:int, password:str = ''):
		"""
		self.connected_players structure: {"ident":Player}
		"""

		self.ident = ident
		self.name = name
		self.max_players = max_players
		self.password = password
		self.password_required = bool(password)

		self.connected_players = {}



	def get_players_data(self) -> dict:
		""" Collects all players game data
			Struct:
				{"ident":{data}}
		"""


	def connect_player(self, player:Player) -> bool:
		""" Connects player if limit is not exceeded """
		if len(self.connected_players.keys()) >= self.max_players:
			return False

		self.connected_players[player.ident] = player
		player.session_id = self.ident
		return True


	def disconnect_player(self, player:Player) -> None:
		""" Disconnect player from sessison """

		if player.ident in self.connected_players.keys():
			del self.connected_players[player.ident]

		player.session_id = None


class GameServer:
	def __init__(self, config = tools.load_server_config()):
		"""
		self.players = {'sender_id':Player}
		self.sessions = {'session_internal_id':Session}

		"""


		self.websocket_server = ws_server_lib.start_server(**config,
															msg_handler = self.message_handler)

		self.sessions = {}
		self.players = {}

		self.misc_handlers = {'ping':self.__ping, 'registration':self.__registration,
						'get_sessions_list':self.__get_sessions_list,
						'create_session':self.__create_session, 'connect_to_session':self.__connect_to_session,
						'disconnect_from_session':self.__disconnect_from_session}


	async def message_handler(self, message:str, sender_id:str) -> None:
		""" Decodes incoming data and process misc reqs """

		message = tools.decompose_request(message)

		if sender_id in self.players:
			self.players[sender_id].last_package_time = time.time()

		response = (400, tuple())

		try:
			if message['flag'] == 'misc':
				# if message['subflag'] == 'ping':
				response = self.misc_handlers.get(message['subflag'], self.__wrong_subflag)(args = message['args'],
																							sender_id = sender_id)
				# elif message['subflag'] == 'registration':
				# 	response = self.__registration(args = message['args'], sender_id = sender_id)
			elif message['flag'] == 'game':
				pass
		except Exception as e:
			response = (400, ("Wrong arguments", f"debug: {e}"))

		response = tools.compose_response(status_code=response[0],
											flag=message['flag'],
											subflag=message['subflag'],
											args=response[1])

		await self.websocket_server.connections[sender_id].send(response)


	def __wrong_subflag(self, **args):
		""" Wrong subflag error response """
		return (400, ("Wrong subflag",))


	def __ping(self, **args) -> tuple[int, tuple]:
		""" Ping response """
		return (200, (time.time(),))


	def __registration(self, args:list, sender_id:str) -> tuple[int, tuple]:
		""" Registrates user to game server """

		if sender_id in self.players.keys():
			return (0, ('You are already registered',))

		user = Player(#ident = sender_id, #tools.gen_random_id(12),
						connection_instance=self.websocket_server.connections[sender_id],
						nickname = args[0])

		self.players[sender_id] = user

		return (1, (sender_id,))


	def __get_sessions_list(self, args:list, sender_id:str) -> tuple[int, tuple]:
		""" Returns sessions list """

		sessions_data = []
		for key in self.sessions.keys():
			x = self.sessions[key]
			sessions_data.append(f'{x.name}|{key}|{len(x.connected_players.keys())}|{x.max_players}|{int(x.password_required)}')

		return (1, (len(sessions_data), *sessions_data))


	def __create_session(self, args:list, sender_id:str) -> tuple[int, tuple]:
		""" Creates session """

		if len(args) != 3:
			return (0, ("Incorrect amount of arguments",))

		if len(args[0]) == 0:
			return (0, ("Session name is empty string",))

		session_id = tools.gen_random_id()
		new_session = Session(session_id, args[0], int(args[1]), args[2])

		self.sessions[session_id] = new_session

		return (1, (session_id,))


	def __connect_to_session(self, args:list, sender_id) -> tuple[int, tuple]:
		""" Connect player to session """

		if sender_id not in self.players:
			return (0, ("You are not registered",))

		if self.players[sender_id].session_id is not None:
			return (0, ("You are already connected to session with id",))

		if args[0] not in self.sessions.keys():
			return (0, ("There is not session with such id",))

		if self.sessions[args[0]].password_required:
			if len(args) == 1 or args[1] != self.sessions[args[0]].password:
				return (0, ("Wrong password",))

		if not self.sessions[args[0]].connect_player(self.players[sender_id]):
			return (0, ("Session overflow",))


		return (1, tuple())

	def __disconnect_from_session(self, args:list, sender_id) -> tuple[int, tuple]:
		""" Disconnect player from session """

		if sender_id not in self.players:
			return (0, ("You are not registered",))

		if self.players[sender_id].session_id is None:
			return (1, tuple())

		self.sessions[self.players[sender_id].session_id].disconnect_player(self.players[sender_id])
		return (1, tuple())



	def stop(self) -> None:
		""" Stops server """

		self.websocket_server.stop_server()


if __name__ == "__main__":
	serv = GameServer()
	serv.websocket_server.join()

