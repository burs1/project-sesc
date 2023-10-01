""" Utilities """

import json
import random
import hashlib

def gen_random_id(length = 32):
	""" Generates random hash for temporary UserConn id """

	return hashlib.shake_256(f'{random.randint(1, 1<<256)}'.encode('utf-8')).hexdigest(length)


def load_server_config(filename:str = 'server_config.json') -> dict:
	""" Loads server constants (such as host, port, etc) from json file"""

	config_file = open(filename, 'r')

	config = json.load(config_file)
	config_file.close()

	return {'host':config['HOST'], 'port':config['PORT'],
			'use_ssl':config['use_ssl'], 'ssl_certfile':config['ssl_certfile'],
			'ssl_keyfile':config['ssl_keyfile']}


def decompose_request(message:str) -> dict:
	""" Decomposes message into simple dict stucture
		DEV VERSION
	"""

	message = message.split('/')
	return {'flag':message[0], 'subflag':message[1], "args":message[2::]}


def compose_responce(status_code:int, flag:str, subflag:str, args:tuple[str]) -> str:
	return f'{status_code}/{flag}/{subflag}/{"|".join(args)}'
