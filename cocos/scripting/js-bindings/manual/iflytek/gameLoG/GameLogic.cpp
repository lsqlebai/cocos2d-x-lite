/**
 * 中文
 */
#include "GameLogic.h"

GameLogic::GameLogic()
{
	//this->_foodLayer = nullptr;
}

GameLogic::~GameLogic()
{

}

void GameLogic::initFoodLayer(Node* foodLayer, const Vector<SpriteFrame*>& foodSkins, const Vector<FoodArea> &foodAreas)
{
	//this->_foodLayer = foodLayer;
	//_foodSkins.clear();
	//_foodSkins.pushBack(foodSkins); // 保存所有食物皮肤

	
}

void GameLogic::addOrRemoveFood(const bool& isAdd, const Vector<FoodArea> &foodAreas)
{

	
	if (isAdd) // 新增食物
	{
		//auto addFoodAreas = foodAreas; // 新增的食物
		//for (int i = 0, sizeI = addFoodAreas.size(); i<sizeI; ++i)
		//{
		//	auto curFoodArea = addFoodAreas.at(i);
		//	auto curFoods = curFoodArea.foods;

		//	if (!curFoods.empty())
		//	{
		//		
		//		auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea.areaId); // 查找当前区域所在图层

		//		if (curFoodLayer)
		//		{
		//			for (int j = 0, sizeJ = curFoods.size(); j <sizeJ; ++j)
		//			{
		//				
		//				auto curFoodInfo = curFoods.at(j);

		//				auto foodSpriteFrame = this->getFoodSkinByIndex(curFoodInfo.skin);
		//				auto foodNode = Sprite::createWithSpriteFrame(foodSpriteFrame); // 创建食物
		//				
		//				foodNode->setTag(curFoodInfo.id);
		//				curFoodLayer->addChild(foodNode);
		//				foodNode->setPosition(Vec2(curFoodInfo.x, curFoodInfo.y)); // 放置到地图中
		//			}
		//		}
		//	}
		//}
	}
	else // 减少食物
	{
		//auto minusFoodAreas = foodAreas; // 减少的食物
		//for (auto i = 0; i<minusFoodAreas.length; ++i)
		//{
		//	auto curFoodArea = minusFoodAreas[i];
		//	auto curFoods = curFoodArea.foods;

		//	if (curFoods.length>0)
		//	{
		//		auto curFoodLayer = this._foodLayer.getChildByTag(curFoodArea.areaId); // 查找待清理的食物区域

		//		if (curFoodLayer)
		//		{
		//			for (auto j = 0; j < curFoods.length; ++j) {
		//				auto curFoodInfo = curFoods[j];
		//				auto food = curFoodLayer.getChildByTag(curFoodInfo.id); // 查找待清理的食物
		//				if (food)
		//				{
		//					food.removeFromParent(false); // 移除指定食物对象
		//					this._foodPool.put(food);
		//				}
		//			}
		//		}
		//	}
		//}
	}

}

cocos2d::SpriteFrame* GameLogic::getFoodSkinByIndex(const int32_t& index)
{
	//if (index >= 0 && index < this->_foodSkins.size())
	//{
	//	return this->_foodSkins.at(index);
	//}

	//if (!this->_foodSkins.empty())
	//{
	//	return this->_foodSkins.at(0);
	//}
	return nullptr;
}
