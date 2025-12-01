# import pandas as pd


# magic = 'HOCUS POCUS'
# magic = magic.swapcase()
# magic = magic.replace('cus' , 'key')
# magic = magic.split()

# print(magic)
# print(type(magic))

# data = [['Mercury', 2440, 0], ['Venus', 6052, 0,], ['Earth', 6371, 1],
#         ['Mars', 3390, 2], ['Jupiter', 69911, 80], ['Saturn', 58232, 83],
#         ['Uranus', 25362, 27], ['Neptune', 24622, 14]
# ]

# cols = ['Planet', 'radius_km', 'moons']

# planets = pd.DataFrame(data, columns=cols)
import pandas as pd
data = [['Mercury', 2440, 0], ['Venus', 6052, 0,], ['Earth', 6371, 1],
        ['Mars', 3390, 2], ['Jupiter', 69911, 80], ['Saturn', 58232, 83],
        ['Uranus', 25362, 27], ['Neptune', 24622, 14]
]

cols = ['Planet', 'radius_km', 'moons']

planets = pd.DataFrame(data, columns=cols)
print(planets.columns)