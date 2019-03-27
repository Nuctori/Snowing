#pragma once
#include <Object.h>
#include <ScriptParser.h>
#include <TextWindow.h>
#include <AVGInput.h>

namespace Yukimi
{
	class AVGPlayer final : public Snowing::Scene::Group<>
	{
	private:
		TextWindow* const textWindow_;

		const Script::Script* const script_;

		// ����ǶԻ�Element�򷵻�true�����򷵻�false
		bool doElement(const Script::Element&);
		void runScriptContinuation();
		
		size_t nextLine_ = 0;

		float clickLimitTimer_ = 0;

		const TextWindowFontStyle* defaultStyle_;

	public:
		AVGPlayer(
			const Script::Script* script,
			TextWindow::TextWindowUserAdapter* textWindowAdapter,
			const TextWindowFontStyle* defaultStyle);

		bool Update() override;
		void Click();
	};
}