import math
import time
from kubarem import PyTransformComponent, Vec3


class DerivedPyTransformComponent(PyTransformComponent):
    def __init__(self, pos, siz):
        super(DerivedPyTransformComponent, self).__init__(pos, siz)

    def on_create(self):
        pass

    def on_update(self, delta_time):
        self.position.y = 10 - 5 * math.sin(self.position.x * time.time() / 100)
        # self.position.z = -160 - 5 * math.cos(self.position.x * time.time() / 30)

    def on_destroy(self):
        pass

