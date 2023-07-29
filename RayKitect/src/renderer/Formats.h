#pragma once


enum class TextureFilterFormat {
	None = 0,
	Linear, Nearest
};

enum class TextureDataFormat {
	None = 0,
	RGBA8,
	RED_INTEGER,

	DEPTH24STENCIL8
};

enum class DataType {
	None = 0,
	UNSIGNED_BYTE, BYTE, UNSIGNED_SHORT, SHORT, UNSIGNED_INT, INT, FLOAT
};
