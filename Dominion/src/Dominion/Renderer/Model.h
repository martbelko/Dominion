#pragma once

#include "Dominion/Core/Base.h"

namespace Dominion {

	// Forward declarations
	class Mesh;
	class Material;

	class Model
	{
	public:
		Model(const Ref<Mesh>& mesh, const Ref<Material>& material)
			: m_Mesh(mesh), m_Material(material) {}

		const Ref<Mesh>& GetMesh() const { return m_Mesh; }
		const Ref<Material>& GetMaterial() const { return m_Material; }

		static Ref<Model> Create(const Ref<Mesh>& mesh, const Ref<Material>& material) { return CreateRef<Model>(mesh, material); }
	private:
		Ref<Mesh> m_Mesh;
		Ref<Material> m_Material;
	};

}
