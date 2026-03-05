bl_info = {
    "name": "PathTracingEngineAddon",
    "author": "Fedotov Ilia",
    "version": (1, 0),
    "blender": (4,0,0),
    "location": "Render Properties > Render Engine",
    "description": "Итоговая бакалаврская дипломная работа по разработке модуля рендеринга для редактора Blender.",
    "category": "Render"
}

from . import engine, properties, ui

def register():
    engine.register()
    properties.register()
    ui.register()

def unregister():
    ui.unregister()
    properties.unregister()
    engine.unregister()

if __name__ == "__main__":
    register()