# Format
All maps levels are formated in the following way:
```
{
    "params": {
        "name": "Map Name",
        "base_money": 100,
        "health": 100,
        "map_size": [8, 8],
        "start_tile": [0, 0],
        "end_tile": [7, 7],
        "path": [[0, 1], [0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2], [6, 2], [7, 2], [7, 3], [7, 4], [7, 5], [7, 6], [7, 7]],
        "decorations": [[1, 1], [6, 6]]
    },
    "waves": [
        {
            "enemies": [
                {
                    "type": "bat",
                    "start_delay": 10,
                    "spawn_delay": 0.5,
                    "spawn_amount": 5,
                    "amount": 10
                },
                {
                    "type": "bat",
                    "start_delay": 0,
                    "spawn_delay": 0.5,
                    "spawn_amount": 2,
                    "amount": 20
                }
            ]
        }
    ]
}

'''