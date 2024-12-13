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

* `maxPlayers` - how many players can play at once [default: 16]
* `millis` - duration of one game step [default: 200]
* `boardX` - x dimention of the board [default: 256]
* `boardY` - y dimention of the board[default: 256]
* `startResources` - how many resources to spawn at the start of the game [default: 25]
* `unitsToWin` - how many units player has to aquire to win the game [default: 50]
* `resourceHp` - starting hit points of every new resource
* `unitHp` - starting hit points of every new unit
* `unitDamage` - how much damage do units deal on every attack

### Example:

``` config.txt
millis 3000
maxPlayers 2
unitsToWin 2
startResources 8
boardX 5
boardY 5
resourceHp 20
unitHp 20
unitDamage 5
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
- `m` `<x1>` ` ` `<y1>` ` ` `<x2>` ` ` `<y2>` `\n` - request unit to move (`<x1><y1>` are coordinates of the unit, `<x2><y2>` designate destination)
- `a` `<x1>` ` ` `<y1>` ` ` `<x2>` ` ` `<y2>` `\n` - request unit to move (`<x1><y1>` are coordinates of the unit, `<x2><y2>` coordinates of the target unit) (possible to attack own units)
- `d` `<x1>` ` ` `<y1>` `\n` - request unit to mine the resource (`<x1><y1>` are coordinates of the unit) (unit can only mine resource that it is stating on)

### From server

- `g` `<boardX>` ` ` `<boardY>` ` ` `<unitsToWin>` `\n` - player was sent to game room (in response to: `j`), `<board x dim>`, `<board y dim>` and `<unitsToWin>` same as in `[config file]`
- `q` - player was sent to queue (in response to: `j`)
- `y` - client request accepted (in response to: `n`)
- `n` - client request denied (in response to: `j` or `n`)
- `L` - client lost the game (and was moved out of game room)
- `W` - client won the game (and was moved out of game room)
 
### Board state update

Board state update is sent to all players in the game room in regular time intervals

Structure as follows:

`p` `<amount of players>` `\n`  
`<player 1 name>` ` ` `<amount of units of player1>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...  
`<player 2 name>` ` ` `<amount of units of player1>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...  
...  
`r` `<amount of fields with resource>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `\n`  
...

Numbers are represented as strings of characters (97 ---> "97" not 'a').
