#pragma once

namespace Snowing::Scene
{
	class[[nodiscard]] Object
	{
	public:
		virtual inline ~Object() {}

		// ����һ֡������trueΪ������falseΪ��������
		virtual inline bool Update() { return true; }
	};
}
