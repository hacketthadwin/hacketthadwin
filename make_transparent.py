# make_transparent.py
import os

source_file = './profile-3d-contrib/profile-night-green.svg'
output_file = './profile-3d-contrib/profile-transparent.svg'

if os.path.exists(source_file):
    with open(source_file, 'r') as file:
        content = file.read()
    
    # Replace background styles with transparent
    content = content.replace('.fill-bg { fill: #00000f; }', '.fill-bg { fill: transparent; }')
    content = content.replace('.stroke-bg { stroke: #00000f; }', '.stroke-bg { stroke: transparent; }')
    
    with open(output_file, 'w') as file:
        file.write(content)
    print(f"Created {output_file} with transparent background.")
else:
    print(f"Error: {source_file} not found!")
    exit(1)
