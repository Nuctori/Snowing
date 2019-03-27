#pragma once
#include <stack>
#include <Object.h>
#include <ScriptParser.h>
#include <TextWindow.h>
#include <AVGInput.h>

namespace Yukimi
{
	class AVGPlayer final : public Snowing::Scene::Group<>
	{
	public:
		class AVGPlayerUserAdapter
		{
		public:
			virtual const TextWindowFontStyle* GetDefaultFontStyle() const = 0;
			virtual const TextWindowFontStyle* GetFontStyle(std::wstring_view name) const = 0;
			virtual const TextWindowFontStyle* GetCharaterDefaultFontStyle(std::wstring_view name) const = 0;

			virtual void OnCommand(const Script::CommandElement& command) = 0;
			virtual void OnCharater(const std::wstring_view name) = 0;
		};

	private:
		TextWindow textWindow_;
		AVGPlayerUserAdapter* adapter_;

		const Script::Script* const script_;

		// ��״̬��ÿ�н�����Ҫ����
		std::vector<TextWindowFontStyle> fontStyleStack_;
		std::stack<size_t> fontStyleStackCounts_;
		Script::CharaterNameElement* charaterNameElement_;


		// �����ǰ�н�������Ҫֹͣ���򷵻�true
		bool doElement(const Script::Element&);
		void runScriptContinuation();
		
		size_t nextLine_ = 0;

		float clickLimitTimer_ = 0;

	public:
		AVGPlayer(
			const Script::Script* script,
			TextWindow::TextWindowUserAdapter* textWindowAdapter,
			AVGPlayerUserAdapter * avgPlayerAdapter);

		bool Update() override;
		void Click();

		void Goto(std::wstring_view labelName);
	};
}