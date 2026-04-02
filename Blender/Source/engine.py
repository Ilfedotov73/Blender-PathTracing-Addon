import bpy
import sys
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
                use_uvs=True,
                use_materials=True,
                path_mode='AUTO',
                axis_forward='-Z',
                axis_up='Y'
            )
        except:
            print(f"ERROR: export to obj file failed: {e}")
            self.report({'ERROR'}, f"Export to obj file failed: {e}")

        render_exe = os.path.join(addon_dir, "render.exe")

        if not os.path.exists(render_exe):
            print(f"ERROR: render executable not found: {render_exe} ")
            self.report({'ERROR'}, f"Render executable not found: {render_exe}")
            return
        
        cam = bpy.context.scene.camera
        matrix = cam.matrix_world

        # Приведение к правосторонней системе координат камеры.
        cam_orig_bl = matrix.translation
        cam_dir_bl = matrix.to_3x3() @ Vector((0,0,-1))
        cam_vup_bl = matrix.to_3x3() @ Vector((0,1,0))

        cam_origin = Vector((cam_orig_bl.x, cam_orig_bl.z, -cam_orig_bl.y))
        cam_direction = Vector((cam_dir_bl.x, cam_dir_bl.z, -cam_dir_bl.y)).normalized()
        cam_vup = Vector((cam_vup_bl.x, cam_vup_bl.z, -cam_vup_bl.y)).normalized()

        scene = depsgraph.scene
        scale = scene.render.resolution_percentage / 100.0

        engine_props = scene.pt_engine_props
        backcolor = bpy.context.scene.world.color
        try:
            print(f"Start process: {render_exe}")
            cmd = [
                render_exe,
                # OBJ
                #--------------------------------------------------#
                obj_path,
                addon_dir,
                # RENDER SETTINGS
                #--------------------------------------------------#
                "--asp_ratio", str(engine_props.asp_ratio),
                "--image_width", str(scene.render.resolution_x * scale),
                "--samples", str(engine_props.samples),
                "--max_depth", str(engine_props.max_depth),
                "--vfov", str(engine_props.vfov),
                "--focus_angle", str(engine_props.focus_angle),
                "--focus_dist", str(engine_props.focus_dist),
                "--time_limit_per_pixel", str(engine_props.time_limit_per_pixel),
                # WORLD -- problem
                # - background
                #--------------------------------------------------#
                "--b1", str(backcolor.r),
                "--b2", str(backcolor.g),
                "--b3", str(backcolor.b),
                # - camera lookfrom
                #--------------------------------------------------#
                "--cam_pos_x", str(cam_origin.x),
                "--cam_pos_y", str(cam_origin.y),
                "--cam_pos_z", str(cam_origin.z),
                # - camera lookat
                #--------------------------------------------------#
                "--cam_dir_x", str(cam_direction.x),
                "--cam_dir_y", str(cam_direction.y),
                "--cam_dir_z", str(cam_direction.z),
                # - View Up Vector
                #--------------------------------------------------#
                "--vup_x", str(cam_vup.x),
                "--vup_y", str(cam_vup.y),
                "--vup_z", str(cam_vup.z),
                "--adapting_rendering", str(int(engine_props.adapting_rendering)),
            ]

            ppm_path = os.path.join(addon_dir, "render.ppm")

            with open(ppm_path, 'w') as ppm:
                subprocess.run(
                    cmd,
                    text=True,
                    check=True,
                    shell=(sys.platform == "win32"),
                    stdout=ppm
                )

            self.parse_to_ppm(ppm_path)

        except subprocess.CalledProcessError as e:
            print(cmd)
            print(f"ERROR: {e.stderr}, {traceback.print_exception()}")
            self.report({'ERROR'}, f"Render failed: {e.stderr}, {traceback.print_exception()}")
        except Exception as e:
            print(cmd)
            print(f"ERROR: {traceback.print_exception()}, {type(e).__name__}: {e}")
    
    def parse_to_ppm(self, ppm_path):
        lines = ""
        with open(ppm_path, 'r') as ppm:
            lines = ppm.read().strip().split('\n')

        if lines[0] != 'P3':
            self.report({'ERROR'}, "Invalid PPM format")
            return
        
        width, height = map(int, lines[1].split())
        max_val = int(lines[2])

        pixel_data = []
        for line in lines[3:]:
            if line.strip():
                pixel_data.extend(map(int, line.split()))

        result = self.begin_result(0, 0, width, height)
        layer = result.layers[0].passes.get("Combined")
        
        # Порядок обхода пикслей в Blender: сначала все пиксели нижней строки (слева направо), 
        # затем следующей строки вверх и так до верхней строки.
        pixels = []
        for i in range(height - 1, -1, -1):
            for j in range(width):
                idx = (i * width + j) * 3
                r = pixel_data[idx] / max_val
                g = pixel_data[idx + 1] / max_val
                b = pixel_data[idx + 2] / max_val
                pixels.append([r,g,b,1.0])  
        

        layer.rect = pixels
        self.end_result(result)

def register():
    bpy.utils.register_class(PTRenderEngine)

def unregister():
    bpy.utils.unregister_class(PTRenderEngine)