# mini-RTS-server

server for minimalistic RTS game

**server is linux only** will build and run only on linux

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

* `millis` - duration of one game step [default: 200]
* `maxPlayers` - how many players can play at once [default: 16]
* `boardX` - x dimention of the board [default: 256]
* `boardY` - y dimention of the board[default: 256]
* `unitsToWin` - how many units player has to aquire to win the game [default: 50]
* `startResources` - how many resources to spawn at the start of the game [default: 25]
* `resourceHp` - starting hit points of every new resource [default: 100]
* `unitHp` - starting hit points of every new unit [default: 100]
* `unitDamage` - how much damage do units deal on every attack [default: 10]
* `allowedNameCharacters` - string of characters that can be used in player names  [default: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_]
* `maxResourceSpawn` - amount of times server will try to spawn a resource in one game tick / maximum amount of resources that can spawn in one tick [default: 1]
* `resourceChance` - chance of resource spawning in one attempt [default: 0.1]

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
maxResourceSpawn 2
resourceChance 0.2
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
- `m` `<id>` ` ` `<x>` ` ` `<y>` `\n` - request unit of id: `<id>` to move to field of coordinates: `<x><y>`
- `a` `<id1>` ` ` `<id2>` `\n` - request unit of id: `<id1>` to attack another unit (of id: `<id2>`) (possible to attack own units)
- `d` `<id>` `\n` - request unit to mine the resource (`<id>` is an id of the controlled unit) (unit can only mine resource that it is standing on)

### From server

- `c` `<millis>` ` ` `<maxPlayers>` ` ` `<boardX>` ` ` `<boardY>` ` ` `<unitsToWin>` ` ` `<startResources>` ` ` `<resourceHp>` ` ` `<unitHp>` ` ` `<unitDamage>` ` ` `<allowedNameCharacters>` `\n` - almost whole server configuration sent to newly joined clients
- `j` `<player name>` `\n` - new player has joined the game room
- `l` `<player name>` `\n` - player `<player name>` has either left or lost the game
- `m` `<id>` ` ` `<x>` ` ` `<y>` `\n` - unit of id `<id>` has moved to `<x>;<y>`
- `a` `<id1>` ` ` `<id2>` ` ` `<hp>` `\n` - unit of id `<id1>` attacked unit of id `<id2>` and target unit has now `<hp>` hp left
- `d` `<id>` ` ` `<hp>` `\n` - unit of id `<id>` mined a resource and resource has now `<hp>` hp left
- `u` `<player name>` ` ` `<id>` ` ` `<x>` ` ` `<y>` - player `<player name>` has aquired unit of id `<id>` on field `<x>;<y>`
- `f` `<x>` ` ` `<y>` ` ` `<hp>` `\n` - new resource spawned on field `<x>;<y>`
- `t` `\n` - sent to all players in game room in regular time intervals, marks the and of each tick and a start of the next one
- `q` `\n` - player was sent to queue (in response to: `j`)
- `y` `\n` - client request accepted (in response to: `n`)
- `n` `\n` - client request denied (in response to: `j` or `n`)
- `L` `<name>` `\n` - client lost the game (and was moved out of game room), `<name>` is the name of the player who won
- `W` `<name>` `\n` - client won the game (and was moved out of game room), `<name>` is the name of the player who won
 
### Board state message

Board state update is sent to every player that joins the game room

Structure as follows:

`p` `<amount of players>` `;`  
`<player 1 name>` ` ` `<amount of units of player1>` `,`  
`<id>` ` ` `<x posion>` ` ` `<y position>` ` ` `<hp>` `,`  
...  
`<id>` ` ` `<x posion>` ` ` `<y position>` ` ` `<hp>` `,` `;`  
...  
`<player N name>` ` ` `<amount of units of playerN>` `,`  
`<id>` ` ` `<x posion>` ` ` `<y position>` ` ` `<hp>` `,`  
...  
`<id>` ` ` `<x posion>` ` ` `<y position>` ` ` `<hp>` `,` `;` `\n`  
...  
...  
`r` `<amount of fields with resource>` `;`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `;`  
`<x posion>` ` ` `<y position>` ` ` `<hp>` `;` `\n`  
...

Numbers are represented as strings of characters (97 ---> "97" not 'a').

### Communication order (client's perspective)

**1:** server sends `c`  
**2:** client sends `n`  
**3:** if server responds `n` => go to step **2**  
**3:** else server responds `y`  
**4:** if client sends `n` => go to step **3**  
**4:** else client sends `j`  
**5:** if server responds `q` => wait until server sends `p`, then `r`  
**5:** else server reponds `p`, then `r`  
**6:** server can send multiple: `j` `l` `m` `a` `d` `u` `f` messages  
**6:** client can send multiple: `m` `a` `d` messages   
**6:** if client sends `q` => go to step **4**  
**6:** if server sends `W` or `L` => go to step **4**  
**6:** if server sends `l` with client's name => go to step **4**  
**7:** server sends `t` => go to step **6**  

# Available Clients:

- [Godot client](https://github.com/pwalig/mini-RTS-godot-client)

feel free to write your own clients
