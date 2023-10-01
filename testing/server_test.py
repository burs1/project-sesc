
import pytest

@pytest.mark.parametrize("amount_of_connections", [1])
def test_registration(amount_of_connections:int, gen_nickname_fixture,
					create_server_fixture, gen_connections_fixture,
					gen_register_message_fixture, load_server_config_fixture):
	config = load_server_config_fixture

	server = create_server_fixture(config)
	connections = {gen_nickname_fixture():conn for conn in gen_connections_fixture(amount_of_connections, host=config['host'], port=config['port'])}


	for nick, conn in connections.items():
		conn.send(gen_register_message_fixture(nick))
		res = conn.recv().split('/')
		assert res[0] == '1'

	server.stop()

	#assert gen_nickname_fixture == gen_nickname_fixture

