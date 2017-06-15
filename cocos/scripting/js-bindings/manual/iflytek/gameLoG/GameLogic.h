/**
 * 中文
 */
#include "cocos2d.h"

USING_NS_CC;
using namespace std;


class PropIdStartOffset
{
public:
	static const int FOOD_START = 100000; // 食物起始偏移量
	static const int ASYLUM_START = 200000;// 障碍物起始偏移量
	static const int SKIN_START = 300000; // 角色皮肤起始偏移量
	static const int SKILL_START = 400000;
};


/************************************************************************/
/* 食物对象                                                                     */
/************************************************************************/
class FoodObj : public Ref
{
public:
	int32_t id; // 食物id
	int32_t skin; // 食物皮肤下标
	float x;  // 食物在地图中的x坐标
	float y; // 食物在地图中的y坐标
	CREATE_FUNC(FoodObj);
	bool init(){ return true; }
	FoodObj();
};

/************************************************************************/
/* 食物区域对象                                                                     */
/************************************************************************/
class FoodAreaObj : public Ref
{
public:
	int32_t areaId; // 食物区域id
	int32_t x; // 食物区域x，锚点(0,0)
	int32_t y; // 食物区域y，锚点(0,0)
	int32_t width; // 食物区域宽度
	int32_t height; // 食物区域高度

	Rect rect; // 食物矩形区域
	Vector<FoodObj*> foods; // 食物集合
	CREATE_FUNC(FoodAreaObj);
	bool init(){ return true; }
	FoodAreaObj();


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
	static bool parseJsonToFoodAreas(const string& jsonData,Vector<FoodAreaObj*>& outData);

	/**
	 * 初始化食物管理器
	 * @param foodLayer 食物图层
	 * @param foodAnimLayer 食物动画图层
	 * @param foodSkins 食物皮肤集合
	 * @param foodAreas 食物区域集合
	 * @param foodRadius 食物初始化半径
	 * @param foodPreCount 预制食物总数
	 */
	void initFoodLayer(Node* foodLayer, Node* foodAnimLayer, const Vector<SpriteFrame*>& foodSkins, const Vector<FoodAreaObj*> &foodAreas, const int32_t& foodRadius, const int32_t& foodPreCount);

	/**
	 * 释放食物图层
	 */
	void releaseFoodLayer();

	/**
	* 添加或删除食物
	* @param isAdd 是否添加食物
	* @param foodAreas 变化的食物
	*/
	void addOrRemoveFood(const bool& isAdd, const Vector<FoodAreaObj*> &foodAreas);

	/**
	 * 删除时候并且附带动画
	 * @param foodAreas
	 * @param animDuration 动画时间
	 * @param targetX 动画飞向的x
	 * @param targetY 动画飞向的y
	 */
	void removeFoodWithAnim(const Vector<FoodAreaObj*> &foodAreas, const float& animDuration, const float& targetX, const float& targetY);
	
	/**
	 * 更新食物可视区域
	 */
	void updateFoodArea(const int32_t& visibleRectX, const int32_t& visibleRectY, const int32_t& visibleRectWidth, const int32_t& visibleRectHeight);
private:


	
	/**
	 * 通过食物下标获取食物皮肤
	 * @param index 食物下标，如果食物下标不存在，则会返回默认皮肤
	 */
	SpriteFrame* getFoodSkinByIndex(const int32_t& index);

	void _initSprite(const int32_t& count);

	Sprite* _getSprite();

	void _putSprite(Sprite* sp);

	void _clearSprite();
	
protected:
	Vector<FoodAreaObj*> _foodAreaRectVector;
	
private:
	Node* _foodLayer; // 食物图层
	Node* _foodAnimLayer; // 食物动画图层
	Vector<SpriteFrame*> _foodSkins; // 食物皮肤集合
	
	int32_t _foodRadius;

	Vector<Sprite*> _foodSpriteVector; // 当前精灵缓存池
	
	Vector<Sprite*> _allfoodSpriteVector; // 所有精灵缓存池，用于保存引用计数
};
