#include "Scene.h"
#include <map>

#include "mass_spring/mass_spring_scene.hpp"
#include "rigid_body/rigid_body_scene.hpp"
#include "pde/pde_scene1.hpp"
#include "pde/pde_scene2.hpp"
#include "fluid/fluid_scene.hpp"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Mass Spring System", creator<MassSpringScene>()},
    {"Rigid Body System", creator<RigidBodyScene>()},
    {"PDE 2D", creator<PDEScene1>()},
    {"PDE 3D", creator<PDEScene2>()},
    {"Fluid Simulation", creator<FluidScene>()},
};
