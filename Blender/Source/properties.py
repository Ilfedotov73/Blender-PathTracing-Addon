import bpy
from bpy.props import IntProperty, FloatProperty

class PTRenderEngineProps(bpy.types.PropertyGroup):
    bl_idname = __package__
    asp_ratio: FloatProperty(
        name="Aspect ratio",
        description="Image aspect ratio",
        default= (16.0 / 9.0),
        min=1.0,
        max=2.0
    ) # type: ignore

    image_width : IntProperty(
        name="Image width",
        description="Width size for image",
        default=1920,
        min=50,
        max=2000
    ) # type: ignore

    samples: IntProperty(
        name="Samples",
        description="Samples per pixel",
        default=16,
        min=1,
        max=500
    ) # type: ignore

    max_depth: IntProperty(
        name="Max depth",
        description="Max ray reflect recursion  depth",
        default=15,
        min=1,
        max=50
    ) # type: ignore

    vfov: FloatProperty(
        name="Vfov",
        description="Vertical angle view",
        default=10.0,
        min=1.0,
        max=90.0
    ) # type: ignore

    focus_angle: IntProperty(
        name="Focus angle",
        description="Focus angle for camera",
        default=0,
        min=0,
        max=15
    ) # type: ignore 

    focus_dist: IntProperty(
        name="Focus distance",
        description="Focus distance for camera",
        default=10,
        min=1,
        max=40
    ) # type: ignore

def register():
    bpy.utils.register_class(PTRenderEngineProps)
    bpy.types.Scene.pt_engine_props = bpy.props.PointerProperty(
        type=PTRenderEngineProps
    )

def unregister():
    del bpy.types.Scene.pt_engine_props
    bpy.utils.unregister_class(PTRenderEngineProps)
