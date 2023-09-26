# Project-sesc


## Network

### Requests structure:
`{flag}/{subflag}/*all_other_data_from_responces`
*`example`: `misc/registration/my nickname/`*


### Responce status codes:
`200` - success
`400` - bad request
`0` - request had been failed
`1` - responce to ping


### Responses structure:
`{int:status_code}/{flag}/{subflag}/*all_other_data_from_responces`
*braces define insertion, it is not a part of request, same as "data_type:"*
*`example`: `200/misc/create_session/6fe3110cca4/`*



### Request Flags:
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
		-	sessions_list: `{int:amount_of_sessions}/{string:session_name|string:internal_session_id|int:amount_of_players|int:max_players|bool:password_required}/{...}`
-	`create_session`:
	-	Args: `{string:session_name|int:max_players|string:session_password [leave empty for no password]}`
	-	Responses:
		-	Success:
			-	`{string:internal_session_id}`
		-	Failure:
			-	`{string:reason}`
-	`connect_to_session`:
	-	Args: `{string:internal_session_id}`
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


