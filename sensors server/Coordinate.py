class Coordinate:
    def __init__(self, x: float, y: float, z: float):
        self.x = x
        self.y = y
        self.z = z

    def to_dict(self):
        return {"lon": self.x, "lat": self.y, "alt": self.z}