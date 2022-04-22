#pragma once

enum class Topology
{
	Points = 0,
	LineStrip,
	LineLoop,
	Lines,
	LineStripAdjacency,
	LinesAdjacency,
	TriangleStrip,
	TriangleFan,
	Triangles,
	TriangleStripAdjacency,
	TrianglesAdjacency,
	Patches
};
