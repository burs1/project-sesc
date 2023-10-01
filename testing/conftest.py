
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
def gen_nickname_fixture():
	def _gen_nickname_fixture() -> str:
		return hashlib.shake_256(bytes(random.randint(1, 1<<6))).hexdigest(5)

	return _gen_nickname_fixture


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
