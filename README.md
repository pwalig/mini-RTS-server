# mini-RTS-server
server for minimalistic RTS game

# Comunicating with server

Each message must start with one byte that designates the type of the message.
Some messages consist of only type character others contain more data.

## Message types

### From client

- `n` `<name>` `\n` - set name or rename self
- `j` - request join (player will be sent to game room or queue)
- `q` - request quit (player will be removed from game room or queue, can still rejoin with `j`)

### From server

- `i` - server could not match message type sent by client with any of the available ones
- `a` - player was sent to game room (in response to: `j`)
- `q` - player was sent to queue (in response to: `j`)
- `y` - client request accepted
  - in response to: `n` - player (re)named succesfully
- `n` - client request accepted
  - in response to: `j` - player was unnamed
  - in response to: `t` - selected name was taken
 
### Board state update

Board state update is sent to all players in the game room in regular time intervals

Structure as follows:

`p` ` ` `<amount of players>` `\n`  
`<player 1 name>` ` ` `<amount of units of player1>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...  
`<player 2 name>` ` ` `<amount of units of player1>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...  
...  
`r` ` ` `<amount of fields with resource>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...
