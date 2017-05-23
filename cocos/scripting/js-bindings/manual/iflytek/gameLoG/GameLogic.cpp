/**
 * 中文
 */
#include "GameLogic.h"
#include "json/document.h"
#include "json/rapidjson.h"


GameLogic::GameLogic()
{
	this->_foodLayer = nullptr;
	this->_foodRadius = 20;
}

GameLogic::~GameLogic()
{

}

bool GameLogic::parseJsonToFoodAreas(const string& jsonData, Vector<FoodArea*>& outData)
{
	rapidjson::Document doc;
	const char* json = jsonData.c_str();
	doc.Parse<0>(json);
	if (!doc.HasParseError())
	{

		if (doc.IsArray())
		{
			for (int i = 0; i < doc.Size(); ++i)
			{
				rapidjson::Value& curValue = doc[i];
				FoodArea* foodArea = FoodArea::create();
				if (curValue.HasMember("areaId") && curValue["areaId"].IsNumber())
				{
					int32_t value = curValue["areaId"].GetInt();
					foodArea->areaId = value;
				}
				if (curValue.HasMember("x") && curValue["x"].IsNumber())
				{
					int32_t value = curValue["x"].GetInt();
					foodArea->x = value;
				}
				if (curValue.HasMember("y") && curValue["y"].IsNumber())
				{
					int32_t value = curValue["y"].GetInt();
					foodArea->y = value;
				}
				if (curValue.HasMember("width") && curValue["width"].IsNumber())
				{
					int32_t value = curValue["width"].GetInt();
					foodArea->width = value;
				}
				if (curValue.HasMember("height") && curValue["height"].IsNumber())
				{
					int32_t value = curValue["height"].GetInt();
					foodArea->height = value;
				}
				if (curValue.HasMember("foods"))
				{
					rapidjson::Value& foods = curValue["foods"];
					if (foods.IsArray())
					{
						
						for (int j = 0; j < foods.Size(); ++j)
						{
							rapidjson::Value& curFood = foods[j];
							Food* food = Food::create();
							
							if (curFood.HasMember("id") && curFood["id"].IsNumber())
							{
								
								int32_t value = curFood["id"].GetInt();
								food->id = value;
							}
							if (curFood.HasMember("skin") && curFood["skin"].IsNumber())
							{
								int32_t value = curFood["skin"].GetInt();
								food->skin = value;
							}
							if (curFood.HasMember("x") && curFood["skin"].IsNumber())
							{
								float value = curFood["x"].GetFloat();
								food->x = value;
							}
							if (curFood.HasMember("y") && curFood["y"].IsNumber())
							{
								float value = curFood["y"].GetFloat();
								food->y = value;
							}
							foodArea->foods.pushBack(food);
						}
					}
				}
				outData.pushBack(foodArea);
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void GameLogic::initFoodLayer(Node* foodLayer, const Vector<SpriteFrame*>& foodSkins, const Vector<FoodArea*> &foodAreas, const int32_t& foodRadius)
{
	this->_foodLayer = foodLayer;
	this->_foodRadius = foodRadius;
	_foodSkins.clear();
	_foodSkins.pushBack(foodSkins); // 保存所有食物皮肤

	addOrRemoveFood(true, foodAreas);
}

void GameLogic::addOrRemoveFood(const bool& isAdd, const Vector<FoodArea*> &foodAreas)
{

	if (isAdd) // 新增食物
	{

		float foodScale = -1;
			
		auto addFoodAreas = foodAreas; // 新增的食物
		for (int i = 0, sizeI = addFoodAreas.size(); i<sizeI; ++i)
		{
			auto curFoodArea = addFoodAreas.at(i);
			auto curFoods = curFoodArea->foods;

			if (!curFoods.empty())
			{
				
				//auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea->areaId); // 查找当前区域所在图层
				auto curFoodLayer = this->_foodLayer;
				if (curFoodLayer)
				{
					for (int j = 0, sizeJ = curFoods.size(); j <sizeJ; ++j)
					{
						auto curFoodInfo = curFoods.at(j);

						auto foodSpriteFrame = this->getFoodSkinByIndex(curFoodInfo->skin);
						auto foodNode = Sprite::createWithSpriteFrame(foodSpriteFrame); // 创建食物

						if (foodScale <= 0)
						{
							foodScale = this->_foodRadius * 2 / foodNode->getContentSize().width;
						}

						// 设置食物尺寸
						foodNode->setScale(foodScale);
						
						foodNode->setTag(curFoodInfo->id);
						curFoodLayer->addChild(foodNode);
						foodNode->setPosition(Vec2(curFoodInfo->x, curFoodInfo->y)); // 放置到地图中
					}
				}
			}
		}
	}
	else // 减少食物
	{
		auto minusFoodAreas = foodAreas; // 减少的食物
		for (int i = 0, sizeI = minusFoodAreas.size(); i<sizeI; ++i)
		{
			auto curFoodArea = minusFoodAreas.at(i);
			auto curFoods = curFoodArea->foods;

			if (!curFoods.empty())
			{
				//auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea->areaId); // 查找待清理的食物区域
				auto curFoodLayer = this->_foodLayer;
				if (curFoodLayer)
				{
					for (int j = 0, sizeJ=curFoods.size(); j < sizeJ; ++j) {
						auto curFoodInfo = curFoods.at(j);
						auto food = curFoodLayer->getChildByTag(curFoodInfo->id); // 查找待清理的食物
						if (food)
						{
							food->removeFromParent(); // 移除指定食物对象
						}
					}
				}
			}
		}
	}

}

cocos2d::SpriteFrame* GameLogic::getFoodSkinByIndex(const int32_t& index)
{
	if (index >= 0 && index < this->_foodSkins.size())
	{
		return this->_foodSkins.at(index);
	}

	if (!this->_foodSkins.empty())
	{
		return this->_foodSkins.at(0);
	}
	return nullptr;
}

Food::Food() :id(0), skin(0), x(0), y(0)
{
	
}

FoodArea::FoodArea() : areaId(0), x(0), y(0), width(0), height(0)
{

}
