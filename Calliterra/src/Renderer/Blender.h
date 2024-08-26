#pragma once
#include "Bindable.h"

class Blender : public Bindable
{
public:
	enum class BlendFunc
	{
		NONE = 0,
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_COLOR,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA,
		BLEND_INV_DEST_ALPHA,
		BLEND_DEST_COLOR,
		BLEND_INV_DEST_COLOR,
		BLEND_SRC_ALPHA_SAT,
	};

	enum class BlendOp
	{
		NONE = 0,
		ADD,			// Add src1 to src2
		SUBTRACT,		// Subtract src1 from src2
		REV_SUBSTRACT,  // Subtract src2 from src1
		MIN,			// Find the minimum of src1 and src2
		MAX				// Find the maximum of src1 and src2
	};

	virtual ~Blender() = default;

	static const std::string GenerateUID(bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp);
	static std::shared_ptr<Blender> Resolve(bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp);

};

