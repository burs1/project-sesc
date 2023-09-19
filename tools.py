""" Utilities """

import random
import hashlib

def gen_random_id():
	""" Generates random sha256 hash for temporary UserConn id """

	return hashlib.sha256(f'{random.randint(1, 1<<256)}'.encode('utf-8')).hexdigest()
