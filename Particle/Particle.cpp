#include <Snowing.h>

using namespace Snowing;


int main()
{
	// 创建粒子类型
	struct ParticleSprite final
	{
		Graphics::Sprite Sp;	// 基本的精灵数据
		float SpeedY;			// Y方向速度
		float Life = 0.0f;		// 生命值
		float OriginX;			// 原始X坐标
	};

	// 创建引擎实例
	auto engine = PlatformImpls::WindowsImpl::MakeEngine(
		L"Particle",	// 窗口标题
		{ 800,600 },	// 窗口大小
		true);			// 使用窗口

	// 坐标系
	Math::Coordinate2DCenter coord
	{
		{ 0.0f,0.0f },		// 屏幕中心坐标为0,0
		{ 800.0f,600.0f }	// 屏幕大小为800,600
	};

	// 最大允许4096个精灵作为粒子
	constexpr size_t MaxParticle = 8192;

	// 要使用的SpriteRenderer类型
	using SpriteRenderer = Graphics::SpriteRenderer<
		ParticleSprite,
		Graphics::SpriteSheet, 
		MaxParticle>;

	// 创建精灵列表
	Graphics::SpriteSheet sheet;

	// 写入0号精灵为{ 0,0,1,1 }，方便几何着色器生成点
	sheet.Sheet[0] = { 0,0,1,1 };

	// 创建GPU上的精灵列表
	auto gpuSheet = SpriteRenderer::MakeGPUSpriteSheet(sheet);

	// 创建顶点缓存
	auto vertexSheet = SpriteRenderer::MakeGPUVertexBuffer();

	// 加载着色器
	Graphics::Effect effect
	{
		LoadAsset("Particle.cso")
	};

	// 加载Particle技术，注意，渲染技术存活期间，Effect不得被销毁或移动
	auto particleTech = effect.LoadTechnique("Particle", Graphics::Sprite::DataLayout);

	// 加载FontRendering技术
	auto fontRenderingTech = effect.LoadTechnique("FontRendering", Graphics::Sprite::DataLayout);

	// 加载用于显示粒子数量的字体
	const Graphics::Font fnt =
		Graphics::LoadFont(
			LoadAsset("Font-chs.fnt"));

	// 创建精灵渲染器
	SpriteRenderer renderer
	{
		&Graphics::Device::MainContext(),
		&particleTech,
		coord,
		nullptr,
		&gpuSheet,
		&vertexSheet
	};

	// 粒子列表
	std::vector<ParticleSprite> particles;
	particles.reserve(MaxParticle);

	// 计时
	float t = 0;

	// 粒子数量监视器，它将实时监视粒子数量，并显示在右下角
	Scene::Debug::DebugDisplay particleCountDisplay
	{
		&fontRenderingTech,		// 传入字体渲染技术
		&fnt,					// 传入字体
		L"粒子数量",				// 被监视的值的名称
		[&particles] { return std::to_wstring(particles.size()); }	// 被监视的值
	};

	// FPS监视器
	Scene::Debug::DebugDisplay fpsDisplay
	{
		&fontRenderingTech,
		&fnt,
		L"FPS",
		Scene::Debug::DebugDisplay::FPSGetter	// 使用引擎自带的FPS监视
	};

	// 帧时间监视器
	Scene::Debug::DebugDisplay frameTimeDisplay
	{
		&fontRenderingTech,
		&fnt,
		L"帧时间",
		Scene::Debug::DebugDisplay::FrameTimeGetter	// 使用引擎自带的帧时间监视
	};

	// 启动主循环
	Engine::Get().Run([&] {

		// 绘制
		Engine::Get().Draw([&] {

			// 获取图形设备
			auto &g = Snowing::Graphics::Device::Get();

			// 清除画布
			g.MainContext().ClearRenderTarget(g.MainRenderTarget());

			// 设置屏幕为主画布
			g.MainContext().SetRenderTarget(&g.MainRenderTarget());

			// 绘制精灵列表
			renderer.Draw(particles.data(), particles.size());
		});

		// 距离上一帧已经经过的时间
		auto dt = Engine::Get().DeltaTime();

		// 创建粒子
		t += dt;
		while (t >= 0.00008f)
		{
			t -= 0.00008f;

			particles.emplace_back();
			auto& sp = particles.back();
			sp.Sp.Position =
			{
				0,
				rand() / float(RAND_MAX) * 200.0f + 200.0f
			};

			sp.Sp.Size = { 72.0f,72.0f };
			sp.SpeedY = rand() / float(RAND_MAX) * 700.0f + 250.0f;
			sp.OriginX = rand() / float(RAND_MAX) * 800.0f - 400.0f;

			sp.Sp.Color = { 1.0f,0.5f,0.5f,1.0f };
			
		}

		// 更新粒子
		for (auto& p : particles)
		{
			p.Sp.Position.y -= dt * p.SpeedY;
			p.Life += dt;
			p.Sp.Position.x = p.OriginX * (1 - p.Life / 2.0f) *  (1 - p.Life / 2.0f);
			p.SpeedY *= 1.5f * dt * 40;

			if (p.Life <= 0.1f)
				p.Sp.Color.w = 10 * p.Life;
			else
				p.Sp.Color.w -= dt * 2;

			p.Sp.Size.x -= dt * 8;
			p.Sp.Size.y -= dt * 8;
		}


		// 杀死无用粒子
		const auto nend = std::remove_if(particles.begin(), particles.end(), [](auto& p) {
			return p.Sp.Color.w <= 0;
		});
		particles.erase(nend, particles.end());

		// 更新监视器
		particleCountDisplay.Update();
		fpsDisplay.Update();
		frameTimeDisplay.Update();
	});

	return 0;
}
