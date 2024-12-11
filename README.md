# mini-RTS-server
server for minimalistic RTS game

# Building with CMake

Configure project:

```
mkdir build
cd build 
cmake ..

```

Once configuration is successful build project with avaliable tool
for example
```
make
```

# Running server

`./mini-rts-server <port> [config file]`

# Configuring server

`[config file]` can adjust game rules. `[config file]` consists of name value pairs.

### Available values:

* `millis` - duration of one game step [default: 1000]
* `boardX` - x dimention of the board [default: 256]
* `boardY` - y dimention of the board[default: 256]
* `startResources` - how many resources to spawn at the start of the game [default: 25]
* `maxPlayers` - how many players can play at once [default: 16]

### Example:

``` config.txt
millis 500
boardX 500
boardY 500
startResources 100
maxPlayers 32
```

Pair can appear in any order. If pair not listed default value will be used.

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

Numbers are represented as strings of characters (97 ---> "97" not 'a').
