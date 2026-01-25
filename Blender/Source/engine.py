import bpy
import os
import subprocess
import traceback
from mathutils import Vector

class PTRenderEngine(bpy.types.RenderEngine):
    bl_idname = "ADDON_PT_ENGINE"
    bl_label = "Path Tracin Engine (add-on)"

    def render(self, depsgraph):
        scene = depsgraph.scene        
        
        addon_dir = os.path.dirname(__file__)
        obj_path = os.path.join(addon_dir, "scene.obj")

        try:      
            bpy.ops.export_scene.obj(
                filepath=obj_path,
                use_selection=False,
                use_mesh_modifiers=True,
                use_uvs=True,
                use_materials=True,
                use_triangles=True,
                path_mode='AUTO'
            )
        except:
            print(f"Export to obj file failed: {e}")
            self.report({'ERROR'}, f"Export to obj file failed: {e}")

        render_exe = os.path.join(addon_dir, "render.exe")

        if not os.path.exists(render_exe):
            self.report({'ERROR'}, f"Render executable not found: {render_exe}")
            return
        
        engine_props = scene.pt_engine_props

        world = bpy.context.scene.world
        bg_node = world.node_tree.nodes.get('Background')
        b_color = bg_node.inputs['Color'].default_value

        camera = bpy.context.scene.camera
        cam_location = camera.location
        cam_direction = camera.matrix_world.to_quaternion() @ Vector((0.0, 0.0, -1.0))

        try:
            print(f"Start process:{render_exe}, {os.path.basename(obj_path)}, {obj_path}")
            cmd = [
                render_exe,
                # OBJ
                #--------------------------------------------------#
                os.path.basename(obj_path),
                obj_path,
                # RENDER SETTINGS
                #--------------------------------------------------#
                "--asp_ratio", str(engine_props.asp_ratio),
                "--image_width", str(engine_props.image_width),
                "--samples", str(engine_props.samples),
                "--max_depth", str(engine_props.max_depth),
                "--vfov", str(engine_props.vfov),
                "--focus_angle", str(engine_props.focus_angle),
                "--focus_dist", str(engine_props.focus_dist),
                # WORLD
                # - background
                #--------------------------------------------------#
                "--b1", str(b_color[0]),
                "--b2", str(b_color[1]),
                "--b3", str(b_color[3]),
                # - camera lookfrom
                #--------------------------------------------------#
                "--cam_pos_x", str(cam_location.x),
                "--cam_pos_y", str(cam_location.y),
                "--cam_pos_z", str(cam_location.z),
                # - camera lookat
                #--------------------------------------------------#
                "--cam_dir_x", str(cam_direction.x),
                "--cam_dir_y", str(cam_direction.y),
                "--cam_dir_z", str(cam_direction.z),
                # - View Up Vector
                #--------------------------------------------------#
                "--vup_x", str(0.0),
                "--vup_y", str(1.0),
                "--vup_z", str(0.0),
            ]

            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                check=True
            )

            self.parse_to_ppm(result.stdout)

        except subprocess.CalledProcessError as e:
            print(f"{e.stderr}, {traceback.print_exception()}")
            self.report({'ERROR'}, f"Render failed: {e.stderr}, {traceback.print_exc()}")
        except Exception as e:
            print(f"Исключение:{traceback.print_exc()}, {type(e).__name__}: {e}")
    
    def parse_to_ppm(self, ppm_data):
        lines = ppm_data.strip().split('\n')

        if lines[0] != 'P3':
            self.report({'ERROR'}, "Invalid PPM format")
            return
        
        width, height = map(int, lines[1].split())

        max_val = int(lines[2])

        pixel_data = []
        for line in lines[3:]:
            if line.strip():
                pixel_data.extend(map(int, line.split()))

        self.result = self.begin_result(0, 0, width, height)
        layer = self.result.layers[0].passes("Combined")

        pixels = []
        for i in range(0, len(pixel_data), 3):
            r = pixel_data[i] / max_val
            g = pixel_data[i + 1] / max_val
            b = pixel_data[i + 2] / max_val
            a = 1.0
            pixels.extend([r, g, b, a])

        layer.rect = pixels
        self.end_result(self.result)

def register():
    bpy.utils.register_class(PTRenderEngine)

def unregister():
    bpy.utils.unregister_class(PTRenderEngine)