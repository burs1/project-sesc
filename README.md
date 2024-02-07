# Project-sesc


## Network

### Requests structure:
`{flag}/{subflag}/*all_other_data_from_responces`
*`example`: `misc/registration/my nickname/`*
`type "bool" is considered to be as 0 or 1 integer`


### Responce status codes:
`200` - responce to ping
`400` - bad request
`1` - success
`0` - request had been failed


### Responses structure:
`{int:status_code}/{flag}/{subflag}/*all_other_data_from_responces`
*braces define insertion, it is not a part of request, same as "data_type:"*
*`example`: `1/misc/create_session/95db522b74f4ad8859526713/`*



### User request flags:
#### `misc` (subflags):
-	`ping`:
	-	Responses:
			- `{float64:local_timestamp}`
-	`registration`:
	-	Args: `{string:user_nickname}`
	-	Responses:
		-	Success:
			-	`{string:internal_user_id}`
		-	Failure:
			-	`{string:reason}`
-	`get_sessions_list`:
	-	Responses:
		-	sessions_list: `{int:amount_of_sessions}/{string:session_name}/{string:internal_session_id}/{int:amount_of_players}/{int:max_players}/{bool:password_required}/{...}`
-	`create_session`:
	-	Args: `{string:session_name}/{int:max_players}/{string:session_password [pass empty string for no password]}`
	-	Responses:
		-	Success:
			-	`{string:internal_session_id}`
		-	Failure:
			-	`{string:reason}`
-	`connect_to_session`:
	-	Args: `{string:internal_session_id}/{string:password [optional argument for no password]}`
	-	Responses:
		-	Success:
			-	`[only status code] [you may begin sending game data]`
		-	Failure:
			-	`{string:"invalid_session_id"}`
			-	`{string:"wrong_password"}`
			-	`{string:"session_overflow"}`
-	`disconnect_from_session`:
	-	Responses:
		-	`[only status code]`
#### `game` (subflags):
-	`data_exchange`:
	-	Args: `{any data}`
	-	Responses:
		-	Susscess:
			- `{string:some_system_info}/{int:amount_of_incoming_players_data}/{...data for each player...}`



### Server request flags:
#### `misc` (subflags):
-	`ping`:
-	-	required responce:
			- `{int:1}`
	-	if no respoce:
			- `disconnecting user from server`



