#pragma once

#include <assimp/scene.h>
#include <rapidobj/rapidobj.hpp>
#include <fastgltf/core.hpp>
#include <ufbx.h>

class UfbxScene;

using AssimpAPI = aiScene;
using ObjAPI = rapidobj::Result;
using GltfAPI = fastgltf::Asset;
using FbxAPI = UfbxScene;

class ModelAPI
{
public:
	enum API
	{
		None = 0,
		Assimp,
		Obj,
		Gltf,
		Fbx
	};
	
public:
	ModelAPI() = default;
	ModelAPI(std::shared_ptr<AssimpAPI> modelAPI);
	ModelAPI(std::shared_ptr<ObjAPI> modelAPI);
	ModelAPI(std::shared_ptr<GltfAPI> modelAPI);
	ModelAPI(std::shared_ptr<FbxAPI> modelAPI);

	API GetAPI();
	std::shared_ptr<AssimpAPI> GetAssimp();
	std::shared_ptr<ObjAPI> GetObj();
	std::shared_ptr<GltfAPI> GetGltf();
	std::shared_ptr<FbxAPI> GetFbx();

private:
	API m_API = API::None;

	std::shared_ptr<AssimpAPI> m_Assimp = nullptr;
	std::shared_ptr<ObjAPI> m_Obj = nullptr;
	std::shared_ptr<GltfAPI> m_Gltf = nullptr;
	std::shared_ptr<FbxAPI> m_Fbx = nullptr;
};

// Wrapper class for ufbx_scene to support RAII
class UfbxScene
{
public:
	UfbxScene(ufbx_scene* scene)
		: m_Scene(scene)
	{

	}

	~UfbxScene()
	{
		ufbx_free_scene(m_Scene);
	}

	ufbx_metadata Metadata() const { return m_Scene->metadata; }

	// Global settings
	ufbx_scene_settings Settings() const { return m_Scene->settings; }

	// Node instances in the scene
	ufbx_node* Root_node() const { return m_Scene->root_node; }

	// Default animation descriptor
	ufbx_anim* Anim() const { return m_Scene->anim; }

	ufbx_unknown_list Unknowns() const { return m_Scene->unknowns; }

	// Nodes
	ufbx_node_list Nodes() const { return m_Scene->nodes; }

	// Node attributes (common)
	ufbx_mesh_list Meshes() const { return m_Scene->meshes; }
	ufbx_light_list Lights() const { return m_Scene->lights; }
	ufbx_camera_list Cameras() const { return m_Scene->cameras; }
	ufbx_bone_list Bones() const { return m_Scene->bones; }
	ufbx_empty_list Empties() const { return m_Scene->empties; }

	// Node attributes (curves/surfaces)
	ufbx_line_curve_list Line_curves() const { return m_Scene->line_curves; }
	ufbx_nurbs_curve_list Nurbs_curves() const { return m_Scene->nurbs_curves; }
	ufbx_nurbs_surface_list Nurbs_surfaces() const { return m_Scene->nurbs_surfaces; }
	ufbx_nurbs_trim_surface_list Nurbs_trim_surfaces() const { return m_Scene->nurbs_trim_surfaces; }
	ufbx_nurbs_trim_boundary_list Nurbs_trim_boundaries() const { return m_Scene->nurbs_trim_boundaries; }

	// Node attributes (advanced)
	ufbx_procedural_geometry_list Procedural_geometries() const { return m_Scene->procedural_geometries; }
	ufbx_stereo_camera_list Stereo_cameras() const { return m_Scene->stereo_cameras; }
	ufbx_camera_switcher_list Camera_switchers() const { return m_Scene->camera_switchers; }
	ufbx_marker_list Markers() const { return m_Scene->markers; }
	ufbx_lod_group_list Lod_groups() const { return m_Scene->lod_groups; }

	// Deformers
	ufbx_skin_deformer_list Skin_deformers() const { return m_Scene->skin_deformers; }
	ufbx_skin_cluster_list Skin_clusters() const { return m_Scene->skin_clusters; }
	ufbx_blend_deformer_list Blend_deformers() const { return m_Scene->blend_deformers; }
	ufbx_blend_channel_list Blend_channels() const { return m_Scene->blend_channels; }
	ufbx_blend_shape_list Blend_shapes() const { return m_Scene->blend_shapes; }
	ufbx_cache_deformer_list Cache_deformers() const { return m_Scene->cache_deformers; }
	ufbx_cache_file_list Cache_files() const { return m_Scene->cache_files; }

	// Materials
	ufbx_material_list Materials() const { return m_Scene->materials; }
	ufbx_texture_list Textures() const { return m_Scene->textures; }
	ufbx_video_list Videos() const { return m_Scene->videos; }
	ufbx_shader_list Shaders() const { return m_Scene->shaders; }
	ufbx_shader_binding_list Shader_bindings() const { return m_Scene->shader_bindings; }

	// Animation
	ufbx_anim_stack_list Anim_stacks() const { return m_Scene->anim_stacks; }
	ufbx_anim_layer_list Anim_layers() const { return m_Scene->anim_layers; }
	ufbx_anim_value_list Anim_values() const { return m_Scene->anim_values; }
	ufbx_anim_curve_list Anim_curves() const { return m_Scene->anim_curves; }

	// Collections
	ufbx_display_layer_list Display_layers() const { return m_Scene->display_layers; }
	ufbx_selection_set_list Selection_sets() const { return m_Scene->selection_sets; }
	ufbx_selection_node_list Selection_nodes() const { return m_Scene->selection_nodes; }

	// Constraints
	ufbx_character_list Characters() const { return m_Scene->characters; }
	ufbx_constraint_list Constraints() const { return m_Scene->constraints; }

	// Audio
	ufbx_audio_layer_list Audio_layers() const { return m_Scene->audio_layers; }
	ufbx_audio_clip_list Audio_clips() const { return m_Scene->audio_clips; }

	// Miscellaneous
	ufbx_pose_list Poses() const { return m_Scene->poses; }
	ufbx_metadata_object_list Metadata_objects() const { return m_Scene->metadata_objects; }

	ufbx_element_list* Elements_by_type() const { return &m_Scene->elements_by_type[0]; }

	// Unique texture files referenced by the scene.
	ufbx_texture_file_list Texture_files() const { return m_Scene->texture_files; }

	// All elements and connections in the whole file
	ufbx_element_list Elements() const { return m_Scene->elements; }           // < Sorted by `id`
	ufbx_connection_list Connections_src() const { return m_Scene->connections_src; } // < Sorted by `src,src_prop`
	ufbx_connection_list Connections_dst() const { return m_Scene->connections_dst; } // < Sorted by `dst,dst_prop`

	// Elements sorted by name, type
	ufbx_name_element_list Elements_by_name() const { return m_Scene->elements_by_name; }

	// Enabled if `ufbx_load_opts.retain_dom == true`.
	ufbx_nullable ufbx_dom_node* Dom_root() const { return m_Scene->dom_root; }
private:
	ufbx_scene* m_Scene;
};
