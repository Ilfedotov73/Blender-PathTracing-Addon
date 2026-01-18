import bpy

class PTRenderEngineUI(bpy.types.Panel):
    bl_idname = "ADDON_PT_ENGINE_UI"
    bl_label = "Path Tracing Render Engine (add-on)"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "render"

    @classmethod
    def poll(cls, context):
        return context.scene.render.engine == 'ADDON_PT_ENGINE'

    def draw(self, context):
        layout = self.layout
        layout.label(text="Настройка рендеринга:")

        props = context.scene.pt_engine_props
        
        layout.prop(props, "render_binary_path")
        layout.prop(props, "aspect_ratio")
        layout.prop(props, "image_width")
        layout.prop(props, "samples")
        layout.prop(props, "max_depth")
        layout.prop(props, "vfov")
        layout.prop(props, "focus_angle")
        layout.prop(props, "focus_dist")

def register():
    bpy.utils.register_class(PTRenderEngineUI)

def unregister():
    bpy.utils.unregister_class(PTRenderEngineUI)
