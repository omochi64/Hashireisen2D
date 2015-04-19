
#pragma once

#include "Tools/IOctreeMapNodeForTool.h"

#define		LENGTH_OF_NAME			64
#define		LENGTH_OF_FILE_NAME		128

#define MAP_FILE_VERSION_INFO	"MAPDATA001"

// Map namespace
namespace Map
{
	// MAP関係のデータ構造
	typedef struct _header_info{
		char version[16];
		int	number_of_kind_of_objects;
		int	number_of_objects;
		int	divide_x,divide_y,divide_z;
		Math::Vector3D minXYZ,maxXYZ;
	}HEADER_INFO;

	typedef struct _model_file_info{
		int	id;
		char	model_name[LENGTH_OF_NAME];
		char	file_name[LENGTH_OF_FILE_NAME];
	}MODEL_FILE_INFO;

	typedef struct _body_info{
		int	id;
		Math::Vector3D pos;
	}BODY_INFO;

	// ファイルにアクセスするメソッド群
	// ここはIOctreeMapNodeを属性として持つ実体クラスとして定義しても良い？
	class AccessToMapFile{
	private:
		static bool WriteHeader( IOctreeMapNodeForTool *root , File::CFile &file , std::vector<MODEL_FILE_INFO> &model_file_name_list ,
				 int depth_x , int depth_y , int depth_z , std::vector<IMapObjectDataForTool *> &object_list );
		static bool WriteBody( IOctreeMapNodeForTool *root , File::CFile &file , std::vector<IMapObjectDataForTool *> &object_list );

		static bool ReadBodyForTool( Scene::ISceneManager *pSceneMgr , Renderer::Object::IModel **models ,
			  File::CFile &file , IOctreeMapNodeForTool *root , MODEL_FILE_INFO *pModelInfo , int kind_of_model , int num_of_objects );
		static bool ReadHeader( File::CFile &file , IOctreeMapNode *root , MODEL_FILE_INFO **model_info , HEADER_INFO *header_info = NULL );
		static bool ReadBody( Scene::ISceneManager *pSceneMgr , Renderer::Object::IModel **models ,
			  File::CFile &file , IOctreeMapNode *root , MODEL_FILE_INFO *pModelInfo , int kind_of_model , int num_of_objects );
	public:
		static bool ReadMapFromFile( const char *file_name , IOctreeMapNode **ppRoute , Map::MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
					Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );
		static bool ReadMapForToolFromFile( const char *file_name , IOctreeMapNodeForTool **ppRoute , MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
								Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr ,int &divide_x, int& divide_y, int& divide_z);
		static bool WriteMap( IOctreeMapNodeForTool *root , const char *file_name ,
					  std::vector<MODEL_FILE_INFO> &model_file_name_list , int depth_x , int depth_y , int depth_z );
	};
	

};
