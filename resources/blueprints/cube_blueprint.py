import math
import time
from kubarem import PyTransformComponent, Vec3


class DerivedPyTransformComponent(PyTransformComponent):
    def __init__(self, pos, siz):
        super(DerivedPyTransformComponent, self).__init__(pos, siz)
        # print("Position x:{} y:{} z:{}".format(self.position.x, self.position.y, self.position.z))
        # print("Size x:{} y:{} z:{}".format(self.size.x, self.size.y, self.size.z))

    def on_update(self, delta_time):
        self.position.x += math.sin(time.time())
        self.position.z += math.cos(time.time())

    def on_create(self):
        pass

    def on_destroy(self):
        pass
