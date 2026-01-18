import bpy
import os
import tempfile

class PTRenderEngine(bpy.types.RenderEngine):
    bl_idname = "ADDON_PT_ENGINE"
    bl_label = "Path Tracin Engine (add-on)"

    def render(self, depsgraph):
        scene = depsgraph.scene
        engine_props = scene.pt_engine_props

        with tempfile.TemporaryDirectory() as tmpdir:
            obj_path = os.path.join(tmpdir, "scene.obj")
            mtl_path = os.path.join(tmpdir, "scene.mtl")

            self.export_scene(scene, obj_path, mtl_path)

            cmd = [
                "--rbp", engine_props.render_binary_path,
                "--image_width", engine_props.image_width,
                "--samples", str(engine_props.samples),
                "--max_depth", str(engine_props.max_depth),
                "--vfov", str(engine_props.vfov),
                "--focus_angle", str(engine_props.focus_angle),
                "--focus_dist", str(engine_props.focus_dist)
            ]

    def export_scene(self, scene, obj_path, mtl_path):
        bpy.ops.object.select_all(action='SELECT')

        bpy.ops.export_scene.obj(
            filepath=obj_path,
            use_selection=True,
            use_materials=True,
            path_mode='COPY'
        )


def register():
    bpy.utils.register_class(PTRenderEngine)

def unregister():
    bpy.utils.unregister_class(PTRenderEngine)