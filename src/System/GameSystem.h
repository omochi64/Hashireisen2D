
#pragma once

#include "Selene.h"
#include "System/Manager/3DModelManager.h"

// 可能なら、このようにexternで変数を外部に晒すのではなく、staticなクラスからGetterで取得できるようにしておく(カプセル化が目的)

// 各データが入っているフォルダ名(MapとかModelとかTextureとか...)も外部データから読み込む形にして、共有したほうがいいね

namespace GameSystem
{
	// こいつらはnamespace内の変数でなく、クラス内の静的メンバとして登録することになるかもしれない。
	// とりあえず、これから実装していくときは、じかに変数を触ることはしない
	extern Selene::Scene::ISceneManager	*pSceneMgr;
	extern Selene::Renderer::IRender *pRender;
	extern Selene::File::IFileManager *pFileMgr;
	extern Selene::ICore *pCore;
//	extern Selene::IGraphicCard *pGraphicCard;
	extern Selene::Scene::ICamera *pCamera;

	class SystemGetter{
	public:
		static Selene::Scene::ISceneManager *GetSceneManager();
		static Selene::Renderer::IRender *GetRender();
		static Selene::File::IFileManager *GetFileManager();
		static Selene::ICore *GetCore();
		static Selene::Scene::ICamera *GetCamera();
		static CModelManager *GetModelManager();
	};
	
	// 後はManager系が参入予定
	// 本来このModelManagerは外には見えない(コイツの初期化もGameSystem.cpp内の初期化関数で本来行う)
	extern CModelManager *pModelMgr;

	
	// メソッド
	bool Initialize(char *pWinTitle , Selene::eFrameRate frameRate , bool is_full_screen , float fovAngle );
	void CleanUp();
};
