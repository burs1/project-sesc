
import tools
import pytest
import random
import hashlib
import websocket
import game_server



@pytest.fixture(scope='session')
def load_server_config_fixture():
	#def _load_server_config_fixture(config_file='test_config.json'):
	return tools.load_server_config(filename = 'testing/test_config.json')

	#return _load_server_config_fixture



@pytest.fixture(scope="function")
def gen_randhash_fixture():
	def _gen_randhash_fixture(size:int = 12) -> str:
		return hashlib.shake_256(f'{random.randint(1, 1<<63)}'.encode()).hexdigest(size)

	return _gen_randhash_fixture


@pytest.fixture(scope='function')
def create_server_fixture():
	def _create_server_fixture(config):
		return game_server.GameServer(config = config)

	return _create_server_fixture


@pytest.fixture(scope='function')
def gen_connections_fixture():
	def _gen_connections_fixture(amount:int, host, port) -> list:
		connections = [websocket.WebSocket() for i in range(amount)]
		for conn in connections:
			conn.connect(f'ws://{host}:{port}')

		return connections

	return _gen_connections_fixture


@pytest.fixture(scope='function')
def gen_register_message_fixture():
	def _gen_register_message_fixture(nickname:str) -> str:
		return f'misc/registration/{nickname}'

	return _gen_register_message_fixture


@pytest.fixture(scope='function')
def gen_session_create_message_fixture():
	def _gen_session_create_message_fixture(session_name:str, max_players:int, password:str) -> str:
		return f'misc/create_session/{session_name}/{max_players}/{password}'

	return _gen_session_create_message_fixture


@pytest.fixture(scope='function')
def gen_get_sessions_list_message_fixture():
	def _gen_get_sessions_list_message_fixture() -> str:
		return f'misc/get_sessions_list'

	return _gen_get_sessions_list_message_fixture


@pytest.fixture(scope='function')
def gen_connect_to_session_message_fixture():
	def _gen_connect_to_session_message_fixture(session_id:str, password:str) -> str:
		return f'misc/connect_to_session/{session_id}/{password}'

	return _gen_connect_to_session_message_fixture

@pytest.fixture(scope='function')
def gen_disconnect_from_session_message_fixture():
	return 'misc/disconnect_from_session'
