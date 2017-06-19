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

bool GameLogic::parseJsonToFoodAreas(const string& jsonData, Vector<FoodAreaObj*>& outData)
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

				FoodAreaObj* foodArea = FoodAreaObj::create();
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
							FoodObj* food = FoodObj::create();
							
							if (curFood.HasMember("id") && curFood["id"].IsNumber())
							{
								
								int32_t value = curFood["id"].GetInt();
								food->id = value;
							}
							if (curFood.HasMember("skin") && curFood["skin"].IsInt())
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

void GameLogic::initFoodLayer(Node* foodLayer, Node* foodAnimLayer, const Vector<SpriteFrame*>& foodSkins, const Vector<FoodAreaObj*> &foodAreas, const int32_t& foodRadius, const int32_t& foodPreCount)
{
	
	this->_foodLayer = foodLayer;

	for (int i = 0, sizeI = foodAreas.size(); i < sizeI; ++i)
	{
		auto curFoodArea = foodAreas.at(i);
		Node* curFoodAreaLayer = Node::create(); // 生成当前食物区域图层
		curFoodAreaLayer->setTag(curFoodArea->areaId);
		curFoodAreaLayer->setAnchorPoint(Vec2(0,0));
		this->_foodLayer->addChild(curFoodAreaLayer); // 添加食物区域

		FoodAreaObj* obj = FoodAreaObj::create();
		obj->areaId = curFoodArea->areaId;
		obj->x = curFoodArea->x;
		obj->y = curFoodArea->y;
		obj->width = curFoodArea->width;
		obj->height = curFoodArea->height;
		obj->rect.setRect(obj->x, obj->y, obj->width, obj->height);

		_foodAreaRectVector.pushBack(obj); // 保存食物区域对象
	}

	this->_foodAnimLayer = foodAnimLayer;
	this->_foodRadius = foodRadius;
	_foodSkins.clear();
	_foodSkins.pushBack(foodSkins); // 保存所有食物皮肤

	_initSprite(foodPreCount); // 初始化缓存池

	addOrRemoveFood(true, foodAreas);
}



void GameLogic::releaseFoodLayer()
{
	if (this->_foodLayer)
	{
		this->_foodLayer->removeAllChildren();
	}
	_clearSprite();
	_foodSkins.clear();	
}

void GameLogic::addOrRemoveFood(const bool& isAdd, const Vector<FoodAreaObj*> &foodAreas)
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
				
				auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea->areaId); // 查找当前区域所在图层
				//auto curFoodLayer = this->_foodLayer;
				if (curFoodLayer)
				{
					for (int j = 0, sizeJ = curFoods.size(); j <sizeJ; ++j)
					{
						auto curFoodInfo = curFoods.at(j);
						
						
						
						auto foodSpriteFrame = this->getFoodSkinByIndex(curFoodInfo->skin - PropIdStartOffset::FOOD_START);
						//auto foodNode = Sprite::createWithSpriteFrame(foodSpriteFrame); // 创建食物
						auto foodNode = _getSprite();
						foodNode->setSpriteFrame(foodSpriteFrame);

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
				auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea->areaId); // 查找待清理的食物区域
				//auto curFoodLayer = this->_foodLayer;
				if (curFoodLayer)
				{
					for (int j = 0, sizeJ=curFoods.size(); j < sizeJ; ++j) {
						auto curFoodInfo = curFoods.at(j);
						auto food = curFoodLayer->getChildByTag(curFoodInfo->id); // 查找待清理的食物
						if (food)
						{
							food->removeFromParent(); // 移除指定食物对象
							_putSprite((Sprite*)food); // 放到缓存池
						}
						//curFoodLayer->removeChildByTag(curFoodInfo->id);
					}
				}
			}
		}
	}

}

void GameLogic::removeFoodWithAnim(const Vector<FoodAreaObj*> &foodAreas, const float& animDuration, const float& targetX, const float& targetY)
{
	auto minusFoodAreas = foodAreas; // 减少的食物
	for (int i = 0, sizeI = minusFoodAreas.size(); i < sizeI; ++i)
	{
		auto curFoodArea = minusFoodAreas.at(i);
		auto curFoods = curFoodArea->foods;

		if (!curFoods.empty())
		{
			
			auto curFoodLayer = this->_foodLayer->getChildByTag(curFoodArea->areaId); // 查找待清理的食物区域
			//auto curFoodLayer = this->_foodLayer;
			if (curFoodLayer)
			{
				
				for (int j = 0, sizeJ = curFoods.size(); j < sizeJ; ++j) {
					auto curFoodInfo = curFoods.at(j);
					auto food = curFoodLayer->getChildByTag(curFoodInfo->id); // 查找待清理的食物
					if (food)
					{
						

						if (curFoodLayer->isVisible())
						{

							food->removeFromParent(); // 移除指定食物对象
							
							this->_foodAnimLayer->addChild(food); // 添加到动画图层
							Action* action = Sequence::create(EaseCircleActionIn::create(MoveTo::create(animDuration, Vec2(targetX, targetY))),
								CallFunc::create([food, this]()
							{
								food->removeFromParent();
								_putSprite((Sprite*)food); // 放到缓存池
							}),
								nullptr);
							food->runAction(action);
						}
						 else
						 {
							 food->removeFromParent(); // 移除指定食物对象
							 _putSprite((Sprite*)food); // 放到缓存池
						 }
					}
				}
			}
		}
	}
}

void GameLogic::updateFoodArea(const int32_t& visibleRectX, const int32_t& visibleRectY, const int32_t& visibleRectWidth, const int32_t& visibleRectHeight)
{

	int count = 0;
	Rect visibleRect = Rect(visibleRectX, visibleRectY, visibleRectWidth, visibleRectHeight);
	for (int i = 0, sizeI = _foodAreaRectVector.size(); i < sizeI; ++i)
	{
		FoodAreaObj* obj = _foodAreaRectVector.at(i);
		
		auto layer = this->_foodLayer->getChildByTag(obj->areaId);

		if (layer)
		{
			if (visibleRect.intersectsRect(obj->rect)) // 可见
			{
				layer->setVisible(true);
				count++;
			}
			else // 不可见
			{
				layer->setVisible(false);
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


void GameLogic::_initSprite(const int32_t& count)
{
	for (int i = 0; i < count; ++i)
	{
		auto sp = Sprite::create();
		_foodSpriteVector.pushBack(sp);
		_allfoodSpriteVector.pushBack(sp);
	}
}

cocos2d::Sprite* GameLogic::_getSprite()
{
	Sprite* result = nullptr;
	if (!_foodSpriteVector.empty())
	{
		result = _foodSpriteVector.back();
		_foodSpriteVector.popBack();
		result->stopAllActions();
	}
	 else 
	 {
		 result = Sprite::create();
		 _allfoodSpriteVector.pushBack(result);
	 }
	//CCLOG("food get count:%d, all:%d", _foodSpriteVector.size(), _allfoodSpriteVector.size());
	return result;
}

void GameLogic::_putSprite(Sprite* sp)
{
	_foodSpriteVector.pushBack(sp);

	//CCLOG("food put count:%d, all:%d", _foodSpriteVector.size(), _allfoodSpriteVector.size());
}

void GameLogic::_clearSprite()
{
	_foodSpriteVector.clear();
	_allfoodSpriteVector.clear();
}

FoodObj::FoodObj() :id(0), skin(0), x(0), y(0)
{
	
}

FoodAreaObj::FoodAreaObj() : areaId(0), x(0), y(0), width(0), height(0)
{

}
