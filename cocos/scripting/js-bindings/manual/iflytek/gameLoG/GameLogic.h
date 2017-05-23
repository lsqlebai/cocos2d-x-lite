/**
 * 中文
 */
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/************************************************************************/
/* 食物对象                                                                     */
/************************************************************************/
class Food : public Ref
{
public:
	int32_t id; // 食物id
	int32_t skin; // 食物皮肤下标
	float x;  // 食物在地图中的x坐标
	float y; // 食物在地图中的y坐标
	CREATE_FUNC(Food);
	bool init(){ return true; }
	Food();
};

/************************************************************************/
/* 食物区域对象                                                                     */
/************************************************************************/
class FoodArea : public Ref
{
public:
	int32_t areaId; // 食物区域id
	int32_t x; // 食物区域x，锚点(0,0)
	int32_t y; // 食物区域y，锚点(0,0)
	int32_t width; // 食物区域宽度
	int32_t height; // 食物区域高度
	Vector<Food*> foods; // 食物集合
	CREATE_FUNC(FoodArea);
	bool init(){ return true; }
	FoodArea();
};

/************************************************************************/
/* 吃货大联盟底层逻辑处理器                                                                     */
/************************************************************************/
class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	/**
	 * 将json数据转换为食物区域数据
	 * @param jsonData json数据
	 * @param outData 解析后的食物区域数据
	 */
	static bool parseJsonToFoodAreas(const string& jsonData,Vector<FoodArea*>& outData);

	/**
	 * 初始化食物管理器
	 * @param foodLayer 食物图层
	 * @param foodSkins 食物皮肤集合
	 * @param foodAreas 食物区域集合
	 * @param foodRadius 食物初始化半径
	 */
	void initFoodLayer(Node* foodLayer, const Vector<SpriteFrame*>& foodSkins, const Vector<FoodArea*> &foodAreas, const int32_t& foodRadius);

	/**
	* 添加或删除食物
	* @param isAdd 是否添加食物
	* @param foodAreas 变化的食物
	*/
	void addOrRemoveFood(const bool& isAdd, const Vector<FoodArea*> &foodAreas);
	
private:


	
	/**
	 * 通过食物下标获取食物皮肤
	 * @param index 食物下标，如果食物下标不存在，则会返回默认皮肤
	 */
	SpriteFrame* getFoodSkinByIndex(const int32_t& index);

	
private:
	Node* _foodLayer; // 食物图层
	Vector<SpriteFrame*> _foodSkins; // 食物皮肤集合
	int32_t _foodRadius;
	
};
