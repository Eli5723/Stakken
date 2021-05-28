union ColorTable {
	glm::vec4 entries[8];
	struct {
		glm::vec4 T;
		glm::vec4 O;
		glm::vec4 L;
		glm::vec4 J;
		glm::vec4 Z;
		glm::vec4 S;
		glm::vec4 I;
		glm::vec4 G;
	};
};

struct Identity {
	InitialRotationTable rotation_table;
	ColorTable color_table;

	std::string name;
};