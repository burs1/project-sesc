""" Utilities """

import json
import random
import hashlib

def gen_random_id():
	""" Generates random sha256 hash for temporary UserConn id """

	return hashlib.sha256(f'{random.randint(1, 1<<256)}'.encode('utf-8')).hexdigest()


def load_server_config(filename:str = 'server_config.json'):
	""" Loads server constants (such as host, port, etc) from json file"""

	config_file = open(filename, 'r')

	config = json.load(config_file)
	config_file.close()

	return {'host':config['HOST'], 'port':config['PORT'],
			'use_ssl':config['use_ssl'], 'ssl_certfile':config['ssl_certfile'],
			'ssl_keyfile':config['ssl_keyfile']}
