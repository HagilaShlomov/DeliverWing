import math
from Coordinate import Coordinate
from flask import Flask, request, jsonify
import random

app = Flask(__name__)

@app.route('/barometer', methods=['GET'])
def get_barometer_details():
    random_temperature = random.uniform(5.0, 40.0)
    random_pressure = random.uniform(950.0, 1050.0)
    return jsonify({"temperature": str(random_temperature), "pressure": str(random_pressure)}), 200



@app.route('/lidar', methods=['POST'])
def get_lidar_details():
    data = request.get_json()

    current_data = data['current']
    next_data = data['next']

    current = Coordinate(current_data['lon'], current_data['lat'], current_data['alt'])
    next_coord = Coordinate(next_data['lon'], next_data['lat'], next_data['alt'])

    result_point = get_point(current, next_coord)

    return jsonify(result_point.to_dict()), 200


def get_point(current: Coordinate, next_coord: Coordinate, max_range_km=2.0) -> Coordinate:
    distance = random.uniform(0, max_range_km) 
    rand_percent = random.randint(0, 99)

    if rand_percent < 20:
        # העצם בכיוון המסלול (20%)
        dx = next_coord.x - current.x
        dy = next_coord.y - current.y
        angle = math.atan2(dy, dx)
        angle += (random.uniform(-0.5, 0.5)) * (math.pi / 6)  # ±15°
    else:
        # כיוון רנדומלי (80%)
        angle = random.uniform(0, 2 * math.pi)

    lat = current.y + (distance * math.cos(angle)) / 111.32 #חישוב קו רוחב וקו אורך חדשים, במרחק distance ובכיוון angle
    lon = current.x + (distance * math.sin(angle)) / (111.32 * math.cos(math.radians(current.y)))
    alt = random.uniform(40, 70)

    return Coordinate(lon, lat, alt)


@app.route('/build', methods=['POST'])
def get_build_details():
    data = request.get_json()

    current_data = data['current']
    current = Coordinate(current_data['lon'], current_data['lat'], current_data['alt'])

    buildings = []
    for _ in range(5):
        result_point = get_building(current)    
        buildings.append(result_point.to_dict())

    return jsonify(buildings), 200


def get_building(origin: Coordinate, max_range_km=2.0) -> Coordinate:
    distance = random.uniform(0, max_range_km)
    angle = random.uniform(0, 2 * math.pi)  # כיוון רנדומלי

    lat = origin.y + (distance * math.cos(angle)) / 111.32
    lon = origin.x + (distance * math.sin(angle)) / (111.32 * math.cos(math.radians(origin.y)))
    alt = random.uniform(40, 60)

    return Coordinate(lon, lat, alt)



if __name__ == '__main__':
    app.run(debug=True)