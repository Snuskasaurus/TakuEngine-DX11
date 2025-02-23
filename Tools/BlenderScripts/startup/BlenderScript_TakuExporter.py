import bpy
import os

class SimpleExportOperator(bpy.types.Operator):
    """Export selected objects to individual OBJ files (Blender 4.x)"""
    bl_idname = "export.simple_obj"
    bl_label = "Export Selected to OBJ"
    bl_options = {'REGISTER', 'UNDO'}

    directory: bpy.props.StringProperty(name="Directory Path", subtype='DIR_PATH', default="//")

    def execute(self, context):
        export_dir = bpy.path.abspath(self.directory)
        
        allObject = bpy.context.selected_objects
        for obj in allObject:
            obj.select_set(False)
        
        for obj in allObject:
        
            obj.select_set(True)
            bpy.context.view_layer.objects.active = obj
            
            oldLocation0 = obj.location[0]
            oldLocation1 = obj.location[1]
            oldLocation2 = obj.location[2]
            
            obj.location[0] = 0 
            obj.location[1] = 0 
            obj.location[2] = 0 
            
            filename = f"{obj.name}.obj"
            export_path = os.path.join(export_dir, filename)
            
            bpy.ops.wm.obj_export(filepath=export_path,
                                check_existing=True,
                                filter_blender=False,
                                filter_backup=False,
                                filter_image=False,
                                filter_movie=False,
                                filter_python=False,
                                filter_font=False,
                                filter_sound=False,
                                filter_text=False,
                                filter_archive=False,
                                filter_btx=False,
                                filter_collada=False,
                                filter_alembic=False,
                                filter_usd=False,
                                filter_obj=False,
                                filter_volume=False,
                                filter_folder=True,
                                filter_blenlib=False,
                                filemode=8,
                                display_type='DEFAULT',
                                sort_method='DEFAULT',
                                export_animation=False,
                                start_frame=-2147483648,
                                end_frame=2147483647,
                                forward_axis='X',
                                up_axis='Z',
                                global_scale=1.0,
                                apply_modifiers=True,
                                export_eval_mode='DAG_EVAL_VIEWPORT',
                                export_selected_objects=True,
                                export_uv=True,
                                export_normals=True,
                                export_colors=False,
                                export_materials=False,
                                export_pbr_extensions=False,
                                path_mode='AUTO',
                                export_triangulated_mesh=True,
                                export_curves_as_nurbs=False,
                                export_object_groups=False,
                                export_material_groups=False,
                                export_vertex_groups=False,
                                export_smooth_groups=False,
                                smooth_group_bitflags=False,
                                filter_glob='*.obj;'
                                )     
                                
            obj.location[0] = oldLocation0
            obj.location[1] = oldLocation1
            obj.location[2] = oldLocation2
            
            obj.select_set(False)  
            self.report({'INFO'}, f"Exported {obj.name} to {export_path}")
        
        for obj in allObject:
            obj.select_set(False)
        
        return {'FINISHED'}

class SimpleExportPanel(bpy.types.Panel):
    """Creates a UI panel for exporting"""
    bl_label = "Simple Exporter"
    bl_idname = "VIEW3D_PT_simple_export"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Export'

    def draw(self, context):
        layout = self.layout
        layout.prop(context.scene, "export_obj_directory")
        layout.operator("export.simple_obj").directory = context.scene.export_obj_directory

# Registering classes
def register():
    bpy.utils.register_class(SimpleExportOperator)
    bpy.utils.register_class(SimpleExportPanel)
    bpy.types.Scene.export_obj_directory = bpy.props.StringProperty(name="Export Directory", subtype='DIR_PATH', default="//")

def unregister():
    bpy.utils.unregister_class(SimpleExportOperator)
    bpy.utils.unregister_class(SimpleExportPanel)
    del bpy.types.Scene.export_obj_directory

if __name__ == "__main__":
    register()
