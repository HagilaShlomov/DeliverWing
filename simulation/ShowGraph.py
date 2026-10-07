import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import threading
import socket
import json
import random

PORT = 14785
HOST = 'localhost'

# סגנונות הצגה לפי סוג האובייקט (type)
type_styles = {
    0: {'color': 'deepskyblue', 'marker': 'o', 'linestyle': '-', 'label': 'drone path', 'size': 6, 'width': 1},
    1: {'color': 'green', 'marker': 'o', 'linestyle': '', 'label': 'current position', 'size': 10, 'width': 1},
    2: {'color': 'red', 'marker': 'x', 'linestyle': '', 'label': 'obstacle', 'size': 10, 'width': 1},
    3: {'color': 'gray', 'marker': 's', 'linestyle': '', 'label': 'building', 'size': 8, 'width': 2},

}


tracks = {}

# מייצר בניינים רנדומליים
def generate_buildings(num_buildings=10):
    buildings = []
    for _ in range(num_buildings):
        x = random.uniform(32.317, 32.33)
        y = random.uniform(34.855, 34.865)
        height = random.uniform(20, 50)
        buildings.append((x, y, height))
    return buildings

    

def plot_thread():
    plt.ion()  # הפעלה במצב אינטראקטיבי-עדכון אוטומטי
    fig = plt.figure("3D Drone Path Simulation")#פתיחת חלון
    ax = fig.add_subplot(111, projection='3d')
    

    while True:
        ax.clear()#ניקוי בכל איטרציה
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_title('Realtime Drone Paths')

        # for bx, by, bh in buildings:
        #     ax.plot([bx, bx], [by, by], [0, bh], color='gray', linewidth=5, alpha=0.6)


        for key, data in tracks.items():
            id, type_ = key
            style = type_styles.get(type_, type_styles[0])
            if data:
                xs, ys, zs = zip(*data)
                label = f"{style['label']} (id={id})"
                ax.plot(xs, ys, zs,
                        linestyle=style['linestyle'],
                        marker=style['marker'],
                        color=style['color'],
                        markersize=style['size'],
                        linewidth=style['width'],
                        label=label)
                
        for key, data in tracks.items():
            id, type_ = key
            if type_ == 3 and data:
                for bx, by, bh in data:
                    ax.plot([bx, bx], [by, by], [0, bh], color='gray', linewidth=5, alpha=0.6)

        ax.legend()#מציג מקרא
        plt.pause(0.1)

def server_thread():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s: #האזנה בUDP
        s.bind((HOST, PORT))
        print(f"Listening on {HOST}:{PORT}...")
        while True:
            msg, _ = s.recvfrom(4096)#האזנה לנתונים
            try:
                point = json.loads(msg.decode())
                key = (point['id'], point['type'])
                if point['type'] == -1:
                    reset_key = (point['id'], 0)
                    tracks[reset_key] = []
                    # אתחול מסלול חדש
                elif point['type'] == 1:
                    tracks[key] = [(point['x'], point['y'], point['z'])]
                # מחיקת מכשולים 
                elif point['type'] == -2:
                    reset_key = (point['id'], 2)
                    tracks[reset_key] = []
                else:
                    if key not in tracks:
                        tracks[key] = []
                    elif point['type'] == -3:
                        reset_key = (point['id'], 3)
                        if reset_key in tracks:
                            print(f"Clearing buildings for ID {point['id']}")
                            tracks[reset_key] = []
                    tracks[key].append((point['x'], point['y'], point['z']))
            except Exception as e:
                print(f"Error parsing message: {e}")

if __name__ == "__main__":
    buildings = []  # יצירת הבניינים

    threading.Thread(target=plot_thread, daemon=True).start()
    server_thread()














# import json
# import time
# import os
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D

# def draw_graph(data, ax):
#     ax.clear()
#     nodes = {node["id"]: (node["x"], node["y"], node["z"]) for node in data["nodes"]}

#     for x, y, z in nodes.values():
#         ax.scatter(x, y, z, color='blue')

#     for edge in data["edges"]:
#         if edge["from"] in nodes and edge["to"] in nodes:
#             x_vals = [nodes[edge["from"]][0], nodes[edge["to"]][0]]
#             y_vals = [nodes[edge["from"]][1], nodes[edge["to"]][1]]
#             z_vals = [nodes[edge["from"]][2], nodes[edge["to"]][2]]
#             ax.plot(x_vals, y_vals, z_vals, color='gray')

#     plt.draw()

# def monitor_graph_json(path):
#     last_mod = 0
#     plt.ion()  # מצב אינטראקטיבי
#     fig = plt.figure()
#     ax = fig.add_subplot(111, projection='3d')

#     while plt.fignum_exists(fig.number):
#         try:
#             mod_time = os.path.getmtime(path)
#             if mod_time != last_mod:
#                 last_mod = mod_time
#                 with open(path, 'r') as f:
#                     data = json.load(f)
#                     draw_graph(data, ax)
#         except Exception as e:
#             print("בעיה:", e)

#         plt.pause(0.2)

# monitor_graph_json("C:/Users/חגילה/Desktop/לימודים/פרויקט גמר/drone project/data/GraphSim.json")
