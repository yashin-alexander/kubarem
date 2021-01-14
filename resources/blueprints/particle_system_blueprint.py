import math
import time
import pprint
from kubarem import Vec3, PyParticleSystem


class DerivedPyParticleSystem(PyParticleSystem):
    def __init__(self, parameters, particles_number):
        super(DerivedPyParticleSystem, self).__init__(parameters, particles_number)

    def parameters_as_dict(self):
        return {
            "position": {
                "x": self.parameters.position.x,
                "y": self.parameters.position.y,
                "z": self.parameters.position.z,
            },
            "velocity": {
                "x": self.parameters.velocity.x,
                "y": self.parameters.velocity.y,
                "z": self.parameters.velocity.z,
            },
            "color": {
                "x": self.parameters.color.x,
                "y": self.parameters.color.y,
                "z": self.parameters.color.z,
                "w": self.parameters.color.w,
            },
            "gravity_effect": self.parameters.gravity_effect,
            "life_length": self.parameters.life_length,
            "rotation": self.parameters.rotation,
            "scale": self.parameters.scale,
        }

    def on_update(self, delta_time):
        if (time.time() % 10 > 5):
            self.parameters.scale += 0.02
        else:
            self.parameters.scale -= 0.02

    def on_create(self):
        print("Particle system is created. Parameters: {}".format(
            pprint.pformat(self.parameters_as_dict())
        ))

    def on_destroy(self):
        print("bye")
        pass
