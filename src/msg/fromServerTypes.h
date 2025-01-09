namespace message
{
    enum class fromServer : char {
        config = 'c',
        join = 'j',
        left = 'l',
        move = 'm',
        attack = 'a',
        mine = 'd',
        new_unit = 'u',
        new_resource = 'f',
        tick = 't',
        queue = 'q',
        yes = 'y',
        no = 'n',
        lost = 'L',
        won = 'W'
    };
}
