
import time
import pytest
import logging

LOGGER = logging.getLogger(__name__)

@pytest.mark.parametrize("amount_of_connections", [1, 3, 5])#, 100, 500])
def test_registration(amount_of_connections:int, gen_randhash_fixture,
					create_server_fixture, gen_connections_fixture,
					gen_register_message_fixture, load_server_config_fixture):
	config = load_server_config_fixture

	server = create_server_fixture(config)
	connections = {gen_randhash_fixture():conn for conn in gen_connections_fixture(amount_of_connections, host=config['host'], port=config['port'])}
	assert len(connections.keys()) == amount_of_connections

	id_nick = {}

	LOGGER.info("Registrating players")

	for nick, conn in connections.items():
		conn.send(gen_register_message_fixture(nick))
		res = conn.recv().split('/')
		assert res[0] == '1'

		id_nick[res[-1]] = nick

	# nicknames_and_ids = {server.players[player].ident:server.players[player].nickname for player in server.players.keys()}

	LOGGER.info("Checking registration")

	for ident, nick in id_nick.items():
		assert ident in server.players
		assert server.players[ident].nickname == nick

	server.stop()

	#assert gen_randhash_fixture == gen_randhash_fixture

@pytest.mark.parametrize("amount_of_sessions, connections_per_session", [(1, 1), (2, 2)])#, (15, 10), (20, 20)])
def test_sessions(amount_of_sessions:int, connections_per_session:int,
				gen_randhash_fixture, create_server_fixture,
				gen_connections_fixture, gen_register_message_fixture,
				load_server_config_fixture, gen_session_create_message_fixture,
				gen_get_sessions_list_message_fixture, gen_connect_to_session_message_fixture,
				gen_disconnect_from_session_message_fixture):

	config = load_server_config_fixture
	server = create_server_fixture(config)

	main_connection = gen_connections_fixture(1, host=config['host'], port=config['port'])[0]
	sessions = {} # {session_id:[active_connections]}
	password = gen_randhash_fixture(8)

	LOGGER.info("Sessions creation")

	for i in range(amount_of_sessions):
		session_name = gen_randhash_fixture()
		main_connection.send(gen_session_create_message_fixture(session_name, connections_per_session, password))
		res = main_connection.recv().split('/')
		# print(res)

		assert res[0] == '1'

		sessions[res[-1]] = []


	main_connection.send(gen_get_sessions_list_message_fixture())
	res = main_connection.recv().split('/')

	assert res[0] == '1'

	result_amount = int(res[3])
	res = res[4::]

	assert result_amount == amount_of_sessions

	LOGGER.info("Sessions created")


	connections = []
	for i in range(amount_of_sessions):
		conns = {gen_randhash_fixture():conn for conn in gen_connections_fixture(connections_per_session, host=config['host'], port=config['port'])}
		assert len(conns) == connections_per_session
		connections.append(conns)

	LOGGER.info("Registrating and connecting players to sessions")

	for i in range(amount_of_sessions):
		session_id = sorted(list(sessions.keys()))[i]
		for nick, conn in connections[i].items():
			conn.send(gen_register_message_fixture(nick))
			res = conn.recv().split('/')
			print(res)
			assert res[0] == '1'

			conn.send(gen_connect_to_session_message_fixture(session_id, 'wrong_password'))
			res = conn.recv().split('/')
			assert res[0] == '0'

			conn.send(gen_connect_to_session_message_fixture(session_id, password))
			res = conn.recv().split('/')
			print(res)
			assert res[0] == '1'

	LOGGER.info("Checking amount of connected players")
	for x, instance in server.sessions.items():
		assert len(instance.connected_players.keys()) == connections_per_session

	LOGGER.info("Disconnecting players")

	for i in range(amount_of_sessions):
		session_id = sorted(list(sessions.keys()))[i]
		for nick, conn in connections[i].items():
			conn.send(gen_disconnect_from_session_message_fixture)
			res = conn.recv().split('/')
			print(res)
			assert res[0] == '1'

	LOGGER.info("Checking disconnection")

	for x, instance in server.sessions.items():
		assert len(instance.connected_players.keys()) == 0

	server.stop()

	# for i in range(amount_of_sessions):
	# 	for player in connections[i].keys():
	# 		sessions[sessions.keys()[i]].append(player)
	# 		connections[i][player].send()


@pytest.mark.parametrize("amount_of_connections", [1, 2, 5, 10, 100])#, 500])
def test_data_exchange(amount_of_connections:int,
				gen_randhash_fixture, create_server_fixture,
				gen_connections_fixture, gen_register_message_fixture,
				load_server_config_fixture, gen_session_create_message_fixture,
				gen_get_sessions_list_message_fixture, gen_connect_to_session_message_fixture,
				gen_disconnect_from_session_message_fixture, decompose_response_fixture,
				gen_player_data_fixture, gen_data_exchange_message_fixture):

	config = load_server_config_fixture
	server = create_server_fixture(config)
	main_connection = gen_connections_fixture(1, host=config['host'], port=config['port'])[0]
	main_connection.send(gen_register_message_fixture("test"))

	res = main_connection.recv().split('/')
	assert res[0] == '1'


	LOGGER.info("Session creation")

	session_name = gen_randhash_fixture()
	password = gen_randhash_fixture(4)
	main_connection.send(gen_session_create_message_fixture(session_name, amount_of_connections + 5, password))

	res = main_connection.recv().split('/')
	assert res[0] == '1'
	session_id = res[-1]

	print(res, session_id)


	main_connection.send(gen_connect_to_session_message_fixture(session_id, password))
	res = main_connection.recv().split('/')
	# print(res, session_id)
	assert res[0] == '1'


	connections = {gen_randhash_fixture(12):conn for conn in gen_connections_fixture(amount_of_connections, host=config['host'], port=config['port'])}

	for nick, conn in connections.items():
		conn.send(gen_register_message_fixture(nick))
		res = conn.recv().split('/')
		# print(res)
		assert res[0] == '1'

		conn.send(gen_connect_to_session_message_fixture(session_id, password))
		res = conn.recv().split('/')
		print(res, session_id)
		assert res[0] == '1'


	data = {i:gen_player_data_fixture(i) for i in connections.keys()}
	# data_message = f'{amount_of_connections}' + '/'.join(data.values())

	for ident, conn in connections.items():
		conn.send(gen_data_exchange_message_fixture(data[ident]))

		res = decompose_response_fixture(conn.recv())
		print(res)
		assert res['status_code'] == 1


	LOGGER.info("Requesting players data")
	main_connection.send("game/get_players_data")

	res = decompose_response_fixture(main_connection.recv())
	print(res)
	assert res['status_code'] == 1

	LOGGER.info("Checking players data")
	for ident, info in data.items():
		assert info in res['args']

	server.stop()
